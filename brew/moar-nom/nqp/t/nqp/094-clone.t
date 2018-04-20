plan(13);
sub foo() {
    'hello there';
}
my $a := &foo;
my $b := nqp::clone($a);

nqp::setcodeobj($a, "obj");
is(nqp::getcodename($a), 'foo', 'sanity check');
is(nqp::getcodename($b), 'foo', 'cloned sub has correct name');
is($b(), 'hello there', 'you can call a cloned sub');
nqp::setcodename($b, 'bumblebee');
is(nqp::getcodename($a), 'foo', "changing the name on the cloned sub doesn't affect orginal");
is(nqp::getcodename($b), 'bumblebee', "you can change the name on the cloned sub");

class Foo {
    has $!foo;
    method get_foo() {
        $!foo;
    }
    method set_foo($value) {
        $!foo := $value;
    }
}

ok(nqp::eqaddr(Foo, Foo), 'nqp::clone on a type object returns it');

{
    my $c := Foo.new(foo => 123);
    my $d := nqp::clone($c);
    $c.set_foo(456);
    ok($d.get_foo == 123, "changing an attr on the cloned object doesn't affect the orginal");
    ok($c.get_foo == 456, "we can change an attr on the new object");
}

my $list := nqp::list(0,100,200,300, nqp::list(400, 401, 402));

my $cloned := nqp::clone($list);

$cloned[2] := 20;

ok($cloned[2] == 20, "we can change an element of a cloned array");
ok($list[2] == 200, "...and the original array remains unchanged");

$cloned[4][2] := 42;

ok($list[4][2] == 42, "cloning is shallow");

{
    my %hash;
    %hash<foo> := 123;
    my $cloned_hash := nqp::clone(%hash);
    ok($cloned_hash<foo> == 123, "the keys in a hash are cloned");
    $cloned_hash<foo> := 456;
    ok(%hash<foo> == 123, "changing the cloned hash doesn't affect the orignal");
}
