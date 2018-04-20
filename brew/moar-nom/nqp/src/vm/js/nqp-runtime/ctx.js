'use strict';
const NQPExceptionWithCtx = require('./nqp-exception-with-ctx.js');
const NQPObject = require('./nqp-object.js');
const Null = require('./null.js');
const exceptionsStack = require('./exceptions-stack.js');

const BOOT = require('./BOOT.js');

const NQPInt = require('./nqp-int.js');

const stackTrace = require('stack-trace');

const NEXT = 4;
const REDO = 8;
const LAST = 16;
const RETURN = 32;
const TAKE = 128;
const WARN = 256;
const SUCCEED = 512;
const PROCEED = 1024;
const LABELED = 4096;
const AWAIT = 8192;
const EMIT = 16384;
const DONE = 32768;

const categoryIDs = {
  NEXT: NEXT,
  REDO: REDO,
  LAST: LAST,
  RETURN: RETURN,
  TAKE: TAKE,
  WARN: WARN,
  SUCCEED: SUCCEED,
  PROCEED: PROCEED,
  AWAIT: AWAIT,
  EMIT: EMIT,
  DONE: DONE,
};

const categoryToName = {};
for (const name of Object.keys(categoryIDs)) {
  categoryToName[categoryIDs[name]] = name;
}

class ResumeException {
  constructor(exception) {
    this.exception = exception;
  }
};

class Ctx extends NQPObject {
  constructor(callerCtx, outerCtx, callThis) {
    super();
    this.$$caller = callerCtx;
    this.$$outer = outerCtx;
    this.$$callThis = callThis;
  }

  codeRef() {
    return this.$$callThis;
  }

  last() {
    this.controlException(LAST);
  }

  lastLabeled(label) {
    this.controlExceptionLabeled(label, LAST);
  }

  next() {
    this.controlException(NEXT);
  }

  nextLabeled(label) {
    this.controlExceptionLabeled(label, NEXT);
  }

  redo() {
    this.controlException(REDO);
  }

  redoLabeled(label) {
    this.controlExceptionLabeled(label, REDO);
  }

  controlException(category) {
    const exType = BOOT.Exception;
    const exception = exType._STable.REPR.allocate(exType._STable);
    exception.$$category = category;
    this.propagateControlException(exception);
  }

  controlExceptionLabeled(label, category) {
    const exType = BOOT.Exception;
    const exception = exType._STable.REPR.allocate(exType._STable);
    exception.$$category = category | LABELED;
    exception.$$payload = label;
    this.propagateControlException(exception);
  }

  propagateControlException(exception) {
    const handler = '$$' + categoryToName[exception.$$category & ~LABELED];
    const labeled = exception.$$category & LABELED;

    let ctx = this;

    while (ctx) {
      if ((ctx[handler] || ctx.$$CONTROL) && (!labeled || ctx.$$label === exception.$$payload)) {
        exception.caught = ctx;
        ctx.exception = exception;

        exceptionsStack().push(exception);
        try {
          if (ctx[handler]) {
            ctx.unwind.ret = ctx[handler]();
          } else {
            ctx.unwind.ret = ctx.$$CONTROL();
          }
        } catch (e) {
          if (e instanceof ResumeException && e.exception === exception) {
            return;
          } else {
            throw e;
          }
        } finally {
          exceptionsStack().pop();
        }

        throw ctx.unwind;
      }
      ctx = ctx.$$caller;
    }

    throw exception;
  }

  propagateException(exception) {
    if (exception.$$category) {
      this.propagateControlException(exception);
      return;
    }

    let ctx = this;

    while (ctx) {
      if (ctx.$$CATCH) {
        exception.caught = ctx;
        ctx.exception = exception;

        exceptionsStack().push(exception);
        try {
          ctx.unwind.ret = ctx.$$CATCH();
        } catch (e) {
          if (e instanceof ResumeException && e.exception === exception) {
            return;
          } else {
            throw e;
          }
        } finally {
          exceptionsStack().pop();
        }

        throw ctx.unwind;
      }
      ctx = ctx.$$caller;
    }

    throw exception;
  }

  catchException(exception) {
    this.exception = exception;
    exceptionsStack().push(exception);
    try {
      return this.$$CATCH();
    } finally {
      exceptionsStack().pop();
    }
  }

  rethrow(exception) {
    this.propagateException(exception);
  }

  die(msg) {
    this.propagateException(new NQPExceptionWithCtx(msg, this, stackTrace.get()));
  }

  resume(exception) {
    throw new ResumeException(exception);
  }

  throw(exception) {
    exception.$$stack = stackTrace.get();
    exception.$$ctx = this;
    this.propagateException(exception);
  }

  throwpayloadlexcaller(category, payload) {
    let ctx = this.$$skipHandlers().$$caller;
    const isThunkOrCompilerStub = code => code.staticCode.isThunk || code.isCompilerStub;
    while (ctx && isThunkOrCompilerStub(ctx.codeRef())) {
      ctx = ctx.$$caller;
    }

    this.$$throwLexicalException(ctx, category, payload);
  }

  throwpayloadlex(category, payload) {
    this.$$throwLexicalException(this, category, payload);
  }

