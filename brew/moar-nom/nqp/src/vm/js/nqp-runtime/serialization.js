'use strict';
const Hash = require('./hash.js');
const CodeRef = require('./code-ref.js');
const NQPInt = require('./nqp-int.js');
const Int64 = require('node-int64');
const nullStr = require('./null_s.js');
const Null = require('./null.js');

const NQPException = require('./nqp-exception.js');

const StaticCtx = require('./static-ctx.js');

const repossession = require('./repossession.js');

const BOOT = require('./BOOT.js');

const constants = require('./constants.js');

const op = {};
exports.op = op;

const CURRENT_VERSION = 20;
const OBJECTS_TABLE_ENTRY_SC_IDX_MAX = 0x000FFFFF;
const OBJECTS_TABLE_ENTRY_SC_MAX = 0x7FE;
const OBJECTS_TABLE_ENTRY_SC_SHIFT = 20;
const OBJECTS_TABLE_ENTRY_SC_OVERFLOW = 0x7FF;
const OBJECTS_TABLE_ENTRY_IS_CONCRETE = 0x80000000;

const STRING_HEAP_LOC_MAX = 0x7FFFFFFF;
const STRING_HEAP_LOC_PACKED_MAX = 0x00007FFF;
const STRING_HEAP_LOC_PACKED_OVERFLOW = 0x00008000;
const STRING_HEAP_LOC_PACKED_LOW_MASK = 0x0000FFFF;
const STRING_HEAP_LOC_PACKED_SHIFT = 16;

/* Possible reference types we can serialize. */
const REFVAR_NULL = 1;
const REFVAR_OBJECT = 2;
const REFVAR_VM_NULL = 3;
const REFVAR_VM_INT = 4;
const REFVAR_VM_NUM = 5;
const REFVAR_VM_STR = 6;
const REFVAR_VM_ARR_VAR = 7;
const REFVAR_VM_ARR_STR = 8;
const REFVAR_VM_ARR_INT = 9;
const REFVAR_VM_HASH_STR_VAR = 10;
const REFVAR_STATIC_CODEREF = 11;
const REFVAR_CLONED_CODEREF = 12;

const STABLE_HAS_CONTAINER_SPEC = 0x10;
const STABLE_HAS_INVOCATION_SPEC = 0x20;
const STABLE_HAS_HLL_OWNER = 0x40;
const STABLE_HAS_HLL_ROLE = 0x80;

const PACKED_SC_IDX_MASK = 0x000FFFFF;
const PACKED_SC_MAX = 0xFFE;
const PACKED_SC_IDX_MAX = 0x000FFFFF;
const PACKED_SC_SHIFT = 20;
const PACKED_SC_OVERFLOW = 0xFFF;

class BinaryWriteCursor {
  constructor(writer) {
    this.buffer = new Buffer(1024);
    this.writer = writer;
    this.offset = 0;
    this.metadata = [];
    this.currentMetadata = [];
  }

  growToHold(space) {
    if (this.offset + space > this.buffer.length) {
      const old = this.buffer;
      this.buffer = new Buffer(old.length * 2);
      old.copy(this.buffer);
    }
  }

  str(str) {
    if (str === undefined) {
      console.trace('undefined string');
      str = '?';
    }

    const heapLoc = this.writer.stringIndex(str);

    if (!(heapLoc >= 0 && heapLoc <= STRING_HEAP_LOC_MAX)) {
      throw `Serialization error: string offset ${heapLoc} can't be serialized`;
    }

    if (heapLoc <= STRING_HEAP_LOC_PACKED_MAX) {
      this.uint16(heapLoc);
    } else {
      this.uint16((heapLoc >> STRING_HEAP_LOC_PACKED_SHIFT) |
          STRING_HEAP_LOC_PACKED_OVERFLOW);
      this.uint16(heapLoc & STRING_HEAP_LOC_PACKED_LOW_MASK);
    }
  }


  /** Write a C String
   * @param {string} string
   */
  cstr(string) {
    if (string === undefined) {
      this.varint(0);
    } else {
      const expected = Buffer.byteLength(string);
      this.varint(expected);
      this.growToHold(expected);
      const wrote = this.buffer.write(string, this.offset);
      if (expected != wrote) {
        throw 'Problem with writing cstr, wrote: ' + wrote + ', expected to write:' + expected;
      }
      this.offset += wrote;
    }
  }

