class QAST::Compiler is HLL::Compiler {
    # Holds information about the current register usage, for when
    # we're allocating tempories.
    my class RegAlloc {
        has int $!cur_p;
        has int $!cur_s;
        has int $!cur_i;
        has int $!cur_n;
        
        method new($cur?) {
            my $obj := nqp::create(self);
            $cur ??
                $obj.BUILD($cur.cur_p, $cur.cur_s, $cur.cur_i, $cur.cur_n) !!
                $obj.BUILD(5000, 5000, 5000, 5000);
            $obj
        }
        
        method handler_allocator() {
            my $obj := nqp::create(self);
            $obj.BUILD(10000, 10000, 10000, 10000);
            $obj
        }
        
        method BUILD($p, $s, $i, $n) {
            $!cur_p := $p;
            $!cur_s := $s;
            $!cur_i := $i;
            $!cur_n := $n;
        }
        
        method fresh_p() {
            $!cur_p := $!cur_p + 1;
            '$P' ~ $!cur_p
        }
        method fresh_s() {
            $!cur_s := $!cur_s + 1;
            '$S' ~ $!cur_s
        }
        method fresh_i() {
            $!cur_i := $!cur_i + 1;
            '$I' ~ $!cur_i
        }
        method fresh_n() {
            $!cur_n := $!cur_n + 1;
            '$N' ~ $!cur_n
        }
        
        method cur_p() { $!cur_p }
        method cur_s() { $!cur_s }
        method cur_i() { $!cur_i }
        method cur_n() { $!cur_n }
    }
    
    # Holds information about the QAST::Block we're currently compiling.
    my class BlockInfo {
        has $!qast;             # The QAST::Block
        has $!outer;            # Outer block's BlockInfo
        has @!params;           # QAST::Var nodes of params
        has @!locals;           # QAST::Var nodes of declared locals
        has @!lexicals;         # QAST::Var nodes of declared lexicals
        has %!local_types;      # Mapping of local registers to type names
        has %!lexical_types;    # Mapping of lexical names to types
        has %!lexical_regs;     # Mapping of lexical names to registers
        has %!reg_types;        # Mapping of all registers to types
        has int $!param_idx;    # Current lexical parameter index
        has @!loadlibs;         # Libraries to load for the target POST::Block.
        has int $!cur_lex_p;    # Current lexical register (P)
        has int $!cur_lex_s;    # Current lexical register (S)
        has int $!cur_lex_i;    # Current lexical register (I)
        has int $!cur_lex_n;    # Current lexical register (N)
        
        method new($qast, $outer) {
            my $obj := nqp::create(self);
            $obj.BUILD($qast, $outer);
            $obj
        }
        
        method BUILD($qast, $outer) {
            $!qast := $qast;
            $!outer := $outer;
            @!params := nqp::list();
            @!locals := nqp::list();
            @!lexicals := nqp::list();
            %!local_types := nqp::hash();
            %!lexical_types := nqp::hash();
            %!lexical_regs := nqp::hash();
            %!reg_types := nqp::hash();
            @!loadlibs := nqp::list();
            $!cur_lex_p := 100;
            $!cur_lex_s := 100;
            $!cur_lex_i := 100;
            $!cur_lex_n := 100;
        }
        
        method add_param($var) {
            if $var.scope eq 'local' {
                self.register_local($var);
            }
            else {
                my $reg := '_lex_param_' ~ $!param_idx;
                $!param_idx := $!param_idx + 1;
                self.register_lexical($var, $reg);
            }
            @!params[+@!params] := $var;
        }
        
        method add_lexical($var, :$is_static, :$is_cont, :$is_state) {
            self.register_lexical($var);
            if $is_static || $is_cont || $is_state {
                my %blv := %*BLOCK_LEX_VALUES;
                unless nqp::existskey(%blv, $!qast.cuid) {
                    %blv{$!qast.cuid} := [];
                }
                my $flags := $is_static ?? 0 !!
                             $is_cont   ?? 1 !! 2;
                nqp::push(%blv{$!qast.cuid}, [$v