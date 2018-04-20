'use strict';
const bignum = require('bignum-browserify');

const sslBignum = require('bignum');

const core = require('./core.js');

const hll = require('./hll.js');

const NQPInt = require('./nqp-int.js');

const op = {};
exports.op = op;
function intishBool(b) {
  return b ? 1 : 0;
}

function makeNum(type, num) {
  const instance = type._STable.REPR.allocate(type._STable);
  instance.$$setNum(num);
  return instance;
}

function makeBI(type, num) {
  const instance = type._STable.REPR.allocate(type._STable);
  instance.$$setBignum(num);
  return instance;
}

function getBI(obj) {
  return obj.$$getBignum();
}

op.fromstr_I = function(str, type) {
  return makeBI(type, bignum(str));
};

op.tostr_I = function(n) {
  return getBI(n).toString();
};

op.base_I = function(n, base) {
  const orig = getBI(n);
  if (orig.eq(0)) return '0';

  if (base == 16 || base == 10) {
    return orig.toString(base).toUpperCase().replace(/^(-?)0+/, '$1');
  } else if (1 < base && base <= 36) {
    let num = orig.abs();
    let string = '';
    const letters = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ';

    while (num.gt(0)) {
      string = letters[num.mod(base).toNumber()] + string;
      num = num.div(base);
    }
    return (orig.lt(0) ? '-' : '') + string;
  } else {
    throw 'base_I with base: ' + base + ' NYI';
  }
};

op.iseq_I = function(a, b) {
  return intishBool(getBI(a).eq(getBI(b)));
};

op.isne_I = function(a, b) {
  return intishBool(!getBI(a).eq(getBI(b)));
};

op.mul_I = function(a, b, type) {
  return makeBI(type, getBI(a).mul(getBI(b)));
};

op.add_I = function(a, b, type) {
  return makeBI(type, getBI(a).add(getBI(b)));
};

op.sub_I = function(a, b, type) {
  return makeBI(type, getBI(a).sub(getBI(b)));
};

op.sub_I = function(a, b, type) {
  return makeBI(type, getBI(a).sub(getBI(b)));
};

op.div_I = function(a, b, type) {
  const divident = getBI(a);
  const divisor = getBI(b);
  // workaround for .div rounding to zero not down
  if (divisor.lt(0) != divident.lt(0) && divident.mod(divisor).ne(0)) {
    return makeBI(type, divident.div(divisor).sub(1));
  }
  return makeBI(type, divident.div(divisor));
};

op.pow_I = function(a, b, numType, biType) {
  const base = getBI(a);
  const exponent = getBI(b);
  if (exponent.lt(0)) {
    return makeNum(numType, Math.pow(base.toNumber(), exponent.toNumber()));
  } else {
    if (exponent.gt(4294967296) && !base.eq(1) && !base.eq(0)) {
      if (base.eq(-1)) {
        // workaround a bug in bignum
        return makeBI(biType, bignum(exponent.mod(2).eq(0) ? 1 : -1));
      } else {
        return makeNum(numType, base.lt(0) && exponent.mod(2).eq(1) ? Number.NEGATIVE_INFINITY : Number.POSITIVE_INFINITY);
      }
    }
    return makeBI(biType, base.pow(exponent));
  }
};

op.mod_I = function(n, m, type) {
  /* TODO - think if this can be optimized. */
  /* We are doing this in complicated way because,
     bignum returns the module with the sign equal to the dividend not the divisor. */
  const a = getBI(n);
  const b = getBI(m);
  if ((a.lt(0) && b.gt(0)) || (a.gt(0) && b.lt(0))) {
    const x = a.div(b).sub(1);
    const ret = a.sub(b.mul(x));
    return makeBI(type, (ret.eq(b) ? bignum(0) : ret));
  }
  return makeBI(type, a.mod(b));
};

op.neg_I = function(a, type) {
  return makeBI(type, getBI(a).neg());
};

op.isbig_I = function(n) {
  /* Check if it needs more bits than a long can offer; note that
   * bitLength excludes sign considerations, thus 31 rather than
   * 32. */
  return getBI(n).bitLength() > 31 ? 1 : 0;
};

op.expmod_I = function(a, b, c, type) {
  return makeBI(type, getBI(a).powm(getBI(b), getBI(c)));
};


/* TODO - optimize by using a smaller bignum when so much isn't needed */
const digits = 325;
const digitsBignum = bignum(10).pow(digits);

