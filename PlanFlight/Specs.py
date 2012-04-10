# file: Specs.py
"This module contains the specs for the aircraft (climb, etc) and airport."
'2012 Copyright University Corporation for Atmospheric Research'
# file Specs.py
import re
import wx
import math
import pickle
from Frame import frame
# the following are the keys for the Specs dictionary. This is used mostly
# to facilitate storing this with the Track:
SpecsKeys = ['Airport','FlightSpeed','Wind','MaxTurn','FuelBurn','FuelLimit',
             'GrossWeight', 'Extra','FormatCode']
Specs = dict.fromkeys(SpecsKeys)
Specs[SpecsKeys[0]] = (-105.1172, 39.9089, 5673)# field location
Specs[SpecsKeys[1]] = 0.8   			# flight speed, Mach
#			wind is superceded by the sounding file if present
Specs[SpecsKeys[2]] = (270.,30.)		# default wind dir/sp (deg,kts)
Specs[SpecsKeys[3]] = 3.        		# default max turn rate, deg/s 
Specs[SpecsKeys[4]] = 3300.         		# default fuel burn rate, lb/h 
Specs[SpecsKeys[5]] = 30000.        		# max fuel for research
Specs[SpecsKeys[6]] = 90500.        		# Gross weight at T/O  
Specs[SpecsKeys[7]] = 0.            		# diversion allowance
#			this provides for future changes in format:
Specs[SpecsKeys[8]] = 1             		# format code - fixed
#	these are the module keys for the module definition, also saved with
#	the track:
ModuleKeys = ["Type","Dimension","Anchor","Orientation","Drift","Legs",\
                "Other", "Specs", "Fuel", "Time", "AC", "FlightDirection",\
                "ModuleNumber", "ManeuverNumber", "Comment"]
#	These are the module types implemented in 'ModuleConstructor.py'.
#	To add a new type, add a name here, add the appropriate code to
#	the section in ModuleConstructor.py that tests for each type, and
#	modify the section in PlanFlight.py that sets the default values
#	and information display for each module type.`
ModuleType = [
    'InitialClimb',	
    'Enroute',         
    'DelayInArea',
    'OutAndBack',
    'VerticalSection',	
    'Sawtooth',
    'StairStep',
    'SpiralProfile',
    'Circle',
    'Racetrack',
    'Lawnmower',
    'SquareSpiral',
    'Figure4',
    'Figure8',
    'Butterfly',
    'ThreeBlade',
    'CalManeuvers',
    'ReturnToBase',   
    ]

Cradeg = math.pi / 180.      # Conversion factor from degrees to radians
FlightLevelTemperature = 0.
ceiling = 0.
SRdata = None			# this is read from data file; here as
				# global so it can be passed back to
				# PlanFlight.py

###############################################
#	This section reads the sounding stored in 'Sound' to get the wind 
#	profile

#	The following code is specific to the text-file format as archived
#	at the U Wyo sounding archive. It tests for lines to skip on the
#	basis of that format. Adaptations will be needed for other formats.
try:
    WindFile = open('./Sounding','r')	# now only using wind and T
    Sounding = []
    for line in WindFile: 
#	#first skip lines not part of sounding
        if line.find('K') != -1: pass
        elif line.find('a') != -1: pass
        elif line.find('----') != -1: pass
        elif line.find('</PRE>') != -1: break
        elif line.find('<') != -1: pass
        else: 
            f = line.replace('\n','')
            f = re.sub(r'  *',r' ', f)
            f = re.sub(r'^ ',r'', f)
            a = str.split(f,' ')
					# replace z with pressure alt.
					# for altitudes at/above FL180
            if len (a) >= 7:
                p = float (a[0])
                if p > 226.3206:
                    alt = 145442.156 * (1. - (p / 1013.25)**0.1902632)
                    if alt < 18000.: alt = float (a[1]) / 0.3048
                else:
                    alt = 36089.24 + 47907.22 * math.log10 (226.3206 / p)
            if len(a) > 10:   		# this is a full sounding record
#               print a[0]+','+a[1]+','+a[6]+','+a[7]
#               b = (float (a[0]), float (a[1]) / 0.3048, float (a[6]),\
                b = (float (a[0]), alt, float (a[6]),\
                     float (a[7]), float (a[2]))
                Sounding.append(b)
            elif len (a) == 7: 		# this is a record where DP is invalid
#               print a[0]+','+a[1]+','+a[3]+','+a[4]
#               b = (float (a[0]), float (a[1]) / 0.3048, float (a[3]),\
                b = (float (a[0]), alt, float (a[3]),\
                     float (a[4]), float (a[2]))
                Sounding.append(b)
    WindFile.close()
except IOError as e:
    print \
                'There is no Sounding file. \nCalculations will '\
              + 'proceed using the single wind value in \'Specs\'.\n'\
              + 'To avoid this, use the \'Download Sounding\' item in '\
              + 'the \'Specs\' menu.'
    Sounding = None

#####################################
#	now read fuel-use table that gives n. mi. per lb fuel as function
#	of altitude and gross weight:
#	(this assumes Mach=0.80; superceded by SpecificRange table that 
#	 follows)
try:
    FuelFile = open('./PerformanceTable.GV','r')	
    NAMPP = []
    ix = 0
    for line in FuelFile: 
        if line[0] == '#': pass
        elif line[0] == 'G': 
            gwtff = line.split ()
            nn = len (gwtff)
            for i in range (1, nn): 
                gwtff[i] = float (gwtff[i])
        else: 
            ff = line.split ()
            nn = len (ff)
            for i in range (0, nn): 
                ff[i] = float (ff[i])
            NAMPP.append (ff)
    FuelFile.close ()
except IOError as e:
    print \
            'There is no fuel-use file.\n '\
          + 'Calculations will use 3300 lb/h'

#####################################
#	The following file was produced by SpecificRange.c to represent
#	fuel usage for ISA conditions, all speeds, gross weights, and
#	altitudes accessible by the GV. This includes information from
#	the above NAMPP table but is more thorough in its coverage.
try:
    SRFile = open('./SpecificRange.pickle','rb')	
    SRdata = pickle.load (SRFile)
    SRFile.close ()
