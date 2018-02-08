package org.perl6.nqp.sixmodel.reprs;

import java.util.concurrent.locks.ReentrantLock;

import org.perl6.nqp.runtime.ExceptionHandling;
import org.perl6.nqp.runtime.ThreadContext;
import org.perl6.nqp.sixmodel.REPR;
import org.perl6.nqp.sixmodel.STable;
import org.perl6.nqp.sixmodel.SerializationReader;
import org.perl6.nqp.sixmodel.SerializationWriter;
import org.perl6.nqp.sixmodel.SixModelObject;
import org.perl6.nqp.sixmodel.TypeObject;

public class ReentrantMutex extends REPR {
    public SixModelObject type_object_for(ThreadContext tc, SixModelObject HOW) {
        STable st = new STable(this, HOW);
        SixModelObject obj = new TypeObject();
        obj.st = st;
        st.WHAT = obj;
        return st.WHAT;
    }

    public SixModelObject allocate(ThreadContext tc, STable st) {
        ReentrantMutexInstance obj = new ReentrantMutexInstance();
        obj.st = st;
        obj.lock = new ReentrantLock();
        return obj;
    }

    public SixModelObject deserialize_stub(ThreadContext tc, STable st) {
        return allocate(tc, st);
    }

    public void deserialize_finish(ThreadContext tc, STable st,
            SerializationReader reader, SixModelObject obj) {
        /* Already did it all in deserialize_stub. */
    }

    public void serialize(ThreadContext tc, SerializationWriter writer, SixModelObject obj) {
        /* Nothing to do, we just re-create the lock on deserialization. */
    }
}
