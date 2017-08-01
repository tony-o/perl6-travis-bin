# Stub a few things the compiler wants to have really early on.
my class Pair { ... }
my class Whatever { ... }
my class HyperWhatever { ... }
my class WhateverCode { ... }
my class Cursor { ... }
my class Failure { ... }
my class Rakudo::Internals { ... }
my class X::Numeric::Overflow { ... }
my class X::Numeric::Underflow { ... }

# Stub these or we can't use any sigil other than $.
my role Positional { ... }
my role Associative { ... }
my role Callable { ... }
my role Iterable { ... }
my role PositionalBindFailover { ... }

# Set up Empty, which is a Slip created with an empty IterationBuffer (which
# we also stub here). This is needed in a bunch of simple constructs (like if
# with only one branch).
my class IterationBuffer is repr('VMArray') { ... }
my constant Empty = nqp::p6bindattrinvres(nqp::create(Slip),
    List, '$!reified', nqp::create(IterationBuffer));

# The value for \n.
my constant $?NL = "\x0A";

# vim: ft=perl6 expandtab sw=4