#   print 'SR file:', SRdata
except IOError as e:
    print \
            'There is no specific-range file.\n '\
          + 'Calculations will use the fixed value as\n'\
          + 'specified in the default specifications'


###############################################
# This section reads the climb-performance file and constructs
# the arrays used to determine climb rate, fuel usage, etc., 
# from the table
zclimb = []
try:
    ClimbFile = open('./ClimbISA.csv','r')	
    climb = []
    for line in ClimbFile: 
        if line[0] == '#': pass
        elif line[0] == 'W': 
            gwt = line.split ()
            nn = len (gwt)
            for i in range (0, nn-1): 
                gwt[i] = 1000. * float (gwt[i+1])
        elif 'TIME' in line: 
            f1 = line.split ()
            zclimb.append (float (f1[0]))
            nn = len (f1)
            for i in range (0, nn-2): 
                f1[i] = float (f1[i+2])
        elif 'FUEL' in line:
            f2 = line.split ()
            nn = len (f2)
            for i in range (0, nn-2): 
                f2[i] = float (f2[i+2])
        elif 'DIST' in line:
            f3 = line.split ()
            nn = len (f3)
            ff = []
            for i in range (0, nn-2): 
                f3[i] = float (f3[i+2])
                ff.append ((f1[i], f2[i], f3[i]))
            climb.append (ff)
				# append zero line for interpolation
    zclimb.append (0.)
    ff = []
    for i in range (0, nn-2): ff.append((0.,0.,0.))
    climb.append (ff)
    ClimbFile.close ()
##  print 'climb results for altitude = ', zclimb[5]
##  ff = climb[5]
##  for i in range (0, nn-2):
##      print gwt[i], ff[i]
except IOError as e:
    print \
            'There is no climb-performance file.\n '\
          + 'Calculations will use instant climb.'
try:
    ClimbFile = open('./ClimbISAm20.csv','r')	
    climbm20 = []
    for line in ClimbFile: 
        if line[0] == '#': pass
        elif line[0] == 'W': 
            gwt = line.split ()
            nn = len (gwt)
            for i in range (0, nn-1): 
                gwt[i] = 1000. * float (gwt[i+1])
        elif 'TIME' in line: 
            f1 = line.split ()
            nn = len (f1)
            for i in range (0, nn-2): 
                f1[i] = float (f1[i+2])
        elif 'FUEL' in line:
            f2 = line.split ()
            nn = len (f2)
            for i in range (0, nn-2): 
                f2[i] = float (f2[i+2])
        elif 'DIST' in line:
            f3 = line.split ()
            nn = len (f3)
            ff = []
            for i in range (0, nn-2): 
                f3[i] = float (f3[i+2])
                ff.append ((f1[i], f2[i], f3[i]))
            climbm20.append (ff)
				# append zero line for interpolation
    ff = []
    for i in range (0, nn-2): ff.append((0.,0.,0.))
    climbm20.append (ff)
    ClimbFile.close ()
except IOError as e:
    print \
            'There is no climb-performance file.\n '\
          + 'Calculations will use instant climb.'
try:
    ClimbFile = open('./ClimbISAm10.csv','r')	
    climbm10 = []
    for line in ClimbFile: 
        if line[0] == '#': pass
        elif line[0] == 'W': 
            gwt = line.split ()
            nn = len (gwt)
            for i in range (0, nn-1): 
                gwt[i] = 1000. * float (gwt[i+1])
        elif 'TIME' in line: 
            f1 = line.split ()
            nn = len (f1)
            for i in range (0, nn-2): 
                f1[i] = float (f1[i+2])
        elif 'FUEL' in line:
            f2 = line.split ()
            nn = len (f2)
            for i in range (0, nn-2): 
                f2[i] = float (f2[i+2])
        elif 'DIST' in line:
            f3 = line.split ()
            nn = len (f3)
            ff = []
            for i in range (0, nn-2): 
                f3[i] = float (f3[i+2])
                ff.append ((f1[i], f2[i], f3[i]))
            climbm10.append (ff)
				# append zero line for interpolation
    ff = []
    for i in range (0, nn-2): ff.append((0.,0.,0.))
    climbm10.append (ff)
    ClimbFile.close ()
except IOError as e:
    print \
            'There is no climb-performance file.\n '\
          + 'Calculations will use instant climb.'
try:
    ClimbFile = open('./ClimbISAp10.csv','r')	
    climbp10 = []
    for line in ClimbFile: 
        if line[0] == '#': pass
        elif line[0] == 'W': 
            gwt = line.split ()
            nn = len (gwt)
            for i in range (0, nn-1): 
                gwt[i] = 1000. * float (gwt[i+1])
        elif 'TIME' in line: 
            f1 = line.split ()
            nn = len (f1)
            for i in range (0, nn-2): 
                f1[i] = float (f1[i+2])
        elif 'FUEL' in line:
            f2 = line.split ()
            nn = len (f2)
            for i in range (0, nn-2): 
                f2[i] = float (f2[i+2])
        elif 'DIST' in line:
            f3 = line.split ()
            nn = len (f3)
            ff = []
            for i in range (0, nn-2): 
                f3[i] = float (f3[i+2])
                ff.append ((f1[i], f2[i], f3[i]))
            climbp10.append (ff)
				# append zero line for interpolation
    ff = []
    for i in range (0, nn-2): ff.append((0.,0.,0.))
    climbp10.append (ff)
    ClimbFile.close ()
except IOError as e:
    print \
            'There is no climb-performance file.\n '\
          + 'Calculations will use instant climb.'
try:
    ClimbFile = open('./ClimbISAp20.csv','r')	
    climbp20 = []
    for line in ClimbFile: 
        if line[0] == '#': pass
        elif line[0] == 'W': 
            gwt = line.split ()
            nn = len (gwt)
            for i in range (0, nn-1): 
                gwt[i] = 1000. * float (gwt[i+1])
        elif 'TIME' in line: 
            f1 = line.split ()
            nn = len (f1)
            for i in range (0, nn-2): 
                f1[i] = float (f1[i+2])
        elif 'FUEL' in line:
            f2 = line.split ()
            nn = len (f2)
            for i in range (0, nn-2): 
                f2[i] = float (f2[i+2])
        elif 'DIST' in line:
            f3 = line.split ()
            nn = len (f3)
            ff = []
            for i in range (0, nn-2): 
                f3[i] = float (f3[i+2])
                ff.append ((f1[i], f2[i], f3[i]))
            climbp20.append (ff)
				# append zero line for interpolation
    ff = []
    for i in range (0, nn-2): ff.append((0.,0.,0.))
    climbp20.append (ff)
    ClimbFile.close ()
