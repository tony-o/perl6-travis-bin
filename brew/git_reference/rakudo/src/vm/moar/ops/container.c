#define MVM_SHARED 1
#include "moar.h"
#include "container.h"

/* Dummy, no-arg callsite. */
static MVMCallsite no_arg_callsite = { NULL, 0, 0, 0 };

/* Dummy callsite for type_check. */
static MVMCallsiteEntry tc_flags[] = { MVM_CALLSITE_ARG_OBJ,
                                       MVM_CALLSITE_ARG_OBJ,
                                       MVM_CALLSITE_ARG_OBJ };
static MVMCallsite     tc_callsite = { tc_flags, 3, 3, 3, 0 };

static void rakudo_scalar_fetch(MVMThreadContext *tc, MVMObject *cont, MVMRegister *res) {
    res->o = ((Rakudo_Scalar *)cont)->value;
}

static void rakudo_scalar_fetch_i(MVMThreadContext *tc, MVMObject *cont, MVMRegister *res) {
    res->i64 = MVM_repr_get_int(tc, ((Rakudo_Scalar *)cont)->value);
}

static void rakudo_scalar_fetch_n(MVMThreadContext *tc, MVMObject *cont, MVMRegister *res) {
    res->n64 = MVM_repr_get_num(tc, ((Rakudo_Scalar *)cont)->value);
}

static void rakudo_scalar_fetch_s(MVMThreadContext *tc, MVMObject *cont, MVMRegister *res) {
    res->s = MVM_repr_get_str(tc, ((Rakudo_Scalar *)cont)->value);
}

MVMObject * get_nil();
MVMObject * get_mu();

static void finish_store(MVMThreadContext *tc, MVMObject *cont, MVMObject *obj) {
    Rakudo_Scalar *rs = (Rakudo_Scalar *)cont;
    MVMObject *whence;

    /* Store the value. */
    MVM_ASSIGN_REF(tc, &(cont->header), rs->value, obj);

    /* Run any whence closure. */
    whence = rs->whence;
    if (whence && IS_CONCRETE(whence)) {
        MVMObject *code = MVM_frame_find_invokee(tc, whence, NULL);
        MVM_args_setup_thunk(tc, NULL, MVM_RETURN_VOID, &no_arg_callsite);
        rs->whence = NULL;
        STABLE(code)->invoke(tc, code, &no_arg_callsite, tc->cur_frame->args);
    }
}

typedef struct {
    MVMObject   *cont;
    MVMObject   *obj;
    MVMRegister  res;
} type_check_data;
void Rakudo_assign_typecheck_failed(MVMThreadContext *tc, MVMObject *cont, MVMObject *obj);
static void type_check_ret(MVMThreadContext *tc, void *sr_data) {
    type_check_data *tcd = (type_check_data *)sr_data;
    MVMObject *cont = tcd->cont;
    MVMObject *obj  = tcd->obj;
    MVMint64   res  = tcd->res.i64;
    free(tcd);
    if (res)
        finish_store(tc, cont, obj);
    else
         Rakudo_assign_typecheck_failed(tc, cont, obj);
}
static void mark_sr_data(MVMThreadContext *tc, MVMFrame *frame, MVMGCWorklist *worklist) {
    type_check_data *tcd = (type_check_data *)frame->special_return_data;
    MVM_gc_worklist_add(tc, worklist, &tcd->cont);
    MVM_gc_worklist_add(tc, worklist, &tcd->obj);
}

