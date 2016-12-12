# This file contains a bunch of classes related to static optimization of Perl
# 6 programs. It takes place after we've done all of the stuff in the grammar
# and actions, which means CHECK time is over. Thus we're allowed to assume that
# lexpads are immutable, declarations are over and done with, multi candidate
# lists won't change and so forth.

use NQPP6QRegex;
use QAST;
use Perl6::Ops;

# A null QAST node, inserted when we want to eliminate something.
my $NULL := QAST::Op.new( :op<null> );

# Represents the current set of blocks we're in and thus the symbols they
# make available, and allows for queries over them.
my class Symbols {
    # The nested blocks we're in; it's the lexical chain, essentially.
    has @!block_stack;

    # Some interesting scopes.
    has $!GLOBALish;
    has $!UNIT;
    has $!SETTING;

    # Cached setting lookups.
    has %!SETTING_CACHE;

    # Some interesting symbols.
    has $!Mu;
    has $!Any;
    has $!Block;
    has $!PseudoStash;
    has $!Routine;
    has $!Nil;
    has $!Failure;

    # Top routine, for faking it when optimizing post-inline.
    has $!fake_top_routine;

    # Constructs a new instance of the symbols handling class.
    method new($compunit) {
        my $obj := nqp::create(self);
        $obj.BUILD($compunit);
        $obj
    }
    method BUILD($compunit) {
        @!block_stack   := [$compunit[0]];
        $!GLOBALish     := $compunit.ann('GLOBALish');
        $!UNIT          := $compunit.ann('UNIT');
        %!SETTING_CACHE := {};
        unless nqp::istype($!UNIT, QAST::Block) {
            nqp::die("Optimizer could not find UNIT");
        }
        nqp::push(@!block_stack, $!UNIT);
        $!Mu          := self.find_lexical('Mu');
        $!Any         := self.find_lexical('Any');
        $!Block       := self.find_lexical('Block');
        $!PseudoStash := self.find_lexical('PseudoStash');
        $!Routine     := self.find_lexical('Routine');
        $!Nil         := self.find_lexical('Nil');
        $!Failure     := self.find_lexical('Failure');
        nqp::pop(@!block_stack);
    }

    # Block handling.
    method push_block($block) {
        nqp::push(@!block_stack, $block);
    }
    method pop_block() {
        nqp::pop(@!block_stack)
    }
    method top_block() {
        @!block_stack[+@!block_stack - 1]
    }
    method top_routine() {
        return $!fake_top_routine if $!fake_top_routine;
        my int $i := nqp::elems(@!block_stack);
        while $i > 0 {
            $i := $i - 1;
            my $co := @!block_stack[$i].ann('code_object');
            if nqp::istype($co, $!Routine) {
                return $co;
            }
        }
        NQPMu
    }
    method faking_top_routine($fake, $run) {
        $!fake_top_routine := $fake;
        my $result := $run();
        $!fake_top_routine := NQPMu;
        $result;
    }
    
    # Accessors for interesting symbols/scopes.
    method GLOBALish()   { $!GLOBALish }
    method UNIT()        { $!UNIT }
    method Mu()          { $!Mu }
    method Any()         { $!Any }
    method Block()       { $!Block }
    method PseudoStash() { $!PseudoStash }
    method Nil()         { $!Nil }
    method Failure()     { $!Failure }

    # The following function is a nearly 1:1 copy of World.find_symbol.
    # Finds a symbol that has a known value at compile time from the
    # perspective of the current scope. Checks for lexicals, then if
    # that fails tries package lookup.
    method find_symbol(@name) {
        # Make sure it's not an empty name.
        unless +@name { nqp::die("Cannot look up empty name"); }

        # GLOBAL is current view of global.
        if +@name == 1 && @name[0] eq 'GLOBAL' {
            return $!GLOBALish;
        }

        # If it's a single-part name, look through the lexical
        # scopes.
        if +@name == 1 {
            my $final_name := @name[0];
            my int $i := +@!block_stack;
            while $i > 0 {
                $i := $i - 1;
                my %sym := @!block_stack[$i].symbol($final_name);
                if +%sym {
                    return self.force_value(%sym, $final_name, 1);
                }
            }
        }

        # If it's a multi-part name, see if the containing package
        # is a lexical somewhere. Otherwise we fall back to looking
        # in GLOBALish.
        my $result := $!GLOBALish;
        if +@name >= 2 {
            my $first := @name[0];
            my int $i := +@!block_stack;
            while $i > 0 {
                $i := $i - 1;
                my %sym := @!block_stack[$i].symbol($first);
                if +%sym {
                    $result := self.force_value(%sym, $first, 1);
                    @name := nqp::clone(@name);
                    @name.shift();
                    $i := 0;
                }
            }
        }

        # Try to chase down the parts of the name.
        for @name {
            if nqp::existskey($result.WHO, ~$_) {
                $result := ($result.WHO){$_};
            }
            else {
                nqp::die("Could not locate compile-time value for symbol " ~
                    join('::', @name));
            }
        }

        $result;
    }

    # Locates a lexical symbol and returns its compile time value. Dies if
    # it does not exist.
    method find_lexical($name) {
        my int $i := +@!block_stack;
        while $i > 0 {
            $i := $i - 1;
            my $block := @!block_stack[$i];
            my %sym := $block.symbol($name);
            if +%sym {
                return self.force_value(%sym, $name, 1);
            }
        }
        nqp::die("Optimizer: No lexical $name found");
    }

    method find_lexical_symbol($name) {
        my int $i := +@!block_stack;
        while $i > 0 {
            $i := $i - 1;
            my $block := @!block_stack[$i];
            my %sym := $block.symbol($name);
            return %sym if +%sym;
        }
        nqp::die("Optimizer: No lexical $name found");
    }

    # Checks if a given lexical is declared, though it needn't have a compile
    # time known value.
    method is_lexical_declared($name) {
        my int $i := +@!block_stack;
        while $i > 0 {
            $i := $i - 1;
            my $block := @!block_stack[$i];
            my %sym := $block.symbol($name);
            if +%sym {
                return 1;
            }
        }
        0
    }

    # Forces a value to be made available.
    method force_value(%sym, $key, int $die) {
        if nqp::existskey(%sym, 'value') {
            %sym<value>
        }
        elsif nqp::existskey(%sym, 'lazy_value_from') {
            %sym<value> := nqp::atkey(nqp::atkey(%sym, 'lazy_value_from'), $key)
        }
        else {
            $die ?? nqp::die("No compile-time value for $key") !! NQPMu
        }
    }

    # Works out how many scopes in from the outermost a given name is. A 0
    # from this means the nearest declaration is from the setting; a 1 means
    # it is in the mainline, etc.
    method scopes_in($name) {
        my int $i := +@!block_stack;
        while $i > 0 {
            $i := $i - 1;
            my $block := @!block_stack[$i];
            my %sym := $block.symbol($name);
            if +%sym {
                return $i;
            }
        }
        nqp::die("Symbol $name not found");
    }

    method is_from_core($name) {
        my int $i := +@!block_stack;
        while $i > 0 {
            $i := $i - 1;
            my $block := @!block_stack[$i];
            my %sym := $block.symbol($name);
            if +%sym && nqp::existskey(%sym, 'value') {
                my %sym := $block.symbol("!CORE_MARKER");
                if +%sym {
                    return 1;
                }
                return 0;
            }
        }
        return 0;
    }

    method find_in_setting($symbol) {
        if !nqp::defined($!SETTING) {
            my int $i := +@!block_stack;
            while $i > 0 && !nqp::defined($!SETTING) {
                $i := $i - 1;
                my $block := @!block_stack[$i];
                my %sym := $block.symbol("!CORE_MARKER");
                if +%sym {
                    $!SETTING := $block;
                }
            }
            if !nqp::defined($!SETTING) {
                nqp::die("Optimizer couldn't find CORE while looking for $symbol.");
            }
        } else {
            if nqp::existskey(%!SETTING_CACHE, $symbol) {
                return %!SETTING_CACHE{$symbol};
            }
        }
        my %sym := $!SETTING.symbol($symbol);
        if +%sym {
            return %!SETTING_CACHE{$symbol} := self.force_value(%sym, $symbol, 1);
        }
    }
}

