#!/usr/bin/perl

$| = 1;

print "PUSH\n";
print get_time(`./bench -b -f tests/300_int_vec_push_10.bench`);
print get_time(`./bench -b -f tests/301_int_vec_push_100.bench`);
print get_time(`./bench -b -f tests/302_int_vec_push_1000.bench`);
print get_time(`./bench -b -f tests/303_int_vec_push_10000.bench`);

print "\nPOP\n";
print get_time(`./bench -b -f tests/400_int_vec_pop_10.bench`);
print get_time(`./bench -b -f tests/401_int_vec_pop_100.bench`);
print get_time(`./bench -b -f tests/402_int_vec_pop_1000.bench`);
print get_time(`./bench -b -f tests/403_int_vec_pop_10000.bench`);

print "\nUPDATE\n";
print get_time(`./bench -b -f tests/500_int_vec_update_10.bench`);
print get_time(`./bench -b -f tests/501_int_vec_update_100.bench`);
print get_time(`./bench -b -f tests/502_int_vec_update_1000.bench`);
print get_time(`./bench -b -f tests/503_int_vec_update_10000.bench`);
print "\n";

sub get_time {
    $_ = $_[1];
    s/.*: (.*)s\n/$1;/;
    return $_;
}
