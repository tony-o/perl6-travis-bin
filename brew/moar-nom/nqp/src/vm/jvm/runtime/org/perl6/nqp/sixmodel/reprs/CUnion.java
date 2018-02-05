package org.perl6.nqp.sixmodel.reprs;

import java.util.List;
import java.util.ArrayList;

import com.sun.jna.Union;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.FieldVisitor;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;

import org.perl6.nqp.sixmodel.REPR;
import org.perl6.nqp.sixmodel.SerializationReader;
import org.perl6.nqp.sixmodel.SixModelObject;
import org.perl6.nqp.sixmodel.STable;
import org.perl6.nqp.sixmodel.StorageSpec;
import org.perl6.nqp.sixmodel.TypeObject;

import org.perl6.nqp.sixmodel.reprs.CUnionREPRData.AttrInfo;
import org.perl6.nqp.sixmodel.reprs.NativeCall.ArgType;

import org.perl6.nqp.runtime.ExceptionHandling;
import org.perl6.nqp.runtime.ThreadContext;

public class CUnion extends REPR {
    public SixModelObject type_object_for(ThreadContext tc, SixModelObject HOW) {
        STable st = new STable(this, HOW);
        SixModelObject obj = new TypeObject();
        obj.st = st;
        st.WHAT = obj;
        return st.WHAT;
    }

    public void compose(ThreadContext tc, STable st, SixModelObject repr_info_hash) {
        SixModelObject repr_info = repr_info_hash.at_key_boxed(tc, "attribute");
        CUnionREPRData repr_data = new CUnionREPRData();

        long mroLength = repr_info.elems(tc);
        List<AttrInfo> attrInfos = new ArrayList<AttrInfo>();
        for (long i = mroLength - 1; i >= 0; i--) {
            SixModelObject entry = repr_info.at_pos_boxed(tc, i);
            SixModelObject attrs = entry.at_pos_boxed(tc, 1);
            long parents = entry.at_pos_boxed(tc, 2).elems(tc);

            if (parents <= 1) {
                long numAttrs = attrs.elems(tc);
                for (long j = 0; j < numAttrs; j++) {
                    SixModelObject attrHash = attrs.at_pos_boxed(tc, j);
                    AttrInfo info    = new AttrInfo();
                    info.name        = attrHash.at_key_boxed(tc, "name").get_str(tc);
                    info.type        = attrHash.at_key_boxed(tc, "type");
                    info.inlined     = (short)attrHash.at_key_boxed(tc, "inlined").get_int(tc);
                    StorageSpec spec = info.type.st.REPR.get_storage_spec(tc, info.type.st);
                    info.bits        = spec.bits;
                    repr_data.fieldTypes.put(info.name, info);

                    if (info.type == null) {
                        ExceptionHandling.dieInternal(tc, "CUnion representation requires the types of all attributes to be specified");
                    }

                    attrInfos.add(info);
                }
            }
            else {
                ExceptionHandling.dieInternal(tc, "CUnion representation does not support multiple inheritance");
            }
        }

        /* XXX: We could generate the structure class lazily the first time we
         * allocate an object, rather than upfront. Not sure if that's
         * necessary though. */
        st.REPRData = repr_data;
        generateStructClass(tc, st, attrInfos);
    }

    public SixModelObject allocate(ThreadContext tc, STable st) {
        /* TODO: Die if someone tries to allocate a CUnion before it's been
         * composed. */
        CUnionInstance obj = new CUnionInstance();
        CUnionREPRData repr_data = (CUnionREPRData) st.REPRData;
        obj.st = st;
        try {
            obj.storage = (Union) repr_data.structureClass.newInstance();
        }
        catch (InstantiationException | IllegalAccessException e) {
            e.printStackTrace();
            throw new RuntimeException(e);
        }
        return obj;
    }

    public SixModelObject deserialize_stub(ThreadContext tc, STable st) {
        /* This REPR can't be serialized. */
        ExceptionHandling.dieInternal(tc, "Can't deserialize_stub a CUnion object.");

        return null;
    }

    public void deserialize_finish(ThreadContext tc, STable st, SerializationReader reader, SixModelObject obj) {
        ExceptionHandling.dieInternal(tc, "Can't deserialize_finish a CUnion object.");
    }

    privat