op.div_In = function(a, b) {
  const divisor = getBI(b);
  if (divisor.eq(0)) {
    return getBI(a).toNumber() / 0;
  }

  let sign = 1;
  let big = getBI(a).mul(digitsBignum).div(divisor).toString();
  if (big.substr(0, 1) == '-') {
    big = big.substr(1);
    sign = -1;
  }

  if (big.length <= digits) {
    return sign * parseFloat('0.' + '0'.repeat(digits - big.length) + big);
  } else {
    return sign * parseFloat(big.substr(0, big.length - digits) + '.' + big.substr(big.length - digits));
  }
};

op.rand_I = function(max, type) {
  return makeBI(type, bignum(sslBignum(getBI(max).toString()).rand().toString()));
};

op.isle_I = function(a, b) {
  return intishBool(getBI(a).le(getBI(b)));
};

op.islt_I = function(a, b) {
  return intishBool(getBI(a).lt(getBI(b)));
};

op.isge_I = function(a, b) {
  return intishBool(getBI(a).ge(getBI(b)));
};

op.isgt_I = function(a, b) {
  return intishBool(getBI(a).gt(getBI(b)));
};

op.cmp_I = function(a, b) {
  const result = getBI(a).cmp(getBI(b));
  return result == 0 ? 0 : (result < 0 ? -1 : 1);
};

op.isprime_I = function(n) {
  return intishBool(sslBignum(getBI(n).toString()).probPrime(50));
};

op.bitshiftl_I = function(a, b, type) {
  return makeBI(type, getBI(a).shiftLeft(b));
};

op.bitshiftr_I = function(a, b, type) {
  return makeBI(type, getBI(a).shiftRight(b));
};

op.bitand_I = function(a, b, type) {
  return makeBI(type, getBI(a).and(getBI(b)));
};

op.bitor_I = function(a, b, type) {
  return makeBI(type, getBI(a).or(getBI(b)));
};

op.bitxor_I = function(a, b, type) {
  return makeBI(type, getBI(a).xor(getBI(b)));
};

op.bitneg_I = function(a, type) {
  return makeBI(type, getBI(a).neg().sub(1));
};

op.lcm_I = function(n, m, type) {
  const a = getBI(n);
  const b = getBI(m);
  return makeBI(type, (a.abs().div(a.gcd(b)).mul(b.abs())));
};

op.gcd_I = function(a, b, type) {
  return makeBI(type, getBI(a).gcd(getBI(b)));
};

op.abs_I = function(n, type) {
  return makeBI(type, getBI(n).abs());
};

op.tonum_I = function(n) {
  return getBI(n).toNumber();
};

op.fromI_I = function(value, type) {
  return makeBI(type, getBI(value));
};

op.fromnum_I = function(num, type) {
  // node-bignum bug workaround, when a negative number is too big it gets turned into 0
  if (num < 0) {
    return makeBI(type, bignum(Math.floor(-num)).neg());
  } else {
    return makeBI(type, bignum(Math.floor(num)));
  }
};

op.bool_I = function(n) {
  return intishBool(getBI(n).toNumber());
};

op.radix_I = function(currentHLL, radix, str, zpos, flags, type) {
  const extracted = core.radixHelper(radix, str, zpos, flags);
  if (extracted == null) {
    return hll.slurpyArray(currentHLL, [makeBI(type, bignum(0)), makeBI(type, bignum(1)), new NQPInt(-1)]);
  }

  if (radix == 10 || radix == 16) {
    const pow = bignum(radix).pow(extracted.power);
    return hll.slurpyArray(currentHLL, [makeBI(type, bignum(extracted.number, radix)), makeBI(type, pow), new NQPInt(extracted.offset)]);
  } else {
    const n = extracted.number;
    let base = bignum(1);
    let result = bignum(0);

    for (let i = n.length - 1; i >= 0; i--) {
      let digit = n.charCodeAt(i);
      if (digit >= 48 && digit <= 57) digit -= 48; // 0-9
      else if (digit >= 97 && digit <= 122) digit = digit - 97 + 10; // a-z
      else if (digit >= 65 && digit <= 90) digit = digit - 65 + 10; // A-Z
      else break;

      result = result.add(base.mul(digit));
      base = base.mul(radix);
    }

    if (n[0] == '-') result = result.neg();

    return hll.slurpyArray(currentHLL, [makeBI(type, result), makeBI(type, base), new NQPInt(extracted.offset)]);
  }
};
