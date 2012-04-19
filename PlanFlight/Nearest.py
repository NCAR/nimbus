# -*- coding: utf-8 -*-
"""
This is a replacement for the Perl script Nearest.pl,
written so that it will be possible to run this program
under Windows without needing Perl there. It is a stand-alone
script that does not depend on the rest of PlanFlight.py; it
reads input from Nearest.in and writes output to Nearest.out.
"""
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
#________
        ## note that distance here differs from DME in being the distance
        ## at zero altitude from the zero-altitude point below the VOR.
        ## DME would take into account the slant distance, but that is not
        ## what is needed here.

import math
import string
def Nearest (lat, lon):
    vlines = open ('./NavPoints/VOR.txt', 'r').readlines ()
    wlines = open ('./NavPoints/WPTS.txt', 'r').readlines ()
    for i in range (len(vlines)): 
        pts = string.split (vlines[i], ',')
        pts[1] = float (pts[1])
        pts[2] = float (pts[2])
        pts[4] = float (pts[4])
        vlines[i] = pts
    for i in range (len(wlines)): 
        pts = string.split (wlines[i], ',')
        pts[1] = float (pts[1])
        pts[2] = float (pts[2])
        pts[4] = float (pts[4])
        wlines[i] = pts
    ralt = 0.
    alt = 0.
#   Pinput = open ('Nearest.in', 'r')
#   Poutput = open ('Nearest.out', 'w')
    R_earth = 6.378206e6
    CRADEG = 3.14159265/180.
    Dmin = [R_earth, R_earth, R_earth, R_earth, R_earth, R_earth, R_earth, R_earth]
    Best = [None, None, None, None, None, None, None, None]
    BestAz = [None, None, None, None, None, None, None, None]
    BestLG = [None, None, None, None, None, None, None, None]
    BestLT = [None, None, None, None, None, None, None, None] 
    lat *= CRADEG
    lon *= CRADEG
    for vline in vlines:
        rlat = vline[2] * CRADEG
        rlon = vline[1] * CRADEG
        magVar = vline[4]
        cosRlat = math.cos (rlat)
        sinRlat = math.sin (rlat)
        cosLat = math.cos (lat)
        sinLat = math.sin (lat)
        cosAlpha = sinLat * sinRlat + cosLat * cosRlat \
                   * math.cos (lon - rlon)
        sinAlpha = (1. - cosAlpha * cosAlpha)**0.5
        if sinAlpha != 0.:
            r2 = R_earth + alt
            tanBeta = (r2 * cosAlpha - R_earth - ralt) / (r2 * sinAlpha)
            cosBeta = 1. / (1. + tanBeta * tanBeta)**0.5
            R = ((R_earth + alt) * sinAlpha) / cosBeta
            xp = r2 * cosLat * math.cos(lon-rlon) - R_earth * cosRlat
            xrr = r2 * cosLat * math.sin(lon - rlon)
            zp = r2 * sinLat - R_earth * sinRlat
            yrr = -xp * sinRlat + zp * cosRlat
            zrr = xp * cosRlat + zp * sinRlat
            azimuth = math.atan2(xrr, yrr) / CRADEG
#convert to magnetic bearing from reference point:
            azimuth -= magVar
            if azimuth < 0.: azimuth += 360.
            sazimuth = int(azimuth+0.5)
        else:
            R = 0.
            sazimuth = 0.
                    # convert from meters to n mi:
        R *= 0.00054
        R = int(10.*(R+0.05))/10.
        used = False
        j = 0
        while not used and (j <= 3):
            if Dmin[j] > R:
                for k in range (2, (j-1), -1):
                    Dmin[k+1] = Dmin[k]
                    Best[k+1] = Best[k]
                    BestAz[k+1] = BestAz[k]
                    BestLG[k+1] = BestLG[k]
                    BestLT[k+1] = BestLT[k]
                Dmin[j] = R
                Best[j] = vline[0]
                BestAz[j] = sazimuth
                BestLG[j] = vline[1]
                BestLT[j] = vline[2]
                used = True
            j += 1
