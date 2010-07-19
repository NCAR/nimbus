#!/usr/bin/perl
#Created by Sean Stroble, Last modified July 19 2010
#This script will read a Google earth KML file and insert YYYYMMDD.hhmmss_hhmmss into the filename before .kml


use strict;
print "\n";

#Variable Declarations
our $dir;
our $ext = "kml";
our $force = 0;
#Helpfile:
if (! defined(@ARGV) ) { print<<EOF;
No directory inputted
usage ./KMLrename [options] TARGET_DIR

	Options:
	-f		Force, does not ask user before renaming kml files.
EOF
exit(1)
}

#Commandline interpreter
foreach my $arg (@ARGV) {
	if ($arg eq "-f") { $force = 1; }
}

#Name generator
$dir = @ARGV[0];
if (! -d $dir) { print "Directory does not exist!\n"; exit(1); }

my @rename;
my @files;
my $count;
my @matches;
if (-d $dir) {
	opendir(DIR, $dir);
	@rename;
	@files = readdir(DIR);
	close(DIR);
	@files = grep(/.*\.$ext/, @files);
	$count = 0;
	foreach my $file (@files) {
		if ($file =~ m/(\S*)\.\d{8}.\d{6}_\d{6}.kml/)
		{
			$rename[$count][0] = $file;
			$rename[$count][1] = "$1.kml";
			$count++;
			next;
		}
		#variables
		@matches;
		
		#open kml file
		open FILE, "$dir/$file" or die $!;
		#read each line, if it matches the regex, stash it in @matches
		while (<FILE>) { if ($_ =~ m/\s*<when>(.*?)<\/when>\s*/) { push(@matches, $1); } }
		close(FILE); #cleanup time!
		my $newFilename;
		#the first and last elements in @matches should be the start and end date/times
		#clean google date format
		if ($matches[0] =~ m/(\d\d\d\d).(\d\d).(\d\d)T(\d\d).(\d\d).(\d\d)Z/) {
			$newFilename = ".$1$2$3.$4$5$6_";
		}
		else {print 'Regex Error: ' . $matches[0] . 'does not match \d\d\d\d.\d\d.\d\dT\d\d.\d\d.\d\d'; }
		if ($matches[$#matches] =~ m/(\d\d\d\d).(\d\d).(\d\d)T(\d\d).(\d\d).(\d\d)Z/) {
			$newFilename .= "$4$5$6"
		}
		else {print 'Regex Error: ' . $matches[0] . 'does not match \d\d\d\d.\d\d.\d\dT\d\d.\d\d.\d\d'; }
		
		#add filenames to array
		$rename[$count][0] = $file;
		if ($file =~ m/(\S*)(\.\S*)/) { $newFilename = $1 . $newFilename . $2; }
		else {print 'Regex Error: ' . $file . 'does not match \S*\w\w\d\d.\S*'; }
		$rename[$count][1] = $newFilename; 
		
		$count++;
		@matches = ();
	}
	#tell the user what we are going to do
	#before we actually do it is usually best :-P
	my $cont;
	if (!$force) {
		for (my $i = 0; $i <= $#rename; $i++){
			print "mv $rename[$i][0] $rename[$i][1]\n"
		}
		print "Execute the commands shown? Yes == Enter, No == anything else:"; #ask for permission
		$cont = <STDIN>;
	}
	if ($cont eq "\n" || $force) {
		#if the user has given the go ahead, rename!
		for (my $i = 0; $i <= $#rename; $i++){
			#rename command will overwrite existing files without a word
			#check to make sure there is not a file already there to get clobbered
			if (! -e $rename[$i][1]) {
				print "Moving $dir/$rename[$1][0] To $dir/$rename[$i][1]\n";
				rename $dir."/".$rename[$i][0], $dir."/".$rename[$i][1] or die $!;
			}
		}
	}
}
else {
	print "Directory does not exist!\n";
}