  str32(str) {
    this.int32(this.writer.stringIndex(str));
  }

  /* Writing function for variable sized integers. Writes out a 32 bit value
     using between 1 and 5 bytes. */
  varint(value) {
    let storageNeeded;

    if (value >= -1 && value <= 126) {
      storageNeeded = 1;
    } else {
      const absVal = value < 0 ? -value - 1 : value;

      if (absVal <= 0x7FF) {
        storageNeeded = 2;
      } else if (absVal <= 0x000000000007FFFF) {
        storageNeeded = 3;
      } else if (absVal <= 0x0000000007FFFFFF) {
        storageNeeded = 4;
      } else if (absVal <= 0x00000007FFFFFFFF) {
        storageNeeded = 5;
      }
    }

    if (storageNeeded == 1) {
      this.uint8(0x80 | (value + 129));
    } else {
      const rest = storageNeeded - 1;
      const nybble = rest == 4 ? 0 : value >> 8 * rest;

      /* All the other high bits should be the same as the top bit of the
             nybble we keep. Or we have a bug.  */

      console.assert((nybble >> 3) == 0 || (nybble >> 3) == ~0);

      this.int8((rest << 4) | (nybble & 0xF));

      const tmp = new Int64(value).toBuffer();
      this.growToHold(rest);
      for (let i = 0; i < rest; i++) {
        this.buffer[this.offset + i] = tmp[8 - (i + 1)];
      }

      this.offset += rest;
    }
  }


  int8(value) {
    this.growToHold(1);
    this.buffer.writeInt8(value, this.offset);
    this.offset += 1;
  }

  uint8(value) {
    this.growToHold(1);
    this.buffer.writeUInt8(value, this.offset);
    this.offset += 1;
  }

  uint16(value) {
    this.growToHold(2);
    this.buffer.writeUInt16LE(value, this.offset);
    this.offset += 2;
  }


  int32(value) {
    this.growToHold(4);
    this.buffer.writeInt32LE(value, this.offset);
    this.offset += 4;
  }

  int16(value) {
    this.growToHold(2);
    this.buffer.writeInt16LE(value, this.offset);
    this.offset += 2;
  }

  double(value) {
    this.growToHold(8);
    this.buffer.writeDoubleLE(value, this.offset);
    this.offset += 8;
  }


  /* Writes the ID, index pair that identifies an entry in a Serialization
     context. */
  idIdx(scId, idx) {
    if (scId <= PACKED_SC_MAX && idx <= PACKED_SC_IDX_MAX) {
      const packed = (scId << PACKED_SC_SHIFT) | (idx & PACKED_SC_IDX_MASK);
      this.varint(packed);
    } else {
      const packed = PACKED_SC_OVERFLOW << PACKED_SC_SHIFT;

      this.varint(packed);
      this.varint(scId);
      this.varint(idx);
    }
  }

  objRef(ref) {
    const writerSc = this.writer.sc;
    if (!ref._STable) {
      throw new NQPException('trying to serialize an object without an STable');
    }
    if (!ref._SC) {
      /* This object doesn't belong to an SC yet, so it must be serialized
       * as part of this compilation unit. Add it to the work list. */
      ref._SC = writerSc;

      this.writer.sc.rootObjects.push(ref);
    }

    /* Write SC index, then object index. */
    this.idIdx(this.writer.getSCId(ref._SC), ref._SC.rootObjects.indexOf(ref));
  }

  stableRef(STable) {
    const ref = this.writer.getSTableRefInfo(STable);
    this.idIdx(ref[0], ref[1]);
  }

