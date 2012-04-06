#! /usr/bin/perl
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
#___________________________________________________________
open(VOR,"NavPoints/VOR.txt");
open(WPT,"NavPoints/WPTS.txt");
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
# get input latitude and longitude:
print " enter zero to end; otherwise provide coordinates below:\n";
print " (enter W longitude or S latitude as degrees negative but minutes positive)\n";
print " enter the latitude and longitude (degrees minutes degrees minutes):";
$inline = <>;
while ($inline != 0) {
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
$lonmin = $in[3];
$lat = $latdeg + $latmin / 60.;
if ($latdeg >= 0.) {$lat = $latdeg + $latmin / 60.;}
else {$lat = $latdeg - $latmin / 60.;}
if ($londeg >= 0.) {$lon = $londeg + $lonmin / 60.;}
else {$lon = $londeg - $lonmin / 60.;}
$R_earth = 6.378206e6;
$CRADEG = 3.14159265/180.;
$Dmin[0] = $R_earth;
$Dmin[1] = $R_earth;
$Dmin[2] = $R_earth;
$Dmin[3] = $R_earth;
$lat *= $CRADEG;
$lon *= $CRADEG;
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
		$BestLG[$k+1] = $BestLG[$k];
		$BestLT[$k+1] = $BestLT[$k];
	    }
	    $Dmin[$j] = $R;
	    $Best[$j] = $vor[0];
	    $BestAz[$j] = $sazimuth;
	    $BestLG[$j] = $vor[1];
	    $BestLT[$j] = $vor[2];
#	    print " new $j best is $vor[0]\n";
	   $used = 1;
	}
	$j++;
    }
}
print "\n\n     Closest VOR is $Best[0] [$BestAz[0]/$Dmin[0]]: $BestLG[0],$BestLT[0]\n";
print " 2nd Closest VOR is $Best[1] [$BestAz[1]/$Dmin[1]]: $BestLG[1],$BestLT[1]\n";
print " 3rd Closest VOR is $Best[2] [$BestAz[2]/$Dmin[2]]: $BestLG[2],$BestLT[2]\n";
print " 4th Closest VOR is $Best[3] [$BestAz[3]/$Dmin[3]]: $BestLG[3],$BestLT[3]\n";
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
		$BestLG[$k+1] = $BestLG[$k];
		$BestLT[$k+1] = $BestLT[$k];
	    }
	    $Dmin[$j] = $R;
	    $Best[$j] = $vor[0];
	    $BestAz[$j] = $sazimuth;
	    $BestLG[$j] = $vor[1];
	    $BestLT[$j] = $vor[2];
#    	    print " new $j best is $vor[0]\n";
	   $used = 1;
	}
	$j++;
    }
}
print "\n\n     Closest WPT is $Best[0] [$BestAz[0]/$Dmin[0]]: $BestLG[0],$BestLT[0]\n";
print " 2nd Closest WPT is $Best[1] [$BestAz[1]/$Dmin[1]]: $BestLG[1],$BestLT[1]\n";
print " 3rd Closest WPT is $Best[2] [$BestAz[2]/$Dmin[2]]: $BestLG[2],$BestLT[2]\n";
print " 4th Closest WPT is $Best[3] [$BestAz[3]/$Dmin[3]]: $BestLG[3],$BestLT[3]\n";
print " enter zero to end; otherwise provide coordinates below:\n";
print " (enter W longitude or S latitude as degrees negative but minutes positive)\n";
print " enter the latitude and longitude (degrees minutes degrees minutes):";
$inline = <>;
}
close(VOR);
close(WPT);
