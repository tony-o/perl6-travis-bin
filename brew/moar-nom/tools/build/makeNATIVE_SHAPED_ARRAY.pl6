# This script reads the native_array.pm file from STDIN, and generates the
# shapedintarray, shapednumarray and shapedstrarray roles in it, and writes
# it to STDOUT.

use v6;

my $generator = $*PROGRAM-NAME;
my $generated = DateTime.now.gist.subst(/\.\d+/,'');
my $start     = '#- start of generated part of shaped';
my $idpos     = $start.chars;
my $idchars   = 3;
my $end       = '#- end of generated part of shaped';

# for all the lines in the source that don't need special handling
for $*IN.lines -> $line {

    # nothing to do yet
    unless $line.starts-with($start) {
        say $line;
        next;
    }

    # found header
    my $type = $line.substr($idpos,$idchars);
    die "Don't know how to handle $type" unless $type eq "int" | "num" | "str";
    say $start ~ $type ~ "array role -----------------------------";
    say "#- Generated on $generated by $generator";
    say "#- PLEASE DON'T CHANGE ANYTHING BELOW THIS LINE";

    # skip the old version of the code
    for $*IN.lines -> $line {
        last if $line.starts-with($end);
    }

    # set up template values
    my %mapper =
      postfix => $type.substr(0,1),
      type    => $type,
      Type    => $type.tclc,
    ;

    # spurt the roles
    say Q:to/SOURCE/.subst(/ '#' (\w+) '#' /, -> $/ { %mapper{$0} }, :g).chomp;

    role shaped#type#array does shapedarray {
        multi method AT-POS(::?CLASS:D: **@indices) is raw {
            nqp::if(
              nqp::iseq_i(
                (my int $numdims = nqp::numdimensions(self)),
                (my int $numind  = @indices.elems),  # reifies
              ),
              nqp::stmts(
                (my $indices := nqp::getattr(@indices,List,'$!reified')),
                (my $idxs := nqp::list_i),
                nqp::while(                          # native index list
                  nqp::isge_i(($numdims = nqp::sub_i($numdims,1)),0),
                  nqp::push_i($idxs,nqp::shift($indices))
                ),
#?if moar
                nqp::multidimref_#postfix#(self,$idxs)
#?endif
#?if !moar
                nqp::atposnd_#postfix#(self,$idxs)
#?endif
              ),
              nqp::if(
                nqp::isgt_i($numind,$numdims),
                X::TooManyDimensions.new(
                  operation => 'access',
                  got-dimensions => $numind,
                  needed-dimensions => $numdims
                ).throw,
                X::NYI.new(
                  feature => "Partially dimensioned views of shaped arrays"
                ).throw
              )
            )
        }

        multi method ASSIGN-POS(::?CLASS:D: **@indices) {
            nqp::stmts(
              (my #type# $value = @indices.pop),
              nqp::if(
                nqp::iseq_i(
                  (my int $numdims = nqp::numdimensions(self)),
                  (my int $numind  = @indices.elems),  # reifies
                ),
                nqp::stmts(
                  (my $indices := nqp::getattr(@indices,List,'$!reified')),
                  (my $idxs := nqp::list_i),
                  nqp::while(                          # native index list
                    nqp::isge_i(($numdims = nqp::sub_i($numdims,1)),0),
                    nqp::push_i($idxs,nqp::shift($indices))
                  ),
                  nqp::bindposnd_#postfix#(self, $idxs, $value)
                ),
                nqp::if(
                  nqp::isgt_i($numind,$numdims),
                  X::TooManyDimensions,
                  X::NotEnoughDimensions
                ).new(
                  operation => 'assign to',
                  got-dimensions => $numind,
                  needed-dimensions => $numdims
                ).throw
              )
            )
        }

        sub NATCPY(Mu \to, Mu \from) is raw {
            class :: does Rakudo::Iterator::ShapeLeaf {
                has Mu $!from;
                method INIT(Mu \to, Mu \from) {
                    nqp::stmts(
                      ($!from := from),
                      self.SET-SELF(to)
                    )
                }
                method new(Mu \to, Mu \from) {
                    nqp::create(self).INIT(to,from)
                }
                method result(--> Nil) {
                    nqp::bindposnd_#postfix#($!list,$!indices,
#?if moar
                      nqp::multidimref_#postfix#($!from,$!indices))
#?endif
#?if !moar
                      nqp::atposnd_#postfix#($!from,$!indices))
#?endif
                }
            }.new(to,from).sink-all;
            to
        }
        sub OBJCPY(Mu \to, Mu \from) is raw {
            class :: does Rakudo::Iterator::ShapeLeaf {
                has Mu $!from;
                method INIT(Mu \to, Mu \from) {
                    nqp::stmts(
                      ($!from := nqp::getattr(from,List,'$!reified')),
                      self.SET-SELF(to)
                    )
                }
                method new(Mu \to, Mu \from) {
                    nqp::create(self).INIT(to,from)
                }
                method result(--> Nil) {
                    nqp::bindposnd_#postfix#($!list,$!indices,
                      nqp::atposnd($!from,$!indices))
                }
            }.new(to,from).sink-all;
            to
        }
        sub ITERCPY(Mu \to, Mu \from) is raw {
            class :: does Rakudo::Iterator::ShapeBranch {
                has $!iterators;
                method INIT(\to,\from) {
                    nqp::stmts(
                      self.SET-SELF(to),
                      ($!iterators := nqp::setelems(
                        nqp::list(from.iterator),
                        nqp::add_i($!maxdim,1)
                      )),
                      self
                    )
                }
                method new(\to,\from) { nqp::create(self).INIT(to,from) }
                method done(--> Nil) {
                    nqp::unless(                        # verify lowest
                      nqp::atpos($!iterators,0).is-lazy # finite iterator
                        || nqp::eqaddr(                 # and something there
                             nqp::atpos($!iterators,0).pull-one,IterationEnd),
                      nqp::atposnd_#postfix#($!list,$!indices)    # boom!
                    )
                }
                method process(--> Nil) {
                    nqp::stmts(
                      (my int $i = $!level),
                      nqp::while(
                        nqp::isle_i(($i = nqp::add_i($i,1)),$!maxdim),
                        nqp::if(
                          nqp::eqaddr((my $item :=      # exhausted ?
                            nqp::atpos($!iterators,nqp::sub_i($i,1)).pull-one),
                            IterationEnd
                          ),
                          nqp::bindpos($!iterators,$i,  # add an empty one
                            Rakudo::Iterator.Empty),
                          nqp::if(                      # is it an iterator?
                            nqp::istype($item,Iterable) && nqp::isconcrete($item),
                            nqp::bindpos($!iterators,$i,$item.iterator),
                            X::Assignment::ToShaped.new(shape => $!dims).throw
                          )
                        )
                      ),
                      (my $iter := nqp::atpos($!iterators,$!maxdim)),
                      nqp::until(                       # loop over highest dim
                        nqp::eqaddr((my $pulled := $iter.pull-one),IterationEnd)
                          || nqp::isgt_i(nqp::atpos_i($!indices,$!maxdim),$!maxind),
                        nqp::stmts(
                          nqp::bindposnd_#postfix#($!list,$!indices,$pulled),
                          nqp::bindpos_i($!indices,$!maxdim,  # increment index
                            nqp::add_i(nqp::atpos_i($!indices,$!maxdim),1))
                        )
                      ),
                      nqp::unless(
                        nqp::eqaddr($pulled,IterationEnd) # if not exhausted
                          || nqp::isle_i(                 # and index too high
                               nqp::atpos_i($!indices,$!maxdim),$!maxind)
                          || $iter.is-lazy,               # and not lazy
                        nqp::atposnd_#postfix#($!list,$!indices)  # boom!
                      )
                    )
                }
            }.new(to,from).sink-all;
            to
        }

        multi method STORE(::?CLASS:D: ::?CLASS:D \from) {
            nqp::if(
              EQV_DIMENSIONS(self,from),
              NATCPY(self,from),
              X::Assignment::ArrayShapeMismatch.new(
                source-shape => from.shape,
                target-shape => self.shape
              ).throw
            )
        }
        multi method STORE(::?CLASS:D: array:D \from) {
            nqp::if(
              nqp::istype(from.of,#Type#),
              nqp::if(
                EQV_DIMENSIONS(self,from),
                NATCPY(self,from),
                X::Assignment::ArrayShapeMismatch.new(
                  source-shape => from.shape,
                  target-shape => self.shape
                ).throw
              ),
              X::TypeCheck::Assignment.new(
                symbol   => self.^name ~ '[' ~ self.shape.join(';') ~ ']',
                expected => #Type#,
                got      => from.of
              ).throw
            )
        }
        multi method STORE(::?CLASS:D: Iterable:D \from) {
            nqp::if(
              nqp::can(from,'shape'),
              nqp::if(
                from.shape eqv self.shape,
                OBJCPY(self,from),
                X::Assignment::ArrayShapeMismatch.new(
                    source-shape => from.shape,
                    target-shape => self.shape
                ).throw
              ),
              ITERCPY(self,from)
            )
        }
        method iterator(::?CLASS:D:) {
            class :: does Rakudo::Iterator::ShapeLeaf {
                method result() is raw {
#?if moar
                    nqp::multidimref_#postfix#($!list,nqp::clone($!indices))
#?endif
#?if !moar
                    nqp::atposnd_#postfix#($!list,nqp::clone($!indices))
#?endif
                }
            }.new(self)
        }
        multi method kv(::?CLASS:D:) {
            Seq.new(class :: does Rakudo::Iterator::ShapeLeaf {
                has int $!on-key;
                method result() is raw {
                    nqp::if(
                      ($!on-key = nqp::not_i($!on-key)),
                      nqp::stmts(
                        (my $result := self.indices),
                        (nqp::bindpos_i($!indices,$!maxdim,  # back 1 for next
                          nqp::sub_i(nqp::atpos_i($!indices,$!maxdim),1))),
                        $result
                      ),
#?if moar
                      nqp::multidimref_#postfix#($!list,nqp::clone($!indices))
#?endif
#?if !moar
                      nqp::atposnd_#postfix#($!list,nqp::clone($!indices))
#?endif
                    )
                }
                # needs its own push-all since it fiddles with $!indices
                method push-all($target --> IterationEnd) {
                    nqp::until(
                      nqp::eqaddr((my $pulled := self.pull-one),IterationEnd),
                      $target.push($pulled)
                    )
                }
            }.new(self))
        }
        multi method pairs(::?CLASS:D:) {
            Seq.new(class :: does Rakudo::Iterator::ShapeLeaf {
                method result() {
                    Pair.new(
                      self.indices,
#?if moar
                      nqp::multidimref_#postfix#($!list,nqp::clone($!indices))
#?endif
#?if !moar
                      nqp::atposnd_#postfix#($!list,nqp::clone($!indices))
#?endif
                    )
                }
            }.new(self))
        }
        multi method antipairs(::?CLASS:D:) {
            Seq.new(class :: does Rakudo::Iterator::ShapeLeaf {
                method result() {
                    Pair.new(nqp::atposnd_#postfix#($!list,$!indices),self.indices)
                }
            }.new(self))
        }
    }  # end of shaped#type#array role

    role shaped1#type#array does shaped#type#array {
        multi method AT-POS(::?CLASS:D: int \one) is raw {
           nqp::atposref_#postfix#(self,one)
        }
        multi method AT-POS(::?CLASS:D: Int:D \one) is raw {
           nqp::atposref_#postfix#(self,one)
        }

        multi method ASSIGN-POS(::?CLASS:D: int \one, #type# \value) {
            nqp::bindpos_#postfix#(self,one,value)
        }
        multi method ASSIGN-POS(::?CLASS:D: Int:D \one, #type# \value) {
            nqp::bindpos_#postfix#(self,one,value)
        }
        multi method ASSIGN-POS(::?CLASS:D: int \one, #Type#:D \value) {
            nqp::bindpos_#postfix#(self,one,value)
        }
        multi method ASSIGN-POS(::?CLASS:D: Int:D \one, #Type#:D \value) {
            nqp::bindpos_#postfix#(self,one,value)
        }

        multi method EXISTS-POS(::?CLASS:D: int \one) {
            nqp::p6bool(
              nqp::isge_i(one,0) && nqp::islt_i(one,nqp::elems(self))
            )
        }
        multi method EXISTS-POS(::?CLASS:D: Int:D \one) {
            nqp::p6bool(
              nqp::isge_i(one,0) && nqp::islt_i(one,nqp::elems(self))
            )
        }

        multi method STORE(::?CLASS:D: ::?CLASS:D \from) {
            nqp::if(
              nqp::iseq_i((my int $elems = nqp::elems(self)),nqp::elems(from)),
              nqp::stmts(
                (my int $i = -1),
                nqp::while(
                  nqp::islt_i(($i = nqp::add_i($i,1)),$elems),
                  nqp::bindpos_#postfix#(self,$i,nqp::atpos_#postfix#(from,$i))
                ),
                self
              ),
              X::Assignment::ArrayShapeMismatch.new(
                source-shape => from.shape,
                target-shape => self.shape
              ).throw
            )
        }
        multi method STORE(::?CLASS:D: Iterable:D \in) {
            nqp::stmts(
              (my \iter := in.iterator),
              (my int $elems = nqp::elems(self)),
              (my int $i = -1),
              nqp::until(
                nqp::eqaddr((my $pulled := iter.pull-one),IterationEnd)
                  || nqp::iseq_i(($i = nqp::add_i($i,1)),$elems),
                nqp::bindpos_#postfix#(self,$i,$pulled)
              ),
              nqp::unless(
                nqp::islt_i($i,$elems) || iter.is-lazy,
                nqp::atpos_#postfix#(list,$i) # too many values on non-lazy it
              ),
              self
            )
        }
        multi method STORE(::?CLASS:D: #Type#:D \item) {
            nqp::stmts(
              nqp::bindpos_#postfix#(self,0,item),
              self
            )
        }
        method iterator(::?CLASS:D:) {
            class :: does Iterator {
                has Mu $!list;
                has int $!pos;
                method !SET-SELF(Mu \list) {
                    nqp::stmts(
                      ($!list := list),
                      ($!pos = -1),
                      self
                    )
                }
                method new(Mu \list) { nqp::create(self)!SET-SELF(list) }
                method pull-one() is raw {
                    nqp::if(
                      nqp::islt_i(
                        ($!pos = nqp::add_i($!pos,1)),
                        nqp::elems($!list)
                      ),
                      nqp::atposref_#postfix#($!list,$!pos),
                      IterationEnd
                    )
                }
                method push-all($target --> IterationEnd) {
                    nqp::stmts(
                      (my int $elems = nqp::elems($!list)),
                      (my int $pos = $!pos),
                      nqp::while(
                        nqp::islt_i(($pos = nqp::add_i($pos,1)),$elems),
                        $target.push(nqp::atpos_#postfix#($!list,$pos))
                      ),
                      ($!pos = $pos)
                    )
                }
                method count-only() { nqp::p6box_i(nqp::elems($!list)) }
                method bool-only()  { nqp::p6bool(nqp::elems($!list)) }
                method sink-all(--> IterationEnd) {
                    $!pos = nqp::elems($!list)
                }
            }.new(self)
        }
        multi method kv(::?CLASS:D:) {
            my int $i = -1;
            my int $elems = nqp::add_i(nqp::elems(self),nqp::elems(self));
            Seq.new(Rakudo::Iterator.Callable({
                nqp::if(
                  nqp::islt_i(($i = nqp::add_i($i,1)),$elems),
                  nqp::if(
                    nqp::bitand_i($i,1),
                    nqp::atposref_#postfix#(self,nqp::bitshiftr_i($i,1)),
                    nqp::bitshiftr_i($i,1)
                  ),
                  IterationEnd
                )
            }))
        }
        multi method pairs(::?CLASS:D:) {
            my int $i = -1;
            my int $elems = nqp::elems(self);
            Seq.new(Rakudo::Iterator.Callable({
                nqp::if(
                  nqp::islt_i(($i = nqp::add_i($i,1)),$elems),
                  Pair.new($i,nqp::atposref_#postfix#(self,$i)),
                  IterationEnd
                )
            }))
        }
        multi method antipairs(::?CLASS:D:) {
            Seq.new(Rakudo::Iterator.AntiPair(self.iterator))
        }
        method reverse(::?CLASS:D:) is nodal {
            nqp::stmts(
              (my int $elems = nqp::elems(self)),
              (my int $last  = nqp::sub_i($elems,1)),
              (my int $i     = -1),
              (my $to := nqp::clone(self)),
              nqp::while(
                nqp::islt_i(($i = nqp::add_i($i,1)),$elems),
                nqp::bindpos_#postfix#($to,nqp::sub_i($last,$i),
                  nqp::atpos_#postfix#(self,$i))
              ),
              $to
            )
        }
        method rotate(::?CLASS:D: Int(Cool) $rotate = 1) is nodal {
            nqp::stmts(
              (my int $elems = nqp::elems(self)),
              (my $to := nqp::clone(self)),
              (my int $i = -1),
              (my int $j =
                nqp::mod_i(nqp::sub_i(nqp::sub_i($elems,1),$rotate),$elems)),
              nqp::if(nqp::islt_i($j,0),($j = nqp::add_i($j,$elems))),
              nqp::while(
                nqp::islt_i(($i = nqp::add_i($i,1)),$elems),
                nqp::bindpos_#postfix#(
                  $to,
                  ($j = nqp::mod_i(nqp::add_i($j,1),$elems)),
                  nqp::atpos_#postfix#(self,$i)
                ),
              ),
              $to
            )
        }
    } # end of shaped1#type#array role

    role shaped2#type#array does shaped#type#array {
        multi method AT-POS(::?CLASS:D: int \one, int \two) is raw {
#?if moar
            nqp::multidimref_#postfix#(self,nqp::list_i(one, two))
#?endif
#?if !moar
            nqp::atpos2d_#postfix#(self,one,two)
#?endif
        }
        multi method AT-POS(::?CLASS:D: Int:D \one, Int:D \two) is raw {
#?if moar
            nqp::multidimref_#postfix#(self,nqp::list_i(one, two))
#?endif
#?if !moar
            nqp::atpos2d_#postfix#(self,one,two)
#?endif
        }

        multi method ASSIGN-POS(::?CLASS:D: int \one, int \two, #Type#:D \value) {
            nqp::bindpos2d_#postfix#(self,one,two,value)
        }
        multi method ASSIGN-POS(::?CLASS:D: Int:D \one, Int:D \two, #Type#:D \value) {
            nqp::bindpos2d_#postfix#(self,one,two,value)
        }

        multi method EXISTS-POS(::?CLASS:D: int \one, int \two) {
            nqp::p6bool(
              nqp::isge_i(one,0)
                && nqp::isge_i(two,0)
                && nqp::islt_i(one,nqp::atpos_i(nqp::dimensions(self),0))
                && nqp::islt_i(two,nqp::atpos_i(nqp::dimensions(self),1))
            )
        }
        multi method EXISTS-POS(::?CLASS:D: Int:D \one, Int:D \two) {
            nqp::p6bool(
              nqp::isge_i(one,0)
                && nqp::isge_i(two,0)
                && nqp::islt_i(one,nqp::atpos_i(nqp::dimensions(self),0))
                && nqp::islt_i(two,nqp::atpos_i(nqp::dimensions(self),1))
            )
        }
    } # end of shaped2#type#array role

    role shaped3#type#array does shaped#type#array {
        multi method AT-POS(::?CLASS:D: int \one, int \two, int \three) is raw {
#?if moar
            nqp::multidimref_#postfix#(self,nqp::list_i(one, two, three))
#?endif
#?if !moar
            nqp::atpos3d_#postfix#(self,one,two,three)
#?endif
        }
        multi method AT-POS(::?CLASS:D: Int:D \one, Int:D \two, Int:D \three) is raw {
#?if moar
            nqp::multidimref_#postfix#(self,nqp::list_i(one, two, three))
#?endif
#?if !moar
            nqp::atpos3d_#postfix#(self,one,two,three)
#?endif
        }

        multi method ASSIGN-POS(::?CLASS:D: int \one, int \two, int \three, #Type#:D \value) {
            nqp::bindpos3d_#postfix#(self,one,two,three,value)
        }
        multi method ASSIGN-POS(::?CLASS:D: Int:D \one, Int:D \two, Int:D \three, #Type#:D \value) {
            nqp::bindpos3d_#postfix#(self,one,two,three,value)
        }

        multi method EXISTS-POS(::?CLASS:D: int \one, int \two, int \three) {
            nqp::p6bool(
              nqp::isge_i(one,0)
                && nqp::isge_i(two,0)
                && nqp::isge_i(three,0)
                && nqp::islt_i(one,nqp::atpos_i(nqp::dimensions(self),0))
                && nqp::islt_i(two,nqp::atpos_i(nqp::dimensions(self),1))
                && nqp::islt_i(three,nqp::atpos_i(nqp::dimensions(self),2))
            )
        }
        multi method EXISTS-POS(::?CLASS:D: Int:D \one, Int:D \two, Int:D \three) {
            nqp::p6bool(
              nqp::isge_i(one,0)
                && nqp::isge_i(two,0)
                && nqp::isge_i(three,0)
                && nqp::islt_i(one,nqp::atpos_i(nqp::dimensions(self),0))
                && nqp::islt_i(two,nqp::atpos_i(nqp::dimensions(self),1))
                && nqp::islt_i(three,nqp::atpos_i(nqp::dimensions(self),2))
            )
        }
    } # end of shaped3#type#array role
SOURCE

    # we're done for this role
    say "#- PLEASE DON'T CHANGE ANYTHING ABOVE THIS LINE";
    say $end ~ $type ~ "array role -------------------------------";
}
