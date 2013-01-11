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

'Screen update routines:
DECLARE SUB PrintNumArr (num#(), fmt$, ind, row, col, clr)
DECLARE SUB PrintLabel (s$, row, col, clr)
DECLARE SUB PrintNumLabel (num#, fmt$, row, col, clr)

COMMON SHARED CNTRBASEADDR%, DIFFBASEADDR%, SINGBASEADDR%
COMMON SHARED LINT&, BN$, DEC&
COMMON SHARED VALVEQUADRANT%, EXPECTQUADRANT%, PREVALVEQUADRANT%, VALVECODE%
COMMON SHARED VALVESKIPPED%, VALVESTOPPED%
COMMON SHARED DWNCNTS&
COMMON SHARED CNTR%, MODE%, MREG&, CHIP%, LOAD%, LREG&
COMMON SHARED DATA1%, DATA2%, CONT1%, CONT2%, DIG1%, DIG2%
COMMON SHARED INTERRUPT1%, INTERRUPT2%, STATUS%, MASTERMODE%
COMMON SHARED MODE1%, LOAD1%, HOLD1%, HOLDCY1%
COMMON SHARED MODE2%, LOAD2%, HOLD2%, HOLDCY2%
COMMON SHARED MODE3%, LOAD3%, HOLD3%, HOLDCY3%
COMMON SHARED MODE4%, LOAD4%, HOLD4%, HOLDCY4%
COMMON SHARED MODE5%, LOAD5%, HOLD5%, HOLDCY5%
COMMON SHARED row%, COLUMN%, VALUE!, ADTEST&
COMMON SHARED TEMP#, RT#, ADVOLTS#, PRES#, ENGUNITS#, VOLTS#, MILLIAMP#
COMMON SHARED R1!, R2!, MBARCAL!
COMMON SHARED FREQA&, FREQB&, CNTSREF&, DELTAFREQA&, DELTAFREQB&, FREQRATIO#, PREVRATIO#
COMMON SHARED MAXPCTUSED!, PCTUSED!, LoopNumber%
COMMON SHARED STARTTIME#, ENDTIME#, CYCLENO&, PREVTIME#

'ScaleFactor should be changed together with the Frequency Divider bit in the INITCOUNTERS routine.
COMMON SHARED ScaleFactor%, ValveEnabled% 'ScaleFactor is number of data records per second
COMMON SHARED HH, MM, SS, MS
COMMON SHARED DATAOUT$
COMMON SHARED ENGOUT$

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
CLS
CALL PARAMETERS     'CONTAINS THE EXPERIMENT PARAMETERS
CALL IOMAP          'DEFINE I/O MAP
CALL DATAPOINTERS   'DEFINE DATA POINTERS FOR COUNTER BOARD
CALL CNTRESET       'RESET COUNTER BOARD
CALL SETMASTER      'SET MASTER MODE FOR BOTH COUNTER CHIPS
CALL OPENDATAFILE   'OPEN DATA AND ENGDATA FILES FOR WRITING
CALL IRIGTIME

ValveFlip1% = 10
ValveFlip2% = 1
ValveInverted% = 1
ValveEnabled% = 0
ScaleFactor% = 10
CYCLENO& = 0
PREVALVEQUADRANT% = 1
VALVESTOPPED% = 0
VALVESKIPPED% = 0
CALL VALVEPOSITION

   STARTTIME# = TIMER
 
     CLOSED% = 1
     CYCLENO& = -1

'OPEN "COM1:9600,N,8,1,BIN,CS0,DS0" FOR RANDOM AS #2 'Turn Off serial output for lab experiments.
MAXPCTUSED! = 0!
PCTUSED! = 0!

CALL INITSCREEN
OPEN DATAOUT$ FOR APPEND AS #1
OPEN ENGOUT$ FOR APPEND AS #2

CALL INITCOUNTERS   'INITIALIZE COUNTER MODE REGISTERS
OUT CONT1%, 233         'Set Toggle Out to High on #1, Chip1. This enables hardware Gates to allow counting.
OUT CONT1%, 127         'Load and arm all 5 counters on Chip1. All of them start counting at this time.
OUT CONT2%, 127         'Load and arm all 5 counters on Chip2. All of them start counting at this time.

'........................TIME LOOP STARTS HERE!
DO
     CYCLENO& = CYCLENO& + 1

	DO 
		STATUS% = INP(CONT1%) AND 2 'Bit 2 will go Low on TC of #1 and stay there since #1 counts once and disarms.
	LOOP UNTIL STATUS% = 0 'If the OS was busy, this loop will check True instantly. Counters will have sat waiting, stopped counting.

	OUT CONT1%, 134         'Disarm and save #2, #3, Chip1
	OUT CONT2%, 134         'Disarm and save #2, #3, Chip2
	OUT CONT1%, 95          'Load all counters, Chip1
	OUT CONT2%, 95          'Load all counters, Chip2
	OUT CONT1%, 233         'Set Toggle Out to High on #1. This enables hardware Gates to allow counting.
	OUT CONT1%, 63          'Arm all counters
	OUT CONT2%, 63          'Arm all counters
	

    ENDTIME# = TIMER 'Take time stamp right after counters are recorded.
	IF ENDTIME# - STARTTIME# < 0 THEN
		ENDTIME# = ENDTIME# + 86400
	END IF

	'Advance valve every 10 seconds and only if valve operation is Enabled.
	IF (CYCLENO& MOD 10 * ScaleFactor% * ValveFlip1% = 0) AND ValveEnabled% THEN
		CALL VALVECHG
		CALL VALVESLIP
		SWAP ValveFlip1%, ValveFlip2%
	END IF

    OUT CONT1%, HOLD2%
	CNTSLOW& = INP(DATA1%)
	CNTSHI& = INP(DATA1%)
	CNTS2A& = CNTSLOW& + CNTSHI& * 256
	
	OUT CONT1%, HOLD3%
	CNTSLOW& = INP(DATA1%)
	CNTSHI& = INP(DATA1%)
	CNTS3A& = CNTSLOW& * 65536
	CNTS& = CNTS2A& + CNTS3A&
	FREQA& = CNTS&
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
	FREQB& = CNTS&
	DELTAFREQB& = FREQB& - FREQBPREV&
	FREQBPREV& = FREQB&

    CALL SINGLEAD
    CALL DIFFERAD
	
    PREVRATIO# = FREQRATIO#
	
IF FREQB& <> 0 THEN
	FREQRATIO# = FREQA& / FREQB&
	ELSE FREQRATIO# = 0
END IF

	CALL VALVEPOSITION
	CALL WRITETOSCREEN
	CALL WRITETODISK
	CALL IRIGTIME
	PREVTIME# = ENDTIME#
	
   'Monitor possible Valve Control command ("v" on keyboard).
	IF INKEY$ = "v" THEN
		SWAP ValveEnabled%, ValveInverted%
	END IF

'	CALL WRITETOSERIAL 'Disable serial port output until needed
	CALL FINETIMER 'This procedure will capture the time used by all procedures in the loop.

LOOP UNTIL INKEY$ = CHR$(27)       'ESCAPE KEY
CLOSE #1
CLOSE #2
END

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

	OUT DIG1%, 0          'ZERO TTL REGISTERS DIG1
	OUT DIG2%, 0          'ZERO TTL REGISTERS DIG2

	OUT CONT1%, &HFF      'CHIP 1 MASTER RESET CODE: FFH
	OUT CONT1%, &H5F      'LOAD ALL CNTRS TO CLEAR TC (TERMINAL COUNT) STATES:5F
	OUT CONT1%, &H15      'SET DATA POINTER TO COUNTER 5: 15
	
	OUT CONT2%, &HFF      'CHIP 2 MASTER RESET CODE: FFH
	OUT CONT2%, &H5F      'LOAD ALL COUNTERS TO CLEAR TC STATES:5F
	OUT CONT2%, &H15      'SET DATA POINTER TO COUNTER 5: 15

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

	IF ADCHANNEL% = 5 THEN          'RESERVED FOR BARITRON
		CALL PRESSUB(233.333, ADVOLTS#)
		'((2K+1.5K)/1.5)*1000/10
		PRESSURE#(3) = PRES#
	END IF

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
'Calculate time elapsed since the loop timer started till this procedure is called.
'This procedure is called at the end of all calculations, disk and screen writes to
'estimate if the CPU is not keeping up with the demand.
SUB FINETIMER

	OUT CONT1%, 161 '161:SAVE CNTR 1 CHIP 1 IN HOLD
	OUT CONT1%, HOLD1% 'HOLD INTO DATA REGISTERS
	CNTSLOW& = INP(DATA1%) 'READ DATA REGISTERS
	CNTSHI& = INP(DATA1%)
	SPENTCOUNTS& = DWNCNTS& - (CNTSLOW& + CNTSHI& * 256) 'Used counts are the difference between DWNCNTS& and the snapshot we just took.
	PCTUSED! = SPENTCOUNTS& / DWNCNTS& * 100!
	IF PCTUSED! > MAXPCTUSED! THEN MAXPCTUSED! = PCTUSED!

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
'This procedure initializes operation of counters. Five are used. #1 serves as 0.01% accurate loop timer to control pulse counting coming into
'Source2 and Source4 on the QMM board from V-F converters on the detector boards Chan.A and Chan.B. #2 and #3 are linked into a 32 bit
'V-F counter for Ch. A, #4 and #5 are another 32 bit counter for Ch. B. #2 and #4 are hardware gated on Gate4 and Gate5 respectively by Out1.
SUB INITCOUNTERS                 'TO INITIALIZE ALL COUNTERS

CHIP% = 1
CNTR% = 1       'CNTR1 controls loop time. It will count down from DWNCNTS at FREQ set in next line.
MREG$ = "0000 1100 0000 0010"   'From LSB to MSB: Toggle output on TC; Cnt Down; Binary; Once; Use Load reg; Special Gate Off; F2=1/10 source; Rising edge; No gating
LREG$ = I2B$(DWNCNTS&)                  'Load register set to DWNCNTS
CALL SETMODE(CHIP%, CNTR%, MREG$, LREG$)

'Configure counters #2 and #3 in 32-bit mode on Chip1 for counting ChanA V-F pulses.
CNTR% = 2                         'LOWER BITS FOR CHANNEL A
MREG$ = "1001 0010 0010 1000"     '9228: Output Off; Count Up; Binary; Repeat; Load; Special Gate Off; Source2; Falling edge; Active High Level GateN
LREG$ = "0000 0000 0000 0000"     '0000: Load register is set to zero.
CALL SETMODE(CHIP%, CNTR%, MREG$, LREG$)

CNTR% = 3                          'UPPER BITS FOR CHANNEL A
MREG$ = "0001 0000 0010 1000"      '1028: Output Off; Count Up; Binary; Repeat; Load; Special Gate Off; Previous counter TC; Falling edge; No gating
LREG$ = "0000 0000 0000 0000"      '0000: Load register is set to zero.
CALL SETMODE(CHIP%, CNTR%, MREG$, LREG$)

'Counters #4, #5 on Chip1 and #1, #4, #5 on Chip 2 are not used. Leave these at default settings.
'Configure counters #2 and #3 in 32-bit mode on Chip2 for counting ChanB V-F pulses.
CHIP% = 2
CNTR% = 2                          'LOWER BITS CHANNEL B
MREG$ = "1001 0010 0010 1000"      '9228: Output Off; Count Up; Binary; Repeat; Load; Special Gate Off; Source2; Falling edge; Active High Level GateN
LREG$ = "0000 0000 0000 0000"      '0000: Load register is set to zero.
CALL SETMODE(CHIP%, CNTR%, MREG$, LREG$)

CNTR% = 3                          'UPPER BITS CHANNEL B
MREG$ = "0001 0000 0010 1000"      '1028: Output Off; Count Up; Binary; Repeat; Load; Special Gate Off; Previous counter TC; Falling edge; No gating
LREG$ = "0000 0000 0000 0000"      '0000: Load register is set to zero.
CALL SETMODE(CHIP%, CNTR%, MREG$, LREG$)

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
    PRINT "Vent          PS Plate         T-Contr          PC-104"
    PRINT
    PRINT
    PRINT "Voltages"
    PRINT "5V REF     5V PS     5V PC     +12V     +24V     -24V     +28V     28V I"
    PRINT
    PRINT
    PRINT "Pressures"
    PRINT "Delta P        Ambient P        Baratron: P    +15V    -15V"
    PRINT
    PRINT
    PRINT "Valve position      Expected position      Skips      Stops"
    PRINT
    PRINT
    PRINT "CPU utilization:    %"
    PRINT "Peak CPU utilization:    %"
    PRINT "Files: ", DATAOUT$, ENGOUT$

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

'Initialize the data file with column headings.
OPEN ENGOUT$ FOR APPEND AS #1
	PRINT #1, "COMP_OP1     TMPL    TMPD    TMPC    TMPF    TMPPS   TMPB   TMPPC   ";
	PRINT #1, "V5REF   V5PS    V28M   V24D    V_24D    V15B   V_15B    V12L   V5PC   ";
	PRINT #1, "I28V     VQ      VEQ     VSQ     VST     PCT"
CLOSE #1

'Initialize engineering data file with column headings.
OPEN DATAOUT$ FOR APPEND AS #1
	PRINT #1, "COMP_OP1   CYCLE_No   CNT_A   CNT_B   DELP    PABS    PBAR    AIN     BIN     AOUT    BOUT"
CLOSE #1

END SUB

'******************************************************************
SUB PARAMETERS                  'SETS VARIOUS PARAMETERS

CNTRBASEADDR% = &H380               'SET COUNTER BASEADDRESS
DIFFBASEADDR% = &H280               'SET DIFF A/D  BASEADDRESS ON PROMETHEUS
SINGBASEADDR% = &H340               'SET SING A/D  BASEADDRESS ON PROMETHEUS
DWNCNTS& = 40000                    'This is initial value for CNTR1 to time the main loop from.

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
OUT DATA1%, &HD2            'SET MASTERMODE UPPER BITS CHIP 1 TO D2
	'D: BCD DIV FOR Fout, INC DISAB, 8 BIT BUS, Fout OFF
	'2: Fout DIV BY 2 (TO DET BOARD?) FOUT is not in use, disable it.

OUT CONT2%, MASTERMODE%     'POINT TO MASTERMODE REGISTER CHIP 2
OUT DATA2%, &HB0            'SET MASTERMODE LOWER BITS CHIP 2 B0
OUT DATA2%, &HD2            'SET MASTERMODE UPPER BITS CHIP 2 to D2

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
IF CYCLENO& MOD 20 * ScaleFactor% = 0 THEN
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

EXPECTQUADRANT% = (1 + (CYCLENO& - 5 * ScaleFactor%) / (10 * ScaleFactor%)) MOD 4
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
'Write engineering data to disk.
SUB WRITETODISK
	'This is the engineering data.
'        OPEN ENGOUT$ FOR APPEND AS #1
      PRINT #2, USING "######.###"; ENDTIME#;
      PRINT #2, USING "####.##"; TEMPERATURE#(5); TEMPERATURE#(6); TEMPERATURE#(7); TEMPERATURE#(8); TEMPERATURE#(9); TEMPERATURE#(10); TEMPERATURE#(11);
      PRINT #2, USING "####.##"; VOLTAGE#(1); VOLTAGE#(2); VOLTAGE#(3); VOLTAGE#(4); VOLTAGE#(5); VOLTAGE#(6); VOLTAGE#(7); VOLTAGE#(8); VOLTAGE#(9);
      PRINT #2, USING "#####"; CURRENT#(1);
      PRINT #2, USING "#######"; VALVEQUADRANT%; EXPECTQUADRANT%; VALVESKIPPED%; VALVESTOPPED%;
      PRINT #2, USING "###.#"; PCTUSED!
'        CLOSE #1
	
	'This is the data file.
'        OPEN DATAOUT$ FOR APPEND AS #1
		PRINT #1, USING "######.###"; ENDTIME#;
		PRINT #1, USING "########"; CYCLENO&; FREQA&; FREQB&;
		PRINT #1, USING "#####.###"; PRESSURE#(1); PRESSURE#(2); PRESSURE#(3); TEMPERATURE#(1); TEMPERATURE#(2); TEMPERATURE#(3); TEMPERATURE#(4)
'        CLOSE #1
END SUB

'******************************************************************
SUB WRITETOSCREEN

VertOffset% = 0
HorizOffset% = 0
IF ValveEnabled% THEN VColor = Green ELSE VColor = RED 'Give visual cue that valve is disabled.
LoopNumber% = LoopNumber% MOD 6 'LoopNumber will increment and different lines will print on different calls.
	PrintNumLabel CDBL(CYCLENO&), "########", VertOffset% + 4, HorizOffset% + 53, VColor 'Cycle counter
SELECT CASE LoopNumber%
CASE 0
	PrintNumLabel CDBL(HH), "##", VertOffset% + 2, HorizOffset% + 12, Yellow'RT stamp
	PrintNumLabel CDBL(MM), "##", VertOffset% + 2, HorizOffset% + 15, Yellow
	PrintNumLabel CDBL(SS + MS / 1000!), "##.###", VertOffset% + 2, HorizOffset% + 18, Yellow
	PrintNumLabel CDBL(FREQA&), "#######", VertOffset% + 3, HorizOffset% + 12, Green'Freq A
	PrintNumLabel CDBL(DELTAFREQA&), "#######", VertOffset% + 3, HorizOffset% + 34, Green'Delta A
	PrintNumLabel (ENDTIME# - PREVTIME#), "######.###", VertOffset% + 3, HorizOffset% + 53, Green'Increm time stamp
	PrintNumLabel CDBL(FREQB&), "#######", VertOffset% + 4, HorizOffset% + 12, Green 'Freq B
CASE 1
	PrintNumLabel CDBL(DELTAFREQB&), "#######", VertOffset% + 4, HorizOffset% + 34, Green 'Delta B
	PrintNumLabel FREQRATIO#, "###.######", VertOffset% + 5, HorizOffset% + 12, Green' Ratio A/B
	PrintNumLabel (PREVRATIO# - FREQRATIO#), "###.######", VertOffset% + 5, HorizOffset% + 34, Green' Delta ratio
	PrintNumArr TEMPERATURE#(), "##.##", 1, VertOffset% + 9, HorizOffset% + 1, Green
	PrintNumArr TEMPERATURE#(), "##.##", 2, VertOffset% + 9, HorizOffset% + 16, Green
	PrintNumArr TEMPERATURE#(), "##.##", 3, VertOffset% + 9, HorizOffset% + 32, Green
	PrintNumArr TEMPERATURE#(), "##.##", 4, VertOffset% + 9, HorizOffset% + 47, Green
CASE 2
	PrintNumArr TEMPERATURE#(), "##.##", 5, VertOffset% + 11, HorizOffset% + 1, Green
	PrintNumArr TEMPERATURE#(), "##.##", 6, VertOffset% + 11, HorizOffset% + 16, Green
	PrintNumArr TEMPERATURE#(), "##.##", 7, VertOffset% + 11, HorizOffset% + 32, Green
	PrintNumArr TEMPERATURE#(), "##.##", 8, VertOffset% + 13, HorizOffset% + 1, Green
	PrintNumArr TEMPERATURE#(), "##.##", 9, VertOffset% + 13, HorizOffset% + 16, Green
	PrintNumArr TEMPERATURE#(), "##.##", 10, VertOffset% + 13, HorizOffset% + 32, Green
	PrintNumArr TEMPERATURE#(), "##.##", 11, VertOffset% + 13, HorizOffset% + 47, Green
CASE 3
	PrintNumArr VOLTAGE#(), "##.##", 1, VertOffset% + 17, HorizOffset% + 1, Green
	PrintNumArr VOLTAGE#(), "##.##", 2, VertOffset% + 17, HorizOffset% + 11, Green
	PrintNumArr VOLTAGE#(), "##.##", 9, VertOffset% + 17, HorizOffset% + 21, Green
	PrintNumArr VOLTAGE#(), "###.##", 8, VertOffset% + 17, HorizOffset% + 30, Green
	PrintNumArr VOLTAGE#(), "###.##", 4, VertOffset% + 17, HorizOffset% + 40, Green
	PrintNumArr VOLTAGE#(), "###.##", 5, VertOffset% + 17, HorizOffset% + 49, Green
	PrintNumArr VOLTAGE#(), "###.##", 3, VertOffset% + 17, HorizOffset% + 58, Green
CASE 4
	PrintNumArr CURRENT#(), "######", 1, VertOffset% + 17, HorizOffset% + 67, Green
	PrintNumArr PRESSURE#(), "###.###", 1, VertOffset% + 21, HorizOffset% + 1, Green
	PrintNumArr PRESSURE#(), "####.##", 2, VertOffset% + 21, HorizOffset% + 18, Green
	PrintNumArr PRESSURE#(), "####.##", 3, VertOffset% + 21, HorizOffset% + 30, Green
	PrintNumArr VOLTAGE#(), "###.##", 6, VertOffset% + 21, HorizOffset% + 45, Green 'Baratron voltage
	PrintNumArr VOLTAGE#(), "###.##", 7, VertOffset% + 21, HorizOffset% + 53, Green 'Baratron voltage
CASE ELSE
	PrintNumLabel CDBL(VALVEQUADRANT%), "###", VertOffset% + 24, HorizOffset% + 0, VColor
	PrintNumLabel CDBL(EXPECTQUADRANT%), "###", VertOffset% + 24, HorizOffset% + 20, VColor
	PrintNumLabel CDBL(VALVESKIPPED%), "####", VertOffset% + 24, HorizOffset% + 43, Green
	PrintNumLabel CDBL(VALVESTOPPED%), "####", VertOffset% + 24, HorizOffset% + 54, Green
	PrintNumLabel CDBL(PCTUSED!), "###", VertOffset% + 26, HorizOffset% + 17, Green
	PrintNumLabel CDBL(MAXPCTUSED!), "###", VertOffset% + 27, HorizOffset% + 22, Green
END SELECT
LoopNumber% = LoopNumber% + 1
END SUB

'******************************************************************
SUB WRITETOSERIAL
 
       PRINT #2, DATE$, TIME$, CYCLENO&
      
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
       PRINT #2, USING "#######"; VALVEQUADRANT%; EXPECTQUADRANT%; VALVESKIPPED%; VALVESTOPPED%

END SUB

'******************************************************************
SUB IRIGTIME

DIM timebyte(1 TO 16)

XBASE% = 768            '0x300
UN100PORT% = 15         '0x0f

	N% = 0

	DO
	  timebyte(N% + 1) = INP(XBASE% + UN100PORT% - N%)
	  N% = N% + 1
	LOOP UNTIL N% = 9

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

'******************************************************************
