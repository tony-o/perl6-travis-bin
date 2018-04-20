'use strict';
const Ctx = require('./ctx.js');
const Iter = require('./iter.js');

class CtxWithStatic extends Ctx {
  constructor(callerCtx, outerCtx, callThis, statics) {
    super(callerCtx, outerCtx, callThis);
    this.$$statics = statics;
  }

  $$atkey(key) {
    return (this.$$statics.hasOwnProperty(key) ? this.$$statics[key] : this.lookup(key));
  }


  $$existskey(key) {
    return (this.hasOwnProperty(key) || this.$$statics.hasOwnProperty(key) ? 1 : 0);
  }

  $$iterator() {
    return new Iter(Object.keys(this).filter(key => key.substr(0, 2) != '$$').concat(Object.keys(this.$$statics)));
  }
};
module.exports = CtxWithStatic;
