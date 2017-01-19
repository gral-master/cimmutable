#!/usr/bin/perl

# Biham-Middleton-Levine Traffic Model

# intalls (run in your terminal):
# (echo y;echo) | perl -MCPAN -e 'install Term::ReadKey'
# perl -MCPAN -e 'install Term::ANSIColo'

use List::Util qw(min);
use Term::ReadKey;
use Term::ANSIColor;
use Time::HiRes 'sleep';
use v5.14;
$\ = $/;


my $percent = 0.13;
my $interval = 0.02;
my $symbol = "▓";

my ($width, $height) = GetTerminalSize();
$width /= 2;
$height -= 2;
$width -= 2;

my @board;
for my $i (0 .. $height-1) {
    for my $j (0 .. $width-1) {
	my $rand = rand 1;
	if ($rand < $percent) {
	    $board[$i][$j] = 1;
	} elsif ($rand < $percent * 2) {
	    $board[$i][$j] = 2;
	} else {
	    $board[$i][$j] = 0;
	}
    }
}


while (1) {
    sleep $interval;

    for (my $i = 0; $i < $height; $i++) {
	for (my $j = 0; $j < $width; $j++) {
	    if ($board[$i][$j] == 1 && $board[$i][($j+1) % $width] == 0) {
		$board[$i][($j+1) % $width] = 3;
		$board[$i][$j] = 0;
	    } elsif ($board[$i][$j] == 2 && $board[($i+1) % $height][$j] == 0 ) {
		$board[($i+1) % $height][$j] = 4;
	        $board[$i][$j] = 0;
	    }
	}
    }

    for my $i (0 .. $height-1) {
	for my $j (0 .. $width-1) {
	    $board[$i][$j] -= 2 if $board[$i][$j] > 2;
	}
    }

    print "\e[H";
    for (@board) {
	print join "", map { $_ x 2 } map { colorize($_) } @$_;
    }
}

sub colorize {
    my ($n) = @_;
    if ($n == 0) {
	return " ";
    } elsif ($n == 1) {
	return colored($symbol, 'blue');
    } else {
	return colored($symbol, 'red');
    } 
}
