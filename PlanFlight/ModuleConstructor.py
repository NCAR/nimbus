# file: ModuleConstructor.py
"Construct maneuvers and modules that are components of the flight plan."
'2012 Copyright University Corporation for Atmospheric Research'
# this file is imported by PlanFlight.py

import Specs
import math
import copy
from Frame import frame
import wx
# Maneuvers are the most basic components: transits and turns"
# Modules are sets of maneuvers; e.g., vertical section or lawnmower
FlightDirection = 0.
ManeuverNumber = 0
ModuleNumber = 0
Track = {}		#Track is a dictionary consisting of Modules and ID
			#Modules consist of sets of Maneuvers
WorkingModuleName = 'InitialClimb'
WorkingModule = {}
Time = 0.		# Time and Fuel accumulate/decrement during the plan
Fuel = Specs.FuelUseLimit()
AC = Specs.TakeoffLocation()	# AC is always the aircraft location
RE = 6371.		# radius of the earth, km
RE = 6366.707025 	# radius of the earth, km, needed for 1 n mi / minute
			# (not taking into account altitude of the AC)
RE /= 1.852		# convert to n mi
PI = math.pi		# Used often here
Cradeg = PI / 180. 	# Conversion factor from degrees to radians
			# (Used many places in this module)

def Maneuver (t = 'Transit', s=100., a = (105.,40.,6000), o = 0.,\
              st = None, r = 0., drift = False):
    "Create a new 'MANVR' dictionary: Called by Transit or Turn."
			# 'Subtype' is not used now, could be omitted
			# 'EndPoint', 'Time' etc. are useful in case 
			# the modules are edited.
			# They are added to the dictionary item later.
    Keys = ["Type","Dimension","Anchor","Orientation","SubType","Rate",\
            "Drifting", "Time", "EndPoint", "Fuel", "FlightDirection"]
    m=dict.fromkeys(Keys)
    m[Keys[0]]=t
    m[Keys[1]]=s
    m[Keys[2]]=a
    m[Keys[3]]=o
    m[Keys[4]]=st
    m[Keys[5]]=r
    m[Keys[5]]=drift
    return m

def Module (t = 'Enroute', s = 100., a = (105.,40.,6000), o = 0., \
            drift = False, l = 2, other = None, frame = None):
    "Core component here: Creates module consisting of a set of maneuvers."
    global Fuel, Time, ManeuverNumber, ModuleNumber, Track, \
           FlightDirection, AC

    ModuleNumber += 1
    m = dict.fromkeys (Specs.ModuleKeys)
    m[Specs.ModuleKeys[0]] = t
    m[Specs.ModuleKeys[1]] = s
    m[Specs.ModuleKeys[2]] = a
    m[Specs.ModuleKeys[3]] = o
    m[Specs.ModuleKeys[4]] = drift
    m[Specs.ModuleKeys[5]] = l
    m[Specs.ModuleKeys[6]] = other
#       implement various types of modules here

    if(t == 'OutAndBack'): 		# OutAndBack
        if (ManeuverNumber != 0): 	# don't turn if first maneuver
                                        # Find angle of turn needed to
                                        # head for the start point
            dx = (a[0] - AC[0]) * math.cos ((a[1] + AC[1]) * Cradeg / 2.)
            dy = a[1] - AC[1]
            oo = math.atan2 (dy, dx) / Cradeg
            oo = 90. - oo
            v = Turn (oo)
					# ky is the key to the maneuver
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
# transit to the start point, a
        v = Transit (AC, a, drift, 0., m)
        if v != None:
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
        v = Turn (o)
        if drift:
            DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
            a = DriftOffset (DT, a)
        ky = "Manvr" + format (ManeuverNumber, '03d')
        m[ky] = v
# now fly in direction o for distance s from a
# (assume Cartesion OK here)
        b = copy.deepcopy (a)
        b0 = b[0] + s * math.sin (o * Cradeg) \
           / (60. * math.cos (b[1] * Cradeg))
        b1 = b[1] + s * math.cos (o * Cradeg) / 60.
        b = (b0, b1, b[2])
        v = Transit (AC, b, drift, 0., m)
        if v != None:
            if drift:
                DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
                a = DriftOffset (DT, a)
                b = DriftOffset (DT, b)
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
        oo = o + 180.
        while oo > 360.: oo -= 360.
        v = Turn (oo)
        if drift:
            DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
            a = DriftOffset (DT, a)
        ky = "Manvr" + format (ManeuverNumber, '03d')
        m[ky] = v
        v = Transit (AC, a, drift, 0., m)
        if v != None:
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
 
# VerticalSection is like OutAndBack except for altitude change
# and variable number of legs; altitude increment specified in 'other'.
    elif (t == 'VerticalSection'):     	# VerticalSection
        legs = l			# need even number to end at start
        AltitudeIncrement = other
        if (ManeuverNumber != 0): 	# don't turn if first maneuver
                                        # Find angle of turn needed to
                                        # head for the start point
            dx = (a[0] - AC[0]) * math.cos ((a[1] + AC[1]) * Cradeg / 2.)
            dy = a[1] - AC[1]
            oo = math.atan2 (dy, dx) / Cradeg
            oo = 90. - oo
            v = Turn (oo)
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
# transit to start point
        v = Transit (AC, a, drift, 0., m)
        if v != None:
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
        v = Turn (o)
        if drift:
            DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
            a = DriftOffset (DT, a)
        ky = "Manvr" + format (ManeuverNumber, '03d')
        m[ky] = v
# save the target point, then alternate between b and a
# (assume Cartesion OK here)
        b = copy.deepcopy (a)
        b0 = b[0] + s * math.sin (o * Cradeg) \
           / (60. * math.cos (b[1] * Cradeg))
        b1 = b[1] + s * math.cos (o * Cradeg) / 60.
        b = (b0, b1, b[2])
        c = copy.deepcopy (b)
        while (legs > 0):
            v = Transit (AC, c, drift, 0., m)
            if v != None:
                if drift:
                    DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
                    a = DriftOffset (DT, a)
                    b = DriftOffset (DT, b)
                    c = DriftOffset (DT, c)
                ky = "Manvr" + format (ManeuverNumber, '03d')
                m[ky] = v
            if (legs > 1):
                fd = FlightDirection + 180.
                while fd > 360.: fd -= 360.
                c = (c[0], c[1], c[2] + AltitudeIncrement)
                AC = c
                v = Turn (fd)
                if drift:
                    DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
                    a = DriftOffset (DT, a)
                    b = DriftOffset (DT, b)
                    c = DriftOffset (DT, c)
                ky = "Manvr" + format (ManeuverNumber, '03d')
                m[ky] = v
                if (c[0] == b[0] and c[1] == b[1]): 
                    c = (a[0], a[1], c[2])
                else: 
                    c = (b[0], b[1], c[2])
            legs -= 1

    elif (t == 'Sawtooth'):
