#! nqp

my @*vms := nqp::list('jvm', 'moar', 'js');

my %documented_ops := find_documented_opcodes();

my %ops := hash_of_vms();

%ops<jvm> := find_opcodes(
    :files([
        "src/vm/jvm/QAST/Compiler.nqp",
        "src/vm/jvm/NQP/Ops.nqp"
    ]),
    :keywords(<map_classlib_core_op add_core_op map_jvm_core_op add_hll_op>)
);

%ops<js> := find_opcodes(
    :files([
        "src/vm/js/Operations.nqp"
    ]),
    :keywords(<add_op add_simple_op add_hll_op add_cmp_op add_infix_op>)
);


%ops<moar> := find_opcodes(
    :files([
        "src/vm/moar/QAST/QASTOperationsMAST.nqp",
        "src/vm/moar/NQP/Ops.nqp"
    ]),
    :keywords(<add_core_op add_core_moarop_mapping add_hll_op add_getattr_op add_bindattr_op>)
);

# Most backends programmatically add these ops - to keep our cheating simple,
# add them to each of the backends manually
for <if unless while until repeat_while repeat_until> -> $op_name {
    for @*vms -> $vm {
        %ops{$vm}{$op_name} := 1;
    }
}

# Are ops that are implemented documented? Fail once per opcode
my %combined_ops := nqp::hash();
for @*vms -> $vm {
    for %ops{$vm} -> $op {
        if !%combined_ops{$op} {
            %combined_ops{$op} := nqp::list($vm);
        } else {
            nqp::push(%combined_ops{$op}, $vm);
        }
    }
}

for %combined_ops -> $opcode {
    my $vms := nqp::join(";", %combined_ops{$opcode});
    ok(%documented_ops<any>{$opcode}, "Opcode '$opcode' ($vms) is documented");
}

# Do documented opcodes actually exist? Fail once per vm if not.
for @*vms -> $vm {
    for %documented_ops{$vm} -> $doc_op {
        ok(%ops{$vm}{$doc_op}, "documented op '$doc_op' exists in $vm");
    }
}

sub find_opcodes(:@files, :@keywords) {
    my %ops := nqp::hash();
    for @files -> $file {
        my @lines := nqp::split("\n", slurp($file));
        for @lines -> $line {
            if $line ~~ / @keywords / {
                my @pieces := nqp::split("'", $line);
                $line := @pieces[1] eq 'nqp' ?? @pieces[3] !! @pieces[1];

                next unless nqp::chars($line);

                if @pieces[1] ne 'nqp' && @pieces[2] ~~ /^ \s* '~' \s* '$suffix' /{
                    for <_s _n _i> -> $suffix {
                        %ops{$line ~ $suffix} := 1;
                    }
                }
                %ops{$line} := 1;
            } elsif $line ~~ /^ \s* for \s* '<' (<[\w\ ]>+) '>' \s* '->' \s* '$func' \s* \{/ -> $match {
                for nqp::split(' ', $match[0]) -> $func {
                    %ops{$func ~ '_n'} := 1;
                }
            } elsif $line ~~ / '%ops<' (<[a..zA..Z0..9_]>+) '> :=' / -> $match {
                if ?$match {
                    %ops{$match[0]} := 1;
                }
            }
        }
    }
    return %ops;
}

sub hash_of_vms() {
    my %hash := nqp::hash();
    for @*vms -> $vm {
        %hash{$vm} := nqp::hash();
    }
    return %hash;
}

sub find_documented_opcodes() {
    my %documented_ops := hash_of_vms();
    %documented_ops<any> := nqp::hash();

    my @doc_lines := nqp::split("\n", slurp("docs/ops.markdown"));
    my @opcode_vms := nqp::list();
    for @doc_lines -> $line {
        my $match := $line ~~ /^ '##' \s* <[a..zA..Z0..9_]>+ \s* ('`' .* '`')? /;
        if ?$match {
            if !?$match[0] {
                @opcode_vms := nqp::clone(@*vms);
            } else {
                @opcode_vms := nqp::list();
                if $match[0] ~~ /jvm/ {
                    nqp::push(@opcode_vms,"jvm");
                }
                if $match[0] ~~ /moar/ {
                    nqp::push(@opcode_vms,"moar");
                }
                if $match[0] ~~ /js/ {
                    nqp::push(@opcode_vms,"js");
                }
            }
        }
        next unless $line ~~ / ^ '* ' .* '(' /;
        $line := nqp::substr($line, 3);
        $line := nqp::split("(", $line)[0];
        for @opcode_vms -> $vm {
            %documented_ops{$vm}{$line} := 1 ;
        }
        %documented_ops<any>{$line} := 1 ;

    }
    return %documented_ops;
}