except IOError as e:
    print \
            'There is no climb-performance file.\n '\
          + 'Calculations will use instant climb.'

###############################################
# This section reads the descent-performance file and constructs
# the arrays used to determine descent rate, fuel usage, etc., 
# from the table
zdescent = []
try:
    DescentFile = open('./DescentISA.csv','r')	
    descent = []
    for line in DescentFile: 
        if line[0] == '#': pass
        elif line[0] == 'W': 
            gwd = line.split ()
            nn = len (gwd)
            for i in range (0, nn-1): 
                gwd[i] = 1000. * float (gwd[i+1])
        elif 'TIME' in line: 
            f1 = line.split ()
            zdescent.append (float (f1[0]))
            nn = len (f1)
            for i in range (0, nn-2): 
                f1[i] = float (f1[i+2])
        elif 'FUEL' in line:
            f2 = line.split ()
            nn = len (f2)
            for i in range (0, nn-2): 
                f2[i] = float (f2[i+2])
        elif 'DIST' in line:
            f3 = line.split ()
            nn = len (f3)
            ff = []
            for i in range (0, nn-2): 
                f3[i] = float (f3[i+2])
                ff.append ((f1[i], f2[i], f3[i]))
            descent.append (ff)
				# append zero line for interpolation
    zdescent.append (0.)
    ff = []
    for i in range (0, nn-2): ff.append((0.,0.,0.))
    descent.append (ff)
    DescentFile.close ()
#   print 'descent results for altitude = ', zdescent[5]
    ff = descent[5]
#   for i in range (0, nn-2):
#       print gwd[i], ff[i]
except IOError as e:
    print \
            'There is no descent-performance file.\n '\
          + 'Calculations will use instant descent.'
try:
    DescentFile = open('./DescentISAm20.csv','r')	
    descentm20 = []
    for line in DescentFile: 
        if line[0] == '#': pass
        elif line[0] == 'W': 
            gwd = line.split ()
            nn = len (gwd)
            for i in range (0, nn-1): 
                gwd[i] = 1000. * float (gwd[i+1])
        elif 'TIME' in line: 
            f1 = line.split ()
            nn = len (f1)
            for i in range (0, nn-2): 
                f1[i] = float (f1[i+2])
        elif 'FUEL' in line:
            f2 = line.split ()
            nn = len (f2)
            for i in range (0, nn-2): 
                f2[i] = float (f2[i+2])
        elif 'DIST' in line:
            f3 = line.split ()
            nn = len (f3)
            ff = []
            for i in range (0, nn-2): 
                f3[i] = float (f3[i+2])
                ff.append ((f1[i], f2[i], f3[i]))
            descentm20.append (ff)
				# append zero line for interpolation
    ff = []
    for i in range (0, nn-2): ff.append((0.,0.,0.))
    descentm20.append (ff)
    DescentFile.close ()
except IOError as e:
    print \
            'There is no descent-performance file.\n '\
          + 'Calculations will use instant descent.'
try:
    DescentFile = open('./DescentISAm10.csv','r')	
    descentm10 = []
    for line in DescentFile: 
        if line[0] == '#': pass
        elif line[0] == 'W': 
            gwd = line.split ()
            nn = len (gwd)
            for i in range (0, nn-1): 
                gwd[i] = 1000. * float (gwd[i+1])
        elif 'TIME' in line: 
            f1 = line.split ()
            nn = len (f1)
            for i in range (0, nn-2): 
                f1[i] = float (f1[i+2])
        elif 'FUEL' in line:
            f2 = line.split ()
            nn = len (f2)
            for i in range (0, nn-2): 
                f2[i] = float (f2[i+2])
        elif 'DIST' in line:
            f3 = line.split ()
            nn = len (f3)
            ff = []
            for i in range (0, nn-2): 
                f3[i] = float (f3[i+2])
                ff.append ((f1[i], f2[i], f3[i]))
            descentm10.append (ff)
				# append zero line for interpolation
    ff = []
    for i in range (0, nn-2): ff.append((0.,0.,0.))
    descentm10.append (ff)
    DescentFile.close ()
except IOError as e:
    print \
            'There is no descent-performance file.\n '\
          + 'Calculations will use instant descent.'
try:
    DescentFile = open('./DescentISAp10.csv','r')	
    descentp10 = []
    for line in DescentFile: 
        if line[0] == '#': pass
        elif line[0] == 'W': 
            gwd = line.split ()
            nn = len (gwd)
            for i in range (0, nn-1): 
                gwd[i] = 1000. * float (gwd[i+1])
        elif 'TIME' in line: 
            f1 = line.split ()
            nn = len (f1)
            for i in range (0, nn-2): 
                f1[i] = float (f1[i+2])
        elif 'FUEL' in line:
            f2 = line.split ()
            nn = len (f2)
            for i in range (0, nn-2): 
                f2[i] = float (f2[i+2])
        elif 'DIST' in line:
            f3 = line.split ()
            nn = len (f3)
            ff = []
            for i in range (0, nn-2): 
                f3[i] = float (f3[i+2])
                ff.append ((f1[i], f2[i], f3[i]))
            descentp10.append (ff)
				# append zero line for interpolation
    ff = []
    for i in range (0, nn-2): ff.append((0.,0.,0.))
    descentp10.append (ff)
    DescentFile.close ()
except IOError as e:
    print \
            'There is no descent-performance file.\n '\
          + 'Calculations will use instant descent.'