# 'Sawtooth" is like an 'OutAndBack' but with repeated climbs and descents
        if (ManeuverNumber != 0): 	# don't turn if first maneuver
                                        # Find angle of turn needed to
                                        # head for the start point
            dx = (a[0] - AC[0]) * math.cos ((a[1] + AC[1]) * Cradeg / 2.)
            dy = a[1] - AC[1]
            oo = math.atan2 (dy, dx) / Cradeg
            oo = 90. - oo
            if (oo < 0.): oo += 360.
            v = Turn (oo)
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
# transit to start point, a
        v = Transit (AC, a, False, 0., m)
        if v != None:
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
        v = Turn (o)
        if drift:
            DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
            a = DriftOffset (DT, a)
        ky = "Manvr" + format (ManeuverNumber, '03d')
        m[ky] = v
# now fly in direction o for distance s from a
# (assume Cartesion OK here)
        b = copy.deepcopy (a)
        b0 = b[0] + s * math.sin (o * Cradeg) \
           / (60. * math.cos (b[1] * Cradeg))
        b1 = b[1] + s * math.cos (o * Cradeg) / 60.
        b = (b0, b1, b[2]+other)
# dist to specified point
        (dist, B) = GreatCircle (AC,b)
        if b[2] > AC[2]: 
            icl = 1
            LowerAltitude = AC[2]
            UpperAltitude = b[2]
        else:
            icl = 0
            LowerAltitude = b[2]
            UpperAltitude = AC[2]
				# find time, fuel, distance for climb
				# and for descent:
        Tclimb,Fclimb,Dclimb,dx,dy = Specs.ClimbValues (LowerAltitude, \
                               UpperAltitude, other)
				# adjust for wind
        if drift: pass
        else:		
            wdir = (90. - math.atan2 (dy, dx) / Cradeg)
            beta = (B - wdir) 	# includes correction to-to-from
            while beta < 0.: beta += 360.
            beta *= Cradeg
            dw = (dx*dx + dy*dy)**0.5
            sina = dw * math.sin (beta) / Dclimb
            alpha = math.asin (sina)
            Dclimb = (Dclimb**2 + dw**2 \
                     -2. * Dclimb * dw * math.cos (alpha + beta))**0.5
        Tdescent,Fdescent,Ddescent,dx,dy = Specs.DescentValues (UpperAltitude, \
                               LowerAltitude, other)
        if drift: pass
        else:		
            wdir = (90. - math.atan2 (dy, dx) / Cradeg)
            beta = (B - wdir) 	# includes correction to-to-from
            while beta < 0.: beta += 360.
            beta *= Cradeg
            dw = (dx*dx + dy*dy)**0.5
            sina = dw * math.sin (beta) / Dclimb
            alpha = math.asin (sina)
            Ddescent = (Ddescent**2 + dw**2 \
                     -2. * Ddescent * dw * math.cos (alpha + beta))**0.5
				# now do series of climbs and descents
				# stopping at 'a'
        dd = 0.
        while dd < dist:	# add drift corrections later XXXXX
            if icl == 1:	# climb
                dd += Dclimb
                ap = (a[0] + (b[0] - a[0]) * dd / dist,\
                      a[1] + (b[1] - a[1]) * dd / dist,\
                      UpperAltitude)
                v = Transit (AC, ap, drift, other, m)
                if v != None:
                    ky = "Manvr" + format (ManeuverNumber, '03d')
                    m[ky] = v
                if drift: 
                    a = DriftOffset (Tclimb, a)
                    b = DriftOffset (Tclimb, acc)
            else:		# descent
                dd += Ddescent
                ap = (a[0] + (b[0] - a[0]) * dd / dist,\
                      a[1] + (b[1] - a[1]) * dd / dist,\
                      LowerAltitude)
                v = Transit (AC, ap, drift, other, m)
                if v != None:
                    ky = "Manvr" + format (ManeuverNumber, '03d')
                    m[ky] = v
                if drift: 
                    a = DriftOffset (Tdescent, a)
                    a = DriftOffset (Tdescent. b)
            icl = 1 - icl
        #end of 'while dd < dist' loop


# Climb at specified rate or max rate, staying at starting location   
# (possibly with drift):
    elif (t == 'SpiralProfile'):      	# SpiralProfile
        if other == 0.:
            print 'ERROR: must specify climb rate (1000 ft/min used'
            other = 1000.
        if (ManeuverNumber != 0): 	# don't turn if first maneuver
                                        # Find angle of turn needed to
                                        # head for the start point
            dx = (a[0] - AC[0]) * math.cos ((a[1] + AC[1]) * Cradeg / 2.)
            dy = a[1] - AC[1]
            oo = math.atan2 (dy, dx) / Cradeg
            oo = 90. - oo
            v = Turn (oo)
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
# transit to start point, without drift and without altitude change
        ax = (a[0],a[1],AC[2])
        v = Transit (AC, ax, False, 0., m)
        if v != None:
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
# add zero-distance Transit with climb, possibly drifting:
        DT = abs ((a[2] - AC[2]) / other) / 60.
        if drift:
            a = DriftOffset (DT, a)
            AC = a
# increment Time and Fuel before calling Transit so those numbers
# are entered appropriately in the zero-distance transit
        Time += DT
        Fuel -= Specs.FuelBurnRate (AC[2]) * DT
# when 'Transit' adjusts time for climb, need to revise this also
        v = Transit (AC, a, drift, 0., m)
        if v != None:
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
#					# add FlightDirection for kml:
        m['FlightDirection'] = FlightDirection

