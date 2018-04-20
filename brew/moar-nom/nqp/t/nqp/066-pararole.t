plan(7);

role ParaTest[$a] {
    method m() { $a }
}

class A {
}

my $a1 := A.new();
my $a2 := A.new();
$a1.HOW.mixin($a1, ParaTest.HOW.curry(ParaTest, "foo"));
$a2.HOW.mixin($a2, ParaTest.HOW.curry(ParaTest, "bar"));
is($a1.m, "foo", "mixin of parametric role with first arg");
is($a2.m, "bar", "mixin of parametric role with second arg");

role ParaNameTest[$n, $rv] {
    method ::($n)() { $rv }
}

class B {
}

my $b := B.new();
$b.HOW.mixin($b, ParaNameTest.HOW.curry(ParaNameTest, "drink", "beer"));
$b.HOW.mixin($b, ParaNameTest.HOW.curry(ParaNameTest, "meat", "beef"));
is($b.drink, "beer", "parametric method name handling works (1)");
is($b.meat, "beef", "parametric method name handling works (2)");

grammar LolGrammar {
    token TOP { <foo> }
    proto token foo {*}
    token foo:sym<a> { <sym> }
}
role AnotherFoo[$x] {
    token foo:sym<more> { $x }
}
is(LolGrammar.parse('abc'), 'a', "parametric mixin/grammar/LTM interaction (sanity)");
my $derived := LolGrammar.HOW.mixin(LolGrammar, AnotherFoo.HOW.curry(AnotherFoo, 'ab'));
is($derived.parse('abc'), 'ab', "parametric mixin/grammar/LTM interaction");


grammar LolerGrammar {
    token TOP { <foo> }
    proto token foo {*}
    token foo:sym<a> { <sym> }
}
role YetAnotherFoo[$method, $x] {
    token ::($method) { $x }
}
my $both := LolerGrammar.HOW.mixin(LolerGrammar, YetAnotherFoo.HOW.curry(YetAnotherFoo, 'foo:sym<more>', 'ab'));
is($both.parse('abc'), 'ab', "parametric mixin/grammar/LTM interaction with parametrized method name");