# Tracks problems (errors and warnings) discovered by the optimizer as it
# does its work.
my class Problems {
    # Symbols object.
    has $!symbols;

    # Things that should be warned about; keys are warnings, value is an array
    # of line numbers.
    has %!worrying;

    # Typed exceptions, these are all deadly currently
    has @!exceptions;

    method new($symbols) {
        my $obj := nqp::create(self);
        $obj.BUILD($symbols);
        $obj
    }
    method BUILD($symbols) {
        $!symbols    := $symbols;
        %!worrying   := nqp::hash();
        @!exceptions := [];
    }

    method add_exception(@name, $op, *%opts) {
        %opts<line>            := HLL::Compiler.lineof($op.node.orig, $op.node.from, :cache(1));
        %opts<modules>         := $*W.p6ize_recursive(@*MODULES);

        # get line numbers - we can't use $*W.locprepost here
        # because the cursor has .from as .pos
        # in contrast to node
        my $pos  := $op.node.from;
        my $orig := $op.node.orig;

        my $prestart := $pos - 40;
        $prestart := 0 if $prestart < 0;
        my $pre := nqp::substr($orig, $prestart, $pos - $prestart);
        $pre    := subst($pre, /.*\n/, "", :global);
        $pre    := '<BOL>' if $pre eq '';

        my $postchars := $pos + 40 > nqp::chars($orig) ?? nqp::chars($orig) - $pos !! 40;
        my $post := nqp::substr($orig, $pos, $postchars);
        $post    := subst($post, /\n.*/, "", :global);
        $post    := '<EOL>' if $post eq '';

        %opts<pre>             := nqp::box_s($pre, $!symbols.find_symbol(['Str']));
        %opts<post>            := nqp::box_s($post, $!symbols.find_symbol(['Str']));
        %opts<is-compile-time> := nqp::p6bool(1);

        for %opts -> $p {
            if nqp::islist($p.value) {
                my @a := [];
                for $p.value {
                    nqp::push(@a, nqp::hllizefor($_, 'perl6'));
                }
                %opts{$p.key} := nqp::hllizefor(@a, 'perl6');
            }
            else {
                %opts{$p.key} := nqp::hllizefor($p.value, 'perl6');
            }
        }
        my $file        := nqp::getlexdyn('$?FILES');
        %opts<filename> := nqp::box_s(
            (nqp::isnull($file) ?? '<unknown file>' !! $file),
            $!symbols.find_symbol(['Str'])
        );

        my $exsym  := $!symbols.find_symbol(@name);
        my $x_comp := $!symbols.find_symbol(['X', 'Comp']);
        unless nqp::istype($exsym, $x_comp) {
            $exsym := $exsym.HOW.mixin($exsym, $x_comp);
        }

        my $ex := $exsym.new(|%opts);
        nqp::push(@!exceptions, $ex);
    }

    method add_worry($past_node, $message, @extras?) {
        self.add_memo($past_node, $message, @extras, :type<worry>);
    }

    method add_memo($past_node, $message, @extras?, :$type!) {
        my $mnode := $past_node.node;
        if !nqp::can($mnode,'orig') {
            # note("[DISLOCATED MESSAGE] " ~ $message);
            return;
        }
        my $line := HLL::Compiler.lineof($mnode.orig, $mnode.from, :cache(1));
        my $key := $message ~ (+@extras ?? "\n" ~ join("\n", @extras) !! "");
        my %cont := %!worrying;
        unless %cont{$key} {
            %cont{$key} := [];
        }
        %cont{$key}.push($line);
    }

    method report() {
        if +@!exceptions {
            if +@!exceptions > 1 {
                my $x_comp_group_sym := $!symbols.find_symbol(['X', 'Comp', 'Group']);
                my @exs := [];
                for @!exceptions {
                    nqp::push(@exs, $_);
                }
                my $x_comp_group := $x_comp_group_sym.new(:sorrows(@exs));
                $x_comp_group.throw();
            }
            else {
                @!exceptions[0].throw();
            }
        }

        # We didn't die from any Exception, so we print warnings now.
        if +%!worrying {
            my $err := nqp::getstderr();
            nqp::sayfh($err, "WARNINGS for " ~ $*W.current_file ~ ":");
            my @fails;
            for %!worrying {
                nqp::printfh($err, $_.key ~ " (line" ~ (+$_.value == 1 ?? ' ' !! 's ') ~
                    join(', ', $_.value) ~ ")\n");
            }
        }
    }
}