static void rakudo_scalar_store(MVMThreadContext *tc, MVMObject *cont, MVMObject *obj) {
    Rakudo_Scalar *rs = (Rakudo_Scalar *)cont;
    Rakudo_ContainerDescriptor *rcd = (Rakudo_ContainerDescriptor *)rs->descriptor;
    MVMint64 rw = 0;

    /* Check it's an assignable container. */
    if (rcd && IS_CONCRETE(rcd))
        rw = rcd->rw;
    if (!rw)
        MVM_exception_throw_adhoc(tc, "Cannot assign to a readonly variable or a value");

    /* Handle Nil and type-checking. */
    if (!obj) {
        MVM_exception_throw_adhoc(tc, "Cannot assign a null value to a Perl 6 scalar");
    }
    else if (STABLE(obj)->WHAT == get_nil()) {
        obj = rcd->the_default;
    }
    else {
        /* Check against the type-check cache first (common, fast-path
         * case). */
        MVMint64 mode = STABLE(rcd->of)->mode_flags & MVM_TYPE_CHECK_CACHE_FLAG_MASK;
        if (rcd->of != get_mu() && !MVM_6model_istype_cache_only(tc, obj, rcd->of)) {
            /* Failed. If the cache is definitive, we certainly have an error. */
            if (STABLE(obj)->type_check_cache &&
                (mode & MVM_TYPE_CHECK_CACHE_THEN_METHOD) == 0 &&
                (mode & MVM_TYPE_CHECK_NEEDS_ACCEPTS) == 0) {
                 Rakudo_assign_typecheck_failed(tc, cont, obj);
                 return;
             }

            /* If we get here, need to call .^type_check on the value we're
             * checking, unless it's an accepts check. */
            if (!STABLE(obj)->type_check_cache || (mode & MVM_TYPE_CHECK_CACHE_THEN_METHOD)) {
                MVMObject *HOW = MVM_6model_get_how_obj(tc, rcd->of);
                MVMObject *meth = MVM_6model_find_method_cache_only(tc, HOW,
                    tc->instance->str_consts.type_check);
                if (meth) {
                    /* Set up the call, using a fake register in special return
                     * data as the target. */
                    MVMObject *code = MVM_frame_find_invokee(tc, meth, NULL);
                    type_check_data *tcd = malloc(sizeof(type_check_data));
                    tcd->cont    = cont;
                    tcd->obj     = obj;
                    tcd->res.i64 = 0;
                    MVM_args_setup_thunk(tc, &tcd->res, MVM_RETURN_INT, &tc_callsite);
                    tc->cur_frame->special_return           = type_check_ret;
                    tc->cur_frame->special_return_data      = tcd;
                    tc->cur_frame->mark_special_return_data = mark_sr_data;
                    tc->cur_frame->args[0].o = HOW;
                    tc->cur_frame->args[1].o = obj;
                    tc->cur_frame->args[2].o = rcd->of;
                    STABLE(code)->invoke(tc, code, &tc_callsite, tc->cur_frame->args);
                    return;
                }
            }

            /* If the flag to call .accepts_type on the target value is set, do so. */
            if (mode & MVM_TYPE_CHECK_NEEDS_ACCEPTS) {
                MVMObject *HOW = MVM_6model_get_how_obj(tc, rcd->of);
                MVMObject *meth = MVM_6model_find_method_cache_only(tc, HOW,
                    tc->instance->str_consts.accepts_type);
                if (meth) {
                    /* Set up the call, using the result register as the target. */
                    MVMObject *code = MVM_frame_find_invokee(tc, meth, NULL);
                    type_check_data *tcd = malloc(sizeof(type_check_data));
                    tcd->cont    = cont;
                    tcd->obj     = obj;
                    tcd->res.i64 = 0;
                    MVM_args_setup_thunk(tc, &tcd->res, MVM_RETURN_INT, &tc_callsite);
                    tc->cur_frame->special_return           = type_check_ret;
                    tc->cur_frame->special_return_data      = tcd;
                    tc->cur_frame->mark_special_return_data = mark_sr_data;
                    tc->cur_frame->args[0].o = HOW;
                    tc->cur_frame->args[1].o = rcd->of;
                    tc->cur_frame->args[2].o = obj;
                    STABLE(code)->invoke(tc, code, &tc_callsite, tc->cur_frame->args);
                    return;
                }
                else {
                    MVM_exception_throw_adhoc(tc,
                        "Expected 'accepts_type' method, but none found in meta-object");
                }
            }
        }
    }

    /* Complete the store. */
    finish_store(tc, cont, obj);
}

static void rakudo_scalar_store_i(MVMThreadContext *tc, MVMObject *cont, MVMint64 value) {
    MVMObject *boxed;
    MVMROOT(tc, cont, {
        boxed = MVM_repr_box_int(tc, MVM_hll_current(tc)->int_box_type, value);
    });
    rakudo_scalar_store(tc, cont, boxed);
}

