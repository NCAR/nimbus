DECLARE SUB ALTITUDE ()
DECLARE SUB TRUEAIRSPEED ()
DECLARE SUB PITCH ()
DECLARE SUB ROLL ()
DECLARE SUB TOTALAIRTEMP ()
DECLARE SUB TRUEHEAD ()
DECLARE SUB GNDSPEED ()
DECLARE SUB LATITUDE ()
DECLARE SUB LONGITUDE ()
DECLARE SUB NSVELOCITY ()
DECLARE SUB EWVELOCITY ()
DECLARE SUB juliandate ()
DECLARE SUB timoday ()
DECLARE FUNCTION valb! (bi$)
DECLARE FUNCTION bin$ (n!)

DIM SHARED hx$, hxtod$
DIM SHARED nt, ont
DIM SHARED Jdate, Jdate$, ch1to4$
DIM SHARED t$, ampm$, ch5to8$     '1=pm, time of day
DIM SHARED lat, lat$, CH9TO12$
DIM SHARED lon, lon$, CH13TO16$
DIM SHARED nsvel, nsvel$, CH17TO20$
DIM SHARED ewvel, ewvel$, CH21TO24$
DIM SHARED thead, thead$, CH25TO28$
DIM SHARED gspeed, gspeed$, CH29TO32$
DIM SHARED tat, tat$, CH33TO36$
DIM SHARED palt, palt$, CH37TO40$
DIM SHARED tas, tas$, CH41TO44$
DIM SHARED pit, pit$, CH45TO48$
DIM SHARED rol, rol$, CH49TO52$
hx$ = "0123456789ABCDEF"
'SHELL "time 03:42:22"     'time/date from examples
'SHELL "date 11-13-1993"
OPEN "NAVPARAM.DAT" FOR OUTPUT AS #1
ont = INT(TIMER)
ON TIMER(1) GOSUB l0

CLS
TIMER ON

l1: GOTO l1
l0:
juliandate
timoday
LATITUDE
LONGITUDE
NSVELOCITY
EWVELOCITY
TRUEHEAD
GNDSPEED
TOTALAIRTEMP
ALTITUDE
TRUEAIRSPEED
PITCH
ROLL

CH1TO52$ = ch1to4$ + ch5to8$ + CH9TO12$ + CH13TO16$ + CH17TO20$ + CH21TO24$
CH1TO52$ = CH1TO52$ + CH25TO28$ + CH29TO32$ + CH33TO36$ + CH37TO40$
CH1TO52$ = CH1TO52$ + CH41TO44$ + CH45TO48$ + CH49TO52$

LOCATE 24, 1
PRINT CH1TO52$;
PRINT #1, CH1TO52$
IF INKEY$ <> "" THEN GOTO XIT
RETURN

XIT:
CLOSE

SUB ALTITUDE
palt = 59999
palt$ = "CFA5"
CH37TO40$ = "CFA5"
LOCATE 9, 6: PRINT "Paltitude = "; palt
END SUB

FUNCTION bin$ (n)
DIM b$(15)
b$(&H0) = "0000": b$(&H8) = "1000"
b$(&H1) = "0001": b$(&H9) = "1001"
b$(&H2) = "0010": b$(&HA) = "1010"
b$(&H3) = "0011": b$(&HB) = "1011"
b$(&H4) = "0100": b$(&HC) = "1100"
b$(&H5) = "0101": b$(&HD) = "1101"
b$(&H6) = "0110": b$(&HE) = "1110"
b$(&H7) = "0111": b$(&HF) = "1111"

n$ = HEX$(n): bi$ = ""
FOR i = 1 TO LEN(n$)
bi$ = bi$ + b$(VAL("&h" + MID$(n$, i, 1)))
NEXT i
bin$ = bi$
END FUNCTION

SUB EWVELOCITY
ewvel = -63.7
ewvel$ = "FD83"
CH21TO24$ = "FD83"
LOCATE 5, 20: PRINT "E-W Velo. = "; ewvel
END SUB

SUB GNDSPEED
gspeed = 63.7
gspeed$ = "7D83"
CH29TO32$ = "7D83"
LOCATE 6, 20: PRINT "Gnd. Spd. = "; gspeed
END SUB

SUB juliandate
DIM ndays(12)
ndays(1) = 31: ndays(5) = 31: ndays(9) = 30
ndays(2) = 28: ndays(6) = 30: ndays(10) = 31
ndays(3) = 31: ndays(7) = 31: ndays(11) = 30
ndays(4) = 30: ndays(8) = 31: ndays(12) = 31

D$ = DATE$