# Implements analsyis related to variable declarations within a block, which
# includes lexical to local handling and deciding when immediate blocks may
# be flattened into their surrounding block.
my class BlockVarOptimizer {
    # Hash mapping variable names declared in the block to the QAST::Var
    # of its declaration.
    has %!decls;

    # Usages of variables in this block, or unioned in from an inlined
    # immediate block.
    has %!usages_flat;

    # Usages of variables in this block, or unioned in from a non-inlined
    # immediate block or a declaration block.
    has %!usages_inner;

    # Have we seen this block (or an inner one) making calls?
    has int $!calls;

    # Usages of getlexouter.
    has @!getlexouter_binds;

    # Setup and bind (hopefully one each) of %_.
    has @!autoslurpy_setups;
    has @!autoslurpy_binds;

    # The takedispatcher operation.
    has $!takedispatcher;

    # If lowering is, for some reason, poisoned.
    has int $!poisoned;

    # If p6bindsig is used.
    has int $!uses_bindsig;

    method add_decl($var) {
        my str $scope := $var.scope;
        if $scope eq 'lexical' || $scope eq 'lexicalref' {
            %!decls{$var.name} := $var;
        }
    }

    method add_usage($var) {
        my str $scope := $var.scope;
        if $scope eq 'lexical' || $scope eq 'lexicalref' {
            my $name   := $var.name;
            my @usages := %!usages_flat{$name};
            unless @usages {
                @usages := [];
                %!usages_flat{$name} := @usages;
            }
            nqp::push(@usages, $var);
        }
    }

    method register_call() { $!calls++; }

    method register_getlexouter_bind($node) {
        nqp::push(@!getlexouter_binds, $node);
    }

    method register_autoslurpy_setup($node) {
        nqp::push(@!autoslurpy_setups, $node);
    }

    method register_autoslurpy_bind($node) {
        nqp::push(@!autoslurpy_binds, $node);
    }

    method register_takedispatcher($node) {
        $!takedispatcher := $node;
    }

    method poison_lowering() { $!poisoned := 1; }

    method uses_bindsig() { $!uses_bindsig := 1; }

    method get_decls() { %!decls }

    method get_usages_flat() { %!usages_flat }

    method get_usages_inner() { %!usages_inner }

    method get_calls() { $!calls }

    method is_poisoned() { $!poisoned }

    method is_flattenable() {
        for %!decls {
            my $var := $_.value;
            my str $scope := $var.scope;
            return 0 if $scope eq 'lexical' || $scope eq 'lexicalref';
            return 0 if $var.decl eq 'param';
        }
        1
    }

    method incorporate_inner($vars_info, $flattened) {
        # We'll exclude anything that the inner or flattened thing has as
        # a declaration, since those are its own.
        my %decls := $vars_info.get_decls;

        # Inner ones always go into our inners set.
        add_to_set(%!usages_inner, $vars_info.get_usages_inner, %decls);

        # Flat ones depend on if we flattened this block into ourself.
        add_to_set($flattened ?? %!usages_flat !! %!usages_inner,
            $vars_info.get_usages_flat, %decls);

        # Add up call counts.
        $!calls := $!calls + $vars_info.get_calls;

        sub add_to_set(%set, %to_add, %exclude) {
            for %to_add {
                my $name := $_.key;
                next if nqp::existskey(%exclude, $name);
                my @existing := %set{$name};
                if @existing {
                    for $_.value { nqp::push(@existing, $_) }
                    #nqp::splice(@existing, $_.value, 0, 0);
                }
                else {
                    %set{$name} := $_.value;
                }
            }
        }
    }

    method delete_unused_magicals($block) {
        # Magicals are contextual, so if we call anything when we're done for.
        return 0 if $!calls || $!poisoned || $!uses_bindsig;

        # Otherwise see if there's any we can kill.
        my %kill;
        if nqp::existskey(%!decls, '$/') {
            if !nqp::existskey(%!usages_flat, '$/') && !nqp::existskey(%!usages_inner, '$/') {
                %kill<$/> := 1;
                nqp::deletekey(%!decls, '$/');
            }
        }
        if nqp::existskey(%!decls, '$!') {
            if !nqp::existskey(%!usages_flat, '$!') && !nqp::existskey(%!usages_inner, '$!') {
                %kill<$!> := 1;
                nqp::deletekey(%!decls, '$!');
            }
        }
        if nqp::existskey(%!decls, '$¢') {
            if !nqp::existskey(%!usages_flat, '$¢') && !nqp::existskey(%!usages_inner, '$¢') {
                %kill<$¢> := 1;
                nqp::deletekey(%!decls, '$¢');
            }
        }
        if nqp::existskey(%!decls, '$_') {
            my str $decl := %!decls<$_>.decl;
            if $decl eq 'var' || $decl eq 'contvar' {
                if !nqp::existskey(%!usages_flat, '$_') && !nqp::existskey(%!usages_inner, '$_') {
                    if !@!getlexouter_binds {
                        %kill<$_> := 1;
                        nqp::deletekey(%!decls, '$_');
                    }
                    elsif nqp::elems(@!getlexouter_binds) == 1 {
                        my $glob := @!getlexouter_binds[0];
                        if $glob[0].name eq '$_' && $glob[1][0].value eq '$_' {
                            $glob.op('null');
                            $glob.shift(); $glob.shift();
                            %kill<$_> := 1;
                            nqp::deletekey(%!decls, '$_');
                        }
                    }
                }
            }
        }

        # If we found things to eliminate, do so.
        if %kill {
            my @setups := @($block[0]);
            my int $i  := 0;
            my int $n  := nqp::elems(@setups);
            while $i < $n {
                my $consider := @setups[$i];
                if nqp::istype($consider, QAST::Var) && nqp::existskey(%kill, $consider.name) {
                    @setups[$i] := $NULL;
                }
                $i++;
            }
        }
    }

    method delete_unused_autoslurpy() {
        if !$!poisoned && !$!uses_bindsig && nqp::existskey(%!decls, '%_')
                && nqp::elems(@!autoslurpy_setups) == 1
                && nqp::elems(@!autoslurpy_binds) == 1 {
            if !nqp::existskey(%!usages_inner, '%_') && nqp::elems(%!usages_flat<%_>) == 1 {
                my $to_null := @!autoslurpy_setups[0];
                nqp::shift($to_null) while @($to_null);
                $to_null.op('null');
                $to_null := @!autoslurpy_binds[0];
                nqp::shift($to_null) while @($to_null);
                $to_null.op('null');
            }
        }
    }

    method simplify_takedispatcher() {
        unless $!calls || $!uses_bindsig {
            if $!takedispatcher {
                $!takedispatcher.op('cleardispatcher');
                $!takedispatcher.shift();
            }
        }
    }

    method lexical_vars_to_locals($block) {
        return 0 if $!poisoned || $!uses_bindsig;
        return 0 unless nqp::istype($block[0], QAST::Stmts);
        for %!decls {
            # We're looking for lexical var decls; these have no magical
            # vivification lexical behavior and so are safe to lower.
            my $qast := $_.value;
            my str $decl := $qast.decl;
            next unless $decl eq 'var';
            my str $scope := $qast.scope;
            next unless $scope eq 'lexical';

            # Also ensure not dynamic.
            my $dynamic := try nqp::getattr($qast.value, nqp::p6var($qast.value).WHAT, '$!descriptor').dynamic;
            next if $dynamic;

            # Consider name. Can't lower if it's used by any nested blocks.
            my str $name := $_.key;
            unless nqp::existskey(%!usages_inner, $name) {
                # Lowerable if it's a normal variable.
                next if nqp::chars($name) < 1;
                unless nqp::iscclass(nqp::const::CCLASS_ALPHABETIC, $name, 0) {
                    my str $sigil := nqp::substr($name, 0, 1);
                    next unless $sigil eq '$' || $sigil eq '@' || $sigil eq '%';
                    next unless nqp::chars($name) >= 2 &&
                                nqp::iscclass(nqp::const::CCLASS_ALPHABETIC, $name, 1);
                }

                # Also must not lexicalref it.
                my int $ref'd := 0;
                if %!usages_flat{$name} {
                    for %!usages_flat{$name} {
                        if $_.scope eq 'lexicalref' {
                            $ref'd := 1;
                            last;
                        }
                    }
                }
                next if $ref'd;

                # Seems good; lower it. Note we need to retain a lexical in
                # case of binder failover to generate errors.
                my $new_name := $qast.unique('__lowered_lex');
                $block[0].unshift(QAST::Var.new( :name($qast.name), :scope('lexical'),
                                                 :decl('var'), :returns($qast.returns) ));
                $qast.name($new_name);
                $qast.scope('local');
                if %!usages_flat{$name} {
                    for %!usages_flat{$name} {
                        $_.scope('local');
                        $_.name($new_name);
                    }
                }
            }
        }
    }
}