try:
    DescentFile = open('./DescentISAp20.csv','r')	
    descentp20 = []
    for line in DescentFile: 
        if line[0] == '#': pass
        elif line[0] == 'W': 
            gwd = line.split ()
            nn = len (gwd)
            for i in range (0, nn-1): 
                gwd[i] = 1000. * float (gwd[i+1])
        elif 'TIME' in line: 
            f1 = line.split ()
            nn = len (f1)
            for i in range (0, nn-2): 
                f1[i] = float (f1[i+2])
        elif 'FUEL' in line:
            f2 = line.split ()
            nn = len (f2)
            for i in range (0, nn-2): 
                f2[i] = float (f2[i+2])
        elif 'DIST' in line:
            f3 = line.split ()
            nn = len (f3)
            ff = []
            for i in range (0, nn-2): 
                f3[i] = float (f3[i+2])
                ff.append ((f1[i], f2[i], f3[i]))
            descentp20.append (ff)
				# append zero line for interpolation
    ff = []
    for i in range (0, nn-2): ff.append((0.,0.,0.))
    descentp20.append (ff)
    DescentFile.close ()
except IOError as e:
    print \
            'There is no descent-performance file.\n '\
          + 'Calculations will use instant descent.'
#end of the section that reads descent information

def ISADelta (altitude):
    "Returns the departure in temperature from the ISA profile."
				# first calculate ISA temperature
    if altitude < 36089.24:
        ISAT = 15. - altitude * 0.3048 * 0.0065
    else:
        ISAT = -56.5
				# then find actual T from sounding
    if Sounding is None:
        print "WARNING: Sounding missing, using ISA values."
        return (0.)
    iS = 1
    while Sounding[iS][1] < altitude and iS < len (Sounding) - 1: iS += 1
    if iS >= len(Sounding): iS = len(Sounding)-1
#				# interpolate (unnecessarily complex?)
    F = (altitude - Sounding[iS-1][1]) \
      / (Sounding[iS][1] - Sounding[iS-1][1])
    Ts = Sounding[iS][4] * F + Sounding[iS-1][4] * (1.-F)
    return (Ts - ISAT)


def FlightSpeed (altitude):
    "Returns the flight speed of the aircraft in knots."
    global FlightLevelTemperature
#	The value of Specs['FlightSpeed'] is a Mach number, so for
#	level flight the airspeed is 38,970*M*(273.15+T)**0.5 n mi / h
#	(See NotesReGVPerformance.pdf)
#	Find temperature from sounding:
    if Sounding is None:
        print "WARNING: Sounding missing; using ISA."
        if altitude <= 36089.24:
            ISAT = 15. - altitude * 0.3048 * 0.0065
            Ps = 1013.25 * (1. - (altitude / 145442.16))**5.255877
        else:
            ISAT = -56.5
            xp = 10.**((altitude - 36089.24) / 47907.22)
            Ps = 226.3206 / xp
        if altitude < 10000.:
            TAS = 250. * (1013.25 * (ISAT+273.15) / (Ps * 288.15))**0.5
        else:
            TAS2 = 260. * (1013.25 * (ISAT+273.15) / (Ps * 288.15))**0.5
            if TAS2 < TAS: TAS = TAS2
        return (TAS)
    iS = 1
#   print len(Sounding)		# yes, len() gives number of quadruples
    while Sounding[iS][1] < altitude: iS += 1
    if iS >= len(Sounding): iS = len(Sounding)-1
#				# interpolate (unnecessarily complex?)
    F = (altitude - Sounding[iS-1][1]) / (Sounding[iS][1] - Sounding[iS-1][1])
    Ts = Sounding[iS][4] * F + Sounding[iS-1][4] * (1.-F)
    FlightLevelTemperature = Ts
    Ps = Sounding[iS][0] * F + Sounding[iS-1][0] * (1.-F)
    TAS = 38.96991 * Specs['FlightSpeed'] * (273.15 + Ts)**0.5
				# find corresponding KIAS:
    ftmp = ((Ps * 288.15) / (1013.25 * (Ts + 273.15)))**0.5
    KIAS = TAS * ftmp
#	level flight the airspeed is 38.970*M*(273.15+T)**0.5 n mi / h
    if KIAS < 218: 		# (provide margin)
        TAStest = 218. / ftmp
        Mtest = 218. / (ftmp * 38.96991 * (273.15 + Ts)**0.5)
        if Mtest > 0.85:
            TAS = 38.96991 * 0.85 * (273.15 + Ts)**0.5
        else: TAS = TAStest
				# also calculate from KIAS = 300/250
    if altitude < 10000.:
        TAS = 250. / ftmp
    else:
        TAS2 = 300. / ftmp
        if TAS2 < TAS: TAS = TAS2
    return (TAS)

def TurbulenceFlightSpeed (altitude):
    "Returns the turbulence-penetration flight speed of the aircraft in knots."
    global FlightLevelTemperature
#	The flight speed to use for turbulence is 270 KIAS or 0.8M, 
#	whichever is lower.
#	The airspeed is 38.970*M*(273.15+T)**0.5 n mi / h
#	(See NotesReGVPerformance.pdf)
#	Find temperature from sounding:
    if Sounding is None:
        print "WARNING: Sounding missing; using ISA."
        if altitude <= 36089.24:
            ISAT = 15. - altitude * 0.3048 * 0.0065
            Ps = 1013.25 * (1. - (altitude / 145442.16))**5.255877
        else:
            ISAT = -56.5
            xp = 10.**((altitude - 36089.24) / 47907.22)
            Ps = 226.3206 / xp
        if altitude < 10000.:
            TAS = 250. * (1013.25 * (ISAT+273.15) / (Ps * 288.15))**0.5
        else:
            TAS2 = 260. * (1013.25 * (ISAT+273.15) / (Ps * 288.15))**0.5
            if TAS2 < TAS: TAS = TAS2
        return (TAS)
    iS = 1
#   print len(Sounding)		# yes, len() gives number of quadruples
    while Sounding[iS][1] < altitude: iS += 1
    if iS >= len(Sounding): iS = len(Sounding)-1
#				# interpolate (unnecessarily complex?)
    F = (altitude - Sounding[iS-1][1]) / (Sounding[iS][1] - Sounding[iS-1][1])
    Ts = Sounding[iS][4] * F + Sounding[iS-1][4] * (1.-F)
    FlightLevelTemperature = Ts
    Ps = Sounding[iS][0] * F + Sounding[iS-1][0] * (1.-F)
    TAS = 38.96991 * Specs['FlightSpeed'] * (273.15 + Ts)**0.5
				# find corresponding KIAS:
    ftmp = ((Ps * 288.15) / (1013.25 * (Ts + 273.15)))**0.5
    KIAS = TAS * ftmp
    if KIAS < 218: 		# (provide margin)
        TAStest = 218. / ftmp
        Mtest = 218. / (ftmp * 38.96991 * (273.15 + Ts)**0.5)
        if Mtest > 0.85:
            TAS = 38.96991 * 0.85 * (273.15 + Ts)**0.5
        else: TAS = TAStest
				# also calculate from KIAS = 270/250
    if altitude < 10000.:
        TAS = 250. / ftmp
    else:
        TAS2 = 270. / ftmp
        if TAS2 < TAS: TAS = TAS2
    return (TAS)