mo = VAL(LEFT$(D$, 2)): da = VAL(MID$(D$, 4, 2)): ye = VAL(RIGHT$(D$, 4))
IF ye MOD 4 = 0 THEN ndays(2) = 29'leap year!

IF mo = 1 THEN Jdate = da: GOTO jdskip

Jdate = 0
FOR i = 1 TO mo - 1
Jdate = Jdate + ndays(i)
NEXT i
Jdate = Jdate + da
jdskip:
jd$ = STR$(Jdate): jd$ = RIGHT$(jd$, LEN(jd$) - 1)
jdb$ = ""
FOR i = 1 TO LEN(jd$)
jdb$ = jdb$ + bin$(VAL(MID$(jd$, i, 1)))
NEXT i
jdb$ = RIGHT$(jdb$, 10)': PRINT jdb$
jdb$ = "11" + ampm$ + "1" + "00" + jdb$   'from example page2

ch1to4$ = ""
'PRINT jdb$
FOR i = 1 TO 16 STEP 4
'PRINT ch1to4$
ch1to4$ = ch1to4$ + MID$(hx$, 1 + valb(MID$(jdb$, i, 4)), 1)
NEXT i
'PRINT ch1to4$
LOCATE 1, 1: PRINT "NAV S/N="; 3
LOCATE 1, 15: PRINT "DATE = "; D$;
LOCATE 1, 35: PRINT "Julian Date = "; Jdate
LOCATE 1, 55: IF ampm$ = "1" THEN PRINT "PM" ELSE PRINT "AM"
LOCATE 1, 60: PRINT "NIST Time = No"
END SUB

SUB LATITUDE
lat = 39.69
lat$ = "1C39"
CH9TO12$ = "1C39"

LOCATE 4, 1: PRINT "Latitude ="; lat
END SUB

SUB LONGITUDE
lon = -123.3
lon$ = "A850"
CH13TO16$ = "A850"
LOCATE 4, 20: PRINT "longitude = "; lon
END SUB

SUB NSVELOCITY
nsvel = 147.6
nsvel$ = "05C4"
CH17TO20$ = "05C4"
LOCATE 5, 1: PRINT "N-S Vel. = "; nsvel

END SUB

SUB PITCH
pit = -19.6
pit$ = "F219"
CH45TO48$ = "F219"
LOCATE 12, 1: PRINT "Pitch = "; pit

END SUB

SUB ROLL
rol = -10
rol$ = "F8E4"
CH49TO52$ = "F8E4"
LOCATE 13, 2: PRINT "Roll = "; rol
END SUB

SUB timoday   ' defines chars 5-8 (page 3) as pg3$

t$ = TIME$
hour = VAL(LEFT$(t$, 2)): min = VAL(MID$(t$, 4, 2)): sec = VAL(RIGHT$(t$, 2))
IF hour >= 12 THEN ampm$ = "1": hour = hour - 12 ELSE ampm$ = "0"

minsfrom12 = hour * 60 + min: remainsecs = ((minsfrom12 MOD 4) * 60) + sec
fourminper = INT(minsfrom12 / 4)
ch5to6$ = RIGHT$("0000" + bin$(fourminper), 8)
ch5 = 1 + valb(LEFT$(ch5to6$, 4))
ch6 = 1 + valb(RIGHT$(ch5to6$, 4))
ch7to8$ = RIGHT$("0000" + bin$(remainsecs), 8)
ch7 = 1 + valb(LEFT$(ch7to8$, 4))
ch8 = 1 + valb(RIGHT$(ch7to8$, 4))
ch5to8$ = MID$(hx$, ch5, 1) + MID$(hx$, ch6, 1) + MID$(hx$, ch7, 1) + MID$(hx$, ch8, 1)

'PRINT t$, ampm, ch5to6$, ch7to8$, ch5to8$

LOCATE 2, 15: PRINT "Time = "; t$
END SUB

SUB TOTALAIRTEMP
tat = -35.8
tat$ = "0A57"
CH33TO36$ = "0A57"

LOCATE 8, 1: PRINT "Total Air Temp ="; tat
END SUB

SUB TRUEAIRSPEED
tas = 124
tas$ = "1FBE"
CH41TO44$ = "1FBE"
LOCATE 10, 1: PRINT "True Air Speed = "; tas
END SUB

SUB TRUEHEAD
thead = 337.1
thead$ = "EFB3"
CH25TO28$ = "EFB3"
LOCATE 6, 1: PRINT "True Hdg.= "; thead
END SUB

FUNCTION valb (bi$)
v = 0
FOR i = LEN(bi$) TO 1 STEP -1
IF MID$(bi$, i, 1) = "1" THEN v = v + 2 ^ (LEN(bi$) - i)
NEXT i
valb = v
END FUNCTION