# for Circle, transit to specified point, turn to specified direction,
# and fly a circle of the specified diameter. Calculate the required
# turn rate given the flight speed and display it.
    elif (t == 'Circle'):      	# Circle
        if (ManeuverNumber != 0): 	# don't turn if first maneuver
                                        # Find angle of turn needed to
                                        # head for the start point
            dx = (a[0] - AC[0]) * math.cos ((a[1] + AC[1]) * Cradeg / 2.)
            dy = a[1] - AC[1]
            oo = math.atan2 (dy, dx) / Cradeg
            oo = 90. - oo
            v = Turn (oo)
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
# transit to start point, without drift 
        v = Transit (AC, a, False, 0., m)
        if v != None:
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
# turn to initial orientation, and start drift calculation if needed:
        v = Turn (o)
        if drift:
            DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
            a = DriftOffset (DT, a)
        ky = "Manvr" + format (ManeuverNumber, '03d')
        m[ky] = v
# find turn rate required 
        DT = PI * s / Specs.FlightSpeed (AC[2])
        TurnRate = 360. / (DT * 3600.)
        if TurnRate > Specs.MaxTurnRate (AC[2]):
            TurnRate = Specs.MaxTurnRate (AC[2])
            s = (360. / TurnRate) * Specs.FlightSpeed (AC[2]) \
              / (PI * 3600.)
            print '*** MAX turn rate exceeded; diameter reset to ', s
            frame.SetStatusText \
                 ('reset distance to match max turn rate', 2)
            if frame != None:
                frame.InputS.SetValue (format (s, '.0f'))
            m['Dimension'] = s
            DT = 360. / (TurnRate * 3600)
# now do zero-distance Transit, possibly with drift:
        if drift:
            a = DriftOffset (DT, a)
            AC = a
#                               Note: time required to fly a ground-
#                               relative track with wind differs
#                               from the estimate without wind, so
#                               revision is needed here if the non-
#                               drifting calculation is actually 
#                               used. 
        Time += DT
        Fuel -= Specs.FuelBurnRate (AC[2]) * DT
        v = Transit (AC, a, drift, 0., m)
        if v != None:
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
            m['EndPoint'] = AC

# Figure8 is much like two consecutive circles.
# Transit to specified point, turn to specified direction,
# and fly a circle with diameter 1/2 the specified distance. Repeat, 
# turning other way. 'Legs' is number of times the full figure8
# pattern is flown, optionally changing altitude between figures-8
# as specified in 'other'
    elif (t == 'Figure8'):      	# Figure8
        if (ManeuverNumber != 0): 	# don't turn if first maneuver
                                        # Find angle of turn needed to
                                        # head for the start point
            dx = (a[0] - AC[0]) * math.cos ((a[1] + AC[1]) * Cradeg / 2.)
            dy = a[1] - AC[1]
            oo = math.atan2 (dy, dx) / Cradeg
            oo = 90. - oo
            v = Turn (oo)
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
# transit to start point, without drift 
        v = Transit (AC, a, False, 0., m)
        if v != None:
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
# turn to initial orientation, and start drift calculation if needed:
        v = Turn (o)
        if drift:
            DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
            a = DriftOffset (DT, a)
        ky = "Manvr" + format (ManeuverNumber, '03d')
        m[ky] = v
# find turn rate required 
        DT = PI * 0.5 * s / Specs.FlightSpeed (AC[2])
        TurnRate = 360. / (DT * 3600.)
        if TurnRate > Specs.MaxTurnRate (AC[2]):
            TurnRate = Specs.MaxTurnRate (AC[2])
            s = 2. * (360. / TurnRate) * Specs.FlightSpeed (AC[2]) \
              / (PI * 3600.)
            print '*** MAX turn rate exceeded; diameter reset to ', s
            frame.SetStatusText \
                 ('reset distance to match max turn rate', 2)
            if frame != None:
                frame.InputS.SetValue (format (s, '.0f'))
            m['Dimension'] = s
            DT = 360. / TurnRate
# now do zero-distance Transit, possibly with drift, twice
#       v = Transit (AC, a, drift, 0., m)
        while l > 0:
            l -= 1
            v = Delay (DT)
            if drift:
                a = DriftOffset (DT, a)
                AC = a
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
            v = Delay (DT)
            if drift:
                a = DriftOffset (DT, a)
                AC = a
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
            if l > 0:
                AC = (AC[0], AC[1], AC[2]+other)
        #end of while look on legs

    elif (t == 'InitialClimb'):      	# InitialClimb
# Climb at specified rate or max rate, then proceed to specified point
        if (ManeuverNumber != 0): 	# don't turn if first maneuver
					# (This should always be first)
                                        # Find angle of turn needed to
                                        # head for the start point
            dx = (a[0] - AC[0]) * math.cos ((a[1] + AC[1]) * Cradeg / 2.)
            dy = a[1] - AC[1]
            oo = math.atan2 (dy, dx) / Cradeg
            oo = 90. - oo
            v = Turn (oo)
            if drift:
                DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
                a = DriftOffset (DT, a)
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
# decrement fuel by approximate amount required for taxi, runway
# acceleration, and initial takeoff:
        Fuel -= 400.
# transit includes the climb, but there is some additional time typically
# required to move through maneuvers required for departure control and
# other traffic control. Add to time and fuel for this (only approximate
# for now; update later):
        Time += 0.2
        Fuel -= 800.
# transit to start point
        v = Transit (AC, a, drift, 0., m)
        if v != None:
            if drift:
                DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
                a = DriftOffset (DT, a)
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
 
# Lawnmower is like VerticalSection but with horizontal displacement 
# (+ve initially to R)
    elif (t == 'Lawnmower' or t == 'Racetrack'):    
        legs = l			# even number to end on start side
        if (ManeuverNumber != 0): 	# don't turn if first maneuver
                                        # Find angle of turn needed to
                                        # head for the start point
            dx = (a[0] - AC[0]) * math.cos ((a[1] + AC[1]) * Cradeg / 2.)
            dy = a[1] - AC[1]
            oo = math.atan2 (dy, dx) / Cradeg
            oo = 90. - oo
            v = Turn (oo)
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
# transit to start point
        v = Transit (AC, a, False, 0., m)	# suppress drift
        if v != None:
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
        HorizontalIncrement = other
        if (HorizontalIncrement >= 0): iturn = 1
        else: iturn = -1
        v = Turn (o)		# drift suppressed
        ky = "Manvr" + format (ManeuverNumber, '03d')
        m[ky] = v