def MachNumber (altitude):
    'Returns the flight speed in terms of the Mach Number.'
    global FlightLevelTemperature
				# for convenience, call FlightSpeed. 
				# This does unnecessary calculations
				# but sets the global variable 
				# 'FlightLevelTemperature', used here
    FS = FlightSpeed (altitude)
    return (FS / (38.96991 * (273.15 + FlightLevelTemperature)**0.5))

def Wind (altitude,time = 0.):
    "Returns the assumed wind in direction (deg.) and speed (kts)."
    if Sounding is None:
        print "WARNING: Using fixed wind speed rather than sounding."
        return (Specs.Wind)
    iS = 1
#   print len(Sounding)		# yes, len() gives number of quadruples
    while Sounding[iS][1] < altitude and iS < len (Sounding) - 1: iS += 1
#   print 'Wind call with altitude ', altitude
#   print ' iS=',iS, ', value ', Sounding[iS]
    if iS == len(Sounding) - 1:
        Wd = Sounding[iS][2]
        Ws = Sounding[IS][3]
        return (Wd, Ws)
#				# interpolate (unnecessarily complex?)
    F = (altitude - Sounding[iS-1][1]) / (Sounding[iS][1] - Sounding[iS-1][1])
#		Protect against 360-wrap-around:
    if (Sounding[iS][2] - Sounding[iS-1][2]) < -180.:
        Wd = Sounding[iS][2] * F + (Sounding[iS-1][2]-360.) * (1.-F)
        if Wd < 0: Wd += 360.
    elif (Sounding[iS][2] - Sounding[iS-1][2]) >= 180.:
        Wd = Sounding[iS][2] * F + (Sounding[iS-1][2]+360.) * (1.-F)
        if Wd > 360: Wd -= 360.
    else:
        Wd = Sounding[iS][2] * F + Sounding[iS-1][2] * (1.-F)
				# It might seem that the interpolated speed
				# should take into account the vector nature
				# of the average -- but a vector diagram
				# will convince that this mean value is
				# correct:
    Ws = Sounding[iS][3] * F + Sounding[iS-1][3] * (1.-F)
    W = (Wd, Ws)
#   print 'W=',W,', F=',F, 'S[i-1]=',Sounding[iS-1]
    return (W)

def MaxTurnRate(altitude):
    "Returns the maximum turn rate of the aircraft in deg/s."
    if altitude < 35000.: return Specs['MaxTurn']
    else: return Specs['MaxTurn'] / 2.

def FuelBurnRate (altitude):	
    "Returns the fuel burn rate in lb/h."
				# to obtain fuel burn, need to use the
				# current weight and the flight speed,
				# with table that shows altitude and T
				# dependence
    GW = GrossWeight () / 1000.
    TAS = FlightSpeed (altitude)
				# find the corresponding Mach No:
    MachNo = MachNumber (altitude)
    Altitudes = SRdata.keys ()
    Altitudes.sort ()
    ix_alt, f_alt = Interp (SRdata, altitude)
				# f_alt is fraction of [ix_alt] to use
				# get the two altitude dictionaries:
    G1 = SRdata[Altitudes[ix_alt]]
    G0 = SRdata[Altitudes[ix_alt-1]]
				# now find interpolation factor in each
				# gross-weight dictionary:
    keysGW = G1.keys ()
    keysGW.sort ()
    ig, f_GW = Interp (G1, GW)
				# get the bracketing Mach numbers
    M1 = G1[keysGW[ig]]
    M0 = G1[keysGW[ig-1]]
    im, f_Mach = Interp (M1, MachNo)
    k1 = M1.keys ()
    k1.sort ()
    sr1 = M1[k1[im-1]] * (1. - f_Mach) + f_Mach * M1[k1[im]]
    im, f_Mach = Interp (M0, MachNo)
    k0 = M0.keys ()
    k0.sort ()
    sr0 = M0[k0[im-1]] * (1. - f_Mach) + f_Mach * M0[k0[im]]
				# interpolate in gross weight
    srZ1 = sr0 + f_GW * (sr1 - sr0)
				# repeat for other altitude
    keysGW = G0.keys ()
    keysGW.sort ()
    ig, f_GW = Interp (G0, GW)
				# get the bracketing Mach numbers
    M1 = G0[keysGW[ig]]
    M0 = G0[keysGW[ig-1]]
    im, f_Mach = Interp (M1, MachNo)
    k1 = M1.keys ()
    k1.sort ()
    sr1 = M1[k1[im-1]] * (1. - f_Mach) + f_Mach * M1[k1[im]]
    im, f_Mach = Interp (M0, MachNo)
    k0 = M0.keys ()
    k0.sort ()
    sr0 = M0[k0[im-1]] * (1. - f_Mach) + f_Mach * M0[k0[im]]
				# interpolate in gross weight
    srZ0 = sr0 + f_GW * (sr1 - sr0)
				# finally, interpolate in altitude.
				# (bilinear interpolation in GW and 
				# altitude would be better...)
    sr = srZ0 + f_alt * (srZ1 - srZ0)
				# compensate for departure from ISA
    sr *= (1. - 0.0007 * ISADelta (altitude))
    return TAS / sr

        
def Interp (D, x):
    'General interpolation in dictionary of values; returns '\
    + 'dictionary key for first item with key larger than x and also '\
    + 'interp factor Fint such that Vint = V0 + Fint * (V1 - V0)'
    Keys = D.keys ()
    Keys.sort ()
    i = 0
    if x >= float (Keys[len (Keys) - 1]):
        i = len (Keys) - 1
    elif x <= float (Keys[0]):
        i = 1
    else:
        while float (Keys[i]) < x: i += 1
    f = (x - float (Keys[i-1])) / (float (Keys[i]) - float (Keys[i-1]))
    return (i,f)