# Junction optimizer, responsible for flattening away simple junctions.
my class JunctionOptimizer {
    # Junctions we can fold, and what short-circuit operator they fold to.
    my %foldable_junction := nqp::hash(
        '&infix:<|>', '&infix:<||>',
        '&infix:<&>', '&infix:<&&>');

    # Contexts in which we can fold a junction.
    my %foldable_outer := nqp::hash(
        '&prefix:<?>',      1,  '&prefix:<!>',      1,
        '&prefix:<so>',     1,  '&prefix:<not>',    1,
        'if',               1,  'unless',           1,
        'while',            1,  'until',            1);

    # The main optimizer.
    has $!optimizer;

    # Symbols lookup object.
    has $!symbols;

    # Constructs a new instance of the junction optimizer.
    method new($optimizer, $symbols) {
        my $obj := nqp::create(self);
        $obj.BUILD($optimizer, $symbols);
        $obj
    }
    method BUILD($optimizer, $symbols) {
        $!optimizer := $optimizer;
        $!symbols   := $symbols;
    }
    
    # Check if the junction is in a context where we can optimize.
    method is_outer_foldable($op) {
        if $op.op eq "call" {
            if nqp::existskey(%foldable_outer, $op.name) && $!symbols.is_from_core($op.name) {
                return 1;
            }
        } elsif nqp::existskey(%foldable_outer, $op.op) {
            return 1;
        }
        return 0;
    }

    # Only if a chain operator handles Any, rather than Mu, in its signature
    # will autothreading actually happen.
    method chain_handles_Any($op) {
        my $obj;
        my int $found := 0;
        try {
            $obj := $!symbols.find_lexical($op);
            $found := 1;
        }
        if $found == 1 {
            my $signature := $!symbols.find_in_setting("Signature");
            my $iter := nqp::iterator(nqp::getattr($obj.signature, $signature, '$!params'));
            while $iter {
                my $p := nqp::shift($iter);
                unless nqp::istype($p.type, $!symbols.Any) {
                    return 0;
                }
            }
            return 1;
        } else {
            return 0;
        }
        return 0;
    }

    method can_chain_junction_be_warped($node) {
        sub has_core-ish_junction($node) {
            if nqp::istype($node, QAST::Op) && $node.op eq 'call' &&
                    nqp::existskey(%foldable_junction, $node.name) {
                if $!symbols.is_from_core($node.name) {
                    # TODO: special handling for any()/all(), because they create
                    #       a Stmts with a infix:<,> in it.
                    if +$node.list == 1 {
                        return 0;
                    }
                    return 1;
                }
            }
            return 0;
        }

        if has_core-ish_junction($node[0]) {
            return 0;
        } elsif has_core-ish_junction($node[1]) {
            return 1;
        }
        return -1;
    }

    method optimize($op) {
        if self.is_outer_foldable($op) && nqp::istype($op[0], QAST::Op) {
            my $proceed := 0;
            my $exp-side;
            if $op[0].op eq "chain" {
                $exp-side := self.can_chain_junction_be_warped($op[0]);
                $proceed := $exp-side != -1 && self.chain_handles_Any($op[0].name) == 1
            } elsif $op[0].op eq 'callmethod' && $op[0].name eq 'ACCEPTS' {
                $exp-side := self.can_chain_junction_be_warped($op[0]);
                # we should only ever find the 0nd child (the invocant) to be a junction anyway.
                $proceed := $exp-side == 0;
            }
            if $proceed {
                return self.apply_transform($op, $exp-side);
            }
        }
        return NQPMu;
    }

    method apply_transform($op, $exp-side) {
        # TODO chain_handles_Any may get more cleverness to check only the parameters that actually have
        # a junction passed to them, so that in some cases the unfolding may still happen.
        my str $juncop    := $op[0][$exp-side].name eq '&infix:<&>' ?? 'if' !! 'unless';
        my str $juncname  := %foldable_junction{$op[0][$exp-side].name};
        my str $chainop   := $op[0].op;
        my str $chainname := $op[0].name;
        my $values := $op[0][$exp-side];
        my $ovalue := $op[0][1 - $exp-side];

        # the first time $valop is refered to, create a bind op for a
        # local var, next time create a reference var op.
        my %reference;
        sub refer_to($valop) {
            my $id := nqp::where($valop);
            if nqp::existskey(%reference, $id) {
                QAST::Var.new(:name(%reference{$id}), :scope<local>);
            } else {
                %reference{$id} := $op.unique('junction_unfold');
                QAST::Op.new(:op<bind>,
                             QAST::Var.new(:name(%reference{$id}),
                                           :scope<local>,
                                           :decl<var>),
                             $valop);
            }
        }

        # create a comparison operation for the inner comparisons
        sub chain($value) {
            if $exp-side == 0 {
                QAST::Op.new(:op($chainop), :name($chainname),
                             $value,
                             refer_to($ovalue));
            } else {
                QAST::Op.new(:op($chainop), :name($chainname),
                             refer_to($ovalue),
                             $value);
            }
        }

        # create a chain of outer logical junction operators with inner comparisons
        sub create_junc() {
            my $junc := QAST::Op.new(:name($juncname), :op($juncop));

            $junc.push(chain($values.shift()));

            if +$values.list > 1 {
                $junc.push(create_junc());
            } else {
                $junc.push(chain($values.shift()));
            }
            return $junc;
        }

        $op.shift;
        $op.unshift(create_junc());
        return $!optimizer.visit_op($op);
    }
}

