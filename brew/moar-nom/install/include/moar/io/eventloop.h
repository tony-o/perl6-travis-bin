/* Operations table for a certain type of asynchronous task that can be run on
 * the event loop. */
struct MVMAsyncTaskOps {
    /* How to set work up on the event loop. */
    void (*setup) (MVMThreadContext *tc, uv_loop_t *loop, MVMObject *async_task, void *data);

    /* How to cancel, if possible. */
    void (*cancel) (MVMThreadContext *tc, uv_loop_t *loop, MVMObject *async_task, void *data);

    /* How to mark the task's data, if needed. */
    void (*gc_mark) (MVMThreadContext *tc, void *data, MVMGCWorklist *worklist);

    /* How to free the task's data, if needed. */
    void (*gc_free) (MVMThreadContext *tc, MVMObject *t, void *data);
};

void MVM_io_eventloop_queue_work(MVMThreadContext *tc, MVMObject *work);
void MVM_io_eventloop_cancel_work(MVMThreadContext *tc, MVMObject *task_obj);
