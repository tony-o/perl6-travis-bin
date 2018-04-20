const CodeRef = require('./code-ref.js');
class CodeRefWithStateVars extends CodeRef {
  freshBlock() {
    return this.closureTemplate();
  }

  $$clone() {
    const clone = new CodeRefWithStateVars(this.name, undefined);
    clone.$$call = this.freshBlock();
    clone.codeObj = this.codeObj;
    clone.staticCode = this.staticCode;
    clone.closureTemplate = this.closureTemplate;
    clone.outerCtx = this.outerCtx;
    return clone;
  }

  closure(block) {
    /* TODO cloning won't work with a closureTemplate */
    throw 'CodeRefWithStateVars.closure NYI';
  }

  closureCtx(outerCtx) {
    const closure = new CodeRefWithStateVars(this.name, this.cuid);
    closure.codeObj = this.codeObj;
    closure.$$call = this.freshBlock();
    closure.closureTemplate = this.closureTemplate;
    closure.staticCode = this;

    closure.outerCtx = outerCtx;

    return closure;
  }

  captureAndClosureCtx(outerCtx) {
    this.capture(this.freshBlock());
    const closure = this.closureCtx(outerCtx);

    if (outerCtx !== null) {
      this.outerCtx = outerCtx;
      closure.outerCtx = outerCtx;
    }

    return closure;
  }
};
module.exports = CodeRefWithStateVars;
