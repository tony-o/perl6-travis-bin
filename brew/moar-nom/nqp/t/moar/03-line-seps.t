#! nqp

# Test nqp::op line reading operations.

plan(18);

my $test-file := 'line-seps-test-file';

{
    my $wfh := nqp::open($test-file, 'w');
    nqp::printfh($wfh, 'abc|def>ghi');
    nqp::closefh($wfh);
    
    my $rfh := nqp::open($test-file, 'r');
    nqp::setinputlineseps($rfh, nqp::list_s('|', '>'));
    ok(nqp::readlinefh($rfh) eq "abc|", 'first separator used');
    ok(nqp::readlinefh($rfh) eq "def>", 'second separator used');
    ok(nqp::readlinefh($rfh) eq "ghi", 'final read to end of file worked');
    nqp::closefh($rfh);
    
    $rfh := nqp::open($test-file, 'r');
    nqp::setinputlineseps($rfh, nqp::list_s('|', '>'));
    ok(nqp::readlinechompfh($rfh) eq "abc", 'first separator used and chomped');
    ok(nqp::readlinechompfh($rfh) eq "def", 'second separator used and chomped');
    ok(nqp::readlinechompfh($rfh) eq "ghi", 'final read to end of file worked');
    nqp::closefh($rfh);
    
    nqp::unlink($test-file);
}

{
    my $wfh := nqp::open($test-file, 'w');
    nqp::printfh($wfh, 'abc|def||ghi>jkl>>mno');
    nqp::closefh($wfh);
    
    my $rfh := nqp::open($test-file, 'r');
    nqp::setinputlineseps($rfh, nqp::list_s('||', '>>'));
    ok(nqp::readlinefh($rfh) eq "abc|def||", 'first multi-char separator used');
    ok(nqp::readlinefh($rfh) eq "ghi>jkl>>", 'second multi-char separator used');
    ok(nqp::readlinefh($rfh) eq "mno", 'final read to end of file worked');
    nqp::closefh($rfh);
    
    $rfh := nqp::open($test-file, 'r');
    nqp::setinputlineseps($rfh, nqp::list_s('||', '>>'));
    ok(nqp::readlinechompfh($rfh) eq "abc|def", 'first multi-char separator used and chomped');
    ok(nqp::readlinechompfh($rfh) eq "ghi>jkl", 'second multi-char separator used and chomped');
    ok(nqp::readlinechompfh($rfh) eq "mno", 'final read to end of file worked');
    nqp::closefh($rfh);
    
    nqp::unlink($test-file);
}

{
    my $wfh := nqp::open($test-file, 'w');
    nqp::printfh($wfh, "abc\ndef\r\nghi");
    nqp::closefh($wfh);

    my $rfh := nqp::open($test-file, 'r');
    nqp::setinputlineseps($rfh, nqp::list_s("\n", "\r\n"));
    ok(nqp::readlinefh($rfh) eq "abc\n", '\n separator used');
    ok(nqp::readlinefh($rfh) eq "def\n", '\r\n separator used'); # \n due to translation
    ok(nqp::readlinefh($rfh) eq "ghi", 'final read to end of file worked');
    nqp::closefh($rfh);

    $rfh := nqp::open($test-file, 'r');
    nqp::setinputlineseps($rfh, nqp::list_s("\n", "\r\n"));
    ok(nqp::readlinechompfh($rfh) eq "abc", '\n separator used and chomped');
    ok(nqp::readlinechompfh($rfh) eq "def", '\r\n separator used and chomped');
    ok(nqp::readlinechompfh($rfh) eq "ghi", 'final read to end of file worked');
    nqp::closefh($rfh);

    nqp::unlink($test-file);
}
