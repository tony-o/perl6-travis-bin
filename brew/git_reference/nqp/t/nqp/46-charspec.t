#! nqp

# check literals

plan(3);

ok("\c111\c107 \c49" eq 'ok 1', '\c###');
ok("\c[111,107,32,50]" eq 'ok 2', '\c[##,##,##]');

# TODO: This test only fails for nqp-p :-(
# ok("\c[LATIN SMALL LETTER O, LATIN SMALL LETTER K, SPACE, DIGIT THREE]" eq 'ok 3', '\c[name,name]');

ok("\e" eq "\c[27]", '\e');
