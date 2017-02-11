#!/usr/bin/perl

open *STDOUT, '>', 'tests/703_int_vec_split_10000.bench' or die $!;

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
	print "0 = push(0,$v)\n";
    } else {
	print "0 = push(0,$v)\n";
    }
}

print "0 = split(0, 5, 1, 2)\n";

# for (0 .. 10) {
#     $v = int rand 10000;
#     if ($_ % 2 == 0) {
# 	print "0 = update(0, 2, $v)\n";
#     } else {
# 	print "0 = update(0, 3, $v)\n";
#     }
# }