# (assume Cartesion OK here)
        c = copy.deepcopy (a)	# make copy to preserve a
        c0 = c[0] + s * math.sin (o * Cradeg) \
             / (60. * math.cos (c[1] * Cradeg))
        c1 = c[1] + s * math.cos (o * Cradeg) / 60.
        c = (c0, c1, c[2])
        while (legs > 0):
            v = Transit (AC, c, drift, 0., m)
            if drift:
                DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
                c = DriftOffset (DT, c)
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
            if (legs > 1):
                fd = FlightDirection
                fd += 90. * iturn
                fd *= Cradeg
                c0 = c[0] + abs (HorizontalIncrement) * math.sin (fd)\
                   / (60. * math.cos (c[1] * Cradeg))
                c1 = c[1] + abs (HorizontalIncrement) * math.cos (fd) / 60.
                c = (c0, c1, c[2])
                if (abs (HorizontalIncrement) < 5.):	# allow for turn
                    v = Turn (FlightDirection, drift)
                    if drift:
                        DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
                        c = DriftOffset (DT, c)
#                   AC = c		# should already be true
                    ky = "Manvr" + format (ManeuverNumber, '03d')
                    m[ky] = v
                v = Transit (AC, c, drift, 0., m)
                if drift:
                    DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
                    c = DriftOffset (DT, c)
                ky = "Manvr" + format (ManeuverNumber, '03d')
                m[ky] = v
# turn the same way again
                FlightDirection += 90. * iturn
                fd = FlightDirection
                fd *= Cradeg
                c0 = c[0] + s * math.sin (fd) \
                   / (60. * math.cos (c[1] * Cradeg))
                c1 = c[1] + s * math.cos (fd) / 60.
                c = (c0, c1, c[2])
                if (t == 'Lawnmower'): iturn *= -1
            legs -= 1

# square spiral is specified by size, anchor point, angle of starting
# direction, legs (given in format 5x4 for number of steps, leading
# to 6x5 grid of drop locations), and Other for angle and direction
# of first turn. +90 gives right turn at end of first line. Set up for 
# now with 90-deg turns; could also modify to use 60 deg turns, e.g.
    elif (t == 'SquareSpiral'):      	# SquareSpiral 
        if (ManeuverNumber != 0): 	# don't turn if first maneuver
                                        # Find angle of turn needed to
                                        # head for the start point
            dx = (a[0] - AC[0]) * math.cos ((a[1] + AC[1]) * Cradeg / 2.)
            dy = a[1] - AC[1]
            oo = math.atan2 (dy, dx) / Cradeg
            oo = 90. - oo
            v = Turn (oo)
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
# transit to start point
        v = Transit (AC, a, drift, 0., m)
        if v != None:
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
        v = Turn (o)
        if drift:
            DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
            a = DriftOffset (DT, a)
        ky = "Manvr" + format (ManeuverNumber, '03d')
        m[ky] = v
# decode 'Other' entry, should be in format 10*n1+n2
        legs = int(l)
        n1 = int (legs/10)
        n2 = legs - n1 * 10
        print 'SquareSpiral: %d in the original direction and %d in the orthogonal direction.' % (n1,n2) 
        ss = s / n1
# the assumption here is that individual legs are useful to specify 
# drop locations, so many waypoints will be produced for the pattern.
# (assume Cartesion OK here)
        c = copy.deepcopy (a)
        ii = 0
        oo = o
        dr = 0				# start in n1 direction
        c0 = c[0] + ss * math.sin (oo * Cradeg) \
             / (60. * math.cos (c[1] * Cradeg))
        c1 = c[1] + ss * math.cos (oo * Cradeg) / 60.
        c = (c0,c1,c[2])
        v = Transit (AC, c, drift, 0., m)
        if drift:
            DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
            c = DriftOffset (DT, c)
        ky = "Manvr" + format (ManeuverNumber, '03d')
        m[ky] = v
#	while n1 > 0 and n2 > 0:
        while n1 >= 0 and n2 >= 0:
            ii += 1
            TurnNeeded = False
            if dr == 0 and ii == n1: 
                TurnNeeded = True
                dr = 1 - dr		# alternate directions
                ii = -1
                n2 -= 1
            elif dr == 1 and ii == n2:
                TurnNeeded = True
                dr = 1 - dr
                ii = -1
                n1 -= 1
            if n1 >= 0 and n2 >= 0:
                if TurnNeeded: 
                    oo += other
                    v = Turn (oo)
                    if drift:
                        DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
                        a = DriftOffset (DT, a)
                    ky = "Manvr" + format (ManeuverNumber, '03d')
                    m[ky] = v
                #end of TurnNeeded section
# calculate displacement to next point:
                c0 = c[0] + ss * math.sin (oo * Cradeg) \
                     / (60. * math.cos (c[1] * Cradeg))
                c1 = c[1] + ss * math.cos (oo * Cradeg) / 60.
                c = (c0,c1,c[2])
                v = Transit (AC, c, drift, 0., m)
                if drift:
                    DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
                    c = DriftOffset (DT, c)
                ky = "Manvr" + format (ManeuverNumber, '03d')
                m[ky] = v
            #end of test section that both indices are non-negative
        #end of while loop over two indices
   #end of SquareSpiral section
            
# Figure4 pattern: Fly leg of length and orientation given, then fly
# to end of track placed perpendicular to initial pass, centered on
# its center point, and having the same length as the initial pass.
# Butterfly pattern is the same exc closing back to the initial point.
    elif (t == 'Figure4' or t == 'Butterfly'):      	
        if (ManeuverNumber != 0): 	# don't turn if first maneuver
                                        # Find angle of turn needed to
                                        # head for the start point
            dx = (a[0] - AC[0]) * math.cos ((a[1] + AC[1]) * Cradeg / 2.)
            dy = a[1] - AC[1]
            oo = math.atan2 (dy, dx) / Cradeg
            oo = 90. - oo
            v = Turn (oo)
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
# transit to start point
        v = Transit (AC, a, drift, 0., m)
        if v != None:
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
        v = Turn (o)		# no drift
        ky = "Manvr" + format (ManeuverNumber, '03d')
        m[ky] = v
# fly leg of specified length:
					# a is adjusted for drift in
					# transit and affects final AC
        b = copy.deepcopy (a)
        b0 = b[0] + s * math.sin (o * Cradeg) \
           / (60. * math.cos (b[1] * Cradeg))
        b1 = b[1] + s * math.cos (o * Cradeg) / 60.
        b = (b0, b1, b[2])
        v = Transit (AC, b, drift, 0., m)
        if drift:
            DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
            b = DriftOffset (DT, b)
            a = DriftOffset (DT, a)
        ky = "Manvr" + format (ManeuverNumber, '03d')
        m[ky] = v