# Drives the optimization process overall.
class Perl6::Optimizer {
    # Symbols tracking object.
    has $!symbols;

    # Stack of block variable optimizers.
    has @!block_var_stack;

    # Junction optimizer.
    has $!junc_opt;

    # Track problems we encounter.
    has $!problems;

    # Optimizer configuration.
    has %!adverbs;

    # The optimization level.
    has $!level;

    # How deep a chain we're in, for chaining operators.
    has int $!chain_depth;

    # Are we in void context?
    has int $!void_context;

    # Are we in a declaration?
    has int $!in_declaration;

    # one shared QAST tree we'll put into every block we've eliminated
    has $!eliminated_block_contents;

    has $!debug;

    # Entry point for the optimization process.
    method optimize($past, *%adverbs) {
        # Initialize.
        $!symbols                 := Symbols.new($past);
        @!block_var_stack         := [];
        $!junc_opt                := JunctionOptimizer.new(self, $!symbols);
        $!problems                := Problems.new($!symbols);
        $!chain_depth             := 0;
        $!in_declaration          := 0;
        $!void_context            := 0;
        $!debug                   := nqp::getenvhash<RAKUDO_OPTIMIZER_DEBUG>;
        my $*DYNAMICALLY_COMPILED := 0;
        my $*W                    := $past.ann('W');

        # Work out optimization level.
        $!level := nqp::existskey(%adverbs, 'optimize') ??
            +%adverbs<optimize> !! 2;
        %!adverbs := %adverbs;

        note("method optimize before\n" ~ $past.dump) if $!debug;

        $!eliminated_block_contents := QAST::Op.new( :op('die_s'),
            QAST::SVal.new( :value('INTERNAL ERROR: Execution of block eliminated by optimizer') ) );

        # Walk and optimize the program.
        self.visit_block($!symbols.UNIT);

        # Report any discovered problems.
        $!problems.report();

        note("method optimize after\n" ~ $past.dump) if $!debug;

        $past
    }
    
