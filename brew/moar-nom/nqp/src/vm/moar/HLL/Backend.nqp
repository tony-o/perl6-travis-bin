# Backend class for the MoarVM.
class HLL::Backend::MoarVM {
    our %moar_config := nqp::backendconfig();

    sub sorted_keys($hash) {
        my @keys;
        for $hash {
            nqp::push(@keys, $_.key);
        }
        if +@keys == 0 {
            return @keys;
        }

        # we expect on the order of 6 or 7 keys here, so bubble sort is fine.
        my int $start := 0;
        my int $numkeys := +@keys;
        my str $swap;
        my int $current;
        while $start < $numkeys - 1 {
            $current := 0;
            while $current < $numkeys - 1 {
                if @keys[$current] lt @keys[$current + 1] {
                    $swap := @keys[$current];
                    @keys[$current] := @keys[$current + 1];
                    @keys[$current + 1] := $swap;
                }
                $current++;
            }
            $start++;
        }
        return @keys;
    }

    sub output_as_json($obj, $output_fh, $esc_backslash, $esc_dquote, $esc_squote?) {
        my @pieces := nqp::list_s();

        sub to_json($obj) {
            if nqp::islist($obj) {
                nqp::push_s(@pieces, '[');
                my $first := 1;
                for $obj {
                    if $first {
                        $first := 0;
                    }
                    else {
                        nqp::push_s(@pieces, ',');
                    }
                    to_json($_);
                }
                nqp::push_s(@pieces, ']');
            }
            elsif nqp::ishash($obj) {
                nqp::push_s(@pieces, '{');
                my $first := 1;
                for sorted_keys($obj) {
                    if $first {
                        $first := 0;
                    }
                    else {
                        nqp::push_s(@pieces, ',');
                    }
                    nqp::push_s(@pieces, '"');
                    nqp::push_s(@pieces, $_);
                    nqp::push_s(@pieces, '":');
                    to_json($obj{$_});
                }
                nqp::push_s(@pieces, '}');
            }
            elsif nqp::isstr($obj) {
                if nqp::index($obj, '\\') {
                    $obj := subst($obj, /'\\'/, $esc_backslash, :global);
                }
                if nqp::index($obj, '"') {
                    $obj := subst($obj, /'"'/, $esc_dquote, :global);
                }
                if nqp::defined($esc_squote) && nqp::index($obj, "'") {
                    $obj := subst($obj, /"'"/, $esc_squote, :global);
                }
                nqp::push_s(@pieces, '"');
                nqp::push_s(@pieces, $obj);
                nqp::push_s(@pieces, '"');
            }
            elsif nqp::isint($obj) || nqp::isnum($obj) {
                nqp::push_s(@pieces, ~$obj);
            }
            elsif nqp::can($obj, 'Str') {
                to_json(nqp::unbox_s($obj.Str));
            }
            else {
                nqp::die("Don't know how to dump a " ~ $obj.HOW.name($obj));
            }
            if nqp::elems(@pieces) > 4096 {
                nqp::printfh($output_fh, nqp::join('', @pieces));
                nqp::setelems(@pieces, 0);
            }
        }

        to_json($obj);
        nqp::printfh($output_fh, nqp::join('', @pieces));
    }

    method apply_transcodings($s, $transcode) {
        $s
    }

    method config() {
        %moar_config
    }

    method force_gc() {
        nqp::force_gc();
    }

    method name() {
        %moar_config<name>
    }

    method nqpevent($spec?) {
        # Doesn't do anything just yet
    }

