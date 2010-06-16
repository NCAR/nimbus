#!/usr/bin/perl
$i = 0;
my @array = (1,2,3,4,5,6);
splice @array, 2, 1;
splice @array, $i, 1;
print join(":", @array);
print "\n";
