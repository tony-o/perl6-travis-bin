# TABLE OF CONTENTS
- [NQP Opcodes](#nqp-opcodes)
- [Arithmetic Opcodes](#-arithmetic-opcodes)
    - [abs](#abs)
    - [add](#add)
    - [div](#div)
    - [gcd](#gcd)
    - [lcm](#lcm)
    - [mod](#mod)
    - [mul](#mul)
    - [neg](#neg)
    - [sub](#sub)
- [Numeric Opcodes](#-numeric-opcodes)
    - [base](#base)
    - [ceil](#ceil)
    - [exp](#exp)
    - [floor](#floor)
    - [inf](#inf)
    - [log](#log)
    - [ln](#ln)
    - [expmod](#expmod)
    - [nan](#nan)
    - [neginf](#neginf)
    - [pow](#pow)
    - [rand](#rand)
    - [sqrt](#sqrt)
- [Trigonometric Opcodes](#-trigonometric-opcodes)
    - [asec](#asec)
    - [asin](#asin)
    - [acos](#acos)
    - [atan](#atan)
    - [atan2](#atan2)
    - [cos](#cos)
    - [cosh](#cosh)
    - [sin](#sin)
    - [sinh](#sinh)
    - [sec](#sec)
    - [sech](#sech)
    - [tan](#tan)
    - [tanh](#tanh)
- [Relational / Logic Opcodes](#-relational--logic-opcodes)
    - [cmp](#cmp)
    - [eqat](#eqat)
    - [iseq](#iseq)
    - [isgt](#isgt)
    - [isge](#isge)
    - [islt](#islt)
    - [isle](#isle)
    - [isne](#isne)
    - [not](#not)
- [Array Opcodes](#-array-opcodes)
    - [atpos](#atpos)
    - [bindpos](#bindpos)
    - [elems](#elems)
    - [existspos](#existspos)
    - [list](#list)
    - [push](#push)
    - [pop](#pop)
    - [setelems](#setelems)
    - [shift](#shift)
    - [splice](#splice)
    - [unshift](#unshift)
    - [iterator](#iterator)
- [Hash Opcodes](#-hash-opcodes)
    - [atkey](#atkey)
    - [bindkey](#bindkey)
    - [existskey](#existskey)
    - [deletekey](#deletekey)
    - [iterkey](#iterkey)
    - [iterval](#iterval)
- [String Opcodes](#-string-opcodes)
    - [chars](#chars)
    - [chr](#chr)
    - [codepointfromname](#codepointfromname)
    - [concat](#concat)
    - [decode](#decode)
    - [decodetocodes](#decodetocodes)
    - [encode](#encode)
    - [encodefromcodes](#encodefromcodes)
    - [encodenorm](#encodenorm)
    - [escape](#escape)
    - [findcclass](#findcclass)
    - [findnotcclass](#findnotcclass)
    - [flip](#flip)
    - [hash](#hash)
    - [index](#index)
    - [iscclass](#iscclass)
    - [join](#join)
    - [lc](#lc)
    - [normalizecodes](#normalizecodes)
    - [ord](#ord)
    - [radix](#radix)
    - [replace](#replace)
    - [rindex](#rindex)
    - [uc](#uc)
    - [split](#split)
    - [strfromcodes](#strfromcodes)
    - [strtocodes](#strtocodes)
    - [substr](#substr)
    - [tc](#tc)
    - [x](#x)
    - [sprintf](#sprintf)
    - [sprintfdirectives](#sprintfdirectives)
    - [sprintfaddargumenthandler](#sprintfaddargumenthandler)
- [Unicode Property Opcodes](#-unicode-property-opcodes)
    - [getuniname](#getuniname)
    - [unipropcode](#unipropcode)
    - [unipvalcode](#unipvalcode)
    - [getuniprop_int](#getuniprop_int)
    - [getuniprop_str](#getuniprop_str)
    - [getuniprop_bool](#getuniprop_bool)
    - [matchuniprop](#matchuniprop)
- [Conditional Opcodes](#-conditional-opcodes)
    - [if](#if)
    - [unless](#unless)
- [Loop/Control Opcodes](#-loopcontrol-opcodes)
    - [for](#for)
    - [ifnull](#ifnull)
    - [defor](#defor)
    - [repeat_until](#repeat_until)
    - [repeat_while](#repeat_while)
    - [until](#until)
    - [while](#while)
    - [control](#control)
- [Exceptional Opcodes](#-exceptional-opcodes)
    - [backtrace](#backtrace)
    - [backtracestrings](#backtracestrings)
    - [die](#die)
    - [exception](#exception)
    - [getextype](#getextype)
    - [getmessage](#getmessage)
    - [getpayload](#getpayload)
    - [newexception](#newexception)
    - [resume](#resume)
    - [rethrow](#rethrow)
    - [setextype](#setextype)
    - [setmessage](#setmessage)
    - [setpayload](#setpayload)
    - [throw](#throw)
- [Input/Output Opcodes](#-inputoutput-opcodes)
    - [closefh](#closefh)
    - [eoffh](#eoffh)
    - [flushfh](#flushfh)
    - [getcfh](#getcfh)
    - [getstderr](#getstderr)
    - [getstdin](#getstdin)
    - [getstdout](#getstdout)
    - [open](#open)
    - [openasync `jvm`](#openasync-jvm)
    - [print](#print)
    - [printfh](#printfh)
    - [readallfh](#readallfh)
    - [readfh](#readfh)
    - [readlinefh](#readlinefh)
    - [readcharsfh](#readcharsfh)
    - [say](#say)
    - [sayfh](#sayfh)
    - [setencoding](#setencoding)
    - [setinputlinesep](#setinputlinesep)
    - [tellfh](#tellfh)
    - [writefh](#writefh)
- [External command Opcodes](#-external-command-opcodes)
    - [shell](#shell)
    - [spawn](#spawn)
- [File / Directory / Network Opcodes](#-file--directory--network-opcodes)
    - [chdir](#chdir)
    - [chmod](#chmod)
    - [closedir](#closedir)
    - [copy](#copy)
    - [cwd](#cwd)
    - [fileexecutable](#fileexecutable)
    - [fileislink](#fileislink)
    - [filereadable](#filereadable)
    - [filewritable](#filewritable)
    - [link](#link)
    - [mkdir](#mkdir)
    - [nextfiledir](#nextfiledir)
    - [opendir](#opendir)
    - [rename](#rename)
    - [rmdir](#rmdir)
    - [stat](#stat)
    - [stat_time](#stat_time)
    - [lstat](#lstat)
    - [stat_time](#stat_time)
    - [symlink](#symlink)
    - [unlink](#unlink)
- [Type/Conversion Opcodes](#-typeconversion-opcodes)
    - [bool](#bool)
    - [bootarray `jvm` `moar`](#bootarray-jvm-moar)
    - [boothash `jvm` `moar`](#boothash-jvm-moar)
    - [bootint `jvm` `moar`](#bootint-jvm-moar)
    - [bootintarray `jvm` `moar`](#bootintarray-jvm-moar)
    - [bootnum `jvm` `moar`](#bootnum-jvm-moar)
    - [bootnumarray `jvm` `moar`](#bootnumarray-jvm-moar)
    - [bootstr `jvm` `moar`](#bootstr-jvm-moar)
    - [bootstrarray `jvm` `moar`](#bootstrarray-jvm-moar)
    - [box](#box)
    - [defined](#defined)
    - [fromnum](#fromnum)
    - [fromstr](#fromstr)
    - [isbig](#isbig)
    - [isconcrete](#isconcrete)
    - [iscont](#iscont)
    - [isfalse](#isfalse)
    - [ishash](#ishash)
    - [isint](#isint)
    - [isinvokable](#isinvokable)
    - [islist](#islist)
    - [isnanorinf](#isnanorinf)
    - [isnull](#isnull)
    - [isnum](#isnum)
    - [isprime](#isprime)
    - [isstr](#isstr)
    - [istrue](#istrue)
    - [istype](#istype)
    - [null](#null)
    - [jvmisnull `jvm`](#jvmisnull-jvm)
    - [tostr](#tostr)
    - [tonum](#tonum)
    - [unbox](#unbox)
- [OO/SixModel Opcodes](#-oosixmodel-opcodes)
    - [bindattr](#bindattr)
    - [bindcomp](#bindcomp)
    - [callmethod](#callmethod)
    - [can](#can)
    - [clone](#clone)
    - [create](#create)
    - [eqaddr](#eqaddr)
    - [findmethod](#findmethod)
    - [getattr](#getattr)
    - [getcomp](#getcomp)
    - [how](#how)
    - [rebless](#rebless)
    - [reprname](#reprname)
    - [setwho](#setwho)
    - [who](#who)
    - [what](#what)
    - [where](#where)
- [Bit Opcodes](#-bit-opcodes)
    - [bitand](#bitand)
    - [bitneg](#bitneg)
    - [bitor](#bitor)
    - [bitshiftl](#bitshiftl)
    - [bitshiftr](#bitshiftr)
    - [bitxor](#bitxor)
- [Context Introspection Opcodes](#-context-introspection-opcodes)
    - [ctx](#ctx)
    - [ctxcaller](#ctxcaller)
    - [ctxlexpad](#ctxlexpad)
    - [curlexpad](#curlexpad)
    - [ctxouter](#ctxouter)
    - [lexprimspec](#lexprimspec)
    - [savecapture](#savecapture)
    - [usecapture](#usecapture)
    - [getlex](#getlex)
    - [bindlex](#bindlex)
    - [getlexdyn](#getlexdyn)
    - [bindlexdyn](#bindlexdyn)
    - [getlexouter](#getlexouter)
    - [getlexcaller](#getlexcaller)
    - [getlexrel](#getlexrel)
    - [getlexreldyn](#getlexreldyn)
    - [getlexrelcaller](#getlexrelcaller)
- [Variable Opcodes](#-variable-opcodes)
    - [bind](#bind)
- [Miscellaneous Opcodes](#-miscellaneous-opcodes)
    - [const](#const)
    - [debugnoop](#debugnoop)
    - [exit](#exit)
    - [getenvhash](#getenvhash)
    - [backendconfig](#backendconfig)
    - [getpid](#getpid)
    - [jvmclasspaths `jvm`](#jvmclasspaths-jvm)
    - [sha1](#sha1)
    - [sleep](#sleep)
    - [takeclosure](#takeclosure)
    - [time](#time)
- [Native Call / Interoperability Opcodes](#-native-call--interoperability-opcodes)
    - [x_posixerrno](#x_posixerrno)
- [Asynchronous Operations](#-asynchronous-operations)
    - [cancel](#cancel)
    - [timer](#timer)
    - [signal](#signal)
    - [watchfile](#watchfile)
    - [asyncconnect](#asyncconnect)
    - [asynclisten](#asynclisten)
    - [asyncwritestr](#asyncwritestr)
    - [asyncwritebytes](#asyncwritebytes)
    - [asyncreadchars](#asyncreadchars)
    - [asyncreadbytes](#asyncreadbytes)
    - [spawnprocasync](#spawnprocasync)
    - [killprocasync](#killprocasync)

# NQP Opcodes

Opcodes (ops) are used both directly when writing NQP, and during code
generation in QAST nodes.

When invoking them directly, you'll need to prefix them with nqp::, e.g.

    nqp::mul_n(6,9);

The ops are listed below by type. Each entry shows the name of the op,
its variants, and their arguments and types, and may provide a short
description.  Some opcodes differ only by argument types - in that case,
they are listed under their common name (e.g. `mul`), with each of their
variants (e.g. `mul_i`, `mul_n`) together with a single description.

Opcode variants may contain a type suffix, which usually indicates:

* `_i` argument is native int
* `_n` argument is native float
* `_s` argument is native string
* `_b` argument is code blocks
* `_I` argument is Big Integer

They may also have a numeric suffix, which typically indicates the number
of arguments required.

In opcode signatures below, we use the following types, which may not
correspond directly to NQP types.

* int       - native int
* num       - native float
* str       - native string
* Int       - BigInt
* Mu        - any NQP or VM object
* Mu:T      - a type object, e.g. `Int`
* Exception - an Exception object
* Handle    - an I/O Handle object
* Iterable  - something iterable
* Context   - a Context object
* LexPad    - a Context object
* @         - this sigil indicates an array parameter
* %         - this sigil indicates a hash parameter
* ...       - indicates variable args are accepted

VM-specific opcodes are denoted with a `jvm`, e.g. on the same line
as the header. No annotation indicates this opcode should be supported on
all nqp backends.

Some individual opcodes may be marked with _Internal_ or _Deprecated_.
Both of these indicate the opcodes are not intended to be used. Deprecated
opcodes will eventually be removed from NQP. Internal opcodes are typically
used at compile time to replace opcodes that take a variable number of
arguments.

The opcodes are grouped into the following categories:

* [Arithmetic Opcodes](#arithmetic)
* [Numeric Opcodes](#numeric)
* [Trigonometric Opcodes](#trig)
* [Relational / Logic Opcodes](#logic)
* [Array Opcodes](#array)
* [Hash Opcodes](#hash)
* [String Opcodes](#string)
* [Unicode Property Opcodes](#unicode)
* [Conditional Opcodes](#conditional)
* [Loop/Control Opcodes](#control)
* [Exceptional Opcodes](#exceptions)
* [Input/Output Opcodes](#io)
* [External command Opcodes](#extern)
* [File / Directory / Network Opcodes](#filedirnet)
* [Type/Conversion Opcodes](#type)
* [OO/SixModel Opcodes](#sixmodel)
* [Bit Opcodes](#bit)
* [Context Introspection Opcodes](#context)
* [Variable Opcodes](#variable)
* [Miscellaneous Opcodes](#misc)
* [Native Call / Interoperability Opcodes](#nativecall)
* [Asynchronous operations](#async)

# <a id="arithmetic"></a> Arithmetic Opcodes

## abs
* `abs_i(int $i)`
* `abs_n(num $n)`
* `abs_I(Int $i, Mu:T $type)`

Return the absolute value of a number.
`_I` variant returns an object of the given type.

## add
* `add_i(int $l, int $r)`
* `add_n(num $l, num $r)`
* `add_I(Int $l, Int $r, Mu:T $type)`

Add two numbers together, returning the result.
`_I` variant returns an object of the given type.

## div
* `div_i(int $l, int $r)`
* `div_n(num $l, num $r)`
* `div_I(Int $l, Int $r, Mu:T $type)`
* `div_In(Int $l, Int $r)`

Divide $l by $r, returning the result.
`_I` variant returns an object of the given type. The `_In` variant returns
a native num, using a scale of 309, and a rounding mode equivalent to Java's
`ROUND_HALF_UP`.

## gcd
* `gcd_i(int $l, int $r)`
* `gcd_I(Int $l, Int $r, Mu:T $type)`

Return the greatest common multiple of two numbers.
`_I` variant returns an object of the given type.

## lcm
* `lcm_i(int $l, int $r)`
* `lcm_I(Int $l, Int $r, Mu:T $type)`

Return the lowest common multiple of two numbers.
`_I` variant returns an object of the given type.

## mod
* `mod_i(int $l, int $r)`
* `mod_n(num $l, num $r)`
* `mod_I(Int $l, Int $r, Mu:T $type)`

Return the modulus of $l by $r.
`_I` variant returns an object of the given type.

## mul
* `mul_i(int $l, int $r)`
* `mul_n(num $l, num $r)`
* `mul_I(Int $l, Int $r, Mu:T $type)`

Multiply two numbers, returning the result.
`_I` variant returns an object of the given type.

## neg
* `neg_i(int $i)`
* `neg_n(num $n)`
* `neg_I(Int $i, Mu:T $type)`

Return the negative of a number.
`_I` variant returns an object of the given type.

## sub
* `sub_i(int $l, int $r)`
* `sub_n(num $l, num $r)`
* `sub_I(Int $l, Int $r, Mu:T $type)`

Subtract $r from $l, returning the result.
`_I` variant returns an object of the given type.

# <a id="numeric"></a> Numeric Opcodes

## base
* `base_I(Int $i, int $radix)`

Returns a string representing the integer `$i` in base `$radix`

## ceil
* `ceil_n(num $n)`

Return the ceiling of a number.

## exp
* `exp_n(num $exponent)`

Return the value of `e` raised to $exponent.

## floor
* `floor_n(num $n)`

Return the floor of a number.

## inf
* `inf()`

Return infinity.

## log
* `log_n(num $n)`

Return the log base 10 of a number.

## ln
* `ln_n(num $n)`

Return the natural logarithm of a number.

## expmod
* `expmod_I(Int $base, Int $exponent, Int $modulus, Mu:T $type)`

Return a bigint that is `$base` raised to `$exponent` modulus `$modulus`.
`_I` variant returns an object of the given type.

## nan
* `nan()`

Return NaN.

## neginf
* `neginf()`

Return negative infinity.

## pow
* `pow_n(num $base, num $exponent)`
* `pow_I(Int $base, Int $exponent, Mu:T $type_num, Mu:T $type_bigint)`

Return the value of $base raised to $exponent;
`_I` variant returns an object of `$type_num` for negative exponents,
and of type `$type_bigint` for positive exponents.

## rand
* `rand_n(num $n)`
* `rand_I(Int $i, Mu:T $type)`

Returns a psuedo-random bigint up to the value of the
given number.
`_I` variant returns an object of the given type.

## sqrt
* `sqrt_n(num $l, num $r)`

# <a id="trig"></a> Trigonometric Opcodes

Each opcode corresponds directly to the trigonometric function of the same
name. `h` indicates a hyperbolic variant.

## asec
* `asec_n(num $n)`

## asin
* `asin_n(num $n)`

## acos
* `acos_n(num $n)`

## atan
* `atan_n(num $n)`

## atan2
* `atan2_n(num $l, num $r)`

## cos
* `cos_n(num $n)`

## cosh
* `cosh_n(num $n)`

## sin
* `sin_n(num $n)`

## sinh
* `sinh_n(num $n)`

## sec
* `sec_n(num $n)`

## sech
* `sech_n(num $n)`

## tan
* `tan_n(num $n)`

## tanh
* `tanh_n(num $n)`

# <a id="logic"></a> Relational / Logic Opcodes

## cmp
* `cmp_i(int $l, int $r)`
* `cmp_n(num $l, num $r)`
* `cmp_s(str $l, str $r)`
* `cmp_I(Int $l, Int $r)`

Compare two values, returns -1 if $l is greater than $r, 0 if they are equal,
and 1 if $r is greater than $l.

## eqat
* `eqat(str $haystack, str $needle, int $pos)`

Return 1 if the string `$haystack` has the string `$needle` at position `$pos`,
or 0 otherwise.

## iseq
* `iseq_i(int $l, int $r)`
* `iseq_n(num $l, num $r)`
* `iseq_s(str $l, str $r)`
* `iseq_I(Int $l, Int $r)`

Return non-zero if the two parameters are equal.

## isgt
* `isgt_i(int $l, int $r)`
* `isgt_n(num $l, num $r)`
* `isgt_s(str $l, str $r)`
* `isgt_I(Int $l, Int $r)`

Return non-zero if $l is greater than $r.

## isge
* `isge_i(int $l, int $r)`
* `isge_n(num $l, num $r)`
* `isge_s(str $l, str $r)`
* `isge_I(Int $l, Int $r)`

Return non-zero if $l is greater than or equal to $r.

## islt
* `islt_i(int $l, int $r)`
* `islt_n(num $l, num $r)`
* `islt_s(str $l, str $r)`
* `islt_I(Int $l, Int $r)`

Return non-zero if $l is less than $r.

## isle
* `isle_i(int $l, int $r)`
* `isle_n(num $l, num $r)`
* `isle_s(str $l, str $r)`
* `isle_I(Int $l, Int $r)`

Return non-zero if $l is less than or equal to $r.

## isne
* `isne_i(int $l, int $r)`
* `isne_n(num $l, num $r)`
* `isne_s(str $l, str $r)`
* `isne_I(Int $l, Int $r)`

Return non-zero if the two parameters are not equal.

## not
* `not_i(int $val)`

Return 1 if `$val` is 0, 0 otherwise.

# <a id="array"></a> Array Opcodes

## atpos
* `atpos(@arr, int $i)`
* `atpos_i(@arr, int $i)`
* `atpos_n(@arr, int $i)`
* `atpos_s(@arr, int $i)`

Return whatever is bound to @arr at position $i.

## bindpos
* `bindpos(@arr, int $i, Mu $v)`
* `bindpos_i(@arr, int $i, int $v)`
* `bindpos_n(@arr, int $i, num $v)`
* `bindpos_s(@arr, int $i, str $v)`

Bind $v to @arr at position $i and return $v.

## elems
* `elems(@arr)`
* `elems(%hash)`

Return the number of elements in `@arr`, or the number of keys in `%hash`.

## existspos
* `existspos(@arr, int $i)`

Return 1 if anything is bound to `@arr` at position `$i`,
0 otherwise.

## list
* `list(...)`
* `list_i(...)`
* `list_n(...)`
* `list_s(...)`
* `list_b(...)`

Create a list of the given parameters. If no arguments are passed,
an empty list is created. If a typed variant is used, the parameters
are coerced to the appropriate type.

## push
* `push(@arr, Mu $v)`
* `push_i(Array int @arr, int $v)`
* `push_n(Array num @arr, num $v)`
* `push_s(Array str @arr, str $v)`

"Push $v onto the end of @arr."
Bind $v to @arr at the position at the end of @arr, i.e., the position that
is just after the last position of @arr that has been bound to.

Return value is not currently defined.

## pop
* `pop(@arr)`
* `pop_i(@arr)`
* `pop_n(@arr)`
* `pop_s(@arr)`

"Pop the last value off the end of @arr."
Return the value of @arr at its last bound position, and unbind @arr at that
position.

## setelems
* `setelems(@arr, int $i)`

Set the size of `@arr` to `$i` elements. If less than the current size,
any elements after the new last position are unbound. If greater, the empty
elements at the end are bound with potentially VM specific null entries.

## shift
* `shift(@arr)`
* `shift_i(@arr)`
* `shift_n(@arr)`
* `shift_s(@arr)`

"Shift the last value from the beginning of @arr."
Return the value of @arr at index 0, unbind @arr at index 0, and move all
other binding of @arr to the index one below what they were previously bound
to.

## splice
* `splice(@arr, @from, int $offset, int $count)`

Remove the elements in `@arr` starting at `$offset`, for `$count` positions.
Replace them with all the elements from `@from`.

## unshift
* `unshift(@arr, Mu $v)`
* `unshift_i(@arr, int $v)`
* `unshift_n(@arr, num $v)`
* `unshift_s(@arr, str $v)`

"Shift $v into the beginning of @arr."
Bind $v to @arr at index 0, move all other bindings of @arr to the index one
above what they were previously bound to.
Return the number of elements of @arr on Parrot, $v on JVM.

## iterator
* `iterator()`

Returns an iterator object to iterate over a list's items.  For example:

```perl

my $list := nqp::list('a', 'b', 'c');
my $iter := nqp::iterator($list);

while $iter {
    say(nqp::shift($iter));
}

```

You can also use `nqp::iterator()` to iterate over a hash's key-value pairs.

# <a id="hash"></a> Hash Opcodes

## atkey
* `atkey(%hash, String $key)`
* `atkey_i(%hash, String $key)`
* `atkey_n(%hash, String $key)`
* `atkey_s(%hash, String $key)`

Return the value of %hash at key $key.

## bindkey
* `bindkey(%hash, String $key, Mu $v)`
* `bindkey_i(%hash, String $key, int $v)`
* `bindkey_n(%hash, String $key, num $v)`
* `bindkey_s(%hash, String $key, str $v)`

Bind key $key of %hash to $v and return $v.

## existskey
* `existskey(%hash, String $key)`

Return non-zero if %hash has key $key bound to something.

## deletekey
* `deletekey(%hash, String $key)`

Delete the given key from %hash.

## iterkey
* `iterkey_s($pair)`

Returns the key associated with the given key-value pair.
For example:

```perl

for %hash {
    say(nqp::iterkey_s($_), ' => ', nqp::iterval($_));
}

```

## iterval
* `iterval($pair)`

Returns the value associated with the given key-value pair.

# <a id="string"></a> String Opcodes

## chars
* `chars(str $str)`

Return the number of characters in the string.

## chr
* `chr(int $codepoint)`

Given a unicode codepoint, return a str containing its character. Will
throw an exception on invalid codepoints.

## codepointfromname
* `codepointfromname(str $name)`

Returns the codepoint for the given unicode character name, or -1 if no
match was found.

## concat
* `concat(str $l, str $r)`

Return 