def FuelBurnRateOld (altitude):	# old version kept for reference
    "Returns the fuel burn rate in lb/h."
				# to obtain fuel burn, need to use the
				# current weight and the flight speed,
				# with table that shows altitude and T
				# dependence
    global frame
    GW = GrossWeight () / 1000.
    TAS = FlightSpeed (altitude)
				# get table entry for NAMLB
    NAMLB = 0.14		# for example
    i = 1
    j = 0
    fx = fy = 0.
    while i < 24 and GW < gwtff[i]: i += 1
    while j < 10  and altitude < NAMPP[j][0]: j += 1
				# improve this via interpolation
    if i > 1:			# index to wt lt GW; i-1 is wt gt GW
        fx = (GW - gwtff[i]) / (gwtff[i-1] - gwtff[i])
        if j > 0 and altitude > NAMPP[10][0]:	# index to altitude lt GV 
						# j-1 is gt
            fy = (altitude - NAMPP[j][0]) / (NAMPP[j-1][0] - NAMPP[j][0])
            NAMLB = NAMPP[j][i] * (1. - fx) * (1. - fy) \
                  + NAMPP[j][i-1] * fx * (1. - fy) \
                  + NAMPP[j-1][i] * fy * (1. - fx) \
                  + NAMPP[j-1][i-1] * fx * fy
        else:			# don't extrapolate beyond table limits
            NAMLB = NAMPP[j][i] * (1. - fx) + NAMPP[j][i-1] * fx
    elif j > 0:
        fy = (altitude - NAMPP[j][0]) / (NAMPP[j-1][0] - NAMPP[j][0])
        NAMLB = NAMPP[j][i] * (1. - fy) * NAMPP[j-1][i]
#   print 'interpolated value from i,j = ',i,j, ' is ',NAMLB
#   print ' altitude and GV are ', altitude, GW, 'fx,fy are ', fx,fy
				# also adjust by -0.07% for each degree
				# above ISA
    NAMLB *= (1. - 0.0007 * ISADelta (altitude))
#   NAMLB = NAMPP[j][i]
				# for -ve return, exceeds ceiling:
    if NAMLB < 0.:
        while NAMLB < 0. and j < 9: 
            j += 1
            ceiling = NAMPP[j][0]
            NAMLB = NAMPP[j][i]
        print " request exceeds altitude ceiling for this GW"
        dlg = wx.MessageDialog (frame,\
                'The altitude exceeds the ceiling for this\n'\
              + 'weight. Please redefine the last module with\n'\
              + 'a lower altitude. (The ceiling at this weight\n'\
              + 'is about ' + format (ceiling, '.0f') + ' so the '\
              + 'altitude was\nreset to that value.)\n', \
                'Altitude Exceeds Ceiling', wx.OK)
        result = dlg.ShowModal ()
        dlg.Destroy ()
        if frame != None:
            frame.SetStatusText \
                 ('reset altitude to ceiling', 2)
            frame.InputZ.SetValue (format (ceiling, '.0f'))
        TAS = FlightSpeed (ceiling)
    return TAS / NAMLB

def ClimbValues (Z1, Z2, RateLimit = None):
    'Returns the time required to climb, the fuel used, the '\
     + 'distance traveled, and the eastward and northward displacements'\
     + 'by wind during the climb.'
    global ceiling
    Sum = (0., 0., 0., 0., 0.)
    ceiling = 0.
				# do climb in 1000-ft steps, to
				# represent the actual sounding T:
    if (Z2 <= Z1): return Sum
    Za = Z1
    Zb = Z1 + 1000.
    Zb = math.floor (Zb/1000.) * 1000.
    while Zb <= Z2:
        delta = DeltaClimb (Za, Zb)
        if delta[0] <= 0.: break
        Sum = (Sum[0] + delta[0], Sum[1] + delta[1], Sum[2] + delta[2],\
               Sum[3] + delta[3], Sum[4] + delta[4])
				# The rate-limited values are quite 
				# uncertain, esp. the Fuel value,
				# because there aren't data for this.
        if RateLimit is not None:
            ClimbRate = (Zb - Za) / (delta[0] * 60.)
            if ClimbRate > RateLimit:
                delta = (delta[0] * ClimbRate / RateLimit, \
                         delta[1] * (ClimbRate / RateLimit)**0.5, \
                         delta[2] * ClimbRate / RateLimit, \
                         delta[3] * ClimbRate / RateLimit, \
                         delta[4] * ClimbRate / RateLimit)
#       print "Za, Zb, delta, Sum = ", Za, Zb, delta, Sum
        Za = Zb
        Zb += 1000.
    if Za < Z2:
        delta = DeltaClimb (Za, Z2)
        Sum = (Sum[0] + delta[0], Sum[1] + delta[1], Sum[2] + delta[2],\
               Sum[3] + delta[3], Sum[4] + delta[4])
				# for comparison:
#   Sum = DeltaClimb (Z1, Z2)
    return Sum

def DeltaClimb (Z1, Z2):
    "Called by ClimbValues to get increment in the climb."
    global frame
    global ceiling
				# check takeoff-level temperature vs
				# ISA, via ISAdelta:
    diff = 0.5 * (ISADelta (Z1) + ISADelta (Z2))
				# find how to interpolate in tables
				# for departure from ISA
    if diff <= -10.: 
        C1 = climbm20
        C2 = climbm10
        Fi = (diff + 20.) / 10.
    elif diff < 0.: 
        C1 = climbm10
        C2 = climb
        Fi = (diff + 10.) / 10.
    elif diff < 10.: 		# interpolate between ISA and ISAp10:
        C1 = climb
        C2 = climbp10
        Fi = diff / 10.
    else:
        C1 = climbp10
        C2 = climbp20
        Fi = (diff - 10.) / 10.
#   print 'Departure from ISA = %.2f, Fi (fraction higher)=%.2f' % (diff, Fi)
				# Fi now specifies how to interpolate
				# between C1 and C2. Next, get altitude
				# index and interpolation factor for Z1
    iz = 0
    while zclimb[iz] >= Z1 and iz < len (zclimb) - 1: iz += 1
    if iz == len (zclimb) - 1: iz -= 1
    Fz = (zclimb[iz-1] - Z1) / (zclimb[iz-1] - zclimb[iz])
				# iz is now index for z < Z1 and iz-1
				# for Z1 < z; Fz is interpolation factor
				# giving fraction iz = smaller altitude