    my $prof_start_sub;
    my $prof_end_sub;
    method ensure_prof_routines() {
        unless $prof_start_sub {
            $prof_start_sub := self.compunit_mainline(self.mbc(self.mast(QAST::CompUnit.new(
                QAST::Block.new(
                    QAST::Op.new( :op('mvmstartprofile'),
                        QAST::Var.new( :name('config'), :scope('local'), :decl('param') ) )
                )))));
            $prof_end_sub := self.compunit_mainline(self.mbc(self.mast(QAST::CompUnit.new(
                QAST::Block.new(
                    QAST::Op.new( :op('mvmendprofile') )
                )))));
        }
    }
    method run_profiled($what, $kind, $filename?) {
        $kind := 'instrumented' unless $kind;

        my @END := nqp::gethllsym('perl6', '@END_PHASERS');
        @END.push(-> { self.dump_profile_data($prof_end_sub(), $kind, $filename) })
            if nqp::defined(@END);

        self.ensure_prof_routines();
        $prof_start_sub(nqp::hash('kind', $kind));
        my $res  := $what();
        unless nqp::defined(@END) {
            my $data := $prof_end_sub();
            self.dump_profile_data($data, $kind, $filename);
        }
        $res;
    }
    method dump_profile_data($data, $kind, $filename) {
        if $kind eq 'instrumented' {
            self.dump_instrumented_profile_data($data, $filename);
        }
        elsif $kind eq 'heap' {
            self.dump_heap_profile_data($data, $filename);
        }
        else {
            nqp::die("Don't know how to dump data for $kind profile");
        }
    }
    method dump_heap_profile_data($data, $filename) {
        my @pieces;

        unless nqp::defined($filename) {
            $filename := 'heap-' ~ nqp::time_n() ~ '.txt';
        }
        my $profile_fh := open($filename, :w);

        nqp::sayfh($profile_fh, '# This file contains MoarVM heap snapshots.');
        nqp::sayfh($profile_fh, '# the stringheap section contains a list of');
        nqp::sayfh($profile_fh, '# strings that are used throughout the file');
        nqp::sayfh($profile_fh, '# by their index in the string heap.');
        nqp::sayfh($profile_fh, '#');
        nqp::sayfh($profile_fh, '# The types list has entries with two string');
        nqp::sayfh($profile_fh, '# indices: The REPR of a type and the name.');
        nqp::sayfh($profile_fh, '#');
        nqp::sayfh($profile_fh, '# The following sections are per snapshot:');
        nqp::sayfh($profile_fh, '#');
        nqp::sayfh($profile_fh, '# Every entry in the collectables section starts');
        nqp::sayfh($profile_fh, '# with the type or frame index as second value.');
        nqp::sayfh($profile_fh, '#');
        nqp::sayfh($profile_fh, '#   1 Object');
        nqp::sayfh($profile_fh, '#   2 Type object');
        nqp::sayfh($profile_fh, '#   3 STable');
        nqp::sayfh($profile_fh, '#   4 Frame');
        nqp::sayfh($profile_fh, '#   5 Perm Roots');
        nqp::sayfh($profile_fh, '#   6 Instance Roots');
        nqp::sayfh($profile_fh, '#   7 CStack Roots');
        nqp::sayfh($profile_fh, '#   8 Thread Roots');
        nqp::sayfh($profile_fh, '#   9 Root');
        nqp::sayfh($profile_fh, '#');
        nqp::sayfh($profile_fh, '# The third field is the size of the GC-managed');
        nqp::sayfh($profile_fh, '# part. The fourth field is the size of extra');
        nqp::sayfh($profile_fh, '# data, for example managed by malloc or the FSA.');
        nqp::sayfh($profile_fh, '#');
        nqp::sayfh($profile_fh, '# The fifth field is the starting index into the');
        nqp::sayfh($profile_fh, '# references list, the sixth field is the number');
        nqp::sayfh($profile_fh, '# of references that belong to this collectable.');
        nqp::sayfh($profile_fh, '#');
        nqp::sayfh($profile_fh, '# Each reference has its ref kind, its "data"');
        nqp::sayfh($profile_fh, '# and the index of the referenced collectable as');
        nqp::sayfh($profile_fh, '# its fields.');
        nqp::sayfh($profile_fh, '#');
        nqp::sayfh($profile_fh, '#   0 for unknown.');
        nqp::sayfh($profile_fh, '#   1 for an index.');
        nqp::sayfh($profile_fh, '#   2 for an entry in the string heap.');
        nqp::sayfh($profile_fh, '#');
        nqp::sayfh($profile_fh, '[ stringheap');
        for $data<strings> {
            if nqp::index($_, '\n') {
                nqp::sayfh($profile_fh, subst($_, /\n/, q{\\n}, :global));
            }
            else {
                nqp::sayfh($profile_fh, $_);
            }
        }
        nqp::sayfh($profile_fh, '[ types');
        nqp::sayfh($profile_fh, $data<types>);

        nqp::sayfh($profile_fh, '[ snapshots');
        my int $snapshotidx := 0;
        for $data<snapshots> -> $snapshot {
            nqp::sayfh($profile_fh, '[ s' ~ $snapshotidx);
            nqp::sayfh($profile_fh, 'r ' ~ $snapshot<references>);
            nqp::sayfh($profile_fh, 'c ' ~ $snapshot<collectables>);
            $snapshotidx := $snapshotidx + 1;
        }

        nqp::closefh($profile_fh);
    }
    method dump_instrumented_profile_data($data, $filename) {
        my @pieces := nqp::list_s();

        unless nqp::defined($filename) {
            $filename := 'profile-' ~ nqp::time_n() ~ '.html';
        }
        nqp::sayfh(nqp::getstderr(), "Writing profiler output to $filename");
        my $profile_fh := open($filename, :w);
        my $want_json  := ?($filename ~~ /'.json'$/);

        my $escaped_backslash;
        my $escaped_squote;
        my $escaped_dquote;

        if $want_json {
            # Single quotes don't require escaping here
            $escaped_backslash := q{\\\\};
            $escaped_dquote    := q{\\"};
        }
        else {
            # Here we're creating a double-quoted JSON string destined for the
            # inside of a single-quoted JS string. Ouch.
            $escaped_backslash := q{\\\\\\\\};
            $escaped_dquote    := q{\\\\"};
            $escaped_squote    := q{\\'};
        }

        my int $new-id-counter := 0;
        my $id_remap    := nqp::hash();
        my $id_to_thing := nqp::hash();

        sub post_process_call_graph_node($node) {
            try {
                if nqp::existskey($id_remap, $node<id>) {
                    $node<id> := $id_remap{$node<id>};
                } else {
                    my str $newkey := ~($new-id-counter++);
                    $id_remap{$node<id>} := $newkey;
                    $node<id> := $newkey;
                }
                if nqp::existskey($node, "allocations") {
                    for $node<allocations> -> %alloc_info {
                        my $type := %alloc_info<type>;
                        if nqp::existskey($id_remap, %alloc_info<id>) {
                            %alloc_info<id> := $id_remap{%alloc_info<id>};
                        } else {
                            my str $newkey := ~($new-id-counter++);
                            $id_remap{%alloc_info<id>} := $newkey;
                            %alloc_info<id> := $newkey;
                        }
                        unless nqp::existskey($id_to_thing, %alloc_info<id>) {
                            $id_to_thing{%alloc_info<id>} := $type.HOW.name($type);
                        }
                        nqp::deletekey(%alloc_info, "type");
                    }
                }
                unless nqp::existskey($id_to_thing, $node<id>) {
                    my $shared_data := nqp::hash(
                        "file", $node<file>,
                        "line", $node<line>,
                        "name", $node<name>,
                    );
                    $id_to_thing{$node<id>} := $shared_data;
                }
                nqp::deletekey($node, "file");
                nqp::deletekey($node, "line");
                nqp::deletekey($node, "name");
                if nqp::existskey($node, "callees") {
                    for $node<callees> {
                        post_process_call_graph_node($_);
                    }
                }
                CATCH {
                    note(nqp::getmessage($!));
                }
            }
        }
        # Post-process the call data, turning objects into flat data.
        for $data {
            post_process_call_graph_node($_<call_graph>);
        }

        nqp::unshift($data, $id_to_thing);

        if $want_json {
            output_as_json($data, $profile_fh, $escaped_backslash, $escaped_dquote);
        }
        else {
            # Get profiler template, split it in half, and write those either
            # side of the JSON itself.
            my $template := try slurp('src/vm/moar/profiler/template.html');
            unless $template {
                $template := slurp(nqp::backendconfig()<prefix> ~ '/share/nqp/lib/profiler/template.html');
            }
            my @tpl_pieces := nqp::split('{{{PROFILER_OUTPUT}}}', $template);

            nqp::printfh($profile_fh, @tpl_pieces[0]);
            output_as_json($data, $profile_fh, $escaped_backslash, $escaped_dquote, $escaped_squote);
            nqp::printfh($profile_fh, @tpl_pieces[1]);
        }
        nqp::closefh($profile_fh);
    }

    method run_traced($level, $what) {
        nqp::die("No tracing support");
    }

    method version_string() {
        my $rev := %moar_config<version> // '(unknown)';
        return "MoarVM version $rev";
    }

    method stages() {
        'mast mbc moar'
    }

    method is_precomp_stage($stage) {
        $stage eq 'mbc'
    }

    method is_textual_stage($stage) {
        0
    }

    method mast($qast, *%adverbs) {
        nqp::getcomp('QAST').to_mast($qast);
    }

    method mbc($mast, *%adverbs) {
        my $assmblr := nqp::getcomp('MAST');
        if %adverbs<target> eq 'mbc' && %adverbs<output> {
            $assmblr.assemble_to_file($mast, %adverbs<output>);
            nqp::null()
        }
        else {
            my $boot_mode := %adverbs<bootstrap> ?? 1 !! 0;
            nqp::usecompileehllconfig() if $boot_mode;
            my $result := $assmblr.assemble_and_load($mast);
            nqp::usecompilerhllconfig() if $boot_mode;
            $result
        }
    }

    method moar($cu, *%adverbs) {
        $cu
    }

    method is_compunit($cuish) {
        nqp::iscompunit($cuish)
    }

    method compunit_mainline($cu) {
        nqp::compunitmainline($cu)
    }

    method compunit_coderefs($cu) {
        nqp::compunitcodes($cu)
    }
}

# Role specifying the default backend for this build.
role HLL::Backend::Default {
    method default_backend() { HLL::Backend::MoarVM }
}
