#include "moar.h"
#include <platform/threads.h>

/* If we have the job of doing GC for a thread, we add it to our work
 * list. */
static void add_work(MVMThreadContext *tc, MVMThreadContext *stolen) {
    MVMint32 i;
    for (i = 0; i < tc->gc_work_count; i++)
        if (tc->gc_work[i].tc == stolen)
            return;
    if (tc->gc_work == NULL) {
        tc->gc_work_size = 16;
        tc->gc_work = MVM_malloc(tc->gc_work_size * sizeof(MVMWorkThread));
    }
    else if (tc->gc_work_count == tc->gc_work_size) {
        tc->gc_work_size *= 2;
        tc->gc_work = MVM_realloc(tc->gc_work, tc->gc_work_size * sizeof(MVMWorkThread));
    }
    tc->gc_work[tc->gc_work_count++].tc = stolen;
}

/* Goes through all threads but the current one and notifies them that a
 * GC run is starting. Those that are blocked are considered excluded from
 * the run, and are not counted. Returns the count of threads that should be
 * added to the finished countdown. */
static MVMuint32 signal_one_thread(MVMThreadContext *tc, MVMThreadContext *to_signal) {
    /* Loop here since we may not succeed first time (e.g. the status of the
     * thread may change between the two ways we try to twiddle it). */
    while (1) {
        switch (MVM_load(&to_signal->gc_status)) {
            case MVMGCStatus_NONE:
                /* Try to set it from running to interrupted - the common case. */
                if (MVM_cas(&to_signal->gc_status, MVMGCStatus_NONE,
                        MVMGCStatus_INTERRUPT) == MVMGCStatus_NONE) {
                    GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : Signalled thread %d to interrupt\n", to_signal->thread_id);
                    return 1;
                }
                break;
            case MVMGCStatus_INTERRUPT:
                GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : thread %d already interrupted\n", to_signal->thread_id);
                return 0;
            case MVMGCStatus_UNABLE:
                /* Otherwise, it's blocked; try to set it to work Stolen. */
                if (MVM_cas(&to_signal->gc_status, MVMGCStatus_UNABLE,
                        MVMGCStatus_STOLEN) == MVMGCStatus_UNABLE) {
                    GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : A blocked thread %d spotted; work stolen\n", to_signal->thread_id);
                    add_work(tc, to_signal);
                    return 0;
                }
                break;
            /* this case occurs if a child thread is Stolen by its parent
             * before we get to it in the chain. */
            case MVMGCStatus_STOLEN:
                GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : thread %d already stolen (it was a spawning child)\n", to_signal->thread_id);
                return 0;
            default:
                MVM_panic(MVM_exitcode_gcorch, "invalid status %"MVM_PRSz" in GC orchestrate\n", MVM_load(&to_signal->gc_status));
                return 0;
        }
    }
}
static MVMuint32 signal_all(MVMThreadContext *tc, MVMThread *threads) {
    MVMThread *t = threads;
    MVMuint32 count = 0;
    while (t) {
        switch (MVM_load(&t->body.stage)) {
            case MVM_thread_stage_starting:
            case MVM_thread_stage_waiting:
            case MVM_thread_stage_started:
                /* Don't signal ourself. */
                if (t->body.tc != tc)
                    count += signal_one_thread(tc, t->body.tc);
                break;
            case MVM_thread_stage_exited:
                GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : queueing to clear nursery of thread %d\n", t->body.tc->thread_id);
                add_work(tc, t->body.tc);
                break;
            case MVM_thread_stage_clearing_nursery:
                GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : queueing to destroy thread %d\n", t->body.tc->thread_id);
                /* last GC run for this thread */
                add_work(tc, t->body.tc);
                break;
            case MVM_thread_stage_destroyed:
                GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : found a destroyed thread\n");
                /* will be cleaned up (removed from the lists) shortly */
                break;
            default:
                MVM_panic(MVM_exitcode_gcorch, "Corrupted MVMThread or running threads list: invalid thread stage %"MVM_PRSz"", MVM_load(&t->body.stage));
        }
        t = t->body.next;
    }
    return count;
}

/* Does work in a thread's in-tray, if any. Returns a non-zero value if work
 * was found and done, and zero otherwise. */