#   print 'Z1 = %f, iz = %d, Fz = %f zclimb[iz-1] and zclimb[iz] are %f %f' % (Z1, iz, Fz, zclimb[iz-1], zclimb[iz])
#   slice1 = C1[iz] * (1. - Fi) + Fi * C2[iz]
#   slice2 = C1[iz-1] * (1. - Fi) + Fi * C2[iz-1]
#   slicez = Fz * slice1 + (1. - Fz) * slice2
    slice1 = weightSlice (C1[iz], C2[iz], Fi)
    slice2 = weightSlice (C1[iz-1], C2[iz-1], Fi)
    slicez = weightSlice (slice2, slice1, Fz)
				# now search through slicez for the 
				# right GW = Takeoff GW - fuel used
    GW = GrossWeight ()
    ig = 0
    while gwt[ig] - slicez[ig][1] > GW and ig < (len (gwt) -1): ig += 1 
    if ig == len (gwt) - 1: 
        Fg = 1.
    elif ig == 0: 
        ig = 1
        Fg = 0.
    else:			# Fg is fraction that is [ig]
        Fg = (gwt[ig-1] - slicez[ig-1][1] - GW) \
           / (gwt[ig-1] - slicez[ig-1][1] - (gwt[ig] - slicez[ig][1]))
#   print 'GW = %f, ig = %d, gwt[ig] and gwt[ig-1] = %f %f, Fg = %f' %\
#           (GW, ig, gwt[ig], gwt[ig-1], Fg)
#   print 'consumed fuel at ig and ig-1: %f %f' % (slicez[ig][1], slicez[ig-1][1])
    Start  =  (((1. - Fg) * slicez[ig-1][0] + Fg * slicez[ig][0])/60.,\
               ((1. - Fg) * slicez[ig-1][1] + Fg * slicez[ig][1]),\
               ((1. - Fg) * slicez[ig-1][2] + Fg * slicez[ig][2]))
				# now repeat all for the end altitude
    iz = 0
    while zclimb[iz] >= Z2 and iz < len (zclimb) - 1: iz += 1
    if iz == len (zclimb) - 1: iz -= 1
    Fz = (zclimb[iz-1] - Z2) / (zclimb[iz-1] - zclimb[iz])
				# iz is now index for z < Z2 and iz-1
				# for Z2 < z; Fz is interpolation factor
#   print 'Z2 = %f, iz = %d, Fz = %f zclimb[iz-1] and zclimb[iz] are %f %f' % (Z2, iz, Fz, zclimb[iz-1], zclimb[iz])
    slice1 = weightSlice (C1[iz], C2[iz], Fi)
    slice2 = weightSlice (C1[iz-1], C2[iz-1], Fi)
    slicez = weightSlice (slice2, slice1, Fz)
    if slicez [ig][0] < 0.:	# Requested altitude above ceiling
        print 'Requested altitude above ceiling;\nCeiling at GW of %.0f lb is %.0f ft' % (GW, ceiling)
        print 'In Specs, frame = ', frame
        if frame != None:
            frame.SetStatusText \
                 ('WARNING: request GT ceiling of ' \
                   + format (ceiling, '.0f') + ' ft', 2)
            frame.SetStatusText ('Warning!!!')
        return (0.,0.,0.,0.,0.)
    else: ceiling = Z2
				# Takeoff weight stays the same so keep
				# index and interpolation factor
    EndPt  =  (((1. - Fg) * slicez[ig-1][0] + Fg * slicez[ig][0])/60.,\
               ((1. - Fg) * slicez[ig-1][1] + Fg * slicez[ig][1]),\
               ((1. - Fg) * slicez[ig-1][2] + Fg * slicez[ig][2]))
#   print 'Start and EndPt are ', Start, EndPt
				# get the wind at this altitude, to 
				# estimate track drift by wind
    Wd, Ws = Wind ((Z1 + Z2) / 2.)
    dx = -Ws * math.sin (Wd * Cradeg) * (EndPt[0] - Start[0]) / 60.
    dy = -Ws * math.cos (Wd * Cradeg) * (EndPt[0] - Start[0]) / 60.
    return  (EndPt[0] - Start[0], EndPt[1] - Start[1], \
             EndPt[2] - Start[2], dx, dy)

def DescentValues (Z1, Z2, RateLimit = None):
    'Returns the time required to descend, the fuel used, the air'\
     + 'distance traveled, and the track displacements (east and'\
     + 'north) by the wind during the climb.'
    Sum = (0., 0., 0., 0., 0.)
				# do descent in 1000-ft steps, to
				# represent the actual sounding T:
    if (Z2 >= Z1): return Sum
    Za = Z1
    Zb = Z1 - 1000.
    Zb = math.floor (Zb/1000.) * 1000.
    if RateLimit != None: RateLimit *= -1 # positive value used as limit
    while Zb >= Z2:
        delta = DeltaDescent (Za, Zb)
        Sum = (Sum[0] + delta[0], Sum[1] + delta[1], Sum[2] + delta[2],\
               Sum[3] + delta[3], Sum[4] + delta[4])
				# The rate-limited values are quite 
				# uncertain, esp. the Fuel value,
				# because there aren't data for this.
        if RateLimit is not None:
            DescentRate = (Zb - Za) / (delta[0] * 60.)
            if DescentRate > RateLimit:
                delta = (delta[0] * DescentRate / RateLimit, \
                         delta[1] * DescentRate / RateLimit, \
                         delta[2] * DescentRate / RateLimit, \
                         delta[3] * DescentRate / RateLimit, \
                         delta[4] * DescentRate / RateLimit)
#       print "Za, Zb, delta, Sum = ", Za, Zb, delta, Sum
        Za = Zb
        Zb -= 1000.
    if Za > Z2:
        delta = DeltaDescent (Za, Z2)
        Sum = (Sum[0] + delta[0], Sum[1] + delta[1], Sum[2] + delta[2],\
               Sum[3] + delta[3], Sum[4] + delta[4])
				# for comparison:
