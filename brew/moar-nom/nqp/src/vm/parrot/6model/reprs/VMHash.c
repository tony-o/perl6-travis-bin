#define PARROT_IN_EXTENSION
#include "parrot/parrot.h"
#include "parrot/extend.h"
#include "../sixmodelobject.h"
#include "VMHash.h"

/* This representation's function pointer table. */
static REPROps *this_repr;

/* Creates a new type object of this representation, and associates it with
 * the given HOW. */
static PMC * type_object_for(PARROT_INTERP, PMC *HOW) {
    /* Create new object instance. */
    VMHashInstance *obj = mem_allocate_zeroed_typed(VMHashInstance);

    /* Build an STable. */
    PMC *st_pmc = create_stable(interp, this_repr, HOW);
    STable *st  = STABLE_STRUCT(st_pmc);

    /* Create type object and point it back at the STable. */
    obj->common.stable = st_pmc;
    st->WHAT = wrap_object(interp, obj);
    PARROT_GC_WRITE_BARRIER(interp, st_pmc);

    /* Flag it as a type object. */
    MARK_AS_TYPE_OBJECT(st->WHAT);

    return st->WHAT;
}

/* Composes the representation. */
static void compose(PARROT_INTERP, STable *st, PMC *repr_info) {
    UNUSED(interp);
    UNUSED(st);
    UNUSED(repr_info);
    /* Nothing to do yet, but should handle type in the future. */
}

/* Creates a new instance based on the type object. */
static PMC * allocate(PARROT_INTERP, STable *st) {
    VMHashInstance *obj = mem_allocate_zeroed_typed(VMHashInstance);
    obj->common.stable = st->stable_pmc;
    return wrap_object(interp, obj);
}

/* Initialize a new instance. */
static void initialize(PARROT_INTERP, STable *st, void *data) {
    UNUSED(interp);
    UNUSED(st);
    UNUSED(data);
    /* Nothing to do here. */
}

/* Copies to the body of one object to another. */
static void copy_to(PARROT_INTERP, STable *st, void *src, void *dest) {
    VMHashBody *src_body = (VMHashBody *)src;
    VMHashBody *dest_body = (VMHashBody *)dest;
    UNUSED(interp);
    UNUSED(st);
    /* Nothing to do yet. */
}

/* This Parrot-specific addition to the API is used to free an object. */
static void gc_free(PARROT_INTERP, PMC *obj) {
    UNUSED(interp);
    mem_sys_free(PMC_data(obj));
    PMC_data(obj) = NULL;
}

/* Gets the storage specification for this representation. */
static void get_storage_spec(PARROT_INTERP, STable *st, storage_spec *spec) {
    UNUSED(interp);
    UNUSED(st);
    spec->inlineable      = STORAGE_SPEC_REFERENCE;
    spec->boxed_primitive = STORAGE_SPEC_BP_NONE;
    spec->can_box         = 0;
    spec->bits            = sizeof(void *) * 8;
    spec->align           = ALIGNOF1(void *);
}

/* Initializes the VMHash representation. */
REPROps * VMHash_initialize(PARROT_INTERP) {
    UNUSED(interp);
    /* Allocate and populate the representation function table. */
    this_repr = mem_allocate_zeroed_typed(REPROps);
    this_repr->type_object_for = type_object_for;
    this_repr->compose = compose;
    this_repr->allocate = allocate;
    this_repr->initialize = initialize;
    this_repr->copy_to = copy_to;
    this_repr->gc_free = gc_free;
    this_repr->get_storage_spec = get_storage_spec;
    return this_repr;
}
