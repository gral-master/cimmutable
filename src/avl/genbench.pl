#!/usr/bin/perl

open *STDOUT, '>', 'tests/203_int_vec.bench' or die $!;

print <<HEADERS;
# Test benchmark.

[struct]
vector

[implem]
AVL
RRB
FINGER

[type]
int

[init]
0 = create()

[bench]
    
HEADERS
    
    for (0 .. 10000) {
	$v = int rand 10000;
    if ($_ % 2 == 0) {
	print "1 = update(0,$v)\n";
	print "unref(0)\n";
    } else {
	print "0 = update(1,$v)\n";
	print "unref(1)\n";	
    }
}

for (0 .. 10000) {
    if ($_ % 2 == 0) {
	print "0 = pop(1)\n";
	print "unref(1)\n";
    } else {
	print "1 = pop(0)\n";
	print "unref(0)\n";	
    }
}