  ref(ref) {
    /* Work out what kind of thing we have and determine the discriminator. */
    let discrim = 0;

    if (ref == Null) {
      discrim = REFVAR_VM_NULL;
    } else if (ref === undefined) {
      discrim = REFVAR_NULL;
    } else if (ref.$$serializeAsNull) {
      discrim = REFVAR_VM_NULL;
    } else if (ref instanceof NQPInt) {
      discrim = REFVAR_VM_INT;
    } else if (typeof ref == 'number') {
      discrim = REFVAR_VM_NUM;
    } else if (typeof ref == 'string') {
      discrim = REFVAR_VM_STR;
    } else if (ref._STable === BOOT.Array._STable) {
      discrim = REFVAR_VM_ARR_VAR;
    } else if (ref instanceof Hash) {
      discrim = REFVAR_VM_HASH_STR_VAR;
    } else if (ref instanceof CodeRef || typeof ref == 'function') {
      discrim = REFVAR_VM_NULL;
      if (ref._SC && ref.isStatic) {
        /* Static code reference. */
        discrim = REFVAR_STATIC_CODEREF;
      } else if (ref._SC) {
        /* Closure, but already seen and serialization already handled. */
        discrim = REFVAR_CLONED_CODEREF;
      } else {
        /* Closure but didn't see it yet. Take care of it serialization, which
             * gets it marked with this SC. Then it's just a normal code ref that
             * needs serializing. */
        this.writer.serializeClosure(ref);
        discrim = REFVAR_CLONED_CODEREF;
      }
    } else {
      /* Just a normal object, with no special serialization needs. */
      discrim = REFVAR_OBJECT;
    }


    this.int8(discrim);

    /* Now take appropriate action. */
    switch (discrim) {
      case REFVAR_NULL:
      case REFVAR_VM_NULL:
        /* Nothing to do for these. */
        break;
      case REFVAR_OBJECT:
        this.objRef(ref);
        break;
      case REFVAR_VM_INT:
        this.varint(ref.value);
        break;
      case REFVAR_VM_NUM:
        this.double(ref);
        break;
      case REFVAR_VM_STR:
        this.str(ref);
        break;
      case REFVAR_VM_ARR_INT:
      case REFVAR_VM_ARR_STR:
      case REFVAR_VM_ARR_VAR:
        ref._STable.REPR.serialize(this, ref);
        break;
      case REFVAR_VM_HASH_STR_VAR:
        this.varint(ref.$$elems());
        ref.content.forEach(function(value, key, map) {
          this.str(key);
          this.ref(value);
        }, this);
        break;
      case REFVAR_STATIC_CODEREF:
      case REFVAR_CLONED_CODEREF:
        const scId = this.writer.getSCId(ref._SC);
        const idx = ref._SC.rootCodes.indexOf(ref);
        if (idx == -1) {
          throw `can't write code ref`;
        }
        this.idIdx(scId, idx);
        break;
      default:
        throw `Serialization Error: Unimplemented object type: ${discrim}`;
    }
  }
};

const HEADER_SIZE = 4 * 18;

class SerializationWriter {
  constructor(sc, sh) {
    this.dependentSCs = [];
    this.contexts = [];
    this.sc = sc;
    this.sh = sh;
    this.sh.push(nullStr);
    this.numClosures = 0;

    this.stables = new BinaryWriteCursor(this);
    this.stablesData = new BinaryWriteCursor(this);
    this.objects = new BinaryWriteCursor(this);
    this.objectsData = new BinaryWriteCursor(this);
    this.deps = new BinaryWriteCursor(this);
    this.closures = new BinaryWriteCursor(this);
    this.contextsHeaders = new BinaryWriteCursor(this);
    this.contextsData = new BinaryWriteCursor(this);
    this.repossessionsData = new BinaryWriteCursor(this);
  }

  serializeObject(obj) {
    /* Get index of SC that holds the STable and its index. */
    if (!obj._STable) {
      throw new NQPException(`Can't serialize an object without an STable`);
    }
    const ref = this.getSTableRefInfo(obj._STable);
    const sc = ref[0];
    const scIdx = ref[1];

    let packed = !obj.typeObject_ ? OBJECTS_TABLE_ENTRY_IS_CONCRETE : 0;

    if (sc <= OBJECTS_TABLE_ENTRY_SC_MAX && scIdx <= OBJECTS_TABLE_ENTRY_SC_IDX_MAX) {
      packed |= (sc << OBJECTS_TABLE_ENTRY_SC_SHIFT) | scIdx;
    } else {
      packed |= OBJECTS_TABLE_ENTRY_SC_OVERFLOW << OBJECTS_TABLE_ENTRY_SC_SHIFT;
      this.objectsData.int32(sc);
      this.objectsData.int32(scIdx);
    }

    /* Make objects table entry. */

    this.objects.int32(packed);
    this.objects.int32(this.objectsData.offset);


    /* Delegate to its serialization REPR function. */
    if (!obj.typeObject_) {
      if (!obj._STable.REPR.serialize) {
        console.trace(`don't know how to serialize ${obj._STable.REPR.name}`);
      } else {
        obj._STable.REPR.serialize(this.objectsData, obj);
      }
    }
  }