    # Called when we encounter a block in the tree.
    method visit_block($block) {
        # Push block onto block stack and create vars tracking.
        $!symbols.push_block($block);
        @!block_var_stack.push(BlockVarOptimizer.new);
        
        # Visit children.
        if $block.ann('DYNAMICALLY_COMPILED') {
            my $*DYNAMICALLY_COMPILED := 1;
            self.visit_children($block, :resultchild(+@($block) - 1),:void_default);
        }
        else {
            self.visit_children($block, :resultchild(+@($block) - 1),:void_default);
        }
        
        # Pop block from block stack and get computed block var info.
        $!symbols.pop_block();
        my $vars_info := @!block_var_stack.pop();

        # If this block is UNIT and we're in interactive mode, poison lexical
        # to local lowering, or else we may lose symbols we need to remember.
        if $block =:= $!symbols.UNIT && %!adverbs<interactive> {
            $vars_info.poison_lowering();
        }

        # We might be able to delete some of the magical variables when they
        # are trivially unused, and also simplify takedispatcher.
        $vars_info.delete_unused_magicals($block);
        $vars_info.delete_unused_autoslurpy();
        $vars_info.simplify_takedispatcher();

        # If the block is immediate, we may be able to inline it.
        my int $flattened := 0;
        my $result        := $block;
        if $block.blocktype eq 'immediate' && $block.arity == 0
                && !$vars_info.is_poisoned && !$block.has_exit_handler {
            # Scan symbols for any non-interesting ones.
            my @sigsyms;
            for $block.symtable() {
                my $name := $_.key;
                if $name ne '$_' && $name ne '$*DISPATCHER' {
                    @sigsyms.push($name);
                }
            }

            # If we dynamically compiled it, then it must not contain
            # any nested blocks.
            if $*DYNAMICALLY_COMPILED {
                for $block[0].list {
                    if nqp::istype($_, QAST::Block) && $_.blocktype ne 'raw' {
                        @sigsyms.push('');
                    }
                }
            }
            
            # If we have no interesting ones, then we can inline the
            # statements.
            if +@sigsyms == 0 {
                if $!level >= 2 {
                    my $outer := $!symbols.top_block;
                    $result := self.inline_immediate_block($block, $outer,
                        nqp::existskey($vars_info.get_decls(), '$_'));
                }
            }
        }

        # Do any possible lexical => local lowering.
        $vars_info.lexical_vars_to_locals($block);

        # Incorporate this block's info into outer block's info.
        @!block_var_stack[nqp::elems(@!block_var_stack) - 1].incorporate_inner($vars_info, $flattened)
            if @!block_var_stack;

        $result
    }

