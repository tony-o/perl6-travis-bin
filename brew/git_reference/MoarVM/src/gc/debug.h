/* Include this file after moar.h in a source file to get some help with GC
 * debugging. */

#define MVM_ASSERT_NOT_FROMSPACE(tc, c) do { \
    if ((char *)(c) >= (char *)tc->nursery_fromspace && \
            (char *)(c) < (char *)tc->nursery_fromspace + MVM_NURSERY_SIZE) \
        MVM_exception_throw_adhoc(tc, "Collectable in fromspace accessed"); \
} while (0)