  /* This handles the serialization of an STable, and calls off to serialize
   * its representation data also. */

  serializeSTable(st) {
    /* Make STables table entry. */
    this.stables.str32(st.REPR.constructor.name);
    this.stables.int32(this.stablesData.offset);

    /* Write HOW, WHAT and WHO. */

    if (!st.HOW) {
      console.log('!.HOW', st, st.HOW);
    }

    /* TODO lazy HOW loading */

    this.stablesData.objRef(st.HOW);
    this.stablesData.objRef(st.WHAT);
    this.stablesData.ref(st.WHO || Null);

    /* Method cache*/
    if (st.methodCache) {
      const hash = new Hash();
      hash.content = st.methodCache;
      this.stablesData.ref(hash);
    } else {
      this.stablesData.ref(Null);
    }

    /* Type check cache. */
    const tcl = !st.typeCheckCache ? 0 : st.typeCheckCache.length;
    this.stablesData.varint(tcl);
    for (let i = 0; i < tcl; i++) {
      this.stablesData.ref(st.typeCheckCache[i]);
    }


    /* Mode flags.
       These are stored as MVMuint16, but currently only the bottom 6 bits are
       used. Whilst we could store these as 2 bytes, we don't actually gain any
       future-proofing from that, because if we start assigning meaning to
       currently unused bits, then we have to bump the serialisation version
       *anyway*. If we didn't, older readers would encounter files with the
       newly used bits, but ignore whatever semantics those bits were meant to
       convey, and hence show buggy behaviour. And if we're bumping the
       serialisation version, then we can increase the storage size.  */

    this.stablesData.uint8(st.modeFlags);

    /* Boolification spec. */
    /* As this only needs 4 bits, also use the same byte to store various
       NULL/not-NULL flag bits. */

    let flags;

    if (st.boolificationSpec) {
      const mode = st.boolificationSpec.mode;
      if (mode >= 0xF) {
        throw `Serialization error: boolification spec mode ${mode} out of range and can't be serialized`;
      }
      flags = mode;
    } else {
      flags = 0xF;
    }

    if (st.containerSpec) {
      flags |= STABLE_HAS_CONTAINER_SPEC;
    }

    if (st.invocationSpec) {
      flags |= STABLE_HAS_INVOCATION_SPEC;
    }

    if (st.hllOwner) {
      flags |= STABLE_HAS_HLL_OWNER;
    }

    if (st.hllRole) {
      flags |= STABLE_HAS_HLL_ROLE;
    }

    this.stablesData.uint8(flags);

    /* Boolification spec. */

    if (st.boolificationSpec) {
      this.stablesData.ref(st.boolificationSpec.method);
    }

    if (st.containerSpec) {
      /* Write container spec name. */
      this.stablesData.str(st.containerSpec.name);

      /* Give container spec a chance to serialize any data it wishes. */
      st.containerSpec.serialize(this.stablesData);
    }

    /* Invocation spec. */

    if (st.invocationSpec) {
      /* cached stuff from the MoarVM backend is just ignored */
      this.stablesData.ref(st.invocationSpec.classHandle);
      this.stablesData.str(st.invocationSpec.attrName);
      this.stablesData.varint(0); // hint
      this.stablesData.ref(st.invocationSpec.invocationHandler);
      this.stablesData.ref(Null); // md_class_handle
      this.stablesData.str(''); // md_cache_attr_name
      this.stablesData.varint(0); // md_cache_hint
      this.stablesData.str(''); // md_valid_attr_name
      this.stablesData.varint(0); // md_valid_hint
    }

    /* HLL info. */

    if (st.hllOwner) {
      this.stablesData.str(st.hllOwner.get('name'));
    }

    if (st.hllRole) {
      this.stablesData.varint(st.hllRole);
    }

    /* If it's a parametric type, save parameterizer. */
    if (st.modeFlags & constants.PARAMETRIC_TYPE) {
      this.stablesData.ref(st.parameterizer);
    }

    if (st.modeFlags & constants.PARAMETERIZED_TYPE) {
      /* TODO - figure out the intern table */
      this.stablesData.ref(st.parametricType);
      const params = st.parameters.array;
      this.stablesData.varint(params.length);
      for (let i = 0; i < params.length; i++) {
        this.stablesData.ref(params[i]);
      }
    }

    this.stablesData.cstr(st.debugName);

    /* Location of REPR data. */
    this.stables.int32(this.stablesData.offset);

    /* If the REPR has a function to serialize representation data, call it. */
    if (st.REPR.serializeReprData) {
      st.REPR.serializeReprData(st, this.stablesData);
    }
  }


