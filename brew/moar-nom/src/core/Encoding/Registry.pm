my class X::Encoding::Unknown { ... }
my class X::Encoding::AlreadyRegistered { ... }

my class Encoding::Registry {
    my $lock := Lock.new;
    my %lookup;  # access for registering builtins at compile time
    my $lookup := nqp::getattr(%lookup,Map,'$!storage');  # access for runtime

    BEGIN {
        my $lookup := nqp::bindattr(%lookup,Map,'$!storage',nqp::hash);
        my $encodings := nqp::list(
          nqp::list("ascii"),
          nqp::list("iso-8859-1","iso_8859-1:1987","iso_8859-1","iso-ir-100",
            "latin1","latin-1","csisolatin1","l1","ibm819","cp819"),
          nqp::list("utf8","utf-8"),
          nqp::list("utf8-c8","utf-8-c8"),
          nqp::list("utf16","utf-16"),
          nqp::list("utf32","utf-32"),
          nqp::list("windows-1252")
        );
        my int $i = -1;
        my int $elems = nqp::elems($encodings);
        while nqp::islt_i(($i = nqp::add_i($i,1)),$elems) {
            my $names := nqp::atpos($encodings,$i);
            my $builtin := nqp::create(Encoding::Builtin).SET-SELF(
              nqp::shift($names),nqp::clone($names));
            nqp::bindkey($lookup,$builtin.name,$builtin);
            while nqp::elems($names) {
                nqp::bindkey($lookup,nqp::shift($names),$builtin);
            }
        }
    }

    method register(Encoding $enc --> Nil) {
        $lock.protect: {
            nqp::stmts(
              nqp::if(
                nqp::existskey($lookup,(my str $key = $enc.name.fc)),
                X::Encoding::AlreadyRegistered.new(name => $enc.name).throw,
                nqp::bindkey($lookup,$key,$enc)
              ),
              (my $names :=
                nqp::getattr($enc.alternative-names,List,'$!reified')),
              (my int $elems = nqp::elems($names)),
              (my int $i = -1),
              nqp::while(
                nqp::islt_i(($i = nqp::add_i($i,1)),$elems),
                nqp::if(
                  nqp::existskey($lookup,($key = nqp::atpos($names,$i).fc)),
                  X::Encoding::AlreadyRegistered.new(
                    name => nqp::atpos($names,$i)).throw,
                  nqp::bindkey($lookup,$key,$enc)
                )
              )
            )
        }
    }

    method find(Str() $name) {
        $lock.protect: {
            nqp::ifnull(
              nqp::atkey($lookup,$name.fc),
              X::Encoding::Unknown.new(:$name).throw
            )
        }
    }
}

# vim: ft=perl6 expandtab sw=4
