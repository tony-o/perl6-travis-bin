use v6;

use lib <lib t/04-nativecall>;
use CompileTestLib;
use NativeCall;
use Test;

plan(1);

# RT #125140

compile_test_lib('19-function-pointers');

sub ReturnFunctionPointer() returns Pointer is native('./19-function-pointers') { * }

my $ptr              = ReturnFunctionPointer();
my &ReturnedFunction = nativecast(:(--> int32), $ptr);

is ReturnedFunction(), 42, 'Pointer cast to Perl 6 Sub';