  serializeContext(ctx) {
    /* Locate the static code ref this context points to. */
    const staticCodeRef = this.closureToStaticCodeRef(ctx.codeRef(), true);
    const staticCodeSC = staticCodeRef._SC;
    if (staticCodeSC == null) {
      throw 'Serialization Error: closure outer is a code object not in an SC';
    }
    const staticSCId = this.getSCId(staticCodeSC);
    const staticIdx = staticCodeSC.getCodeIndex(staticCodeRef);


    /* Make contexts table entry. */
    this.contextsHeaders.int32(staticSCId);
    this.contextsHeaders.int32(staticIdx);
    this.contextsHeaders.int32(this.contextsData.offset);

    /* See if there's any relevant outer context, and if so set it up to
     * be serialized. */
    if (ctx.$$outer != null) {
      this.contextsHeaders.int32(this.getSerializedContextIdx(ctx.$$outer));
    } else {
      this.contextsHeaders.int32(0);
    }


    const lexicalsTypeInfo = staticCodeRef.lexicalsTypeInfo;

    let lexicals = 0;

    for (const name in lexicalsTypeInfo) lexicals++; // eslint-disable-line no-unused-vars

    this.contextsData.varint(lexicals);

    for (const name in lexicalsTypeInfo) {
      this.contextsData.str(name);
      switch (lexicalsTypeInfo[name]) {
        case 0: // obj
          this.contextsData.ref(ctx[name]);
          break;
        case 1: // int
          this.contextsData.varint(ctx[name]);
          break;
        case 2: // num
          this.contextsData.double(ctx[name]);
          break;
        case 3: // str
          this.contextsData.str(ctx[name]);
      }
    }
  }

  getSerializedContextIdx(ctx) {
    if (!ctx._SC) {
      /* TODO - think if it's truly correct */

      if (ctx instanceof StaticCtx) {
        /* A StaticCtx is used when a closure is invoke when it's outer doesn't exist yet */
        return 0;
      }

      /* Make sure we should chase a level down. */
      if (this.closureToStaticCodeRef(ctx.codeRef(), false) == null) {
        return 0;
      } else {
        this.contexts.push(ctx);
        ctx._SC = this.sc;
        return this.contexts.length;
      }
    } else {
      if (ctx._SC != this.sc) {
        throw 'Serialization Error: reference to context outside of SC';
      }
      const idx = this.contexts.indexOf(ctx);
      if (idx < 0) {
        throw 'Serialization Error: could not locate outer context in current SC';
      }
      return idx + 1;
    }
  }

  getSerializedOuterContextIdx(closure) {
    if (closure.isCompilerStub) {
      return 0;
    }
    if (closure.outerCtx == null) {
      return 0;
    }
    return this.getSerializedContextIdx(closure.outerCtx);
  }

  closureToStaticCodeRef(closure, fatal) {
    const staticCode = closure.staticCode;
    if (!staticCode) {
      if (fatal) {
        throw 'Serialization Error: missing static code ref for closure';
      } else {
        return null;
      }
    }

    if (!staticCode._SC) {
      if (fatal) {
        throw 'Serialization Error: could not locate static code ref for closure';
      } else {
        return null;
      }
    }

    return staticCode;
  }

