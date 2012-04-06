#! /usr/bin/perl
#
# this is like "Nearest.pl" except it takes an input file with lat/lon
# coordinates and outputs a similar list with VOR/DME info appended
#
# find nav references closest to given point(s) specified by lat/lon
#
# This script searches the data files "VOR.txt" and "WPTS.txt" to find
# the four VORs and four waypoints that are closest to a specified point. 
#
# The specified point should be provided in degrees and decimal degrees 
# for latitude, followed by the same for longitude, following this pattern:
#        40,25.1,-105.32.995
# (precision is arbitrary as are decimal points for integer values;
#  the minus sign ahead of degrees is required for W longitude)
#
# The calculations use full spherical geometry for accuracy, although
# rectangular geometry would probably be adequate for most cases.
# An exception is the rotation of coordinates, where simple rectangular
# translation is used with approximate compensation for the difference
# in scales for longitude and latitude (i.e., 60 n mi = 1 deg. latitude but
# 1./cos(latitude) deg longitude). This could be done better by using the
# full spherical-geometry formulas. There is probably an analytical solution
# to the inversion problem -- my formulas take lat/lon and produce VOR/DME,
# while the rotation needs to go from VOR/DME to lat/lon. However, it may
# be easier if less elegant to use the rectilinear approximation as a first
# guess at lat/lon, then iterate to obtain the spherical-geometry solution.
# See match_radar notes in home/asp_files
#___________________________________________________________
#
#Define the advection speed (U and V components), rotation, and translation
$U =  0.;
$V =  0.;
$rotation = 0.;
$RotationLatitude = 43.;
$RotationLongitude = -105.6;
$RotationFirstPoint = 10;
$RotationLastPoint = 35;
$offset_lat = 0.;
$offset_lon = 0.;
# approximate flight speed:
# vtrue = 250 + (altitude - 10000.) * 180. / 35000.; 
$time_of_flight = 0.;
open(TRK,">FlightPlan.kml");
open(VOR,"VOR.txt");
open(WPT,"WPTS.txt");
open(OPT,">NewCoordinates");
open(KHD,"kml.header");
@lines = <KHD>;
foreach (@lines) {
      print TRK;
}
close(KHD);

@vlines = <VOR>;
@wlines = <WPT>;
# test point:
$lat = 41.;
$lon = -103.;
$alt = 0.;
	## note that this result differs from DME in being the distance
	## at zero altitude from the zero-altitude point below the VOR.
	## DME would take into account the slant distance, but that is not
	## what is needed here