    # Gets the last statement if the thing passed as a QAST::Stmts or a
    # QAST::Stmt. Works recursively. Otherwise returns what was passed.
    sub get_last_stmt($op) {
        if nqp::istype($op, QAST::Stmt) || nqp::istype($op, QAST::Stmts) {
            my int $resultchild := $op.resultchild // +@($op) - 1;
            $resultchild >= 0 ?? get_last_stmt($op[$resultchild]) !! $op
        }
        else {
            $op
        }
    }
    
    # Range operators we can optimize into loops, and how to do it.
    sub get_bound($node) {
        if nqp::istype($node, QAST::Want) && $node[1] eq 'Ii' {
            my int $value := $node[2].value;
            if $value > -2147483648 && $value < 2147483647 {
                return [$value];
            }
        }
        []
    }
    my %range_bounds := nqp::hash(
        '&infix:<..>', -> $op {
            my @lb := get_bound($op[0]);
            my @ub := get_bound($op[1]);
            @lb && @ub ?? [@lb[0], @ub[0]] !! []
        },
        '&infix:<..^>', -> $op {
            my @lb := get_bound($op[0]);
            my @ub := get_bound($op[1]);
            @lb && @ub ?? [@lb[0], @ub[0] - 1] !! []
        },
        '&infix:<^..>', -> $op {
            my @lb := get_bound($op[0]);
            my @ub := get_bound($op[1]);
            @lb && @ub ?? [@lb[0] + 1, @ub[0]] !! []
        },
        '&infix:<^..^>', -> $op {
            my @lb := get_bound($op[0]);
            my @ub := get_bound($op[1]);
            @lb && @ub ?? [@lb[0] + 1, @ub[0] - 1] !! []
        },
        '&prefix:<^>', -> $op {
            my @ub := get_bound($op[0]);
            @ub ?? [0, @ub[0] - 1] !! []
        },
        );