# now turn by 135 deg 
        oo = o - 135.
        if oo < 360.: oo += 360.
        v = Turn (oo)
        if drift:
            DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
            b = DriftOffset (DT, b)
            a = DriftOffset (DT, a)
        ky = "Manvr" + format (ManeuverNumber, '03d')
        m[ky] = v
        oor = oo * Cradeg
        b0 = AC[0] + s / 1.41421356 * math.sin (oor)\
                     / (60. * math.cos (AC[1] * Cradeg))
        b1 = AC[1] + s / 1.41421356 * math.cos (oor) / 60.
        b = (b0, b1, AC[2])
        v = Transit (AC, b, drift, 0., m)
        if drift:
            DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
            b = DriftOffset (DT, b)
            a = DriftOffset (DT, a)
        ky = "Manvr" + format (ManeuverNumber, '03d')
        m[ky] = v
# and do it again:
        oo = oo - 135.
        if oo < 360.: oo += 360.
        v = Turn (oo)
        if drift:
            DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
            b = DriftOffset (DT, b)
            a = DriftOffset (DT, a)
        ky = "Manvr" + format (ManeuverNumber, '03d')
        m[ky] = v
        oor = oo * Cradeg
        b0 = AC[0] + s * math.sin (oor)\
                     / (60. * math.cos (AC[1] * Cradeg))
        b1 = AC[1] + s * math.cos (oor) / 60.
        b = (b0, b1, AC[2])
        v = Transit (AC, b, drift, 0., m)
        if drift:
            DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
            b = DriftOffset (DT, b)
            a = DriftOffset (DT, a)
        ky = "Manvr" + format (ManeuverNumber, '03d')
        m[ky] = v
        #end of Figure4 section
        if t == 'Butterfly':
            oo += 135.
            if oo > 360.: oo -= 360.
            v = Turn (oo)
            if drift:
                DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
                b = DriftOffset (DT, b)
                a = DriftOffset (DT, a)
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
            v = Transit (AC, a, drift, 0., m)
            if drift:
                DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
                a = DriftOffset (DT, a)
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
        #end of Butterfly-only pattern. Need additional code here for
        #possibility of repeated Butterfly patterns, possible with
        #altitude changes between patterns

# Three-Blade pattern: Fly leg of length and orientation given, then 
# turn 120 deg and fly length/2, turn 120 and fly length, turn 120
# deg and fly length/2, turn 120 deg and fly length, turn 120 deg
# and fly length/2 to start point.
# to end of track placed perpendicular to initial pass, centered on
# its center point, and having the same length as the initial pass.
# Butterfly pattern is the same exc closing back to the initial point.
    elif (t == 'ThreeBlade'):      	
        if (ManeuverNumber != 0): 	# don't turn if first maneuver
                                        # Find angle of turn needed to
                                        # head for the start point
            dx = (a[0] - AC[0]) * math.cos ((a[1] + AC[1]) * Cradeg / 2.)
            dy = a[1] - AC[1]
            oo = math.atan2 (dy, dx) / Cradeg
            oo = 90. - oo
            v = Turn (oo)
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
# transit to start point
        v = Transit (AC, a, drift, 0., m)
        if v != None:
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
        v = Turn (o)
        ky = "Manvr" + format (ManeuverNumber, '03d')
        m[ky] = v
        oo = o
        b = copy.deepcopy (a)
# repeat 3 times, long leg, 120 deg turn, short leg, 120 deg turn
        for i in range(0,3):
					# fly leg of specified length:
					# a is adjusted for drift in
					# transit and affects final AC
            b0 = b[0] + s * math.sin (oo * Cradeg) \
               / (60. * math.cos (b[1] * Cradeg))
            b1 = b[1] + s * math.cos (oo * Cradeg) / 60.
            b = (b0, b1, b[2])
            v = Transit (AC, b, drift, 0., m)
            if drift:
                DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
                b = DriftOffset (DT, b)
                a = DriftOffset (DT, a)
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
					# now turn by 120 deg 
            oo += 120.
            if oo > 360.: oo -= 360.
            v = Turn (oo)
            if drift:
                DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
                b = DriftOffset (DT, b)
                a = DriftOffset (DT, a)
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
            oor = oo * Cradeg
            b0 = AC[0] + s / 2. * math.sin (oor)\
                         / (60. * math.cos (AC[1] * Cradeg))
            b1 = AC[1] + s / 2. * math.cos (oor) / 60.
            b = (b0, b1, AC[2])
            v = Transit (AC, b, drift, 0., m)
            if drift:
                DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
                b = DriftOffset (DT, b)
                a = DriftOffset (DT, a)
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
            oo += 120.
            if oo > 360.: oo -= 360.
        #end of 3-repeat-loop for 'blades'
    #End of ThreeBlade section


# for DelayInArea, transit to specified coords, use distance for size 
# of area and add to flight time according to special argument 'other'
    elif (t == 'DelayInArea'):      	
        if (ManeuverNumber != 0): 	# don't turn if first maneuver
                                        # Find angle of turn needed to
                                        # head for the start point
            dx = (a[0] - AC[0]) * math.cos ((a[1] + AC[1]) * Cradeg / 2.)
            dy = a[1] - AC[1]
            oo = math.atan2 (dy, dx) / Cradeg
            oo = 90. - oo
            v = Turn (oo)
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
# transit to start point
        v = Transit (AC, a, drift, 0., m)
        if v != None:
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
        v = Delay (other)		# Time and Fuel are incremented
					# in Delay ()
        ky = "Manvr" + format (ManeuverNumber, '03d')
        m[ky] = v
        m['EndPoint'] = AC
            

    elif (t == 'ReverseModule'): pass 	# ReverseModule
 
# 'Enroute" is a simple 'Transit' maneuver, possibly with a turn
    elif (t == 'Enroute'):      	
        if (ManeuverNumber != 0): 	# don't turn if first maneuver
                                        # Find angle of turn needed to
                                        # head for the start point
            dx = (a[0] - AC[0]) * math.cos ((a[1] + AC[1]) * Cradeg / 2.)
            dy = a[1] - AC[1]
            oo = math.atan2 (dy, dx) / Cradeg
            oo = 90. - oo
            if (oo < 0.): oo += 360.
            v = Turn (oo)