static int process_in_tray(MVMThreadContext *tc, MVMuint8 gen) {
    GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : Considering extra work\n");
    if (MVM_load(&tc->gc_in_tray)) {
        GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE,
            "Thread %d run %d : Was given extra work by another thread; doing it\n");
        MVM_gc_collect(tc, MVMGCWhatToDo_InTray, gen);
        return 1;
    }
    return 0;
}

/* Called by a thread when it thinks it is done with GC. It may get some more
 * work yet, though. */
static void clear_intrays(MVMThreadContext *tc, MVMuint8 gen) {
    MVMuint32 did_work = 1;
    while (did_work) {
        MVMThread *cur_thread;
        did_work = 0;
        cur_thread = (MVMThread *)MVM_load(&tc->instance->threads);
        while (cur_thread) {
            if (cur_thread->body.tc)
                did_work += process_in_tray(cur_thread->body.tc, gen);
            cur_thread = cur_thread->body.next;
        }
    }
}
static void finish_gc(MVMThreadContext *tc, MVMuint8 gen, MVMuint8 is_coordinator) {
    MVMuint32 i, did_work;

    /* Do any extra work that we have been passed. */
    GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE,
        "Thread %d run %d : doing any work in thread in-trays\n");
    did_work = 1;
    while (did_work) {
        did_work = 0;
        for (i = 0; i < tc->gc_work_count; i++)
            did_work += process_in_tray(tc->gc_work[i].tc, gen);
    }

    /* Decrement gc_finish to say we're done, and wait for termination. */
    GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : Voting to finish\n");
    uv_mutex_lock(&tc->instance->mutex_gc_orchestrate);
    MVM_decr(&tc->instance->gc_finish);
    uv_cond_broadcast(&tc->instance->cond_gc_finish);
    while (MVM_load(&tc->instance->gc_finish))
        uv_cond_wait(&tc->instance->cond_gc_finish, &tc->instance->mutex_gc_orchestrate);
    uv_mutex_unlock(&tc->instance->mutex_gc_orchestrate);
    GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : Termination agreed\n");

    /* Co-ordinator should do final check over all the in-trays, and trigger
     * collection until all is settled. Rest should wait. Additionally, after
     * in-trays are settled, coordinator walks threads looking for anything
     * that needs adding to the finalize queue. It then will make another
     * iteration over in-trays to handle cross-thread references to objects
     * needing finalization. For full collections, collected objects are then
     * cleaned from all inter-generational sets, and finally any objects to
     * be freed at the fixed size allocator's next safepoint are freed. */
    if (is_coordinator) {
        GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE,
            "Thread %d run %d : Co-ordinator handling in-tray clearing completion\n");
        clear_intrays(tc, gen);

        GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE,
            "Thread %d run %d : Co-ordinator handling finalizers\n");
        MVM_finalize_walk_queues(tc, gen);
        clear_intrays(tc, gen);

        if (gen == MVMGCGenerations_Both) {
            MVMThread *cur_thread = (MVMThread *)MVM_load(&tc->instance->threads);
            GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE,
                "Thread %d run %d : Co-ordinator handling inter-gen root cleanup\n");
            while (cur_thread) {
                if (cur_thread->body.tc)
                    MVM_gc_root_gen2_cleanup(cur_thread->body.tc);
                cur_thread = cur_thread->body.next;
            }
        }

        GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE,
            "Thread %d run %d : Co-ordinator handling fixed-size allocator safepoint frees\n");
        MVM_fixed_size_safepoint(tc, tc->instance->fsa);

        MVM_profile_heap_take_snapshot(tc);

        GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE,
            "Thread %d run %d : Co-ordinator signalling in-trays clear\n");
        uv_mutex_lock(&tc->instance->mutex_gc_orchestrate);
        MVM_store(&tc->instance->gc_intrays_clearing, 0);
        uv_cond_broadcast(&tc->instance->cond_gc_intrays_clearing);
        uv_mutex_unlock(&tc->instance->mutex_gc_orchestrate);
    }
    else {
        GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE,
            "Thread %d run %d : Waiting for in-tray clearing completion\n");
        uv_mutex_lock(&tc->instance->mutex_gc_orchestrate);
        while (MVM_load(&tc->instance->gc_intrays_clearing))
            uv_cond_wait(&tc->instance->cond_gc_intrays_clearing, &tc->instance->mutex_gc_orchestrate);
        uv_mutex_unlock(&tc->instance->mutex_gc_orchestrate);
        GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE,
            "Thread %d run %d : Got in-tray clearing complete notice\n");
    }

    /* Reset GC status flags. This is also where thread destruction happens,
     * and it needs to happen before we acknowledge this GC run is finished. */
    for (i = 0; i < tc->gc_work_count; i++) {
        MVMThreadContext *other = tc->gc_work[i].tc;
        MVMThread *thread_obj = other->thread_obj;
        if (MVM_load(&thread_obj->body.stage) == MVM_thread_stage_clearing_nursery) {
            GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE,
                "Thread %d run %d : transferring gen2 of thread %d\n", other->thread_id);
            MVM_gc_gen2_transfer(other, tc);
            GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE,
                "Thread %d run %d : destroying thread %d\n", other->thread_id);
            MVM_tc_destroy(other);
            tc->gc_work[i].tc = thread_obj->body.tc = NULL;
            MVM_store(&thread_obj->body.stage, MVM_thread_stage_destroyed);
        }
        else {
            /* Free gen2 unmarked if full collection. */
            if (gen == MVMGCGenerations_Both) {
                GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE,
                    "Thread %d run %d : freeing gen2 of thread %d\n",
                    other->thread_id);
                MVM_gc_collect_free_gen2_unmarked(other, 0);
            }

            /* Contribute this thread's promoted bytes. */
            MVM_add(&tc->instance->gc_promoted_bytes_since_last_full, other->gc_promoted_bytes);

            /* Collect nursery. */
            GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE,
                "Thread %d run %d : collecting nursery uncopied of thread %d\n",
                other->thread_id);
            MVM_gc_collect_free_nursery_uncopied(other, tc->gc_work[i].limit);

            /* Handle exited threads. */
            if (MVM_load(&thread_obj->body.stage) == MVM_thread_stage_exited) {
                /* Don't bother freeing gen2; we'll do it next time */
                MVM_store(&thread_obj->body.stage, MVM_thread_stage_clearing_nursery);
                GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE,
                    "Thread %d run %d : set thread %d clearing nursery stage to %d\n",
                    other->thread_id, (int)MVM_load(&thread_obj->body.stage));
            }

            /* Mark thread free to continue. */
            MVM_cas(&other->gc_status, MVMGCStatus_STOLEN, MVMGCStatus_UNABLE);
            MVM_cas(&other->gc_status, MVMGCStatus_INTERRUPT, MVMGCStatus_NONE);
        }
    }

    /* Signal acknowledgement of completing the cleanup,
     * except for STables, and if we're the final to do
     * so, free the STables, which have been linked. */
    if (MVM_decr(&tc->instance->gc_ack) == 2) {
        /* Set it to zero (we're guaranteed the only ones trying to write to
         * it here). Actual STable free in MVM_gc_enter_from_allocator. */
        MVM_store(&tc->instance->gc_ack, 0);

        /* Also clear in GC flag. */
        uv_mutex_lock(&tc->instance->mutex_gc_orchestrate);
        tc->instance->in_gc = 0;
        uv_cond_broadcast(&tc->instance->cond_blocked_can_continue);
        uv_mutex_unlock(&tc->instance->mutex_gc_orchestrate);
    }
}

