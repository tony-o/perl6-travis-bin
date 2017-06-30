plan(4);

sub foo($arg) {
  my $this := nqp::curcode();
  if $arg == 1 {
    $this(7);
  } elsif $arg == 7 {
    ok("nqp::curcode returns the correct sub");
  }
}
foo(1);

sub bar($arg) {
  my $this := nqp::curcode();
  if $arg == 1 {
    ok(nqp::getcodeobj($this) eq "first","nqp::getcodeobj works on result of nqp::curcode");
    nqp::setcodeobj($this,"second");
    $this(7);
  } elsif $arg == 7 {
    ok(nqp::getcodeobj($this) eq "second","nqp::setcodeobj works on result of nqp::curcode");
  }
}
nqp::setcodeobj(&bar,"first");
bar(1);

class Foo {
  method foo($arg) {
    my $this := nqp::curcode();
    if $arg == 1 {
      nqp::setcodeobj($this,"third");
      $this(self, 7);
    } elsif $arg == 7 {
      ok(nqp::getcodeobj($this) eq "third","nqp::curcode in combination with methods");
    }
  }
}

Foo.foo(1);