#					# adjust a for drift during turn
            if drift:
                DT = v['Dimension'] / Specs.FlightSpeed (AC[2])
                a = DriftOffset (DT, a)
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v
# transit to specified point
        acc = AC
					# a is adjusted for drift in
					# transit and affects final AC
        v = Transit (acc, a, drift, other, m)
        if v != None:
            ky = "Manvr" + format (ManeuverNumber, '03d')
            m[ky] = v

    elif (t == 'ReturnToBase'):      	# ReturnToBase
					# allowance for ATC
        Time += 0.2
        Fuel -= 500.
        acc = AC
        a = Specs.TakeoffLocation ()
        dx = (a[0] - AC[0]) * math.cos ((a[1] + AC[1]) * Cradeg / 2.)
        dy = a[1] - AC[1]
        oo = math.atan2 (dy, dx) / Cradeg
        oo = 90. - oo
        if (oo < 0.): oo += 360.
        v = Turn (oo)
        ky = "Manvr" + format (ManeuverNumber, '03d')
        m[ky] = v
        v = Transit (acc, a, False, 0., m)
        ky = "Manvr" + format (ManeuverNumber, '03d')
        m[ky] = v

    else: print "ERROR, undefined module type = ",t
    return m

def GreatCircle (a, b):	
    "Returns (distance, angle) in n mi and deg for transit a -> b."
    lat1 = a[1] * Cradeg
    lon1 = a[0] * Cradeg
    lat2 = b[1] * Cradeg
    lon2 = b[0] * Cradeg
    t1 = PI/2. - lat1
    t2 = PI/2. - lat2
    cosa = math.cos (t1) * math.cos (t2) \
         + math.sin (t1) * math.sin (t2) * math.cos (lon2 - lon1)
    if (cosa >= 1.): return (0., 0.)
    alpha = math.acos (cosa)
#   find point on meridional circle equidistant from point 2
    dtst = t1 + alpha
    cosc = math.cos (dtst) * math.cos (t2) \
         + math.sin (dtst) * math.sin (t2) * math.cos (lon2 - lon1)
    cosp = (cosc - cosa**2) / (1. - cosa**2)
    if (cosp < 0.): cosp *= -1.		# check: Why is this here?
    if (cosp < 1. and cosp > -1.): B = math.acos (cosp)
    else: B = 0.;
    if (B < 0.): B *= -1.
    if(lat1 >= lat2 and lon1 >= lon2): B += PI
    elif (lat1 < lat2 and lon1 >= lon2): B = 2.*PI - B
    elif (lat1 >= lat2 and lon1 < lon2): B = PI - B
    B /= Cradeg
    dist = RE  * alpha	
    return (dist, B)

def Transit (a, b, drift = False, limit = 0., modl = None):	
    "Maneuver: from location a to location b, possibly changing altitude."
# 	a,b are (long,lat,z) tuples, limit is max climb/descent rate
#       if drift, target point b drifts with the wind during transit
    global Fuel, Time, AC, ManeuverNumber, Track, FlightDirection
    (dist, B) = GreatCircle (a,b)
    if dist <= 0.: dist = 0.001
    EndPoint = b	# (this gets reset for descent)
    if a[2] < b[2]:	
			# Put a 'climb' into Maneuvers. This is not
			# called directly as a mencuber like Transit
			# or Turn; it is put into the
			# maneuvers as an indication of where it
			# occurs in the 'Waypoints' listing
        m = Maneuver("Climb", dist, a, B, None, 0., drift)
			# get the time, fuel, and distance required
			# for the specified climb:
        if limit != 0:
            Tclimb, Fclimb, Dclimb, dx, dy \
                = Specs.ClimbValues (a[2], b[2], limit)
        else:
            Tclimb, Fclimb, Dclimb, dx, dy \
                = Specs.ClimbValues (a[2], b[2])
			# Dclimb is the air distance for the climb;
			# need to adjust for wind
			# for drifting case, target drifts but still
			# need to correct for difference between
			# target drift and aircraft drift. Will ignote
			# this for now, improve later? For ground-
			# relative case, make adjustment to distance
			# covered:
        if drift: 
            b = DriftOffset (Tclimb, b)
            a = DriftOffset (Tclimb, a)
        else:		
            wdir = (90. - math.atan2 (dy, dx) / Cradeg)
            beta = (B - wdir) 	# includes correction to-to-from
            while beta < 0.: beta += 360.
            beta *= Cradeg
            dw = (dx*dx + dy*dy)**0.5
            sina = dw * math.sin (beta) / Dclimb
            alpha = math.asin (sina)
            Dclimb = (Dclimb**2 + dw**2 \
                     -2. * Dclimb * dw * math.cos (alpha + beta))**0.5
#       print "return from ClimbValues = ", Tclimb, Fclimb, Dclimb
        		# check if there is room to climb in transit
        if dist < Dclimb and modl['Type'] != 'SpiralProfile'\
           and modl['Type'] != 'Sawtooth':	
            print " Not enough length in leg for the requested climb."
            dlg = wx.MessageDialog (frame,\
                'The requested climb covers more distance than\n'\
              + 'in the transit. Pick a point farther from the\n'\
              + 'start of this module.', \
                'Not enough distance for requested climb', wx.OK)
#           result = dlg.ShowModal ()
#           dlg.Destroy ()
            if frame != None:
                frame.SetStatusText \
                     ('insufficient distance for climb', 2)
        Time += Tclimb
        Fuel -= Fclimb
        m['Dimension'] = Dclimb
				# update AC location
				# drift: a,b have already drifted
        AC = (a[0] + (b[0] - a[0]) * Dclimb / dist,
              a[1] + (b[1] - a[1]) * Dclimb / dist, b[2])
        dist -= Dclimb
        FlightDirection = B
        m['Time'] = Time
        m['Fuel'] = Fuel
        m['EndPoint'] = AC
        ManeuverNumber += 1
        ky = "Manvr" + format (ManeuverNumber, '03d')
        modl[ky] = m
    #end of section that handles climb at start of transit
    # Descent is different, usually is better at end of transit.
    # Harder, because don't know characteristics like GW for
    # descent because you don't know point to start until you
    # calculate descent. So, as compromise, calculate the
    # descent twice, once at start of track, then again at
    # the end of the track, allowing a few minutes at the end
    # for uncertainty that arises from a lower GW than in the
    # preliminary calculation. Don't use the preliminary calculation,
    # just predict where to do descent on the basis of the prelim.
    # calculation.
    				# Here is the preliminary calculation:
    if a[2] > b[2]:		# Section for a descending track:
        if limit != 0:
            Tdescent, Fdescent, Ddescent, dx, dy \
                = Specs.DescentValues (a[2], b[2], limit)
        else:
            Tdescent, Fdescent, Ddescent, dx, dy \
                = Specs.DescentValues (a[2], b[2])
