/* This flag enables/disables various bits of GC debugging sanity checks:
 * 0 = No checking
 * 1 = Checks on reference assignments and other relatively cheap cases
 * 2 = Checks on every object register access (slow)
 */
#define MVM_GC_DEBUG 0

#if MVM_GC_DEBUG
#define MVM_ASSERT_NOT_FROMSPACE(tc, c) do { \
    MVMThread *cur_thread = tc->instance->threads; \
    while (cur_thread) { \
        MVMThreadContext *thread_tc = cur_thread->body.tc; \
        if (thread_tc && thread_tc->nursery_fromspace && \
                (char *)(c) >= (char *)thread_tc->nursery_fromspace && \
                (char *)(c) < (char *)thread_tc->nursery_fromspace + \
                    thread_tc->nursery_fromspace_size) \
            MVM_panic(1, "Collectable %p in fromspace accessed", c); \
        cur_thread = cur_thread->body.next; \
    } \
} while (0)
#define MVM_CHECK_CALLER_CHAIN(tc, f) do { \
    MVMFrame *check = f; \
    while (check) { \
        MVM_ASSERT_NOT_FROMSPACE(tc, check); \
        if ((check->header.flags & MVM_CF_SECOND_GEN) && \
                check->caller && \
                !(check->caller->header.flags & MVM_CF_SECOND_GEN) && \
                !(check->header.flags & MVM_CF_IN_GEN2_ROOT_LIST)) \
            MVM_panic(1, "Illegal Gen2 -> Nursery in caller chain (not in inter-gen set)"); \
        check = check->caller; \
    } \
} while (0)
#else
#define MVM_ASSERT_NOT_FROMSPACE(tc, c)
#define MVM_CHECK_CALLER_CHAIN(tc, f)
#endif
