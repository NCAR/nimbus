#!/usr/bin/python
# -*- coding: utf-8 -*-
'Read FIX.txt file from FADDS, write .kml and .txt for waypoints'

import re
Airspace = 'High'       # set to 'Low' for low-altitude waypoints
f = open ('/home/cooperw/Research/DC3/FADDS/FIX.txt', 'r')
if Airspace == 'Low':
    ECK = 'ENROUTE LOW'
                                # Some notes about these header files:
                                # Fixes on the Enroute chart are plotted with
                                # a different symbol (blue target) and have
                                # visible labels. Other points are plotted
                                # with a smaller symbol and a red/tan color.
                                # Bubbles indicate the type of fix and if
                                # the fix is on the enroute map.
    h = open ('./kml.header.low', 'r') # header file to copy for .kml
else:
    h = open ('./kml.header', 'r')
    ECK = 'ENROUTE HIGH'
o = open ('WptsFADDS', 'w')     # text listing suitable for PlanFlight
k = open ('NewWPTS.kml', 'w')   # output kml for input to Google-Earth
                                # (change URLs for MC display use on GV)
for line in h: k.write (line)
h.close()
ENRC = False
NeedOutput = False
# here is a sample test line, used in development
#line = 'FIX1COKRI                         COLORADO                      K237-04-18.220N 107-28-34.420WFIX                                                                             RNAV                                  YREP-PT         COKRIZDV ZDV                               NNN                                                                                                                               '
kount = 0
nl = 0
oline = ''      # here set some values to prevent error flags for use of
name = ''       # variables not yet defined. These aren't real errors because
lat = 0.        # the variables are defined lower in the loop before they
lon = 0.        # are used.
kind = ''
latdms = [0.,0.,0.]
londms = [0.,0.,0.]
signlat = 1.
signlon = -1.
CheckForENRC = False
for line in f:
    nl += 1         # count lines, used during debugging.
                    # the reason for the following is that a FIX5 line
                    # follows a FIX1 line, perhaps with intervening lines
                    # but before the next FIX1 line, to indicate if the fix
                    # is on the enroute map. These will be plotted differently.
    if ('FIX5' in line) and (ECK in line) and CheckForENRC:
        if Airspace == 'Low':
            oline = re.sub (r'\n', r'ENRL\n', oline, 1)
        else:
            oline = re.sub (r'\n', r'ENRH\n', oline, 1)
        ENRC = True
    if 'FIX1' not in line: continue
    CheckForENRC = False
    if NeedOutput:          # output is delayed to see if a FIX5 line
                            # indicates a map that shows the fix. If so, that
                            # information is included in the kml description.
        o.write (oline)
        NeedOutput = False
        if True:        # this is just to avoid reformating lines copied here
            k.write ('          <Placemark>\n')
            k.write ('              <name>' + name + '</name>\n')
            #   print ' for name = ', name, ' ENRC = ', ENRC
            if ENRC:
                k.write ('              <styleUrl>#ENR</styleUrl>\n')
            else:
                k.write ('              <styleUrl>#WAC</styleUrl>\n')
            k.write ('              <Point>\n')
            k.write ('                  <coordinates>' \
                     + format (lon, '.6f') + ',' \
                     + format (lat, '.6f') + ',0</coordinates>\n')
            k.write ('              </Point>\n')
            dline = '          <description><![CDATA[' + name\
                     + '<br> Lat: ' + latdms[0]\
                     + ' ' + format (float (latdms[1])+float (latdms[2])/60., '.1f')
            if signlat > 0.: dline += 'N'
            else: dline += 'S'
            dline += '<br> Lon: ' + londms[0]\
                     + ' ' + format (float (londms[1])+float (londms[2])/60., '.1f')
            if signlon > 0.: dline += 'E'
            else: dline += 'W'
            dline += '<br> ' + kind
            if ENRC: 
                if Airspace == 'Low':
                    dline += ' - ENRL chart'
                else:
                    dline += ' - ENRH chart'
            dline += ']]></description>\n'
            k.write (dline)
            k.write ('          </Placemark>\n')
    ENRC = False
    """Here, for reference, is the format:
        1. (4) line type, FIX1->FIX5
        2. (5) fix ID
        3. (25) blank
        4. (30) State
        5. (2)  ICAO region code
        6. (14) latitude
        7. (14) longitude
        8. (3)  MIL or FIX (latter, civilian)
        9. (22) other
        10. (22) other
        11. (33) other
        12. (38) charting info
        13. (1)  publish? (Y/N)
        14. (15) type of fix
        15. (5)  repeat of ID
        16. (4)  high-altitude ARTCC center
        17. (4)  low-altitude ARTCC center
        18. (30) country name if outside CONUS
        19. (195) mostly blanks, few flags"""
    line = line.replace (r',', r'') # spurious commas interfere with parsing
    line = re.sub (r'(....)(.{5})(.{25})(.{30})(..)(.{14})(.{14})(...)',\
               r'\1,\2,\4,\6,\7,\8,', line, 1)