#       print "return from DescentValues = ", Tdescent, Fdescent, Ddescent
        EndPoint = (a[0] + (b[0] - a[0]) * dist / (dist + Ddescent),
              a[1] + (b[1] - a[1]) * dist / (dist + Ddescent), a[2])
        if drift: pass		# preliminary; don't drift yet
        else:		
            wdir = (90. - math.atan2 (dy, dx) / Cradeg)
            beta = (B - wdir) 	# includes correction to-to-from
            while beta < 0.: beta += 360.
            beta *= Cradeg
            dw = (dx*dx + dy*dy)**0.5
            sina = dw * math.sin (beta) / Ddescent
            alpha = math.asin (sina)
            Ddescent = (Ddescent**2 + dw**2 \
                     -2. * Ddescent * dw * math.cos (alpha + beta))**0.5
        		# check if there is room to descend in transit
        dist -= Ddescent
				# expect 5 n mi extra in leg
        if dist < 5. and modl['Type'] != 'SpiralProfile'\
           and modl['Type'] != 'Sawtooth':
            print " Not enough length in leg for the requested descent."
            print ' (Module #',ModuleNumber,') dist/Ddescent = ', dist, Ddescent
            dlg = wx.MessageDialog (frame,\
                'The requested descent covers more distance than\n'\
              + 'in the transit. Pick a point farther from the\n'\
              + 'start of module '+format(ModuleNumber, 'd'), \
                'Not enough distance for requested descent', wx.OK)
#           result = dlg.ShowModal ()
#           dlg.Destroy ()
            if frame != None:
                frame.SetStatusText \
                     ('insufficient distance for descent', 2)
        #end of the preliminary-descent calculation (no Maneuver added)
        # Handle descent later; for now, distance has been reduced to
        # allow space for it. Process is to now calculate Transit but
        # not save it, do another descent and hold it without saving,
        # then finally do the transit and descent.

    m = Maneuver("Transit", dist, AC, B, None, 0., drift)
    if drift:
#	drifting transit time is just that required without wind. 
#	However, must calculate new position after drift and use 
#       that as target, allowing for 'extra' diversions
        DT = dist / Specs.FlightSpeed (AC[2]) * (1. + Specs.Extra ())
        a = DriftOffset (DT, a)
        b = DriftOffset (DT, b)
        EndPoint = DriftOffset (DT, EndPoint)
    else:
# 	find the transit time with wind
#	(eventually should use spherical geometry instead of rect.)
#       For equations, see 'TransitTimeWithWind' note.
        wdir = Specs.Wind (AC[2], Time)[0]
        beta = (B - wdir - 180.) 
        while beta < 0.: beta += 360.
        beta *= Cradeg
        sina = Specs.Wind (AC[2], Time)[1] * math.sin (beta)\
             / Specs.FlightSpeed (AC[2])
        alpha = math.asin (sina)
        gamma = alpha + beta
        actual = dist * math.sin (beta) / math.sin (PI - gamma) 
        DT = actual / Specs.FlightSpeed (AC[2]) * (1. + Specs.Extra ())
    # end of no-drift section that accounts for wind but flies 
    # ground track
    Fuel -= Specs.FuelBurnRate (AC[2]) * DT
    Time += DT
    m['Time'] = Time
    m['Fuel'] = Fuel
    m['EndPoint'] = EndPoint
    FlightDirection = B
#		FlightDirection is heading for drifting case, ground-track
#		direction for no-drift case.
    m['FlightDirection'] = FlightDirection
    if a[2] > b[2]:		# Section for a descending track:
				# Now add the descent
			# Recalculate the time, fuel, and distance 
			# required for the specified descent:
        Dprime = Ddescent
        Tprime = Tdescent
        Fprime = Fdescent
        if limit != 0.:
            Tdescent, Fdescent, Ddescent, dx, dy \
                = Specs.DescentValues (a[2], b[2], limit)
        else:
            Tdescent, Fdescent, Ddescent, dx, dy\
                = Specs.DescentValues (a[2], b[2])
			# again, small error here because drift during
			# descent does not match drift of target
			# point; need revision someday
        if drift: 
            b = DriftOffset (Tdescent, b)
            a = DriftOffset (Tdescent, a)
        else:		
            wdir = (90. - math.atan2 (dy, dx) / Cradeg)
            beta = (B - wdir) 	# includes correction to-to-from
            while beta < 0.: beta += 360.
            beta *= Cradeg
            dw = (dx*dx + dy*dy)**0.5
            sina = dw * math.sin (beta) / Ddescent
            alpha = math.asin (sina)
            Ddescent = (Ddescent**2 + dw**2 \
                     -2. * Ddescent * dw * math.cos (alpha + beta))**0.5
        		# check if there is room to descend in transit
#       print "return from DescentValues = ", Tdescent, Fdescent, Ddescent
			# now, before saving, adjust transit and save it
        Time -= DT	# adjust time, fuel, position 
        Fuel += Specs.FuelBurnRate (AC[2]) * DT
        DT *= (dist + Dprime - Ddescent) / dist
        Time += DT
        Fuel -= Specs.FuelBurnRate (AC[2]) * DT
        dist += (Dprime - Ddescent)
			# could revise the drift distance also;
			# probably unnecessary for now
        AC = (a[0] + (b[0] - a[0]) * dist / (dist + Ddescent),
              a[1] + (b[1] - a[1]) * dist / (dist + Ddescent), AC[2])
        m['Time'] = Time
        m['Fuel'] = Fuel
        m['EndPoint'] = AC
        m['Dimension'] = dist
			# finally add maneuver for descent 
        mm = Maneuver("Descent", Ddescent, AC, B, None, 0., drift)
        Time += Tdescent
        Fuel -= Fdescent
        if drift: 
            b = DriftOffset (Tdescent, b)
        FlightDirection = B
        mm['Time'] = Time
        mm['Fuel'] = Fuel
        mm['EndPoint'] = b
        mm['FlightDirection'] = FlightDirection
			# to get these in the right order, need to
			# save module m first, so save it and then
			# return mm which will be saved after return
        ManeuverNumber += 1
        ky = "Manvr" + format (ManeuverNumber, '03d')
        modl[ky] = m
        m = mm
    AC = b
    ManeuverNumber += 1
    return m