$ralt = 0.;
$lastlat = -999.;
# get input latitude and longitude:
#print " enter zero to end; otherwise provide coordinates below:\n";
#print " (enter W longitude or S latitude as degrees negative but minutes positive)\n";
#print " enter the latitude and longitude (degrees minutes degrees minutes):";
$inline = <>;
chomp($inline);
$seq = 0;
#while ($inline != 0) {
while (!($inline =~ /^0/) && length($inline) > 1) {
print " inline = $inline\n";
$inline =~ s/\t/ /g;
$inline =~ s/  / /g;
$inline =~ s/  / /g;
$inline =~ s/  / /g;
$inline =~ s/  / /g;
$inline =~ s/  / /;
$inline =~ s/  / /;
$inline =~ s/  / /;
$inline =~ s/  / /;
$inline =~ s/  / /;
$inline =~ s/^\(//;
$inline =~ s/\)$//;
$inline =~ s/^ *//;
@in = split(/[, ]/,$inline);
$latdeg = $in[0];
$latmin = $in[1];
$londeg = $in[2];
$londeg *= -1;
$lonmin = $in[3];
if ($latdeg >= 0.) {$lat = $latdeg + $latmin / 60.;}
else {$lat = $latdeg - $latmin / 60.;}
if ($londeg >= 0.) {$lon = $londeg + $lonmin / 60.;}
else {$lon = $londeg - $lonmin / 60.;}
print " in[0,1,2] = $in[0], $in[1], $in[2], $in[3], $in[4]\n";
print " latitude = $lat, longitude = $lon\n";
#$lat = $in[1];
#$lon = $in[0];
$lat += $offset_lat;
$lon += $offset_lon;
#$altitude = int($in[2] / 0.3048);
$altitude = $in[4];
$latitude = $lat;
$longitude = $lon;
#
# now do rotation if needed:
if ($rotation != 0.) {
    if ($seq+1 >= $RotationFirstPoint && $seq+1 <= $RotationLastPoint) {
        @Rtn = FindAngle($latitude, $longitude, $RotationLatitude, $RotationLongitude);
	print " input lat/lon, pivot pt = $latitude $longitude $RotationLatitude, $RotationLongitude seq no = $seq\n";
        $az = $Rtn[0] + $rotation;
        if ($az > 360.) {$az -= 360.;}
        if ($az < 0.) {$az += 360.;}
        $legL = $Rtn[1];
        $latitude = $RotationLatitude + $legL * cos($CRADEG * $az) / 60.;
        $longitude = $RotationLongitude + $legL * sin($CRADEG * $az) / (60.*cos($CRADEG*$latitude));
	print " az = $az, L = $legL, new lat/lon = $latitude $longitude\n";
    }
}

#print OPT " inline=$inline, lat=$lat, lon=$lon\n";
$R_earth = 6.378206e6;
$CRADEG = 3.14159265/180.;
$Dmin[0] = $R_earth;
$Dmin[1] = $R_earth;
$Dmin[2] = $R_earth;
$Dmin[3] = $R_earth;
		    $seq++;
		    if ($lastlat != -999.) {
			$latitude += $V * ($time_of_flight) / 60.;
			$longitude += $U * ($time_of_flight) / (60. * cos($latitude * $CRADEG));
		 	@Rtn = FindAngle($latitude, $longitude, $lastlat, $lastlon);
			$legLength = $Rtn[1];
			$tas = 250. + ($altitude - 10000.) * 180. / 35000.;
			$legTime = $legLength / $tas;
			$latitude += $V * ($legTime) / 60.;
			$longitude += $U * ($legTime) / (60. * cos($latitude * $CRADEG));
			print " point $seq legLength $legLength tof $time_of_flight $latitude, $longitude, $lastlat, $lastlon\n";
			$time_of_flight += $legLength / (250. + ($altitude - 10000.) * 180. / 35000.);
			print " tas = $tas, dist = $legLength, leg time = $legTime \n";
			print " original lon = $longitude, adjusted for tof=$time_of_flight and U=$U gives ";
			print "$longitude\n";
#  calculate again with drifted final point:
		 	@Rtn = FindAngle($latitude, $longitude, $lastlat, $lastlon);
			$azimuth = $Rtn[0];
			$legLength = $Rtn[1];
			print TRK " <Placemark>\n";
                        print TRK "             <styleUrl>#WAC</styleUrl>\n";
		        print TRK " <LineString>\n";
		        print TRK " <coordinates> $lastlon,$lastlat,0 $longitude,$latitude,0</coordinates>\n";
		        print TRK " </LineString>\n";
			$sazimuth = int($azimuth+0.5);
			$legLength = int($legLength*10.+0.5)/10.;
			print TRK " <description><![CDATA[$legLength n mi<br>Alt $altitude ft<br>Dir: $sazimuth true<br> -> pt $seq]]></description>\n";
			print TRK " </Placemark>\n";
                    }
		    $lastlat = $latitude;
		    $lastlon = $longitude;
$lat = $latitude * $CRADEG;
$lon = $longitude * $CRADEG;
foreach (@vlines) {
    @vor = split(/,/);	# 0,1,2,3,4 are ID, lon,lat,name,magvar
#   if (($vor[0] =~ /BJC/)) {
    $rlat = $vor[2] * $CRADEG;
    $rlon = $vor[1] * $CRADEG;
    $magVar = $vor[4];
    $cos_rlat = cos($rlat);
    $sin_rlat = sin($rlat);
    $cos_lat  = cos($lat);
    $sin_lat  = sin($lat);
#   print "rlat=$rlat, rlon=$rlon, cos/sin(lat)=$cos_lat/$sin_lat, cos/sin(rlat)=$cos_rlat,$sin_rlat\n";
    $cos_alpha = $sin_lat * $sin_rlat 
		+ $cos_lat * $cos_rlat * cos($lon - $rlon);
    $sin_alpha = sqrt((1. - $cos_alpha*$cos_alpha));
#   print " cos_alpha=$cos_alpha, sin_alpha=$sin_alpha\n";
    $r2 = $R_earth + $alt;
    $tan_beta = ($r2 * $cos_alpha - $R_earth - $ralt)
		/ ($r2 * $sin_alpha);
#   print " tan_beta=$tan_beta\n";
    $cos_beta = 1. / sqrt((1. + $tan_beta*$tan_beta));
    $R = (($R_earth + $alt) * $sin_alpha) / $cos_beta;
#   print " R=$R, sin_alpha=$sin_alpha, cos_beta=$cos_beta, R_earth = $R_earth, alt = $alt\n";
    $xp = $r2 * $cos_lat * cos($lon-$rlon) 
		- $R_earth * $cos_rlat;
    $xrr = $r2 * $cos_lat * sin($lon - $rlon);
    $zp = $r2 * $sin_lat - $R_earth * $sin_rlat;
    $yrr = -$xp * $sin_rlat + $zp * $cos_rlat;
    $zrr = $xp * $cos_rlat + $zp * $sin_rlat;
    $azimuth = atan2($xrr, $yrr) / $CRADEG;
#convert to magnetic bearing from reference point:
    $azimuth -= $magVar;
    if ($azimuth < 0.) {$azimuth += 360.;}
    $sazimuth = int($azimuth+0.5);
# convert from meters to n mi:
    $R /= 1852.;
    $R = int(10.*($R+0.05))/10.;
#   print " $vor[0] $vor[1] $vor[2] $sazimuth/$R\n";
    $used = 0;
    $j = 0;
    while (!$used && ($j <= 3)) {
        if ($Dmin[$j] > $R) {
	    for ($k = 2; $k >= $j; $k--) {
		$Dmin[$k+1] = $Dmin[$k];
		$Best[$k+1] = $Best[$k];
		$BestAz[$k+1] = $BestAz[$k];
	    }
	    $Dmin[$j] = $R;
	    $Best[$j] = $vor[0];
	    $BestAz[$j] = $sazimuth;
#	    print " new $j best is $vor[0]\n";
	   $used = 1;
	}
	$j++;
    }
}
#print "\n\n     Closest VOR is $Best[0] [$BestAz[0]/$Dmin[0]]\n";
#print " 2nd Closest VOR is $Best[1] [$BestAz[1]/$Dmin[1]]\n";
#print " 3rd Closest VOR is $Best[2] [$BestAz[2]/$Dmin[2]]\n";
#print " 4th Closest VOR is $Best[3] [$BestAz[3]/$Dmin[3]]\n";
$lat /= $CRADEG;
$lon /= $CRADEG;
$latdeg = int($lat);
$latmin = ($lat - $latdeg) * 60.;
$latmin = int(10 * ($latmin+0.5))/10.;
$londeg = int($lon);
$lonmin = ($lon - $londeg) * 60.;
$lonmin = int(10 * ($lonmin+0.5))/10.;
print OPT " $latdeg,$latmin,$londeg,$lonmin,$Best[0],$BestAz[0]/$Dmin[0],$Best[1],$BestAz[1]/$Dmin[1],$Best[2],$BestAz[2]/$Dmin[2],";
                    print TRK "         <Placemark>\n";
                    print TRK "             <name>$seq</name>\n";
                    print TRK "             <styleUrl>#WAC</styleUrl>\n";
                    print TRK "             <Point>\n";
                    print TRK "                 <coordinates>$longitude,$latitude,0</coordinates>\n";
                    print TRK "             </Point>\n";
                    print TRK "             <description><![CDATA[$seq<br> $Best[0] $BestAz[0]/$Dmin[0]]]></description>\n";
                    print TRK "         </Placemark>\n";

$lat *= $CRADEG;
$lon *= $CRADEG;
$Dmin[0] = $R_earth;
$Dmin[1] = $R_earth;
$Dmin[2] = $R_earth;
$Dmin[3] = $R_earth;
foreach (@wlines) {
    @vor = split(/,/);	# 0,1,2,3 are ID, lon,lat,name
    $rlat = $vor[2] * $CRADEG;
    $rlon = $vor[1] * $CRADEG;
    $magVar = $vor[4];
    $cos_rlat = cos($rlat);
    $sin_rlat = sin($rlat);
    $cos_lat  = cos($lat);
    $sin_lat  = sin($lat);
#   print "rlat=$rlat, rlon=$rlon, cos/sin(lat)=$cos_lat/$sin_lat, cos/sin(rlat)=$cos_rlat,$sin_rlat\n";
    $cos_alpha = $sin_lat * $sin_rlat 
		+ $cos_lat * $cos_rlat * cos($lon - $rlon);
    $sin_alpha = sqrt((1. - $cos_alpha*$cos_alpha));
#   print " cos_alpha=$cos_alpha, sin_alpha=$sin_alpha\n";
    $r2 = $R_earth + $alt;
    $tan_beta = ($r2 * $cos_alpha - $R_earth - $ralt)
		/ ($r2 * $sin_alpha);
#   print " tan_beta=$tan_beta\n";
    $cos_beta = 1. / sqrt((1. + $tan_beta*$tan_beta));
    $R = (($R_earth + $alt) * $sin_alpha) / $cos_beta;
#   print " R=$R, sin_alpha=$sin_alpha, cos_beta=$cos_beta, R_earth = $R_earth, alt = $alt\n";
    $xp = $r2 * $cos_lat * cos($lon-$rlon) 
		- $R_earth * $cos_rlat;
    $xrr = $r2 * $cos_lat * sin($lon - $rlon);
    $zp = $r2 * $sin_lat - $R_earth * $sin_rlat;
    $yrr = -$xp * $sin_rlat + $zp * $cos_rlat;
    $zrr = $xp * $cos_rlat + $zp * $sin_rlat;
    $azimuth = atan2($xrr, $yrr) / $CRADEG;
#convert to magnetic bearing from reference point:
    $azimuth -= $magVar;
    if ($azimuth < 0.) {$azimuth += 360.;}
    $sazimuth = int($azimuth+0.5);
# convert from meters to n mi:
    $R *= 0.00054;
    $R = int(10.*($R+0.05))/10.;
#   print " $vor[0] $vor[1] $vor[2] $sazimuth/$R\n";
    $used = 0;
    $j = 0;
    while (!$used && ($j <= 3)) {
        if ($Dmin[$j] > $R) {
	    for ($k = 2; $k >= $j; $k--) {
		$Dmin[$k+1] = $Dmin[$k];
		$Best[$k+1] = $Best[$k];
		$BestAz[$k+1] = $BestAz[$k];
	    }
	    $Dmin[$j] = $R;
	    $Best[$j] = $vor[0];
	    $BestAz[$j] = $sazimuth;
#    	    print " new $j best is $vor[0]\n";
	   $used = 1;
	}
	$j++;
    }
}
#print "\n\n     Closest WPT is $Best[0] [$BestAz[0]/$Dmin[0]]\n";
#print " 2nd Closest WPT is $Best[1] [$BestAz[1]/$Dmin[1]]\n";
#print " 3rd Closest WPT is $Best[2] [$BestAz[2]/$Dmin[2]]\n";
#print " 4th Closest WPT is $Best[3] [$BestAz[3]/$Dmin[3]]\n";
print OPT "$Best[0],$BestAz[0]/$Dmin[0]\n";
#print " enter zero to end; otherwise provide coordinates below:\n";
#print " (enter W longitude or S latitude as degrees negative but minutes positive)\n";
#print " enter the latitude and longitude (degrees minutes degrees minutes):";
$inline = <>;
chomp($inline);
}
close(VOR);
close(WPT);
close(OPT);
print TRK "</Document>\n";
print TRK "</kml>\n";
close(TRK);