static void rakudo_scalar_store_n(MVMThreadContext *tc, MVMObject *cont, MVMnum64 value) {
    MVMObject *boxed;
    MVMROOT(tc, cont, {
        boxed = MVM_repr_box_num(tc, MVM_hll_current(tc)->num_box_type, value);
    });
    rakudo_scalar_store(tc, cont, boxed);
}

static void rakudo_scalar_store_s(MVMThreadContext *tc, MVMObject *cont, MVMString *value) {
    MVMObject *boxed;
    MVMROOT(tc, cont, {
        boxed = MVM_repr_box_str(tc, MVM_hll_current(tc)->str_box_type, value);
    });
    rakudo_scalar_store(tc, cont, boxed);
}

static void rakudo_scalar_store_unchecked(MVMThreadContext *tc, MVMObject *cont, MVMObject *obj) {
    finish_store(tc, cont, obj);
}

static void rakudo_scalar_serialize(MVMThreadContext *tc, MVMSTable *st, MVMSerializationWriter *writer) {
    /* Nothing to do. */
}

static void rakudo_scalar_deserialize(MVMThreadContext *tc, MVMSTable *st, MVMSerializationReader *reader) {
    /* Nothing to do. */
}

static void rakudo_scalar_spesh(MVMThreadContext *tc, MVMSTable *st, MVMSpeshGraph *g, MVMSpeshBB *bb, MVMSpeshIns *ins) {
    switch (ins->info->opcode) {
    case MVM_OP_decont: {
        MVMSpeshOperand *old_operands = ins->operands;
        ins->info = MVM_op_get_op(MVM_OP_sp_p6oget_o);
        ins->operands = MVM_spesh_alloc(tc, g, 3 * sizeof(MVMSpeshOperand));
        ins->operands[0] = old_operands[0];
        ins->operands[1] = old_operands[1];
        ins->operands[2].lit_i16 = offsetof( Rakudo_Scalar, value ) - offsetof( MVMObjectStooge, data );
        break;
        }
    default: break;
    }
}

static MVMint32 rakudo_scalar_can_store(MVMThreadContext *tc, MVMObject *cont) {
    Rakudo_Scalar *rs = (Rakudo_Scalar *)cont;
    Rakudo_ContainerDescriptor *rcd = (Rakudo_ContainerDescriptor *)rs->descriptor;
    return rcd && IS_CONCRETE(rcd) && rcd->rw;
}

static const MVMContainerSpec rakudo_scalar_spec = {
    "rakudo_scalar",
    rakudo_scalar_fetch,
    rakudo_scalar_fetch_i,
    rakudo_scalar_fetch_n,
    rakudo_scalar_fetch_s,
    rakudo_scalar_store,
    rakudo_scalar_store_i,
    rakudo_scalar_store_n,
    rakudo_scalar_store_s,
    rakudo_scalar_store_unchecked,
    rakudo_scalar_spesh,
    NULL,
    NULL,
    rakudo_scalar_serialize,
    rakudo_scalar_deserialize,
    rakudo_scalar_can_store,
    1
};

static void rakudo_scalar_set_container_spec(MVMThreadContext *tc, MVMSTable *st) {
    st->container_spec = &rakudo_scalar_spec;
}

static void rakudo_scalar_configure_container_spec(MVMThreadContext *tc, MVMSTable *st, MVMObject *config) {
    /* Nothing to do. */
}

static const MVMContainerConfigurer ContainerConfigurer = {
    rakudo_scalar_set_container_spec,
    rakudo_scalar_configure_container_spec
};

void Rakudo_containers_setup(MVMThreadContext *tc) {
    MVM_6model_add_container_config(tc,
        MVM_string_ascii_decode_nt(tc, tc->instance->VMString, "rakudo_scalar"), &ContainerConfigurer);
}

MVMContainerSpec * Rakudo_containers_get_scalar() {
    return (MVMContainerSpec *)&rakudo_scalar_spec;
}