/* Called by a thread to indicate it is about to enter a blocking operation.
 * This tells any thread that is coordinating a GC run that this thread will
 * be unable to participate. */
void MVM_gc_mark_thread_blocked(MVMThreadContext *tc) {
    /* This may need more than one attempt. */
    while (1) {
        /* Try to set it from running to unable - the common case. */
        if (MVM_cas(&tc->gc_status, MVMGCStatus_NONE,
                MVMGCStatus_UNABLE) == MVMGCStatus_NONE)
            return;

        /* The only way this can fail is if another thread just decided we're to
         * participate in a GC run. */
        if (MVM_load(&tc->gc_status) == MVMGCStatus_INTERRUPT)
            MVM_gc_enter_from_interrupt(tc);
        else
            MVM_panic(MVM_exitcode_gcorch,
                "Invalid GC status observed while blocking thread; aborting");
    }
}

/* Called by a thread to indicate it has completed a block operation and is
 * thus able to particpate in a GC run again. Note that this case needs some
 * special handling if it comes out of this mode when a GC run is taking place. */
void MVM_gc_mark_thread_unblocked(MVMThreadContext *tc) {
    /* Try to set it from unable to running. */
    while (MVM_cas(&tc->gc_status, MVMGCStatus_UNABLE,
            MVMGCStatus_NONE) != MVMGCStatus_UNABLE) {
        /* We can't, presumably because a GC run is going on. We should wait
         * for that to finish before we go on; try using a condvar for it. */
        uv_mutex_lock(&tc->instance->mutex_gc_orchestrate);
        if (tc->instance->in_gc) {
            uv_cond_wait(&tc->instance->cond_blocked_can_continue,
                &tc->instance->mutex_gc_orchestrate);
            uv_mutex_unlock(&tc->instance->mutex_gc_orchestrate);
        }
        else {
            uv_mutex_unlock(&tc->instance->mutex_gc_orchestrate);
            MVM_platform_thread_yield();
        }
    }
}

