my role QuantHash does Associative {
    method Int     ( --> Int)     { self.total.Int }
    method Num     ( --> Num)     { self.total.Num }
    method Numeric ( --> Numeric) { self.total.Numeric }
    method Real    ( --> Real)    { self.total.Real }

    method list() { self.pairs.cache }

    method minpairs {
        my @found;
        my $min = Inf;
        for self.pairs {
            my $value := .value;
            if $value > $min {
                next;
            }
            elsif $value < $min {
                @found = $_;
                $min = $value;
            }
            else {
                @found.push: $_;
            }
        }
        @found;
    }

    method maxpairs {
        my @found;
        my $max = -Inf;
        for self.pairs {
            my $value := .value;
            if $value < $max {
                next;
            }
            elsif $value > $max {
                @found = $_;
                $max = $value;
            }
            else {
                @found.push: $_;
            }
        }
        @found;
    }

    method fmt(QuantHash: Cool $format = "%s\t\%s", $sep = "\n") {
        if nqp::p6box_i(nqp::sprintfdirectives( nqp::unbox_s($format.Stringy) )) == 1 {
            self.keys.fmt($format, $sep);
        }
        else {
            self.pairs.fmt($format, $sep);
        }
    }
}

# vim: ft=perl6 expandtab sw=4