  $$throwLexicalException(lookFrom, category, payload) {
    const exType = BOOT.Exception;
    const exception = exType._STable.REPR.allocate(exType._STable);
    exception.$$category = category;
    exception.$$payload = payload;
    const handler = '$$' + categoryToName[category];

    let ctx = lookFrom;

    while (ctx) {
      if (ctx[handler] || ctx.$$CONTROL) {
        exception.caught = ctx;
        ctx.exception = exception;

        exceptionsStack().push(exception);
        try {
          if (ctx[handler]) {
            ctx.unwind.ret = ctx[handler]();
          } else {
            ctx.unwind.ret = ctx.$$CONTROL();
          }
        } catch (e) {
          if (e instanceof ResumeException && e.exception === exception) {
            return;
          } else {
            throw e;
          }
        } finally {
          exceptionsStack().pop();
        }

        throw ctx.unwind;
      }
      ctx = ctx.$$outer;
    }

    this.$$getHLL().get('lexical_handler_not_found_error').$$call(this, null, new NQPInt(category), new NQPInt(0));
  }

  lookupDynamic(name) {
    let ctx = this;
    while (ctx) {
      if (ctx.hasOwnProperty(name)) {
        return ctx[name];
      }
      ctx = ctx.$$caller;
    }
    return Null;
    /* Looking up of a contextual is allowed to fail,
       nqp code usually fallbacks to looking up of global */
  }

  lookupDynamicFromCaller(name) {
    let ctx = this.$$caller;
    while (ctx) {
      if (ctx.hasOwnProperty(name)) {
        return ctx[name];
      }
      ctx = ctx.$$caller;
    }
    return Null;
    /* Looking up of a contextual is allowed to fail,
       nqp code usually fallbacks to looking up of global */
  }

  lookupWithCallers(name) {
    let currentCallerCtx = this;
    while (currentCallerCtx) {
      let currentCtx = currentCallerCtx;
      while (currentCtx) {
        if (currentCtx.hasOwnProperty(name)) {
          return currentCtx[name];
        }
        currentCtx = currentCtx.$$outer;
      }
      currentCallerCtx = currentCallerCtx.$$caller;
    }
    return Null;
  }

  lookup(name) {
    let ctx = this;
    while (ctx) {
      if (ctx.hasOwnProperty(name)) {
        return ctx[name];
      }
      ctx = ctx.$$outer;
    }
    /* Rakudo depends on returning null when we can't lookup a lexical */
    return Null;
  }

  lookupFromOuter(name) {
    return this.$$outer.lookup(name);
  }

  $$getHLL() {
    let ctx = this;
    while (ctx) {
      if (ctx.$$hll) {
        return ctx.$$hll;
      }
      ctx = ctx.$$outer;
    }
  }

  $$atkey(key) {
    return this.lookup(key);
  }

  $$bindkey(key, value) {
    this[key] = value;
    return value;
  }

  $$existskey(key) {
    return (this.hasOwnProperty(key) ? 1 : 0);
  }

  $$skipHandlers() {
    return this;
  }

  bind(name, value) {
    let ctx = this;
    while (ctx) {
      if (ctx.hasOwnProperty(name)) {
        ctx[name] = value;
        return ctx[name];
      }
      ctx = ctx.$$outer;
    }
    throw `Can't bind: ${name}`;
  }

  bindDynamic(name, value) {
    let ctx = this;
    while (ctx) {
      if (ctx.hasOwnProperty(name)) {
        ctx[name] = value;
        return ctx[name];
      }
      ctx = ctx.$$caller;
    }
    throw `Can't bind dynamic: ${name}`;
  }

  $$iterator() {
    return new CtxIter(this);
  }

  $$elems() {
    return Object.keys(this).filter(key => key.substr(0, 2) != '$$').length;
  }

  $$toBool(ctx) {
    return 1;
  }
};

class CtxIter extends NQPObject {
  constructor(ctx) {
    super();
    this.$$ctx = ctx;
    this.$$keys = Object.keys(ctx).filter(key => key.substr(0, 2) != '$$');
    this.$$target = this.$$keys.length - 1;
    this.$$idx = -1;
  }

  $$shift() {
    this.$$idx++;
    return this;
  }

  $$iterval() {
    return this.$$ctx[this.$$keys[this.$$idx]];
  }

  $$iterkey_s() {
    return this.$$keys[this.$$idx];
  }

  $$toBool(ctx) {
    return this.$$idx < this.$$target;
  }

  Str(ctx, _NAMED, self) {
    return this.$$iterkey_s();
  }

  key(ctx, _NAMED, self) {
    return this.$$iterkey_s();
  }

  value(ctx, _NAMED, self) {
    return this.$$iterval();
  }
};

Ctx.prototype.$$atkey_i = Ctx.prototype.$$atkey_n = Ctx.prototype.$$atkey_s = Ctx.prototype.$$atkey;
Ctx.prototype.$$bindkey_i = Ctx.prototype.$$bindkey_n = Ctx.prototype.$$bindkey_s = Ctx.prototype.$$bindkey;

module.exports = Ctx;