  serializeClosure(closure) {
    /* Locate the static code object. */
    const staticCodeRef = this.closureToStaticCodeRef(closure, true);
    const staticCodeSC = staticCodeRef._SC;

    /* Add an entry to the closures table. */
    const staticSCId = this.getSCId(staticCodeSC);
    const staticIdx = staticCodeSC.getCodeIndex(staticCodeRef);

    /* Get the index of the context (which will add it to the todo list if
     * needed). */
    const contextIdx = this.getSerializedOuterContextIdx(closure);

    this.closures.int32(staticSCId);
    this.closures.int32(staticIdx);
    this.closures.int32(contextIdx);


    /* Check if it has a static code object. */
    if (closure.codeObj) {
      const codeObj = closure.codeObj;
      this.closures.int32(1);

      if (!codeObj._SC) {
      }
      if (!codeObj._SC) {
        codeObj._SC = this.sc;
        this.writer.sc.rootObjects.push(ref);
      }

      this.closures.int32(this.getSCId(codeObj._SC));
      this.closures.int32(codeObj._SC.rootObjects.indexOf(codeObj));
    } else {
      this.closures.int32(0);
      this.closures.int32(0);
      this.closures.int32(0);
    }

    /* Increment count of closures in the table. */
    this.numClosures++;


    /* Add the closure to this SC, and mark it as as being in it. */
    closure._SC = this.sc;
    this.sc.rootCodes.push(closure);
  }

  /* This is the overall serialization loop. It keeps an index into the list of
   * STables and objects in the SC. As we discover new ones, they get added. We
   * finished when we've serialized everything. */
  serializationLoop() {
    let workTodo = true;

    let sTablesListPos = 0;
    let objectsListPos = 0;
    let contextsListPos = 0;
    while (workTodo) {
      /* Current work list sizes. */
      const sTablesTodo = this.sc.rootSTables.length;
      const objectsTodo = this.sc.rootObjects.length;
      const contextsTodo = this.contexts.length;

      /* Reset todo flag - if we do some work we'll go round again as it
         * may have generated more. */
      workTodo = false;

      /* Serialize any STables on the todo list. */
      while (sTablesListPos < sTablesTodo) {
        this.serializeSTable(this.sc.rootSTables[sTablesListPos]);
        sTablesListPos++;
        workTodo = true;
      }

      /* Serialize any objects on the todo list. */
      while (objectsListPos < objectsTodo) {
        this.serializeObject(this.sc.rootObjects[objectsListPos]);
        objectsListPos++;
        workTodo = true;
      }

      /* Serialize any contexts on the todo list. */
      while (contextsListPos < contextsTodo) {
        this.serializeContext(this.contexts[contextsListPos]);
        contextsListPos++;
        workTodo = true;
      }
    }

    /* Finally, serialize repossessions table (this can't make any more
     * work, so is done as a separate step here at the end). */
    this.serializeRepossessions();
  }


  serializeRepossessions() {
    /* Make entries. */
    for (let i = 0; i < this.sc.repIndexes.length; i++) {
      const objIdx = this.sc.repIndexes[i] >> 1;
      const isST = this.sc.repIndexes[i] & 1;

      const origSC = this.sc.repScs[i];

      /* Work out original object's SC location. */
      const origSCIdx = this.getSCId(origSC);
      const origIdx = isST != 0 ?
          origSC.rootSTables.indexOf(this.sc.rootSTables[objIdx]) :
          origSC.rootObjects.indexOf(this.sc.rootObjects[objIdx]);

      if (origIdx < 0) {
        throw 'Could not find object when writing repossessions; ' +
          (isST != 0 ? 'STable' : 'REPR = ' + this.sc.rootObjects[objIdx]._STable.REPR.name);
      }

      /* Write table row. */
      this.repossessionsData.int32(isST);
      this.repossessionsData.int32(objIdx);
      this.repossessionsData.int32(origSCIdx);
      this.repossessionsData.int32(origIdx);
    }
  }


  stringIndex(str) {
    /* The first entry in the heap represents the null string */
    let idx = this.sh.indexOf(str);
    if (idx == -1) {
      this.sh.push(str);
      idx = this.sh.length - 1;
    }
    return idx;
  }


  headerInt32(value) {
    this.buffer.writeUInt32LE(value, this.headerOffset);
    this.headerOffset += 4;
  }

  writeChunk(cursor) {
    if (cursor.offset) {
      cursor.buffer.copy(this.buffer, this.offset, 0, cursor.offset);
    }
    this.offset += cursor.offset;
  }

