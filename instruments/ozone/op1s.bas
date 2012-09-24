DECLARE SUB VALVESLIP ()
DECLARE SUB VALVEPOSITION ()
DECLARE SUB VALVECHG ()
DECLARE SUB FINETIMER ()
DECLARE FUNCTION B2I& (BN$)    'CONVERTS FORMATTED BINARY BYTES TO A LONG INTEGER
DECLARE FUNCTION I2B$ (DEC&)   'CONVERTS LONG INTEGER TO 4 FORMATTED BINARY BYTES
DECLARE SUB INITSCREEN ()
DECLARE SUB CNTRESET ()
DECLARE SUB DATAPOINTERS ()
DECLARE SUB INITCOUNTERS ()
DECLARE SUB IOMAP ()
DECLARE SUB PARAMETERS ()
DECLARE SUB SETMASTER ()
DECLARE SUB SETMODE (CHIP%, CNTR%, MREG$, LREG$)
DECLARE SUB WRITETOSCREEN ()
DECLARE SUB WRITETODISK ()
DECLARE SUB VOLTSUB (R1!, R2!, ADVOLTS#)
DECLARE SUB TEMPSUB (R1!, R2!, ADVOLTS#)
DECLARE SUB PRESSUB (MBARCAL!, ADVOLTS#)
DECLARE SUB CURRSUB (RES!, ADVOLTS#)
DECLARE SUB SINGLEAD ()
DECLARE SUB DIFFERAD ()
DECLARE SUB WRITETOSERIAL ()
DECLARE SUB IRIGTIME ()
DECLARE SUB OPENDATAFILE ()
DECLARE SUB SETSYSTEMDATE ()
DECLARE SUB SETSYSTEMTIME ()

'Screen update routines:
DECLARE SUB PrintNumArr (num#(), fmt$, ind, row, col, clr)
DECLARE SUB PrintLabel (s$, row, col, clr)
DECLARE SUB PrintNumLabel (num#, fmt$, row, col, clr)

COMMON SHARED CNTRBASEADDR%, DIFFBASEADDR%, SINGBASEADDR%
COMMON SHARED LINT&, BN$, DEC&
COMMON SHARED VALVEQUADRANT%, EXPECTQUADRANT%, PREVALVEQUADRANT%, VALVECODE%
COMMON SHARED VALVESKIPPED%, VALVESTOPPED%, VALVEPOS%
COMMON SHARED OSCFREQ&, CNTINTERVAL&, DWNCNTS&
COMMON SHARED CNTR%, MODE%, MREG&, CHIP%, LOAD%, LREG&
COMMON SHARED DATA1%, DATA2%, CONT1%, CONT2%, DIG1%, DIG2%
COMMON SHARED INTERRUPT1%, INTERRUPT2%, STATUS%, MASTERMODE%
COMMON SHARED MODE1%, LOAD1%, HOLD1%, HOLDCY1%
COMMON SHARED MODE2%, LOAD2%, HOLD2%, HOLDCY2%
COMMON SHARED MODE3%, LOAD3%, HOLD3%, HOLDCY3%
COMMON SHARED MODE4%, LOAD4%, HOLD4%, HOLDCY4%
COMMON SHARED MODE5%, LOAD5%, HOLD5%, HOLDCY5%
COMMON SHARED ROW%, COLUMN%, VALUE!, ADTEST&
COMMON SHARED TEMP#, RT#, ADVOLTS#, PRES#, ENGUNITS#, VOLTS#, MILLIAMP#
COMMON SHARED R1!, R2!, MBARCAL!
COMMON SHARED FREQA&, FREQB&, CNTSREF&, DELTAFREQA&, DELTAFREQB&, FREQRATIO#, PREVRATIO#
COMMON SHARED MAXPCT%, PCTTIME%, PCTEXPIRED%
COMMON SHARED MICSECS#, STARTTIME#, ENDTIME#, FIRSTIME#, MILSEC#, CYCLENO#, NOW#

COMMON SHARED HH, MM, SS, MS

COMMON SHARED DATAOUT$
COMMON SHARED ENGOUT$
COMMON SHARED COMBDATA$

DIM SHARED TEMPERATURE#(1 TO 16)   'EXTRAS INCLUDED
DIM SHARED PRESSURE#(1 TO 6)       'EXTRAS INCLUDED
DIM SHARED VOLTAGE#(1 TO 12)       'EXTRAS INCLUDED
DIM SHARED CURRENT#(1 TO 6)        'EXTRAS INCLUDED
DIM SHARED MISSINGTIME#(1 TO 100)  'FOR MISSED DATA TIME
DIM SHARED MISSINGCYCLE#(1 TO 100) 'FOR MISSED CYCLE NUMBER

'Colors
CONST Black = 0, BLUE = 1, Green = 2, cyan = 3
CONST RED = 4, Magenta = 5, brown = 6, white = 7
CONST grey = 8, LBlue = 9, Lgreen = 10, Lcyan = 11
CONST Lmagenta = 13, Yellow = 14, Bwhite = 15
CONST PlotBorderColor = Bwhite

'******************************************************************

CALL PARAMETERS     'CONTAINS THE EXPERIMENT PARAMETERS
CALL IOMAP          'DEFINE I/O MAP
CALL DATAPOINTERS   'DEFINE DATA POINTERS FOR COUNTER BOARD
CALL CNTRESET       'RESET COUNTER BOARD
CALL SETMASTER      'SET MASTER MODE FOR BOTH COUNTER CHIPS
CALL INITCOUNTERS   'INITIALIZE COUNTER MODE REGISTERS
CALL OPENDATAFILE   'OPEN DATA AND ENGDATA FILES FOR WRITING

CALL IRIGTIME
'CALL SETSYSTEMDATE  'SET THE PC DATE AND TIME.  EVENTUALLY USE IRIG TIME
'CALL SETSYSTEMTIME

CYCLENO# = 0
PREVALVEQUADRANT% = 1
VALVESTOPPED% = 0
VALVESKIPPED% = 0
CALL VALVEPOSITION

LA$ = "0101 1111"
OUT CONT1%, B2I&(LA$)                'LOAD COUNTERS(5F) 1,2,3,4,5 CHIP 1 WITH DATA IN LOAD REGISTERS
LA$ = "0101 1111"
OUT CONT2%, B2I&(LA$)                'LOAD COUNTERS(3F) 1,2,3,4,5 CHIP 2 WITH DATA IN LOAD REGISTERS
LA$ = "0011 1111"
OUT CONT1%, B2I&(LA$)                 'ARM COUNTERS 1,2,3,4,5 CHIP 1
LA$ = "0011 1111"
OUT CONT2%, B2I&(LA$)                 'ARM COUNTERS 1,2,3,4,5 CHIP 2

   STARTTIME# = TIMER
 
     CLOSED% = 1
     CYCLENO# = -1
     MAXPCT% = 0
     PCTEXPIRED% = 0

OPEN "COM1:9600,N,8,1,BIN,CS0,DS0" FOR RANDOM AS #2
VALVEPOS% = 1    'start valve flag in B position because it gets set to A on first cycle

CALL INITSCREEN

'........................TIME LOOP STARTS HERE!

DO
     CYCLENO# = CYCLENO# + 1

     STATUS% = 2                            'SET STATUS OF CNTR 1 CHIP 1
     DO UNTIL STATUS% = 0
		STATUS% = INP(CONT1%) AND 2		'CHECK STATUS OF CNTR 1 CHIP 1
     LOOP
				'START 1 SEC HERE: CNTR=4000

    OUT CONT1%, 166                        '166:SAVE CNTR 2,3 CHIP 1
    OUT CONT2%, 166                        '166:SAVE CNTR 2,3 CHIP 2
    OUT CONT1%, 70                         '70:LOAD CNTR 2,3 CHIP 1
    OUT CONT2%, 70                         '70:LOAD CNTR 2,3 CHIP 2


	IF CYCLENO# MOD 10 = 0 THEN
		CALL VALVECHG
		CALL VALVESLIP
		 VALVEPOS% = ABS(1 - VALVEPOS%)  'flip the state of the Valve flag
	END IF

    ENDTIME# = TIMER
    NOW# = ENDTIME#
    OUT CONT1%, HOLD2%
     CNTSLOW& = INP(DATA1%)
     CNTSHI& = INP(DATA1%)
     CNTS2A& = CNTSLOW& + CNTSHI& * 256
    
     OUT CONT1%, HOLD3%
     CNTSLOW& = INP(DATA1%)
     CNTSHI& = INP(DATA1%)
     CNTS3A& = CNTSLOW& * 65536
     CNTS& = CNTS2A& + CNTS3A&
     FREQA& = (CNTS& * 10) / CNTINTERVAL&
     DELTAFREQA& = FREQA& - FREQAPREV&
     FREQAPREV& = FREQA&

     OUT CONT2%, HOLD2%
     CNTSLOW& = INP(DATA2%)
     CNTSHI& = INP(DATA2%)
     CNTS2B& = CNTSLOW& + CNTSHI& * 256
     OUT CONT2%, HOLD3%
     CNTSLOW& = INP(DATA2%)
     CNTSHI& = INP(DATA2%)
     CNTS3B& = CNTSLOW& * 65536
     CNTS& = CNTS2B& + CNTS3B&
     FREQB& = (CNTS& * 10) / CNTINTERVAL&
     DELTAFREQB& = FREQB& - FREQBPREV&
     FREQBPREV& = FREQB&

    CALL SINGLEAD
    CALL DIFFERAD
    PREVRATIO# = FREQRATIO#
IF FREQB& <> 0 THEN
	FREQRATIO# = FREQA& / FREQB&
	ELSE FREQRATIO# = 0
END IF

IF ENDTIME# - STARTTIME# < 0 THEN
	ENDTIME# = ENDTIME# + 86400
END IF
   CALL VALVEPOSITION

   'added 1/25/07
   CALL IRIGTIME

   CALL WRITETOSCREEN 'Added 5/16/2012, screen update re-write

   'added 1/25/07
   CALL WRITETOSERIAL

   CALL WRITETODISK

LOOP UNTIL INKEY$ = CHR$(27)       'ESCAPE KEY
END

'******************************************************************

'******************************************************************
FUNCTION B2I& (BN$)            'INPUT: BINARY STRING BN$ = BBBBXBBBBXBBBB...
			'OUTPUT: LONG INTEGER VALUE LINT&
			'CONVERTS FORMATTED BINARY BYTES TO A LONG INTEGER
LINT& = 0
LENGTH% = LEN(BN$)
E% = 0
DO UNTIL LENGTH% = 0
	D% = -1
	A& = 2 ^ E%
	CH$ = MID$(BN$, LENGTH%, 1)   'READ CHARS FROM RIGHT
	IF CH$ = "0" THEN D% = 0
	IF CH$ = "1" THEN D% = 1
	IF D% <> -1 THEN
	LINT& = A& * D% + LINT&
	E% = E% + 1
	END IF
	LENGTH% = LENGTH% - 1
     
LOOP
B2I& = LINT&

END FUNCTION

'******************************************************************
SUB CNTRESET               'INITIALIZE/RESET COUNTER BOARD AT STARTUP

     OUT DIG1%, 0          'ZERO TTL REGISTERS DIG1 & DIG2
     OUT DIG2%, 0

		 'INITIALIZE CHIP 1

     OUT CONT1%, &HFF      'CHIP 1 MASTER RESET CODE: FFH
     OUT CONT1%, &H5F      'LOAD ALL CNTRS TO CLEAR TC (TERMINAL COUNT)
				'STATES:5F
     OUT CONT1%, &H15      'SET DATA POINTER TO COUNTER 5: 15
				   
		  'INITIALIZE CHIP 2

     OUT CONT2%, &HFF      'CHIP 2 MASTER RESET CODE: FFH
     OUT CONT2%, &H5F      'LOAD ALL COUNTERS TO CLEAR TC STATES:5F
     OUT CONT2%, &H15      'SET DATA POINTER TO COUNTER 5: 15
    
     PRINT "COUNTERS MASTER MODE HAS BEEN RESET"
     PRINT "COUNTERS LOADED TO CLEAR TERMINAL COUNT STATES"
     PRINT "COUNTERS DATA POINTER SET TO COUNTER 5"

END SUB

'******************************************************************
SUB CURRSUB (RES!, ADVOLTS#)

IF RES! > 0 THEN
MILLIAMP# = 1000 * ADVOLTS# / RES!
ELSE MILLIAMP# = 0
END IF

END SUB

'******************************************************************
SUB DATAPOINTERS         'DEFINES DATAPOINTERS FOR CNTR BOARD
	'DATAPOINTERS ARE ADDED TO THE REF I/O ADDR 380H
MASTERMODE% = &H17                   'MASTERMODE
STAT% = &H1F                         'STATUS
MODE1% = 1                           'CNTR 1 MODE
MODE2% = 2                           'CNTR 2 MODE
MODE3% = 3                           'CNTR 3 MODE
MODE4% = 4                           'CNTR 4 MODE
MODE5% = 5                           'CNTR 5 MODE
LOAD1% = 9                           'CNTR 1 LOAD
LOAD2% = &HA                         'CNTR 2 LOAD
LOAD3% = &HB                         'CNTR 3 LOAD
LOAD4% = &HC                         'CNTR 4 LOAD
LOAD5% = &HD                         'CNTR 5 LOAD
HOLD1% = &H11                        'CNTR 1 HOLD
HOLD2% = &H12                        'CNTR 2 HOLD
HOLD3% = &H13                        'CNTR 3 HOLD
HOLD4% = &H14                        'CNTR 4 HOLD
HOLD5% = &H15                        'CNTR 5 HOLD
HOLDCY1% = &H19                      'CNTR 1 HOLDCYCLE
HOLDCY2% = &H1A                      'CNTR 2 HOLDCYCLE
HOLDCY3% = &H1B                      'CNTR 3 HOLDCYCLE
HOLDCY4% = &H1C                      'CNTR 4 HOLDCYCLE
HOLDCY5% = &H1D                      'CNTR 5 HOLDCYCLE

PRINT "ALL DATA POINTERS HAVE BEEN DEFINED"

END SUB

'******************************************************************
SUB DIFFERAD                     'READS 8 DIFF A/D CHANS ON PROMETHEUS

	'DIFFBASEADDR% = 280H; BASEADDRESS SET TO 640 IN PARAMETERS SUB
	'CHANCODE% = 112; FOR I/O CHANNELS START 0 END 7; 70H

FIFO% = 99
OUT DIFFBASEADDR%, 16            'RESET FIFO (FIRST IN FIRST OUT)
OUT DIFFBASEADDR% + 2, 112       'I/O CHANNEL TO START AT 0 END 7 (70H)
OUT DIFFBASEADDR% + 3, 6         'SET ANALOG INPUT RANGE TO GAIN = 4

M% = 0
ADSTATUS = 99
DO                               'WAIT FOR SETTLING TIME
	M% = M% + 1
	STATUS% = INP(DIFFBASEADDR% + 3)
	ADSTATUS = STATUS% AND 32
LOOP UNTIL ADSTATUS = 0

OUT DIFFBASEADDR%, 128                      'INITIATE A/D CONVERSION (80H=128)

     N% = 0
     DO                                     'WAIT FOR CONVERSION TO FINISH
	STATUS% = INP(DIFFBASEADDR% + 3)  'READ STATUS REGISTER
	'BIT 7 = 0 MEANS CONVERSION COMPLETE
	N% = N% + 1                       'N% TO IDENTIFY WAIT LOOPS ONLY
     LOOP UNTIL STATUS% < 128
       '  PRINT "DIFFERENTIAL A/D WAIT VALUES", M%, N%
 DO
    FIFO% = INP(DIFFBASEADDR% + 6)           'FIFO% IS NO. CHAN READ & AVAILABLE
    LSB& = INP(DIFFBASEADDR%)               'READ LSB DATA
    ADCHANNEL% = ABS(FIFO% - 8)        'IDENTIFIES CHANNEL SAMPLED IN REG O AND 1
  
    MSB& = INP(DIFFBASEADDR% + 1)
    ADVALUE& = LSB& + MSB& * 256
    IF ADVALUE& > 32767 THEN
	ADVOLTS# = (ADVALUE& - 32768) / 13107  'CONVERT TO VOLTS 65535/5VOLTS=13107
    ELSE
	ADVOLTS# = (ADVALUE& + 32768) / 13107   'TWOS COMPLIMENT REPRESENTATION FROM A/D
    END IF

    IF ADCHANNEL% < 4 THEN        'AIN, BIN, AOUT, BOUT TEMPS
		CALL TEMPSUB(10, 10, ADVOLTS#)
		TEMPERATURE#(ADCHANNEL% + 1) = TEMP#
     END IF

     IF ADCHANNEL% = 4 THEN          '5 VOLT REFERENCE FOR A/D CONVERTERS
		CALL VOLTSUB(.5, 2, ADVOLTS#)
		VOLTAGE#(1) = VOLTS#
     END IF

	' IF ADCHANNEL% = 5 THEN          'RESERVED FOR BARITRON
		' CALL PRESSUB(233.333, ADVOLTS#)
		'((2K+1.5K)/1.5)*1000/10
		' PRESSURE#(3) = PRES#
	' END IF

     IF ADCHANNEL% = 6 THEN
		CALL PRESSUB(257.68, ADVOLTS#)
		PRESSURE#(2) = PRES# - 58.386 'ATMOSPHERIC PRESSURE GAUGE
     END IF

     IF ADCHANNEL% = 7 THEN
		CALL CURRSUB(.24, ADVOLTS#)
		CURRENT#(1) = MILLIAMP#      '28 POWER SUPPLY CURRENT
     END IF

LOOP UNTIL FIFO% = 0

END SUB

'******************************************************************
SUB FINETIMER                          'USEC ELAPSED FROM START OF GATE

     OUT CONT1%, 161                    '161:SAVE CNTR 1 CHIP 1 IN HOLD
     OUT CONT1%, HOLD1%                  'HOLD INTO DATA REGISTERS
     CNTSLOW& = INP(DATA1%)              'READ DATA REGISTERS
     CNTSHI& = INP(DATA1%)
     CNTSFIRST& = 4000 - (CNTSLOW& + CNTSHI& * 256)
     MICSECS# = CNTSFIRST& * 250	'ONE COUNT IS 250 MICROSECONDS
									'REPRESENTS USEC ELAPSED FROM
									'START OF GATE
END SUB

'******************************************************************
FUNCTION I2B$ (DEC&)    'CONVERTS LONG INTEGER TO 4 FORMATTED BINARY BYTES

HX$ = HEX$(DEC&)
BN$ = "0000 0000 0000 0000"     'INITIALIZE STRING
LENGTH% = LEN(HX$)
N% = 1
DO UNTIL LENGTH% = 0
    
      CH$ = MID$(HX$, N%, 1) 'READ CHARS FROM LEFT
      IF CH$ = "0" THEN D$ = "0000"
      IF CH$ = "1" THEN D$ = "0001"
      IF CH$ = "2" THEN D$ = "0010"
      IF CH$ = "3" THEN D$ = "0011"
      IF CH$ = "4" THEN D$ = "0100"
      IF CH$ = "5" THEN D$ = "0101"
      IF CH$ = "6" THEN D$ = "0110"
      IF CH$ = "7" THEN D$ = "0111"
      IF CH$ = "8" THEN D$ = "1000"
      IF CH$ = "9" THEN D$ = "1001"
      IF CH$ = "A" THEN D$ = "1010"
      IF CH$ = "B" THEN D$ = "1011"
      IF CH$ = "C" THEN D$ = "1100"
      IF CH$ = "D" THEN D$ = "1101"
      IF CH$ = "E" THEN D$ = "1110"
      IF CH$ = "F" THEN D$ = "1111"
      SELECT CASE LENGTH%
      CASE 1
      PS% = 16
      CASE 2
      PS% = 11
      CASE 3
      PS% = 6
      CASE 4
      PS% = 1
      END SELECT
     MID$(BN$, PS%, 4) = D$
     LENGTH% = LENGTH% - 1
     N% = N% + 1
    LOOP

I2B$ = BN$

END FUNCTION

'******************************************************************
SUB INITCOUNTERS                 'TO INITIALIZE ALL COUNTERS

CHIP% = 1
CNTR% = 1                         'CONTROLS COUNTING INTERVAL (1 SEC NOM)
				  'REPETITIVELY COUNTS DOWN FROM "HOLD"
MREG$ = "0000 1110 1010 0101"     '0EA5 NO GATE,F1/1000,SP,RPT,DN,ACT LO TC
LREG$ = I2B$(DWNCNTS&)            'SET COUNTER GATE INTERVAL
CALL SETMODE(CHIP%, CNTR%, MREG$, LREG$)
PRINT "CNTR 1/1 MODE REGISTERS", MREG$, I2B$(DWNCNTS&)

		    'SET COUNTERS 2 & 3 CHIP 1 FOR 32 BIT FREQ MEASUREMENTS
CNTR% = 2                         'LOWER BITS FOR CHANNEL A
MREG$ = "1001 0010 0010 1000"     '9228
LREG$ = "0000 0000 0000 0000"     '0000: PRESET TO ZERO
CALL SETMODE(CHIP%, CNTR%, MREG$, LREG$)
PRINT "CNTR 2/1 MODE REGISTERS", MREG$, LREG$

CNTR% = 3                          'UPPER BITS FOR CHANNEL A
MREG$ = "0001 0000 0010 1000"      '1028
LREG$ = "0000 0000 0000 0000"      '0000: PRESET TO ZERO
CALL SETMODE(CHIP%, CNTR%, MREG$, LREG$)
PRINT "CNTR 3/1 MODE REGISTERS", MREG$, LREG$

CNTR% = 4                          'NOT USED
MREG$ = "0000 0100 0000 1000"
LREG$ = "0000 0000 0000 0000"
CALL SETMODE(CHIP%, CNTR%, MREG$, LREG$)

CNTR% = 5                          'NOT USED
MREG$ = "0000 0100 0000 1000"
LREG$ = "0000 0000 0000 0000"
CALL SETMODE(CHIP%, CNTR%, MREG$, LREG$)

CHIP% = 2
CNTR% = 1                         'NOT USED
MREG$ = "0000 1110 1010 0101"
LREG$ = I2B$(DWNCNTS&)
'CALL SETMODE(CHIP%, CNTR%, MREG$, LREG$)

		    'SET COUNTERS 2 & 3 CHIP 1 FOR 32 BIT FREQ MEASUREMENTS
CNTR% = 2                          'LOWER BITS CHANNEL B
MREG$ = "1001 0010 0010 1000"      '8BA8
LREG$ = "0000 0000 0000 0000"      '0000: PRESET TO ZERO
CALL SETMODE(CHIP%, CNTR%, MREG$, LREG$)
PRINT "CNTR 2/2 MODE REGISTERS", MREG$, LREG$

CNTR% = 3                          'UPPER BITS CHANNEL B
MREG$ = "0001 0000 0010 1000"      '1028
LREG$ = "0000 0000 0000 0000"      '0000: PRESET TO ZERO
CALL SETMODE(CHIP%, CNTR%, MREG$, LREG$)
PRINT "CNTR 3/2 MODE REGISTERS", MREG$, LREG$

CNTR% = 4                          'NOT USED
MREG$ = "1001 0100 0000 1000"
LREG$ = "0000 0000 0000 0000"
CALL SETMODE(CHIP%, CNTR%, MREG$, LREG$)

CNTR% = 5                          'NOT USED
MREG$ = "0000 0100 0000 1000"
LREG$ = "0000 0000 0000 0000"
'CALL SETMODE(CHIP%, CNTR%, MREG$, LREG$)

END SUB

'******************************************************************
SUB INITSCREEN        'Configures static part of screen layout

    SCREEN 12
	    
    PRINT
    PRINT "IRIG Time:   :  :"
    PRINT "COUNTER A:              1s Delta:            TIME:"
    PRINT "COUNTER B:              1s Delta:            CYCLE:"
    PRINT "RATIO A/B:              1s Delta:"
    PRINT
    PRINT "Temperatures"
    PRINT "Cell A In      Cell B In       Cell A Out     Cell B Out"
    PRINT
    PRINT "LAMP           DET             SCRUB"
    PRINT
    PRINT "FAN            PS Plate        FAN            CPU"
    PRINT
    PRINT
    PRINT "Voltages"
    PRINT "5V REF     5V PS     5V PC     +12V     +24V     -24V     +28V     28V I"
    PRINT
    PRINT
    PRINT "Pressures"
    PRINT "Delta P        Ambient P"
    PRINT
    PRINT
    PRINT "Valve position      Expected position      Skips      Stops"
    PRINT
    PRINT
    PRINT "CPU utilization:    %"
    PRINT "Peak CPU utilization:    %"

END SUB

'******************************************************************
SUB IOMAP               'SETS I/O BASE PORT ADDRESSES FOR COUNTER BOARD
DATA1% = CNTRBASEADDR% + 0             'CHIP 1 DATA I/O
DATA2% = CNTRBASEADDR% + 4             'CHIP 2 DATA I/O
CONT1% = CNTRBASEADDR% + 1             'CHIP 1 STATUS (I) CONTROL (O)
CONT2% = CNTRBASEADDR% + 5             'CHIP 2 STATUS (I) CONTROL (O)
DIG1% = CNTRBASEADDR% + 2              'CHIP 1 TTL I/O
DIG2% = CNTRBASEADDR% + 3              'CHIP 2 TTL I/O
INTERRUPT1% = CNTRBASEADDR% + 6        'CHIP 1 INTERRUPT NOT USED
INTERRUPT2% = CNTRBASEADDR% + 7        'CHIP 2 INTERRUPT NOT USED

PRINT "I/O MAP FOR COUNTERS HAS BEEN SET"

END SUB

'******************************************************************
SUB IRIGTIME

DIM timebyte(1 TO 16)

XBASE% = 768            '0x300
UN100PORT% = 15         '0x0f

	n% = 0

	DO
	  timebyte(n% + 1) = INP(XBASE% + UN100PORT% - n%)
	  n% = n% + 1
	LOOP UNTIL n% = 9

	'hh 10s
	left = (INT(timebyte(7) * (2 ^ 4)) AND 255) / 16
	'hh
	right = INT(timebyte(6) / (2 ^ 4))
	HH = left * 10 + right
       
	'mm 10s
	left = (INT(timebyte(6) * (2 ^ 4)) AND 255) / 16
	'mm
	right = INT(timebyte(5) / (2 ^ 4))
	MM = left * 10 + right
       
	'ss 10s
	left = (INT(timebyte(5) * (2 ^ 4)) AND 255) / 16
	'ss
	right = INT(timebyte(4) / (2 ^ 4))
	SS = left * 10 + right

	'ms 100s
	left = (INT(timebyte(4) * (2 ^ 4)) AND 255) / 16 * 100
	'ms 10s
	right = INT(timebyte(3) / (2 ^ 4)) * 10
	'ms
	left1 = (INT(timebyte(3) * (2 ^ 4)) AND 255) / 16
	MS = left + right + left1

END SUB

'*********************************************************************
SUB oldWRITETOSERIAL

'''''' opened COM1 as unit 2

PRINT #2, DATE$; " "; TIME$; CYCLENO#; FREQA&; FREQB&
PRINT #2, USING "#####.##"; PRESSURE#(1); PRESSURE#(2); TEMPERATURE#(1); TEMPERATURE#(2); TEMPERATURE#(3);
PRINT #2, USING "#####.##"; TEMPERATURE#(4); TEMPERATURE#(5); TEMPERATURE#(6); TEMPERATURE#(7); TEMPERATURE#(8);
PRINT #2, USING "#####.##"; TEMPERATURE#(9); TEMPERATURE#(10); TEMPERATURE#(11); VOLTAGE#(1); VOLTAGE#(2);
PRINT #2, USING "#####.##"; VOLTAGE#(3); VOLTAGE#(4); VOLTAGE#(5); VOLTAGE#(6); VOLTAGE#(7);
PRINT #2, USING "######.##"; VOLTAGE#(8); VOLTAGE#(9); CURRENT#(1); CURRENT#(2);
PRINT #2, USING "####"; VALVEPOS%; ENCODROT%; STEPSTOREF%

'      PRINT "LAMP, DET, SCRUB, PROC",
'      PRINT "FAN, BOX, PS PANEL    ",
'      PRINT "5V R, 5V R, +28V, +24V",
'      PRINT "-24V, 15V, -15V, +12V ",
'      PRINT "+5V, 28V I, HEAT I    ",
'      PRINT "DELTA PRES, ATMOS PRES",
			
'      PRINT USING "####"; STEPSDONE%; ENCODCNTS%; ENCODROT%; STEPSTOREF%; STEPSLIP%

END SUB

'******************************************************************
SUB OPENDATAFILE

TODAY$ = DATE$
NOW$ = TIME$
YR$ = MID$(TODAY$, 9, 2)
MO$ = MID$(TODAY$, 1, 2)
DA$ = MID$(TODAY$, 4, 2)
HR$ = MID$(NOW$, 1, 2)
MN$ = MID$(NOW$, 4, 2)
SEC$ = MID$(NOW$, 7, 1)
DATAOUT$ = "C:\DATA\" + YR$ + MO$ + DA$ + HR$ + "." + MN$ + "R"
ENGOUT$ = "C:\DATA\" + YR$ + MO$ + DA$ + HR$ + "." + MN$ + "H"
COMBDATA$ = "C:\DATA\" + YR$ + MO$ + DA$ + HR$ + "." + MN$ + SEC$

'Initialize the data file with column headings.
OPEN ENGOUT$ FOR APPEND AS #1
	PRINT #1, "COMP_T       TMPL    TMPD    TMPC    TMPF    TMPPS   TMPB   TMPPC   ";
	PRINT #1, "V5REF   V5PS    V28M   V24D    V_24D    V15B   V_15B    V12L   V5PC   ";
	PRINT #1, "I28V     VQ      VEQ     VSQ     VST     PCT"
CLOSE #1

'Initialize engineering data file with column headings.
OPEN DATAOUT$ FOR APPEND AS #3
	PRINT #3, "COMP_T   CYCLE_No   CNT_A   CNT_B   DELP    PABS    PBAR    AIN     BIN     AOUT    BOUT"
CLOSE #3

'Create an empty file.
OPEN COMBDATA$ FOR APPEND AS #1
CLOSE #1

END SUB

'******************************************************************
SUB PARAMETERS                  'SETS VARIOUS PARAMETERS

CLS
CNTRBASEADDR% = &H380               'SET COUNTER BASEADDRESS
DIFFBASEADDR% = &H280               'SET DIFF A/D  BASEADDRESS ON PROMETHEUS
SINGBASEADDR% = &H340               'SET SING A/D  BASEADDRESS ON PROMETHEUS

OSCFREQ& = 4                       'OSC FREQ IN MHZ
CNTINTERVAL& = 10                   'SET COUNTER INTERVAL IN TENTHS OF SECOND
DWNCNTS& = 100 * CNTINTERVAL& * OSCFREQ&'SET DOWN CNTS FOR GATE COUNTER
DEC& = DWNCNTS&

END SUB

'******************************************************************
SUB PRESSUB (MBARCAL!, ADVOLTS#)
	'CALC PRESSURE FOR PRESSURE SENSOR
	'MBARCAL IS NUMBER OF MILLIBARS/VOLT ON SENSOR
	'ADVOLTS IS VOLTAGE MEASURED BY A/D
    PRES# = ADVOLTS# * MBARCAL!

END SUB

'******************************************************************
'Print a text label on the screen at specified coordinates.
SUB PrintLabel (s$, row, col, clr)

    IF (col < 1) THEN col = 1
    IF (row < 1) THEN row = 1
    LOCATE row, col
    COLOR clr
    PRINT s$

END SUB

'******************************************************************
'Print a numeric value on the screen at specified coordinates.
SUB PrintNumArr (num#(), fmt$, ind, row, col, clr)

    IF (col < 1) THEN col = 1
    IF (row < 1) THEN row = 1
    IF (col > 80) THEN col = 80
    IF (row >= 59) THEN row = 58
    IF (fmt$ = "") THEN fmt$ = "####"

    LOCATE row, col
    COLOR clr
    PRINT USING fmt$; num#(ind)

END SUB

'******************************************************************
'Print a numeric value on the screen at specified coordinates.
SUB PrintNumLabel (num#, fmt$, row, col, clr)

    IF (col < 1) THEN col = 1
    IF (row < 1) THEN row = 1
    IF (col > 80) THEN col = 80
    IF (row >= 59) THEN row = 58
    IF (fmt$ = "") THEN fmt$ = "####"

    LOCATE row, col
    COLOR clr
    PRINT USING fmt$; num#

END SUB

'******************************************************************
SUB SETMASTER       'SET MASTERMODE REGISTERS

	'MASTERMODE REGISTER BITS CONTROL AS FOLLOWS:
	'BIT 15 BCD DIV = 1
	'BIT 14 DATA POINTER INCREMENT ENABLE = 0
	'BIT 13 DATA BUS WIDTH 8 BITS = 0
	'BIT 12 FOUT GATE ON = 0
	'BITS 11-8 FOUT DIVIDER N=DIVIDE BY N
	'BITS 7-4 FOUT SOURCE F1=B,F2=C,F3=D,F4=E,F5=F
	'BITS 1-3 COMPARE AND TIME OF DAY DISABLE (0000)

OUT CONT1%, MASTERMODE%     'POINT TO MASTERMODE REGISTER CHIP 1
OUT DATA1%, &HB0            'SET MASTERMODE LOWER BITS CHIP 1 TO B0
	'B:Fout=F1=OSC. FREQ 0:DISABLE COMP AND TIME OF DAY
OUT DATA1%, &HC2            'SET MASTERMODE UPPER BITS CHIP 1 TO C2
	'C: BCD DIV FOR Fout, INC DISAB, 8 BIT BUS, Fout ON
	'2: Fout DIV BY 2 (TO DET BOARD?)

OUT CONT2%, MASTERMODE%     'POINT TO MASTERMODE REGISTER CHIP 2
OUT DATA2%, &HB0            'SET MASTERMODE LOWER BITS CHIP 2 B0
			    'AS ON CHIP 1
OUT DATA2%, &HC1            'SET MASTERMODE UPPER BITS CHIP 2 C1
			    'AS ON CHIP 1 EXCEPT Fout/1
PRINT "COUNTER BOARD MASTERMODE SET"

END SUB

'******************************************************************
SUB SETMODE (CHIP%, CNTR%, MREG$, LREG$)    'FOR EACH COUNTER

	'FOR CHIP NO. (CHIP%) AND COUNTER NO. (CNTR%)
	'LOAD MODE REGISTER WITH HEX STRING (MREG$) AND
	'PRESET ITS LOAD REGISTER TO HEX STRING (LREG$)

	'MODE REGISTER BITS
		'15,14,13 HARDWARE GATE OPTIONS 0 = NO GATING
		'12 GATE ON RISE/FALL OF PULSE 0/1
		'11-8 COUNT SOURCE
		'7 SPECIAL GATE DISABLE/ENABLE 0/1
		'6 RELOAD FROM LOAD = 0
		'5 COUNT ONCE/REPETITIVELY  0/1
		'4 COUNT BINARY/BCD  0/1
		'3 COUNT DOWN/UP   0/1
		'2,1,0 COUNTERS OUTPUT CONTROL

IF CHIP% = 1 THEN
     CONT% = CONT1%
     DAT% = DATA1%
ELSE
     CONT% = CONT2%
     DAT% = DATA2%
END IF

SELECT CASE CNTR%
     CASE 1
     MODE% = MODE1%
     LOAD% = LOAD1%
     CASE 2
     MODE% = MODE2%
     LOAD% = LOAD2%
     CASE 3
     MODE% = MODE3%
     LOAD% = LOAD3%
     CASE 4
     MODE% = MODE4%
     LOAD% = LOAD4%
     CASE 5
     MODE% = MODE5%
     LOAD% = LOAD5%
END SELECT

	'READ MODE BYTES
LOW$ = MID$(MREG$, 11, 9)
LOWBITS% = B2I&(LOW$)
HI$ = MID$(MREG$, 1, 9)
HIBITS% = B2I&(HI$)

	'LOAD MODE REGISTER
OUT CONT%, MODE%                 'POINT AT MODE REGISTER
OUT DAT%, LOWBITS%               'COUNT/OUT CONTROLS
OUT DAT%, HIBITS%                 'GATE CONTROLS, COUNT SOURCE

	'READ LOAD BYTES
LOW$ = MID$(LREG$, 11, 9)
LOWBITS% = B2I&(LOW$)
HI$ = MID$(LREG$, 1, 9)
HIBITS% = B2I&(HI$)

	'LOAD LOAD REGISTER: PRESETS FOR UP/DOWN COUNT
OUT CONT%, LOAD%                   'POINT AT LOAD REGISTER
OUT DAT%, LOWBITS%
OUT DAT%, HIBITS%

END SUB

'******************************************************************
SUB SETSYSTEMDATE

PRINT DATE$

PRINT "Enter the date below (default year is 1989)."
INPUT "    Month:   ", Month$
INPUT "    Date:    ", Day$
INPUT "    Year:    ", Year$
IF Year$ = "" THEN Year$ = "89"
DATE$ = Month$ + "/" + Day$ + "/" + Year$

END SUB

SUB SETSYSTEMTIME

H$ = STR$(HH + 10)
M$ = STR$(MM)
s$ = STR$(SS)

H$ = LTRIM$(RTRIM$(STR$(HH + 10)))
M$ = LTRIM$(RTRIM$(STR$(MM)))
s$ = LTRIM$(RTRIM$(STR$(SS)))

'Set the system clock to match the IRIG clock

IF (HH > 0) THEN
   TIME$ = H$ + ":" + M$ + ":" + s$
END IF

END SUB

'******************************************************************
SUB SINGLEAD

SINGBASEADDR% = 832      'BASEADDRESS SET TO 340H IN PARAMETERS SUB
CHANCODE% = 240          'CODE FOR START AND STOP (0-15:240)
CHANTOSAMPLE% = 16       'NUMBER OF CHANNELS TO SAMPLE

OUT SINGBASEADDR% + 2, CHANCODE%   'SET START AND STOP

N% = 0
NFINAL% = 1000
DO                                 'WAIT FOR 10 MICROSECS FOR SETTLING TIME
     N% = N% + 1
LOOP UNTIL N% = NFINAL%

DO                                 'LOOP THROUGH ALL CHANNELS TO SAMPLE
     OUT SINGBASEADDR%, 69               'CONVERT A CHANNEL
   
     STATUS% = INP(SINGBASEADDR% + 8)    'READ STATUS REGISTER INTO STATUS%
     ADSTATUS = STATUS% AND 128           'BIT 7 = 0 MEANS CONVERSION COMPLETE
     N% = 0
     DO
		STATUS% = INP(SINGBASEADDR% + 8) 'READ STATUS REGISTER INTO STATUS%
		ADSTATUS = STATUS% AND 128       'READ STATUS OF A/D CONVERSION
		N% = N% + 1
     LOOP UNTIL ADSTATUS = 0

     REG0% = INP(SINGBASEADDR%)
     ADCHANNEL% = REG0% AND 15    'IDENTIFIES CHANNEL SAMPLED IN REG O AND 1
     LSB% = REG0% / 16
     REG1% = INP(SINGBASEADDR% + 1)
     MSB% = REG1% * 16
     ADVALUE% = LSB% + MSB%
     ADVOLTS# = ADVALUE% / 409.5 - 5

	IF ADCHANNEL% < 7 THEN         'LAMP, DET, BODY, FAN, PS PANEL, BOX, PROC
		CALL TEMPSUB(20, 0, ADVOLTS#)
		TEMPERATURE#(ADCHANNEL% + 5) = TEMP#
	END IF

	IF ADCHANNEL% = 7 THEN                  'DELTA P GAUGE
		CALL PRESSUB(1.25, ADVOLTS# - 2.25)
		PRESSURE#(1) = PRES#
	END IF

	IF ADCHANNEL% = 8 THEN                    '5 VOLT POWER SUPPLY
		CALL VOLTSUB(10, 20, ADVOLTS#)
		VOLTAGE#(2) = VOLTS#    '
	END IF

	IF ADCHANNEL% > 8 AND ADCHANNEL < 12 THEN '#9 FOR +28 V PS
		CALL VOLTSUB(10, 2, ADVOLTS#)        '#10 FOR +24 V PS
		VOLTAGE#(ADCHANNEL% - 6) = VOLTS#    '#11 FOR -24 V PS
	END IF

	IF ADCHANNEL% = 12 THEN                    'FOR +15 VOLT POWER SUPPLY
		'CALL VOLTSUB(20, 10, ADVOLTS#)
		'VOLTAGE#(ADCHANNEL% - 6) = VOLTS#
		VOLTAGE#(6) = 0
	END IF
	'FOR -15 VOLT POWER SUPPLY
	IF ADCHANNEL% = 13 THEN
		'CALL VOLTSUB(20, 10, ADVOLTS#)
		'VOLTAGE#(ADCHANNEL% - 6) = VOLTS#
		VOLTAGE#(7) = 0
	END IF


	IF ADCHANNEL% = 14 THEN             '+12 VOLT POWER SUPPLY
		CALL VOLTSUB(20, 10, ADVOLTS#)
		VOLTAGE#(8) = VOLTS#
	END IF
	IF ADCHANNEL% = 15 THEN               'PC104 5 VOLTS
		CALL VOLTSUB(10, 20, ADVOLTS#)
		VOLTAGE#(9) = VOLTS#
	END IF


     CHANTOSAMPLE% = CHANTOSAMPLE% - 1
     LOOP UNTIL CHANTOSAMPLE% = 0

     

END SUB

'******************************************************************
SUB TEMPSUB (R1!, R2!, ADVOLTS#)

		'CALC TEMPERATURE FOR 10K THERMISTOR WITH VCC=5.0 VOLTS
		'VCC REFERENCE VOLTAGE.. R1..THERMISTOR (RT)..R2..REF COMMON
		'ADVOLTS IS VOLTAGE MEASURED ACROSS THERMISTOR
		'RESISTANCE VALUES IN K OHMS

IF 5 - ADVOLTS# <> 0 THEN RT# = ADVOLTS# * (R1! + R2!) / (5 - ADVOLTS#)
IF RT# < .726 THEN RT# = .726#
IF RT# > 191.492 THEN RT# = 191.492#
RT# = RT# * 1000#
TEMP# = 1 / (.001126# + .0002346# * LOG(RT#) + .0000000861# * (LOG(RT#)) ^ 3)
TEMP# = TEMP# - 273.16#


'IF 5 - ADVOLTS# <> 0 THEN RTOM# = (2 * ADVOLTS# + 20) / (5 - ADVOLTS#)
'IF RTOM# < .726 THEN RTOM# = .726
'IF RTOM# > 191.492 THEN RTOM# = 191.492
'RTOM# = RTOM# * 1000#
'TOMTEMP# = 1 / (.00112846# + .000234227# * LOG(RT#) + .0000000873395# * (LOG(RT#)) ^ 3)
'TOMTEMP# = TOMTEMP# - 273.16#


END SUB

'******************************************************************
SUB VALVECHG

OUT DIFFBASEADDR% + 11, 0  'OUTPUT DATA TO TTL PORT B
IF CYCLENO# MOD 20 = 0 THEN
	OUT DIFFBASEADDR% + 9, 32  'OUTPUT HI TO TTL PORT B BIT NO 5
	'SET TTL FOR MODE A (HI LEVEL DURING MODE A)
ELSE
	OUT DIFFBASEADDR% + 9, 0   'OUTPUT LOW TO TTL PORT B BIT NO 5
	'ELSE SET TTL FOR MODE B (LOW LEVEL DURING MODE B)
END IF

END SUB

'******************************************************************
SUB VALVEPOSITION

OUT DIFFBASEADDR% + 11, 1               'INPUT DATA FROM PORT C BITS 0-3
VALVEQUADRANT% = INP(DIFFBASEADDR% + 10)'READ DATA FROM VALVE POSITION SENSORS
VALVECODE% = VALVEQUADRANT%
SELECT CASE VALVEQUADRANT%

CASE 7
	VALVEQUADRANT% = 4
CASE 11
	VALVEQUADRANT% = 3
CASE 13
	VALVEQUADRANT% = 2
CASE 14
	VALVEQUADRANT% = 1

END SELECT

EXPECTQUADRANT% = (1 + (CYCLENO# - 5) / 10) MOD 4
IF EXPECTQUADRANT% = 0 THEN EXPECTQUADRANT% = 4

END SUB

SUB VALVESLIP     'DETECTS WHETHER VALVE MOVED PROPERLY

VALVEMOVE% = VALVEQUADRANT% - PREVALVEQUADRANT%
IF VALVEMOVE% = 0 THEN
	VALVESTOPPED% = VALVESTOPPED% + 1
END IF

IF VALVEMOVE% = -1 OR VALVEMOVE% = 3 THEN
	VALVESKIPPED% = VALVESKIPPED% + 1
END IF

PREVALVEQUADRANT% = VALVEQUADRANT%

END SUB

'******************************************************************
SUB VOLTSUB (R1!, R2!, ADVOLTS#)    'CONVERTS A/D VOLTS TO ACTUAL VOLTS
									'IN VOLTAGE R1/R2 RESISTANCE DIVIDER

   VOLTS# = ADVOLTS# * (R1! + R2!) / R2!
END SUB

'******************************************************************
SUB WRITETODISK
	'This is the engineering data for Proffitt format.
	OPEN ENGOUT$ FOR APPEND AS #1
      PRINT #1, TIMER;
      PRINT #1, USING "####.##"; TEMPERATURE#(5); TEMPERATURE#(6); TEMPERATURE#(7); TEMPERATURE#(8); TEMPERATURE#(9); TEMPERATURE#(10); TEMPERATURE#(11);
      PRINT #1, USING "####.##"; VOLTAGE#(1); VOLTAGE#(2); VOLTAGE#(3); VOLTAGE#(4); VOLTAGE#(5); VOLTAGE#(6); VOLTAGE#(7); VOLTAGE#(8); VOLTAGE#(9);
      PRINT #1, USING "#####"; CURRENT#(1);
      PRINT #1, USING "#######"; VALVEQUADRANT%; EXPECTQUADRANT%; VALVESKIPPED%; VALVESTOPPED%;

      CALL FINETIMER
      PCTEXPIRED% = 100 * MICSECS# / 1000000
      PRINT #1, USING "###.#"; PCTEXPIRED%
	CLOSE #1
	
	'This is the data file for Proffitt format.
	OPEN DATAOUT$ FOR APPEND AS #1
		PRINT #1, TIMER;
		PRINT #1, USING "########"; CYCLENO#; FREQA&; FREQB&;
		PRINT #1, USING "#####.###"; PRESSURE#(1); PRESSURE#(2); PRESSURE#(3); TEMPERATURE#(1); TEMPERATURE#(2); TEMPERATURE#(3); TEMPERATURE#(4)
	CLOSE #1
	
	'This is for PSI format.
	OPEN COMBDATA$ FOR APPEND AS #1
		PRINT #1, ""
		PRINT #1, DATE$, TIME$, CYCLENO#
		PRINT #1, USING "##:##:##.###"; HH; MM; SS + MS / 1000!
		'Delta P, Total P (mbar)
		PRINT #1, USING "#####.###"; PRESSURE#(1); PRESSURE#(2)
		PRINT #1, USING "########"; FREQA&; FREQB&
		'Chamber temps, AInlet, BInlet, Aoutlet, BOutlet
		PRINT #1, USING "####.##"; TEMPERATURE#(1); TEMPERATURE#(2); TEMPERATURE#(3); TEMPERATURE#(4);
		IF CYCLENO# MOD 10 = 0 OR CYCLENO# MOD 10 = 5 THEN
			   'lamp, det, body Temp (C)
			   PRINT #1, USING "####.##"; TEMPERATURE#(5); TEMPERATURE#(6); TEMPERATURE#(7);
			   
			   'fan, ps panel, box, proc
			   PRINT #1, USING "####.##"; TEMPERATURE#(8); TEMPERATURE#(9); TEMPERATURE#(10); TEMPERATURE#(11)
			   
			   '(V)
			   '5V REF, 5V PS, +28V, +24V, -24V, spare1, spare2, +12V", 5V PC104
			   PRINT #1, USING "####.##"; VOLTAGE#(1); VOLTAGE#(2); VOLTAGE#(3); VOLTAGE#(4); VOLTAGE#(5); VOLTAGE#(6); VOLTAGE#(7); VOLTAGE#(8); VOLTAGE#(9)
			   
			   '(mA)
			   PRINT #1, USING "#####"; CURRENT#(1);
			   
			   PRINT #1, USING "#######"; VALVEQUADRANT%; EXPECTQUADRANT%; VALVESKIPPED%; VALVESTOPPED%
			   CALL FINETIMER
			   PCTEXPIRED% = 100 * MICSECS# / 1000000
			   PRINT #1, USING "###.#"; PCTEXPIRED%
		END IF
	CLOSE #1

END SUB

'******************************************************************
SUB WRITETOSCREEN

VertOffset% = 0
HorizOffset% = 0

	PrintNumLabel CDBL(HH), "##", VertOffset% + 2, HorizOffset% + 12, Yellow'RT stamp
	PrintNumLabel CDBL(MM), "##", VertOffset% + 2, HorizOffset% + 15, Yellow
	PrintNumLabel CDBL(SS + MS / 1000!), "##.###", VertOffset% + 2, HorizOffset% + 18, Yellow
	
	PrintNumLabel CDBL(FREQA&), "#######", VertOffset% + 3, HorizOffset% + 12, Green'Freq A
	PrintNumLabel CDBL(DELTAFREQA&), "#######", VertOffset% + 3, HorizOffset% + 34, Green'Delta A
	PrintNumLabel (ENDTIME# - STARTTIME#), "######.#", VertOffset% + 3, HorizOffset% + 53, Green 'Increm time stamp
	
	PrintNumLabel CDBL(FREQB&), "#######", VertOffset% + 4, HorizOffset% + 12, Green 'Freq B
	PrintNumLabel CDBL(DELTAFREQB&), "#######", VertOffset% + 4, HorizOffset% + 34, Green 'Delta B
	PrintNumLabel CYCLENO#, "########", VertOffset% + 4, HorizOffset% + 53, Green 'Cycle counter
	
	PrintNumLabel FREQRATIO#, "###.######", VertOffset% + 5, HorizOffset% + 12, Green' Ratio A/B
	PrintNumLabel (PREVRATIO# - FREQRATIO#), "###.######", VertOffset% + 5, HorizOffset% + 34, Green' Delta ratio
	  
	PrintNumArr TEMPERATURE#(), "##.##", 1, VertOffset% + 9, HorizOffset% + 1, Green
	PrintNumArr TEMPERATURE#(), "##.##", 2, VertOffset% + 9, HorizOffset% + 16, Green
	PrintNumArr TEMPERATURE#(), "##.##", 3, VertOffset% + 9, HorizOffset% + 32, Green
	PrintNumArr TEMPERATURE#(), "##.##", 4, VertOffset% + 9, HorizOffset% + 47, Green

	PrintNumArr TEMPERATURE#(), "##.##", 5, VertOffset% + 11, HorizOffset% + 1, Green
	PrintNumArr TEMPERATURE#(), "##.##", 6, VertOffset% + 11, HorizOffset% + 16, Green
	PrintNumArr TEMPERATURE#(), "##.##", 7, VertOffset% + 11, HorizOffset% + 32, Green
	  
	PrintNumArr TEMPERATURE#(), "##.##", 8, VertOffset% + 13, HorizOffset% + 1, Green
	PrintNumArr TEMPERATURE#(), "##.##", 9, VertOffset% + 13, HorizOffset% + 16, Green
	PrintNumArr TEMPERATURE#(), "##.##", 10, VertOffset% + 13, HorizOffset% + 32, Green
	PrintNumArr TEMPERATURE#(), "##.##", 11, VertOffset% + 13, HorizOffset% + 47, Green
	
	PrintNumArr VOLTAGE#(), "##.##", 1, VertOffset% + 17, HorizOffset% + 1, Green
	PrintNumArr VOLTAGE#(), "##.##", 2, VertOffset% + 17, HorizOffset% + 11, Green
	PrintNumArr VOLTAGE#(), "##.##", 9, VertOffset% + 17, HorizOffset% + 21, Green
	PrintNumArr VOLTAGE#(), "###.##", 8, VertOffset% + 17, HorizOffset% + 30, Green
	PrintNumArr VOLTAGE#(), "###.##", 4, VertOffset% + 17, HorizOffset% + 40, Green
	PrintNumArr VOLTAGE#(), "###.##", 5, VertOffset% + 17, HorizOffset% + 49, Green
	PrintNumArr VOLTAGE#(), "###.##", 3, VertOffset% + 17, HorizOffset% + 58, Green
	PrintNumArr CURRENT#(), "######", 1, VertOffset% + 17, HorizOffset% + 67, Green
	
	PrintNumArr PRESSURE#(), "###.###", 1, VertOffset% + 21, HorizOffset% + 1, Green
	PrintNumArr PRESSURE#(), "####.##", 2, VertOffset% + 21, HorizOffset% + 18, Green
	
	PrintNumLabel CDBL(VALVEQUADRANT%), "###", VertOffset% + 24, HorizOffset% + 0, Green
	PrintNumLabel CDBL(EXPECTQUADRANT%), "###", VertOffset% + 24, HorizOffset% + 20, Green
	PrintNumLabel CDBL(VALVESKIPPED%), "####", VertOffset% + 24, HorizOffset% + 43, Green
	PrintNumLabel CDBL(VALVESTOPPED%), "####", VertOffset% + 24, HorizOffset% + 54, Green
	
	CALL FINETIMER
	PCTTIME% = 100 * MICSECS# / 1000000
	IF PCTEXPIRED% > MAXPCT% THEN MAXPCT% = PCTEXPIRED%
	PrintNumLabel CDBL(PCTTIME%), "###", VertOffset% + 26, HorizOffset% + 17, Green
	PrintNumLabel CDBL(MAXPCT%), "###", VertOffset% + 27, HorizOffset% + 22, Green
     
END SUB

'******************************************************************
SUB WRITETOSERIAL
 
       PRINT #2, DATE$, TIME$, CYCLENO#
      
       PRINT #2, USING "##:##:##.###"; HH; MM; SS + MS / 1000!
      
       PRINT #2, USING "########"; FREQA&; FREQB&;
       PRINT #2, USING "#####.##"; PRESSURE#(1); PRESSURE#(2);

       'Chamber temps
       'lamp, det, scrub
       'fan, ps panel, box, proc
      
       PRINT #2, USING "####.##"; TEMPERATURE#(1); TEMPERATURE#(2); TEMPERATURE#(3); TEMPERATURE#(4);
       PRINT #2, USING "####.##"; TEMPERATURE#(5); TEMPERATURE#(6); TEMPERATURE#(7);
       PRINT #2, USING "####.##"; TEMPERATURE#(8); TEMPERATURE#(9); TEMPERATURE#(10); TEMPERATURE#(11);

       '5V Ref, 5V PS, 5V PC, +12V
       '+24V, -24V, +28V

       PRINT #2, USING "####.##"; VOLTAGE#(1); VOLTAGE#(2); VOLTAGE#(9); VOLTAGE#(8);
       PRINT #2, USING "####.##"; VOLTAGE#(4); VOLTAGE#(5); VOLTAGE#(3)
      
       '28VI
       PRINT #2, USING "#####"; CURRENT#(1)
     
       'PRINT #2, USING "#######"; VALVEQUADRANT%; EXPECTQUADRANT%; VALVESKIPPED%; VALVESTOPPED%
       PRINT #2, USING "#######"; VALVEPOS%; VALVEQUADRANT%; EXPECTQUADRANT%; VALVESKIPPED%; VALVESTOPPED%

END SUB

