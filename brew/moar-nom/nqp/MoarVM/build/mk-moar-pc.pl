#! /usr/bin/perl
## ---------------------------------------------------------------------
## mk-moar-pc.in -- Describe MoarVM configuration
##
##  Copyright © 2014 Daniel Dehennin <daniel.dehennin@baby-gnu.org>
##
##
use strict;
use warnings;

use File::Basename;
use File::Path qw{mkpath};

my @args = @ARGV;
die "Missing target filename argument" if @args < 1;

my $prefix = '/home/travis/brew/moar-nom/install';
my $bindir = '/home/travis/brew/moar-nom/install/bin';
my $libdir = '/home/travis/brew/moar-nom/install/lib';
my $mastdir = '/home/travis/brew/moar-nom/install/share/nqp/lib/MAST';
my $includedir = "${prefix}/include";
my $package = 'moar';
my $version = '2017.10';
my $exeext = '';
my $moar = "${bindir}/${package}${exeext}";
my $ldopts = ' -O3 -DNDEBUG -Wl,-rpath,"//home/travis/brew/moar-nom/install/lib"';
my $ldflags = "-L${libdir} -l${package}";
my $libs = "${ldflags}";
my $cflags = "-I${includedir}";
my $static_libs = '';
if ('--optimize --prefix=/home/travis/brew/moar-nom/install --make-install' =~ m/--static/) {
    my $static_libs = "${libdir}/lib${package}.a";
}
else {
    my $static_libs = '';
}
if ($libdir =~ m{^/(?:usr/)?lib(?:64)?}) {
    $ldopts = '';
    $ldflags = "-l$package";
}
else {
    $ldflags = "${ldopts} ${ldflags}";
}
$libs = "${ldflags}";
$cflags = '' if ${includedir} eq "/usr/include";

my $dirname = dirname $args[0];
if ( ! -d $dirname ) {
    mkpath($dirname);
}

open my $pcfile, '>', "$args[0]"
  or die "Can not open > $args[0]: $!";

print $pcfile <<EOF;
# pkg-config information for MoarVM ${version}
#
prefix=$prefix
package=$package
includedir=$includedir
bindir=$bindir
libdir=$libdir
mastdir=$mastdir
ldopts=$ldopts
exeext=$exeext
version=$version
moar=$moar
libs=$libs
static_libs=$static_libs

Name:           MoarVM
Description:    A virtual machine for Rakudo Perl 6 and NQP
URL:            http://moarvm.org
Version:        $version
Libs:           $ldflags
Cflags:         $cflags
EOF

close $pcfile;
## end of mk-moar-pc.in