############################################
sub FindAngle {
    my ($lat, $lon, $rlat, $rlon) = @_;
    if (abs($lat - $rlat) < 0.001 && abs($lon - $rlon) < 0.001) {
        $Rt[0] = 0.;
        $Rt[1] = 0.;
        return @Rt;
    } else {
        $ralt = 0.;
        $R_earth = 6.378206e6;
        $CRADEG = 3.14159265/180.;
        $lat *= $CRADEG;
        $lon *= $CRADEG;
        $rlat *= $CRADEG;
        $rlon *= $CRADEG;
        $cos_rlat = cos($rlat);
        $sin_rlat = sin($rlat);
        $cos_lat  = cos($lat);
        $sin_lat  = sin($lat);
        $cos_alpha = $sin_lat * $sin_rlat 
		    + $cos_lat * $cos_rlat * cos($lon - $rlon);
        $sin_alpha = sqrt((1. - $cos_alpha*$cos_alpha));
        $r2 = $R_earth + $alt;
        $tan_beta = ($r2 * $cos_alpha - $R_earth - $ralt)
		    / ($r2 * $sin_alpha);
        $cos_beta = 1. / sqrt((1. + $tan_beta*$tan_beta));
        $R = (($R_earth + $alt) * $sin_alpha) / $cos_beta;
        $xp = $r2 * $cos_lat * cos($lon-$rlon) 
		    - $R_earth * $cos_rlat;
        $xrr = $r2 * $cos_lat * sin($lon - $rlon);
        $zp = $r2 * $sin_lat - $R_earth * $sin_rlat;
        $yrr = -$xp * $sin_rlat + $zp * $cos_rlat;
        $zrr = $xp * $cos_rlat + $zp * $sin_rlat;
        $azimuth = atan2($xrr, $yrr) / $CRADEG;
#convert to magnetic bearing from reference point:
#   $azimuth -= $magVar;
        if ($azimuth < 0.) {$azimuth += 360.;}
# convert from meters to n mi:
        $R /= 1852.;
        $Rt[0] = $azimuth;
        $Rt[1] = $R;
        return @Rt;
    }
}