/* Checks if a thread has marked itself as blocked. Considers that the GC may
 * have stolen its work and marked it as such also. So what this really
 * answers is, "did this thread mark itself blocked, and since then not mark
 * itself unblocked", which is useful if you need to conditionally unblock
 * or re-block. If the status changes from blocked to stolen or stolen to
 * blocked between checking this and calling unblock, it's safe anyway since
 * these cases are handled in MVM_gc_mark_thread_unblocked. Note that this
 * relies on a thread itself only ever calling block/unblock. */
MVMint32 MVM_gc_is_thread_blocked(MVMThreadContext *tc) {
    AO_t gc_status = MVM_load(&(tc->gc_status));
    return gc_status == MVMGCStatus_UNABLE ||
           gc_status == MVMGCStatus_STOLEN;
}

static MVMint32 is_full_collection(MVMThreadContext *tc) {
    MVMuint64 percent_growth, promoted;
    size_t rss;

    /* If it's below the absolute minimum, quickly return. */
    promoted = (MVMuint64)MVM_load(&tc->instance->gc_promoted_bytes_since_last_full);
    if (promoted < MVM_GC_GEN2_THRESHOLD_MINIMUM)
        return 0;

    /* If we're heap profiling then don't consider the resident set size, as
     * it will be hugely distorted by the profile data we record. */
    if (MVM_profile_heap_profiling(tc))
        return 1;

    /* Otherwise, consider percentage of resident set size. */
    if (uv_resident_set_memory(&rss) < 0 || rss == 0)
        rss = 50 * 1024 * 1024;
    percent_growth = (100 * promoted) / (MVMuint64)rss;

    return percent_growth >= MVM_GC_GEN2_THRESHOLD_PERCENT;
}

static void run_gc(MVMThreadContext *tc, MVMuint8 what_to_do) {
    MVMuint8   gen;
    MVMuint32  i, n;

    unsigned int interval_id;

#if MVM_GC_DEBUG
    if (tc->in_spesh)
        MVM_panic(1, "Must not GC when in the specializer/JIT\n");
#endif

    /* Decide nursery or full collection. */
    gen = tc->instance->gc_full_collect ? MVMGCGenerations_Both : MVMGCGenerations_Nursery;

    if (tc->instance->gc_full_collect) {
        interval_id = MVM_telemetry_interval_start(tc, "start full collection");
    } else {
        interval_id = MVM_telemetry_interval_start(tc, "start minor collection");
    }

    /* Do GC work for ourselves and any work threads. */
    for (i = 0, n = tc->gc_work_count ; i < n; i++) {
        MVMThreadContext *other = tc->gc_work[i].tc;
        tc->gc_work[i].limit = other->nursery_alloc;
        GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : starting collection for thread %d\n",
            other->thread_id);
        other->gc_promoted_bytes = 0;
        MVM_gc_collect(other, (other == tc ? what_to_do : MVMGCWhatToDo_NoInstance), gen);
    }

    /* Wait for everybody to agree we're done. */
    finish_gc(tc, gen, what_to_do == MVMGCWhatToDo_All);

    MVM_telemetry_interval_stop(tc, interval_id, "finished run_gc");
}