#   Sum = DeltaDescent (Z1, Z2)
    return Sum

def DeltaDescent (Z1, Z2):
    "Called by DescentValues to get increment in the descent."
				# check takeoff-level temperature vs
				# ISA, via ISAdelta:
    diff = 0.5 * (ISADelta (Z1) + ISADelta (Z2))
				# find how to interpolate in tables
				# for departure from ISA
    if diff <= -10.: 
        C1 = descentm20
        C2 = descentm10
        Fi = (diff + 20.) / 10.
    elif diff < 0.: 
        C1 = descentm10
        C2 = descent
        Fi = (diff + 10.) / 10.
    elif diff < 10.: 		# interpolate between ISA and ISAp10:
        C1 = descent
        C2 = descentp10
        Fi = diff / 10.
    else:
        C1 = descentp10
        C2 = descentp20
        Fi = (diff - 10.) / 10.
				# Fi now specifies how to interpolate
				# between C1 and C2. Next, get altitude
				# index and interpolation factor for Z1
    iz = 0
    while zdescent[iz] >= Z1 and iz < len (zdescent) - 1: iz += 1
    if iz == len (zdescent) - 1: iz -= 1
    Fz = (zdescent[iz-1] - Z1) / (zdescent[iz-1] - zdescent[iz])
				# iz is now index for z < Z1 and iz-1
				# for Z1 < z; Fz is interpolation factor
#   print 'Z1 = %f, iz = %d, Fz = %f zdescent[iz-1] and zdescent[iz] are %f %f' % (Z1, iz, Fz, zdescent[iz-1], zdescent[iz])
#   slice1 = C1[iz] * (1. - Fi) + Fi * C2[iz]
#   slice2 = C1[iz-1] * (1. - Fi) + Fi * C2[iz-1]
#   slicez = Fz * slice1 + (1. - Fz) * slice2
    slice1 = weightSlice (C1[iz], C2[iz], Fi)
    slice2 = weightSlice (C1[iz-1], C2[iz-1], Fi)
    slicez = weightSlice (slice2, slice1, Fz)
				# now search through slicez for the 
				# right GW = Takeoff GW - fuel used
    GW = GrossWeight ()
    ig = 0
    while gwt[ig] - slicez[ig][1] > GW and ig < (len (gwt) -1): ig += 1 
    if ig == len (gwt) - 1: 
        Fg = 1.
    elif ig == 0: 
        ig = 1
        Fg = 0.
    else:			# Fg is fraction that is [ig]
        Fg = (gwt[ig-1] - slicez[ig-1][1] - GW) \
           / (gwt[ig-1] - slicez[ig-1][1] - (gwt[ig] - slicez[ig][1]))
#   print 'GW = %f, ig = %d, gwt[ig] and gwt[ig-1] = %f %f, Fg = %f' %\
#           (GW, ig, gwt[ig], gwt[ig-1], Fg)
#   print 'consumed fuel at ig and ig-1: %f %f' % (slicez[ig][1], slicez[ig-1][1])
    Start  =  (((1. - Fg) * slicez[ig-1][0] + Fg * slicez[ig][0])/60.,\
               ((1. - Fg) * slicez[ig-1][1] + Fg * slicez[ig][1]),\
               ((1. - Fg) * slicez[ig-1][2] + Fg * slicez[ig][2]))
				# now repeat all for the end altitude
    iz = 0
    while zdescent[iz] >= Z2 and iz < len (zdescent) - 1: iz += 1
    if iz == len (zdescent) - 1: iz -= 1
    Fz = (zdescent[iz-1] - Z2) / (zdescent[iz-1] - zdescent[iz])
				# iz is now index for z < Z2 and iz-1
				# for Z2 < z; Fz is interpolation factor
#   print 'Z2 = %f, iz = %d, Fz = %f zdescent[iz-1] and zdescent[iz] are %f %f' % (Z2, iz, Fz, zdescent[iz-1], zdescent[iz])
    slice1 = weightSlice (C1[iz], C2[iz], Fi)
    slice2 = weightSlice (C1[iz-1], C2[iz-1], Fi)
    slicez = weightSlice (slice2, slice1, Fz)
				# Takeoff weight stays the same so keep
				# index and interpolation factor
    EndPt  =  (((1. - Fg) * slicez[ig-1][0] + Fg * slicez[ig][0])/60.,\
               ((1. - Fg) * slicez[ig-1][1] + Fg * slicez[ig][1]),\
               ((1. - Fg) * slicez[ig-1][2] + Fg * slicez[ig][2]))
    Wd, Ws = Wind ((Z1 + Z2) / 2.)
    dx = -Ws * math.sin (Wd * Cradeg) * (EndPt[0] - Start[0]) / 60.
    dy = -Ws * math.cos (Wd * Cradeg) * (EndPt[0] - Start[0]) / 60.
    return  (Start[0] - EndPt[0], Start[1] - EndPt[1], \
             Start[2] - EndPt[2], dx, dy)

def weightSlice (S1, S2, f):
    """Calculates the interpolated result intermediate between two slices:
         result = (1.-f)*S1 + f*S2"""
    ns = len (S1)
    rt = []
    for i in range (0, ns):
        if S1[i][0] < 0 or S2[i][0] < 0:
            rt.append ((-32767.,-32767.,-32767.))
        else:
            r0 = (1. - f) * S1[i][0] + f * S2[i][0]
            r1 = (1. - f) * S1[i][1] + f * S2[i][1]
            r2 = (1. - f) * S1[i][2] + f * S2[i][2]
            rt.append((r0,r1,r2))
    return rt
        
def GrossWeight ():
    "Returns the gross weight of the aircraft including remaining fuel."
    import ModuleConstructor
    GW = Specs['GrossWeight'] - Specs['FuelLimit'] \
         + ModuleConstructor.Fuel
    return GW

def FuelUseLimit():
    "Returns the fuel available for research only -- not reserves etc."
    return Specs['FuelLimit']

def TakeoffLocation():
    "Returns the location (lat, lon) in decimal deg of the takeoff point."
    return Specs['Airport']

def Extra ():
    "Returns the extra allowance for diversions; e.g., 0.1 for 10%."
    return Specs['Extra']