#   Poutput.write ("\n\n     Closest VOR is %s [%.0f/%.1f]: %.2f, %.2f\n" % (Best[0], BestAz[0], Dmin[0], BestLG[0],BestLT[0]))
#   Poutput.write (" 2nd Closest VOR is %s [%.0f/%.1f]: %.2f, %.2f\n" % (Best[1], BestAz[1], Dmin[1], BestLG[1], BestLT[1]))
#   Poutput.write (" 3rd Closest VOR is %s [%.0f/%.1f]: %.2f, %.2f\n" % (Best[2], BestAz[2], Dmin[2], BestLG[2], BestLT[2]))
#   Poutput.write (" 4th Closest VOR is %s [%.0f/%.1f]: %.2f, %.2f\n" % (Best[3], BestAz[3], Dmin[3], BestLG[3], BestLT[3]))
    for vline in wlines:
        rlat = vline[2] * CRADEG
        rlon = vline[1] * CRADEG
        magVar = vline[4]
        cosRlat = math.cos (rlat)
        sinRlat = math.sin (rlat)
        cosLat = math.cos (lat)
        sinLat = math.sin (lat)
        cosAlpha = sinLat * sinRlat + cosLat * cosRlat \
                   * math.cos (lon - rlon)
        sinAlpha = (1. - cosAlpha * cosAlpha)**0.5
        if sinAlpha != 0.:
            r2 = R_earth + alt
            tanBeta = (r2 * cosAlpha - R_earth - ralt) / (r2 * sinAlpha)
            cosBeta = 1. / (1. + tanBeta * tanBeta)**0.5
            R = ((R_earth + alt) * sinAlpha) / cosBeta
            xp = r2 * cosLat * math.cos(lon-rlon) - R_earth * cosRlat
            xrr = r2 * cosLat * math.sin(lon - rlon)
            zp = r2 * sinLat - R_earth * sinRlat
            yrr = -xp * sinRlat + zp * cosRlat
            zrr = xp * cosRlat + zp * sinRlat
            azimuth = math.atan2(xrr, yrr) / CRADEG
#convert to magnetic bearing from reference point:
            azimuth -= magVar
            if azimuth < 0.: azimuth += 360.
        else:
            R = 0.
            sazimuth = 0.
        sazimuth = int(azimuth+0.5)
# convert from meters to n mi:
        R *= 0.00054
        R = int(10.*(R+0.05))/10.
        used = False
        j = 4
        while not used and (j <= 7):
            if Dmin[j] > R:
                for k in range (6, (j-1), -1):
                    Dmin[k+1] = Dmin[k]
                    Best[k+1] = Best[k]
                    BestAz[k+1] = BestAz[k]
                    BestLG[k+1] = BestLG[k]
                    BestLT[k+1] = BestLT[k]
                Dmin[j] = R
                Best[j] = vline[0]
                BestAz[j] = sazimuth
                BestLG[j] = vline[1]
                BestLT[j] = vline[2]
                used = True
            j += 1
#   Poutput.write ("\n\n     Closest WPT is %s [%.0f/%.1f]: %.2f, %.2f\n" % (Best[4], BestAz[4], Dmin[4], BestLG[4],BestLT[4]))
#   Poutput.write (" 2nd Closest WPT is %s [%.0f/%.1f]: %.2f, %.2f\n" % (Best[5], BestAz[5], Dmin[5], BestLG[5], BestLT[5]))
#   Poutput.write (" 3rd Closest WPT is %s [%.0f/%.1f]: %.2f, %.2f\n" % (Best[6], BestAz[6], Dmin[6], BestLG[6], BestLT[6]))
#   Poutput.write (" 4th Closest WPT is %s [%.0f/%.1f]: %.2f, %.2f\n" % (Best[7], BestAz[7], Dmin[7], BestLG[7], BestLT[7]))
#   Poutput.close ()
    return (Dmin, Best, BestAz, BestLG, BestLT)
    



   





