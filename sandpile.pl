#!/usr/bin/perl

# Abelian Sandpile model


# intalls (run in your terminal):
# (echo y;echo) | perl -MCPAN -e 'install Term::ReadKey'
# perl -MCPAN -e 'install Term::ANSIColo'

use List::Util qw(min);
use Term::ReadKey;
use Term::ANSIColor;
use Time::HiRes 'sleep';
use v5.14;
$\ = $/;

my $max = 4;
my $symbol = "▓";
my $interval = 0.1;

my ($width, $height) = GetTerminalSize();
my $size = min (int($width/2)-1, $height);
my $middle = int ($size/2);
my @board;
push @board, [(-1)x($size+2)] for 0 .. $size + 1;

$board[$middle][$middle] = 0;

my $start = 0; # number of cycle to skip.
while (1) {
    $board[$middle][$middle]++;
    
    my @tmp;
    for my $i (1 .. $size) {
	for my $j (1 .. $size) {
	    if ($board[$i][$j] >= $max) {
		$tmp[$i-1][$j]++;
		$tmp[$i+1][$j]++;
		$tmp[$i][$j+1]++;
		$tmp[$i][$j-1]++;
		$tmp[$i][$j] += $board[$i][$j] - $max;
	    } else {
		$tmp[$i][$j] += $board[$i][$j];
	    }
	}
    }
    @board = @tmp;

    print "\e[H";
    if ($start > 0 ) {
	$start --;
    } else {
	sleep $interval;
	for (@board[1..$size-1]) {
	    print join "", map {$_ x 2} map { colorize($_) } @{$_}[1..$size-1];
	}
    }
}

sub colorize {
    my ($n) = @_;
    if ($n == -1) {
	return " ";
    } elsif ($n == 0) {
	return colored($symbol, 'white');
    } elsif ($n == 1) {
	return colored($symbol, 'yellow');
    } elsif ($n == 2) {
	return colored($symbol, 'red');
    } else {
	return colored($symbol, 'black');
    } 
}