#print 'line A is ', line               
    line = re.sub (r'(.{76})(.{22})(.{22})(.{33})', r'\1', line, 1)
#print 'line B is ',line
    line = re.sub (r'(.{76})(.{38})(.)(.{15})(.{5})(.{4})(.{4})',\
               r'\1\2,\3,\4,\5,\6,\7,', line, 1)
#print 'line C is ', line               
    line = re.sub (r'(.{147})', r'\1', line, 1)              
#print 'line D is ', line
    ll = line.split(',')
    ftype = ll[0]           # line type, FIX1 to FIX5
    name = ll[1]            # identifier (5 char)
    state = ll[2]
    lattxt = ll[3]          # latitude, d-m-s[N/S] format
    lontxt = ll[4]          # longitude, d-m-s[E/W] format
    mil = ll[5]             # military (MIL) or civilian (FIX)
    chart = ll[6]
    pub = ll[7]             # publish? [Y/N]
    kind = ll[8]            # RNAV, Reporting point, grid reference, etc
    kind = re.sub (r' *$', r'', kind, 1)    # truncate trailing blanks
    HA = ll[10]             # is it on high-altitude control charts?
    LA = ll[11]             # is it on low-altitude control charts?
# the following found no fixes, so all HA fixes are also LA fixes
#    if re.match ('[A-Z]', HA) and not re.match ('[A-Z]', LA):
#        print name, HA, LA
    valid = True                # test for type of fixes to output
    if Airspace == 'Low':       # check for the flag for ARTCC control
        if not re.match ('[A-Za-z]', LA): valid = False
        else:
            kind += ' LA'
            if re.match ('[A-Za-z]', HA): kind += ' HA'
                            # This is not needed for high-altitude version
                            # because all high-altitude fixes are also
                            # low-altitude fixes, with the validity tests
                            # being used.
    if re.match (r'MIL', mil): valid = False    # skip military fixes
    if not re.match (r'Y', pub): valid = False  # require it to be published
    if re.match (r'MIL', kind): valid = False   # some fixes with military 
                                                # names aren't tagged 'MIL' above
    if re.match (r'CNF', kind): valid = False   # Computer fixes aren't permitted
    if re.match (r'COOR', kind): valid = False  # not sure if this is necessary
    if re.match (r'STAR', kind): valid = False  # don't use approach fixes
    if re.match (r'GPS', kind): valid = False   # or GPS fixes (not necessary?)
    if not valid: continue
    comma = ','
    latsign = +1.
    if 'S' in lattxt: latsign = -1.
    lonsign = +1.
    if 'W' in lontxt: lonsign = -1.
    lattxt = re.sub (r'N.*', r'', lattxt)
    lattxt = re.sub (r'S.*', r'', lattxt)
    lontxt = re.sub (r'E.*', r'', lontxt)
    lontxt = re.sub (r'W.*', r'', lontxt)
    latdms = lattxt.split('-')
    londms = lontxt.split ('-')
    #   print nl
    lat = float (latdms[0]) + float (latdms[1]) / 60. + float (latdms[2]) / 3600.
    lat *= latsign
    lon = float (londms[0]) + float (londms[1]) / 60. + float (londms[2]) / 3600.
    lon *= lonsign
    if lat < 28. or lat > 45.: continue     # these are limits for DC3
    if lon < -108. or lon > -80.: continue  # they are extensive because of the
                                            # need to follow storms downstream.
    oline = name + comma + format (lon, '.4f') \
          + comma + format (lat, '.4f') + comma + kind
    if Airspace == 'Low':
        oline = re.sub (r' *$', ',0,\n', oline, 1)
    else:
        oline = re.sub (r' *$', ',0,\n', oline, 1)
    #   print ftype, name, lattxt, lontxt, mil, chart, kind, HA, LA
    #   print 'oline is ', oline
    kount += 1
    #   if kount > 120: break            
    NeedOutput = True
    CheckForENRC = True
o.close ()
k.write ('</Document>\n')
k.write ('</kml>\n')
k.close ()
   
    
    
