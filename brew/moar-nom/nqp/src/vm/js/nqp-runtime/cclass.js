'use strict';
const xregexp = require('xregexp');
const op = {};
exports.op = op;

function boolish(bool) {
  return bool ? 1 : 0;
}

const UPPERCASE = xregexp('^\\p{Lu}');
const LOWERCASE = xregexp('^\\p{Ll}');
const ALPHABETIC = xregexp('^\\pL');
const NUMERIC = xregexp('^\\p{Nd}');
const HEXADECIMAL = xregexp('^[0-9A-Fa-f]');
const WHITESPACE = xregexp('^\\p{White_Space}');
const BLANK = xregexp('^[\t\\p{Zs}]');
const NEWLINE = xregexp('[\n\r\u0085\u2029\f\u000b\u2028]');
const PUNCTUATION = xregexp('^\\pP');
const ALPHANUMERIC = xregexp('^[\\pL\\p{Nd}]');
const WORD = xregexp('^[\\pL_\\p{Nd}]');

function iscclass(cclass, target, offset) {
  if (offset < 0 || offset >= target.length) return 0;
  switch (cclass) {
    // ANY
    case 65535: return 1;
    // UPPERCASE
    case 1: return boolish(UPPERCASE.test(target[offset]));
    // LOWERCASE
    case 2: return boolish(LOWERCASE.test(target[offset]));
    // ALPHABETIC
    case 4: return boolish(ALPHABETIC.test(target[offset]));
    // NUMERIC
    case 8: return boolish(NUMERIC.test(target[offset]));
    // HEXADECIMAL
    case 16: return boolish(HEXADECIMAL.test(target[offset]));
    // WHITESPACE
    case 32: return boolish(WHITESPACE.test(target[offset]));
    // BLANK
    case 256: return boolish(BLANK.test(target[offset]));
    // PRINTING
    case 64: {
      const cp = target.codePointAt(offset);
      return boolish(!((cp >= 0 && cp < 32) || (cp >= 127 && cp < 160)));
    }
    // CONTROL
    case 512: {
      const cp = target.codePointAt(offset);
      return boolish((cp >= 0 && cp < 32) || (cp >= 127 && cp < 160));
    }
    // PUNCTUATION
    case 1024: return boolish(PUNCTUATION.test(target[offset])); // HACK
    // ALPHANUMERIC
    case 2048: return boolish(ALPHANUMERIC.test(target[offset]));
    // NEWLINE
    case 4096: return boolish(NEWLINE.test(target[offset]));
    // WORD
    case 8192: return boolish(WORD.test(target[offset]));
    default: throw 'cclass ' + cclass + ' not yet implemented';
  }
}

op.iscclass = function(cclass, target, offset) {
  return iscclass(cclass, target, offset);
};

op.findcclass = function(cclass, target, offset, count) {
  let end = offset + count;
  end = target.length < end ? target.length : end;

  for (let pos = offset; pos < end; pos++) {
    if (iscclass(cclass, target, pos) > 0) {
      return pos;
    }
  }

  return end;
};

op.findnotcclass = function(cclass, target, offset, count) {
  let end = offset + count;
  end = target.length < end ? target.length : end;

  for (let pos = offset; pos < end; pos++) {
    if (iscclass(cclass, target, pos) == 0) {
      return pos;
    }
  }

  return end;
};