def Delay (DelayTime):	
    "To implement unspecified maneuvers, delay and increment fuel for cruise."
    global Fuel, Time, AC, ManeuverNumber, Track, FlightDirection
    Time += DelayTime
    Fuel -= Specs.FuelBurnRate (AC[2]) * DelayTime
    ss = Specs.FlightSpeed (AC[2]) * DelayTime
    m = Maneuver ("Delay", ss, AC, FlightDirection)
    m['Time'] = Time
    m['Fuel'] = Fuel
    m['EndPoint'] = AC
    m['FlightDirection'] = FlightDirection
    ManeuverNumber += 1
    return m

def Turn (direction, drift = False):	
    "Turn to new direction and increment fuel and time for turn."
    global Fuel, Time, AC, ManeuverNumber, Track, FlightDirection
# NEW Structure:
#    Set up track to go directly toward next point, but compensate time
#    required and fuel use for time to make turn. Do turns as follows:
#    1. If turn is <= 90 deg., do simple inside turn, saving some in t/f.
#       This rounds the corner so doesn't quite get to the target; this
#       may be a worry for some dropsonde patterns.
#    2. For turn 90 < angle <145, do looping-outside turn. This is done
#       by flying R*tan(Angle/2) past the target point, turning in the 
#       outside direction to end up on course R*tan(Angle/2) before the 
#       target.  This takes some time vs simple inside turn, but the 
#       simple turn becomes too displaced from the target as the angle 
#       increases.
#    3. For >145 deg turn, do teardrop pattern.  This includes the case 
#       of 180 deg turn or course reversal.
#    Distance increments: R=FlightSpeed/(Cradeg*3600*trate) is turn 
#    radius, or time increment is distance / FlightSpeed:
#    1. A simple turn saves 2R(Angle/2-tan(Angle/2)) in distance.
#    2. A looping-outside turn takes and additional 
#       2R*(tan(Angle/2)-Angle/2+PI) in distance vs the point-to-point 
#       path.
#    3. The teardrop turn adds R(4*beta+Angle) to the path, where
#       beta arises from an implicit equation but has values that vary
#       from about 80 to 60 as the turn angle increases from 145 to 180 deg.
#       The resulting distance varies from 7.33R for 180 deg to about 8R
#       for 145 deg.
#
#    For the drifting case, must use time to translate AC location
#    
		# find the turn radius:
    trate = Specs.MaxTurnRate (AC[2])
		# determine the angle change needed:
    AngleOfTurn = abs (direction - FlightDirection)
    if (AngleOfTurn > 180.): 
        AngleOfTurn -= 360.
        AngleOfTurn *= -1.
		#   ttime = AngleOfTurn / trate
    TurnFactor = 1. / (10. * trate * 2.*PI)
		#   TurnRadius = FlightSpeed (altitude) * TurnFactor
    if (AngleOfTurn <= 90.):		# simple turn (note: DT < 0)
        AngleOfTurn *= Cradeg
        DT = 2. * TurnFactor * (AngleOfTurn \
           / 2. - math.tan (AngleOfTurn / 2.))
    elif (AngleOfTurn < 145.): 		# do a looping outside turn
        AngleOfTurn *= Cradeg
        DT = 2. * TurnFactor \
             * (math.tan (AngleOfTurn / 2.) - AngleOfTurn / 2. + PI)
    else:				# do a teardrop turn
        if (AngleOfTurn < 150.): DT = 8. * TurnFactor
        elif (AngleOfTurn < 170.): DT = 7.5 * TurnFactor
        else: DT = 7.3333 * TurnFactor	# this is the value for 180 deg
    Time += DT
    Fuel -= Specs.FuelBurnRate (AC[2]) * DT
    FlightDirection = direction
    ss = Specs.FlightSpeed (AC[2]) * DT
    m = Maneuver ("Turn", ss, AC, direction, None, 0., drift)
    m['Time'] = Time
    m['Fuel'] = Fuel
    if drift:		# get aircraft drift during turn
        AC = DriftOffset (DT, AC)
    m['EndPoint'] = AC
#		FlightDirection is heading for drifting case, ground-track
#		direction for no-drift case.
    m['FlightDirection'] = FlightDirection
    ManeuverNumber += 1
    return m

def DriftOffset (DT, a):
    "Returns the adjusted location of a point after drift by time DT."
    W = Specs.Wind (AC[2], Time)
#   print 'DriftOffset: DT=',DT,', a=',a,', Wind=',W
    Wd = (W[0] + 180.)		# convert 'from' to 'to' direction
    Wda = (90. - Wd) * Cradeg	# get angle in x,y coordinate system
#   print 'Wda=',Wda
    Dx = W[1] * math.cos (Wda) * DT	# eastward offset
    Dy = W[1] * math.sin (Wda) * DT	# northward offset
    Ay = Dy / (RE * Cradeg)
    Ax = Dx * math.cos (AC[1] * Cradeg) / (RE * Cradeg)
#   print 'Dx,Dy,Ax,Ay=', Dx, Dy, Ax, Ay
    a0 = a[0] + Ax
    a1 = a[1] + Ay
    b = (a0, a1, a[2])
    return(b)

# the following is a code remnant, can be deleted.
# This is now incorporated into 'Turn'
def TearDrop (direction):	
    "Fly 60/300/60 so as to end up at b flying in opposite direction."
    global Fuel
    global Time
    global AC
    global ManeuverNumber
    global Track
    global FlightDirection
# Don't try to plot course reversal path; just add time and reverse course
    altitude = AC[2]
    trate = Specs.MaxTurnRate(altitude)
# teardrop turn is 60/300/60 or 420 deg turning
    ttime = 420. / trate
    DT = ttime / 3600.
    Fuel -= Specs.FuelBurnRate(altitude) * DT
    Time += DT
    FlightDirection = direction + 180.
    if (FlightDirection > 360.): FlightDirection -= 360.
    ss = Specs.FlightSpeed (altitude) * DT
    m = Maneuver("TearDrop",ss,AC,direction,None,0.)
    m['Time'] = Time
    m['Fuel'] = Fuel
    m['EndPoint'] = AC
    ManeuverNumber += 1
    return m

