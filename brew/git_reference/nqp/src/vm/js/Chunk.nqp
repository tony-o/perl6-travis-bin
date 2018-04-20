# The different types of js things a Chunk expr can be
my $T_OBJ  := 0; 
my $T_INT  := 1; # We use a javascript number but always treat it as a 32bit integer
my $T_NUM  := 2; # We use a javascript number for that
my $T_STR  := 3; # We use a javascript str for that
my $T_BOOL := 4; # Something that can be used in boolean context in javascript. To the user it should be presented as a 0 or 1
my $T_CALL_ARG := 5; # Something that will be passed to a sub/method call

my $T_INT16 := 6; # We use a javascript number but always treat it as a 16bit integer
my $T_INT8 := 7; # We use a javascript number but always treat it as a 8bit integer

my $T_RETVAL := 8; # Something that will be returned from a sub/method call

my $T_UINT16 := 9; # We use a javascript number but always treat it as a 16bit integer
my $T_UINT8 := 10; # We use a javascript number but always treat it as a 8bit integer
my $T_UINT32 := 11; # We use a javascript number but always treat it as a unsigned 32bit integer

my $T_VOID := -1; # Value of this type shouldn't exist, we use a "" as the expr
my $T_NONVAL := -2; # something that is not a nqp value

my $T_ARGS := -3; # comma separated arguments to a js call
my $T_ARGS_ARRAY := -4; # an array of arguments to a js call

class Chunk {
    has int $!type; # the js type of $!expr
    has str $!expr; # a javascript expression without side effects
    has $!node; # a QAST::Node that contains info for source maps
    has $!setup; # stuff that needs to be executed before the value of $!expr can be used, this contains strings and Chunks.

    method new($type, $expr, $setup = nqp::null(), :$node) {
        my $obj := nqp::create(self);
        $obj.BUILD($type, $expr, $setup, :$node);
        $obj
    }

    method void(*@setup, :$node) {
        self.new($T_VOID, "", @setup, :$node);
    }

    method BUILD($type, $expr, @setup, :$node) {
        $!type := $type;
        $!expr := $expr;
        $!setup := @setup;
        $!node := $node if nqp::defined($node);
    }

    method collect(@strs) {
        if nqp::isnull($!setup) {
        }
        elsif nqp::istype($!setup, Chunk) {
            $!setup.collect(@strs);
        }
        else {
            for $!setup -> $part {
                if nqp::isstr($part) {
                    nqp::push_s(@strs, $part);
                }
                else {
                    $part.collect(@strs);
                }
            }
        }
    }

    method join() {
        my @strs := nqp::list_s();
        self.collect(@strs);
        nqp::join('', @strs);
    }

    method collect_with_source_map_info($offset, @strs, @mapping) {
        # HACK sometimes a QAST::Op sneaks into $!node.node, so call nqp::can
        my int $sourcemap_info := nqp::defined($!node) && $!node.node && nqp::can($!node.node, 'from');
        if $sourcemap_info {
            nqp::push_i(@mapping, $!node.node.from());
            nqp::push_i(@mapping, $offset);
        }

        #if nqp::defined($!node) && $!node.node && !nqp::can($!node.node, 'from') {
        #    stderr().print("problem with node.node");
        #}


        my int $count := 0;

        if nqp::isnull($!setup) {
        }
        elsif nqp::istype($!setup, Chunk) {
            $count := $!setup.collect_with_source_map_info($offset, @strs, @mapping);
        }
        else {
            for $!setup -> $part {
                if nqp::isstr($part) {
                    $count := $count + nqp::chars($part);
                    nqp::push_s(@strs, $part);
                }
                else {
                    $count := $count + $part.collect_with_source_map_info($offset + $count, @strs, @mapping);
                }
            }
        }

        if $sourcemap_info {
            nqp::push_i(@mapping, -1);
            nqp::push_i(@mapping, $offset + $count);
        }

        $count;
    }

    method type() {
        $!type;
    }

    method expr() {
        $!expr;
    }

    method setup() {
        $!setup;
    }

    method node() {
        $!node;
    }

    method is_args_array() {
        $!type == $T_ARGS_ARRAY;
    }
}
