#! /usr/bin/perl
#
# construct GE locations of nav points (VOR, waypoints, etc)
#___________________________________________________________
#$fileName = "./US_Nav_8.xml";
open(KML,">NAVPOINTS.kml");
open(GMT,">GMTPOINTS.txt");
open(KHD,"kml.header");
@lines = <KHD>;
foreach (@lines) {
    print KML;
}
close(KHD);
#print KML "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
#print KML "<kml xmlns=\"http://earth.google.com/kml/2.2\">\n";
#print KML "<Document>\n";
#print KML "\t <name>NAV.kmz</name>\n";
$il = -1;
$WPfile[0] = "./US_Nav";
$WPfile[1] = "./US_Waypoint";
$WPfile[2] = "./US_Inst_Waypoint";
$WPfile[3] = "./US_Airport";
#@files = <./US_Nav_*\.xml>;
#@files = <./US_Waypoint*\.xml>;
for ($jfl = 0; $jfl <= 3; $jfl++) {
    @files = <$WPfile[$jfl]*\.xml>;
foreach $file (@files) {
    print " file is $file\n";
    open(TRF, $file);
    @lines = <TRF>;
    close(TRF);
# this is the first pass to get station IDs for alphabetizing:
    foreach $line(@lines) {
        $line =~ s//\n/g;
        @ln = split(/\n/, $line);
        foreach $l(@ln) {
	    $_ = $l;
	    if (/^<waypoint>/) {
	        s/.*<ident>(.*)<\/ident>//;
	        $ident = $1;
	        s/.*<name>(.*)<\/name>//;
	        $name = $1;
	        s/.*<latitude>(.*)<\/latitude>//;
	        $latitude = $1;
	        s/.*<longitude>(.*)<\/longitude>//;
	        $longitude = $1;
		$_ = $l;
		s/.*<waypointType>(.*)<\/waypointType>//;
		$type = $1;
		s/.*<frequency>(.*)<\/frequency>//;
		$frequency = $1;
         	print "$ident,\U$name,$latitude,$longitude,$type,$frequency\n";
 		if ($jfl == 0 && $frequency > 100. && $frequency < 200. && $type >=20 && $latitude >= 34. && $latitude <= 44. &&
 			$longitude <= -96. && $longitude >= -107.) {
#    select jfl == 0 for VORs, == 1 for waypoints, == 2 to add instr. waypts
# 		if (($jfl == 1) && $latitude >= 34. && $latitude <= 44. &&
# 			$longitude <= -96. && $longitude >= -107.) {
		    $il++;
		    $alines[$il] = "$ident#$l";
		}
	    }
	}
    }
}
}
my @sortedList = sort @alines;
foreach $l (@sortedList) {
#	print " --- $l\n";
	$l =~ s/[A-Z]*#//;
	$_ = $l;
	        s/.*<ident>(.*)<\/ident>//;
	        $ident = $1;
	        s/.*<name>(.*)<\/name>//;
	        $name = $1;
	        s/.*<latitude>(.*)<\/latitude>//;
	        $latitude = $1;
	        s/.*<longitude>(.*)<\/longitude>//;
	        $longitude = $1;
		$_ = $l;
		s/.*<waypointType>(.*)<\/waypointType>//;
		$type = $1;
		s/.*<frequency>(.*)<\/frequency>//;
		$frequency = $1;
		$_ = $l;
		s/.*<magVar>(.*)<\/magVar>//;
		$magVar = $1;
		    $lonDeg = int($longitude);
		    $lonMin = ($longitude - $lonDeg) * 60.;
		    $latDeg = int($latitude);
		    $latMin = ($latitude - $latDeg) * 60.;
		    $lonDeg *= -1;
		    $lonMin *= -1;
		    if ($latMin < 10.) {
			$slatMin = substr($latMin,0,3);
		    } else {
        	        $slatMin = substr($latMin,0,4);
		    }
		    if ($lonMin < 10.) {
        	        $slonMin = substr($lonMin,0,3);
		    } else {
        	        $slonMin = substr($lonMin,0,4);
		    }
#		    print " Longitude is $longitude, deg/min is $lonDeg, $slonMin\n";
#		    print " Latitude is $latitude, deg/min is $latDeg, $slatMin\n";
		    print KML "         <Placemark>\n";
                    print KML "		    <name>$ident</name>\n";
                    print KML "		    <styleUrl>#GMT-4</styleUrl>\n";
                    print KML "		    <Point>\n";
                    print KML "			<coordinates>$longitude,$latitude,0</coordinates>\n";
                    print KML "		    </Point>\n";
#		    print KML "		<ExtendedData>\n";
#		    print KML "		    <Data name=\"ID\">\n";
#		    print KML "			<value>$ident</value>\n";
#		    print KML "		    </Data>\n";
		    $smagVar = substr($magVar,0,5);
#		    print KML "		    <Data name=\"mag Var\">\n";
#		    print KML "			<value>$smagVar</value>\n";
#		    print KML "		    </Data>\n";
#		    print KML "		    <Data name=\"Name\">\n";
#		    print KML "			<value>$name</value>\n";
#		    print KML "		    </Data>\n";
#		    print KML "		    <Data name=\"lat\">\n";
#		    print KML "			<value>$latDeg $slatMin</value>\n";
#		    print KML "		    </Data>\n";
#		    print KML "		    <Data name=\"lon\">\n";
#		    print KML "			<value>W $lonDeg $slonMin</value>\n";
#		    print KML "		    </Data>\n";
#		    print KML "		</ExtendedData>\n";
		    $vlat = "$latDeg $slatMin";
		    $vlon = "$lonDeg $slonMin";
		    print KML "         <description><![CDATA[$name<br> Mag. Var. (subt from true): $smagVar<br> Lat: N $vlat <br> Lon: W $vlon]]></description>\n";
        	    print KML "		</Placemark>\n";
#	            print KML "$jfl,$ident,\U$name,$latitude,$longitude\n";
	            print GMT "$ident,$longitude,$latitude,$name,$magVar\n";
}
print KML "</Document>\n";
print KML "</kml>\n";
close(KML);
close(GMT);