    # Poisonous calls.
    my %poison_calls := nqp::hash(
        'EVAL',     NQPMu, '&EVAL',     NQPMu,
        'EVALFILE', NQPMu, '&EVALFILE', NQPMu,
        'callwith', NQPMu, '&callwith', NQPMu,
        'callsame', NQPMu, '&callsame', NQPMu,
        'nextwith', NQPMu, '&nextwith', NQPMu,
        'nextsame', NQPMu, '&nextsame', NQPMu,
        'samewith', NQPMu, '&samewith', NQPMu);

    # Called when we encounter a QAST::Op in the tree. Produces either
    # the op itself or some replacement opcode to put in the tree.
    method visit_op($op) {
        note("method visit_op $!void_context\n" ~ $op.dump) if $!debug;
        # If it's a QAST::Op of type handle, needs some special attention.
        my str $optype := $op.op;
        if $optype eq 'handle' {
            return self.visit_handle($op);
        }
        elsif $optype eq 'locallifetime' {
            return self.visit_children($op,:first);
        }

        # If it's a for 1..1000000 { } we can simplify it to a while loop. We
        # check this here, before the tree is transformed by call inline opts.
        if $optype eq 'p6for' && $op.ann('context') eq 'sink' && @($op) == 2 {
            my $theop := $op[0];
            if nqp::istype($theop, QAST::Stmts) { $theop := $theop[0] }

            if nqp::istype($theop, QAST::Op) && nqp::existskey(%range_bounds, $theop.name) && $!symbols.is_from_core($theop.name) {
                self.optimize_for_range($op, $op[1], $theop);
                self.visit_op_children($op);
                return $op;
            }
        }

        # It could also be that the user explicitly spelled out the for loop
        # with a method call to "map".
        if $optype eq 'callmethod' && $op.name eq 'sink' &&
             