/* This is called when the allocator finds it has run out of memory and wants
 * to trigger a GC run. In this case, it's possible (probable, really) that it
 * will need to do that triggering, notifying other running threads that the
 * time has come to GC. */
void MVM_gc_enter_from_allocator(MVMThreadContext *tc) {
    GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : Entered from allocate\n");

    MVM_telemetry_timestamp(tc, "gc_enter_from_allocator");

    /* Try to start the GC run. */
    if (MVM_trycas(&tc->instance->gc_start, 0, 1)) {
        MVMThread *last_starter = NULL;
        MVMuint32 num_threads = 0;

        /* Stash us as the thread to blame for this GC run (used to give it a
         * potential nursery size boost). */
        tc->instance->thread_to_blame_for_gc = tc;

        /* Need to wait for other threads to reset their gc_status. */
        while (MVM_load(&tc->instance->gc_ack)) {
            GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE,
                "Thread %d run %d : waiting for other thread's gc_ack\n");
            MVM_platform_thread_yield();
        }

        /* We are the winner of the GC starting race. This gives us some
         * extra responsibilities as well as doing the usual things.
         * First, increment GC sequence number. */
        MVM_incr(&tc->instance->gc_seq_number);
        GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE,
            "Thread %d run %d : GC thread elected coordinator: starting gc seq %d\n",
            (int)MVM_load(&tc->instance->gc_seq_number));

        /* Decide if it will be a full collection. */
        tc->instance->gc_full_collect = is_full_collection(tc);

        MVM_telemetry_timestamp(tc, "won the gc starting race");

        /* If profiling, record that GC is starting. */
        if (tc->instance->profiling)
            MVM_profiler_log_gc_start(tc, tc->instance->gc_full_collect);

        /* Ensure our stolen list is empty. */
        tc->gc_work_count = 0;

        /* Flag that we didn't agree on this run that all the in-trays are
         * cleared (a responsibility of the co-ordinator. */
        MVM_store(&tc->instance->gc_intrays_clearing, 1);

        /* We'll take care of our own work. */
        add_work(tc, tc);

        /* Find other threads, and signal or steal. Also set in GC flag. */
        uv_mutex_lock(&tc->instance->mutex_threads);
        tc->instance->in_gc = 1;
        num_threads = signal_all(tc, tc->instance->threads);
        uv_mutex_unlock(&tc->instance->mutex_threads);

        /* Bump the thread count and signal any threads waiting for that. */
        uv_mutex_lock(&tc->instance->mutex_gc_orchestrate);
        MVM_add(&tc->instance->gc_start, num_threads);
        uv_cond_broadcast(&tc->instance->cond_gc_start);
        uv_mutex_unlock(&tc->instance->mutex_gc_orchestrate);

        /* If there's an event loop thread, wake it up to participate. */
        if (tc->instance->event_loop_wakeup)
            uv_async_send(tc->instance->event_loop_wakeup);

        /* Wait for other threads to be ready. */
        uv_mutex_lock(&tc->instance->mutex_gc_orchestrate);
        while (MVM_load(&tc->instance->gc_start) > 1)
            uv_cond_wait(&tc->instance->cond_gc_start, &tc->instance->mutex_gc_orchestrate);
        uv_mutex_unlock(&tc->instance->mutex_gc_orchestrate);

        /* Sanity check finish votes. */
        if (MVM_load(&tc->instance->gc_finish) != 0)
            MVM_panic(MVM_exitcode_gcorch, "Finish votes was %"MVM_PRSz"\n",
                MVM_load(&tc->instance->gc_finish));

        /* gc_ack gets an extra so the final acknowledger
         * can also free the STables. */
        MVM_store(&tc->instance->gc_finish, num_threads + 1);
        MVM_store(&tc->instance->gc_ack, num_threads + 2);
        GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : finish votes is %d\n",
            (int)MVM_load(&tc->instance->gc_finish));

        /* Now we're ready to start, zero promoted since last full collection
         * counter if this is a full collect. */
        if (tc->instance->gc_full_collect)
            MVM_store(&tc->instance->gc_promoted_bytes_since_last_full, 0);

        /* This is a safe point for us to free any STables that have been marked
         * for deletion in the previous collection (since we let finalization -
         * which appends to this list - happen after we set threads on their
         * way again, it's not safe to do it in the previous collection). */
        GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : Freeing STables if needed\n");
        MVM_gc_collect_free_stables(tc);

        /* Signal to the rest to start */
        GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : coordinator signalling start\n");
        uv_mutex_lock(&tc->instance->mutex_gc_orchestrate);
        if (MVM_decr(&tc->instance->gc_start) != 1)
            MVM_panic(MVM_exitcode_gcorch, "Start votes was %"MVM_PRSz"\n", MVM_load(&tc->instance->gc_start));
        uv_cond_broadcast(&tc->instance->cond_gc_start);
        uv_mutex_unlock(&tc->instance->mutex_gc_orchestrate);

        /* Start collecting. */
        GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : coordinator entering run_gc\n");
        run_gc(tc, MVMGCWhatToDo_All);

        /* If profiling, record that GC is over. */
        if (tc->instance->profiling)
            MVM_profiler_log_gc_end(tc);

        MVM_telemetry_timestamp(tc, "gc finished");

        GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : GC complete (cooridnator)\n");
    }
    else {
        /* Another thread beat us to starting the GC sync process. Thus, act as
         * if we were interrupted to GC. */
        GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : Lost coordinator election\n");
        MVM_gc_enter_from_interrupt(tc);
    }
}

