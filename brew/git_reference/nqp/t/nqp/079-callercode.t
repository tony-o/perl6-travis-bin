plan(11);

sub foo_inner() {
  my $caller := nqp::callercode();
  $caller(7);
}
sub foo_outer($arg) {
  if $arg == 1 {
    foo_inner();
  } elsif $arg == 7 {
    ok(1,"nqp::callercode returns the correct sub");
  }
}
foo_outer(1);

sub bar($arg) {
  if $arg == 1 {
    bar(2);
  } elsif $arg == 2 {
    my $this := nqp::callercode();
    is(nqp::getcodeobj($this), "first","nqp::getcodeobj works on result of nqp::callercode");
    nqp::setcodeobj($this,"second");
    $this(7);
  } elsif $arg == 7 {
    my $this := nqp::callercode();
    is(nqp::getcodeobj($this), "second","nqp::setcodeobj works on result of nqp::callercode");
  }
}
nqp::setcodeobj(&bar,"first");
bar(1);

class Foo {
  has $!attr;
  method a($arg) {
    if $arg == 1 {
      $!attr := 100;
      self.b(1);
    } elsif $arg == 2 {
      ok(1,'nqp::callercode work for methods'); 
      ok($!attr == 110,'the attribute has the correct value'); 
    }
  }
  method b($arg) {
    $!attr := $!attr + 10;
    my $caller := nqp::callercode();
    $caller(self,2);
  }
}
my $foo := Foo.new();
$foo.a(1);

sub bottom($mode) {
    return 'this is bottom' if $mode == 1;
    nqp::ctx();
}
sub middle($mode) {
    return 'this is middle' if $mode == 1;
    my $bottom_ctx := bottom(0);
    is(nqp::ctxcode($bottom_ctx)(1), "this is bottom", 'ctxcode on a ctx from a sub we called');
    is(nqp::ctxcode(nqp::ctxcaller(nqp::ctx()))(1), 'this is toplevel', 'ctxcode on a caller ctx');
    is(nqp::ctxcode(nqp::ctx())(1), 'this is middle', 'ctxcode on current ctx');
}
sub toplevel($mode) {
    return 'this is toplevel' if $mode == 1;
    middle(0);
}

toplevel(0);
{
    my sub outer() {
        my $*foo := "wow!";
        nqp::handle(nqp::ctx()<$*foo> := 'hello', 'NEXT', say("handling NEXT"));
        is($*foo, 'hello', "nqp::handle doesn't interfere with nqp::ctx");
        nqp::handle(inner(), 'NEXT', say("handling NEXT"));
        is($*foo, 'hi', "nqp::handle doesn't interfere with nqp::ctxcaller");
    }
    my sub inner() {
        nqp::ctxcaller(nqp::ctx())<$*foo> :=  'hi';
    }

    outer();
}

{
    my sub reach_end() {
      my $count := 0;
      my $ctx := nqp::ctx(); 
      while !nqp::isnull($ctx) {
        $ctx := nqp::ctxcaller($ctx);
        $count := $count + 1;
      }

      ok($count > 0 && nqp::isnull($ctx), 'nqp::ctxcaller get nqp::null when we have no caller');
    }
    
    reach_end();
}