  getSCId(sc) {
    if (!sc) {
      throw new NQPException('getting an id of an undefined SC');
    }

    /* Easy if it's in the current SC. */
    if (sc == this.sc) {
      return 0;
    }

    /* If not, try to find it in our dependencies list. */
    const found = this.dependentSCs.indexOf(sc);
    if (found >= 0) {
      return found + 1;
    }

    /* Otherwise, need to add it to our dependencies list. */
    this.dependentSCs.push(sc);

    this.deps.str32(sc.handle);
    this.deps.str32(sc.description);

    return this.dependentSCs.length; /* Deliberately index + 1. */
  }

  getSTableRefInfo(st) {
    /* Add to this SC if needed. */
    if (!st._SC) {
      st._SC = this.sc;
      this.sc.rootSTables.push(st);
    }

    return [this.getSCId(st._SC), st._SC.rootSTables.indexOf(st)];
  }


  concatenateOutputs() {
    let outputSize = 0;

    this.headerOffset = 0;
    this.offset = HEADER_SIZE;

    /* Calculate total size. */
    outputSize += HEADER_SIZE;
    outputSize += this.stables.offset;
    outputSize += this.stablesData.offset;
    outputSize += this.objects.offset;
    outputSize += this.objectsData.offset;
    outputSize += this.deps.offset;
    outputSize += this.closures.offset;
    outputSize += this.contextsData.offset;
    outputSize += this.contextsHeaders.offset;
    outputSize += this.repossessionsData.offset;

    this.buffer = new Buffer(outputSize);

    /* Write version into header. */
    this.headerInt32(CURRENT_VERSION);

    /* Put dependencies table in place and set location/rows in header. */
    this.headerInt32(this.offset);
    this.headerInt32(this.dependentSCs.length);


    this.writeChunk(this.deps);

    /* Put STables table in place, and set location/rows in header. */
    this.headerInt32(this.offset);
    this.headerInt32(this.sc.rootSTables.length);

    this.writeChunk(this.stables);

    /* Put STables data in place. */
    this.headerInt32(this.offset);

    this.writeChunk(this.stablesData);

    /* Put objects table in place, and set location/rows in header. */

    this.headerInt32(this.offset);
    this.headerInt32(this.sc.rootObjects.length);

    this.writeChunk(this.objects);

    /* Put objects data in place. */
    this.headerInt32(this.offset);

    this.writeChunk(this.objectsData);

    /* Put closures table in place, and set location/rows in header. */
    this.headerInt32(this.offset);
    this.headerInt32(this.numClosures);

    this.writeChunk(this.closures);

    /* Put contexts table in place, and set location/rows in header. */
    this.headerInt32(this.offset);
    this.headerInt32(this.contexts.length);

    this.writeChunk(this.contextsHeaders);

    /* Put contexts data in place. */
    this.headerInt32(this.offset);

    this.writeChunk(this.contextsData);

    /* Put repossessions table in place, and set location/rows in header. */
    this.headerInt32(this.offset);
    this.headerInt32(this.sc.repScs.length);

    this.writeChunk(this.repossessionsData);


    /* Sanity check. */
    if (this.offset != outputSize) {
      throw `Serialization sanity check failed: offset != outputSize (${offset} != ${outputSize})`;
    }

    return this.buffer.toString('base64');
  }

  serialize() {
    this.serializationLoop();
    return this.concatenateOutputs();
  }
};

op.serialize = function(sc, sh) {
  const writer = new SerializationWriter(sc, sh.array);
  return writer.serialize();
};

op.scsetobj = function(sc, idx, obj) {
  sc.setObj(idx, obj);
  return obj;
};

op.scgetobj = function(sc, idx) {
  return sc.rootObjects[idx];
};

op.getobjsc = function(obj) {
  return obj._SC || Null;
};

op.scgetobjidx = function(sc, obj) {
  const idx = sc.rootObjects.indexOf(obj);
  if (idx < 0) {
    throw 'Object does not exist in this SC';
  }
  return idx;
};

op.setobjsc = function(obj, sc) {
  obj._SC = sc;
  return obj;
};

op.scsetcode = function(sc, idx, obj) {
  sc.rootCodes[idx] = obj;
  obj._SC = sc;
  return obj;
};

op.neverrepossess = function(obj) {
  repossession.neverRepossess.set(obj, true);
  return obj;
};

exports.BinaryWriteCursor = BinaryWriteCursor;