/* This is called when a thread hits an interrupt at a GC safe point. This means
 * that another thread is already trying to start a GC run, so we don't need to
 * try and do that, just enlist in the run. */
void MVM_gc_enter_from_interrupt(MVMThreadContext *tc) {
    AO_t curr;

    GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : Entered from interrupt\n");

    MVM_telemetry_timestamp(tc, "gc_enter_from_interrupt");

    /* If profiling, record that GC is starting. */
    if (tc->instance->profiling)
        MVM_profiler_log_gc_start(tc, is_full_collection(tc));

    /* We'll certainly take care of our own work. */
    tc->gc_work_count = 0;
    add_work(tc, tc);

    /* Indicate that we're ready to GC. Only want to decrement it if it's 2 or
     * greater (0 should never happen; 1 means the coordinator is still counting
     * up how many threads will join in, so we should wait until it decides to
     * decrement.) */
    uv_mutex_lock(&tc->instance->mutex_gc_orchestrate);
    while (MVM_load(&tc->instance->gc_start) < 2)
        uv_cond_wait(&tc->instance->cond_gc_start, &tc->instance->mutex_gc_orchestrate);
    MVM_decr(&tc->instance->gc_start);
    uv_cond_broadcast(&tc->instance->cond_gc_start);
    uv_mutex_unlock(&tc->instance->mutex_gc_orchestrate);

    /* Wait for all threads to indicate readiness to collect. */
    GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : Waiting for other threads\n");
    uv_mutex_lock(&tc->instance->mutex_gc_orchestrate);
    while (MVM_load(&tc->instance->gc_start))
        uv_cond_wait(&tc->instance->cond_gc_start, &tc->instance->mutex_gc_orchestrate);
    uv_mutex_unlock(&tc->instance->mutex_gc_orchestrate);

    GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : Entering run_gc\n");
    run_gc(tc, MVMGCWhatToDo_NoInstance);
    GCDEBUG_LOG(tc, MVM_GC_DEBUG_ORCHESTRATE, "Thread %d run %d : GC complete\n");

    /* If profiling, record that GC is over. */
    if (tc->instance->profiling)
        MVM_profiler_log_gc_end(tc);
}

/* Run the global destruction phase. */
void MVM_gc_global_destruction(MVMThreadContext *tc) {
    char *nursery_tmp;

    /* Fake a nursery collection run by swapping the semi-
     * space nurseries. */
    nursery_tmp = tc->nursery_fromspace;
    tc->nursery_fromspace = tc->nursery_tospace;
    tc->nursery_tospace = nursery_tmp;

    /* Run the objects' finalizers */
    MVM_gc_collect_free_nursery_uncopied(tc, tc->nursery_alloc);
    MVM_gc_root_gen2_cleanup(tc);
    MVM_gc_collect_free_gen2_unmarked(tc, 1);
    MVM_gc_collect_free_stables(tc);
}
