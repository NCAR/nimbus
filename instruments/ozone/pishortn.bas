DECLARE SUB VALVESLIP ()
DECLARE SUB WRITETODISK ()
DECLARE SUB VALVEPOSITION ()
DECLARE SUB VALVECHG ()
DECLARE SUB FINETIMER ()
DECLARE FUNCTION B2I& (BN$)    'CONVERTS FORMATTED BINARY BYTES TO A LONG INTEGER
DECLARE FUNCTION I2B$ (DEC&)   'CONVERTS LONG INTEGER TO 4 FORMATTED BINARY BYTES
DECLARE SUB BKGCOLOR ()
DECLARE SUB CNTRESET ()
DECLARE SUB DATAPOINTERS ()
DECLARE SUB INITCOUNTERS ()
DECLARE SUB IOMAP ()
DECLARE SUB PARAMETERS ()
DECLARE SUB SETMASTER ()
DECLARE SUB SETMODE (CNTR%, MREG$, LREG$)
DECLARE SUB WRITETOSCREEN ()
DECLARE SUB WRITETODISK ()
DECLARE SUB VOLTSUB (R1!, R2!, ADVOLTS#)
DECLARE SUB TEMPSUB (R1!, R2!, ADVOLTS#)
DECLARE SUB PRESSUB (MBARCAL!, ADVOLTS#)
DECLARE SUB CURRSUB (RES!, ADVOLTS#)
DECLARE SUB SINGLEAD ()
DECLARE SUB DIFFERAD ()

COMMON SHARED CNTRBASEADDR%, DIFFBASEADDR%, SINGBASEADDR%
COMMON SHARED LINT&, BN$, DEC&
COMMON SHARED VALVEQUADRANT%, EXPECTQUADRANT%, PREVALVEQUADRANT%, VALVECODE%
COMMON SHARED VALVESKIPPED%, VALVESTOPPED%
COMMON SHARED XTALFREQ&, CNTINTERVAL&, DWNCNTS&, XTALDIVIDE&
COMMON SHARED CNTR%, MODE%, MREG&, LOAD%, LREG&
COMMON SHARED DATA1%, CONT1%, DIG1%
COMMON SHARED INTERRUPT1%, INTERRUPT2%, STATUS%, MASTERMODE%
COMMON SHARED MODE1%, LOAD1%, HOLD1%, HOLDCY1%
COMMON SHARED MODE2%, LOAD2%, HOLD2%, HOLDCY2%
COMMON SHARED MODE3%, LOAD3%, HOLD3%, HOLDCY3%
COMMON SHARED MODE4%, LOAD4%, HOLD4%, HOLDCY4%
COMMON SHARED MODE5%, LOAD5%, HOLD5%, HOLDCY5%
COMMON SHARED ROW%, COLUMN%, VALUE!, ADTEST&'COMMON SHARED FILENAME$, DATETIME$
COMMON SHARED TEMP#, RT#, ADVOLTS#, PRES#, ENGUNITS#, VOLTS#, MILLIAMP#
COMMON SHARED R1!, R2!, MBARCAL!
COMMON SHARED FREQA&, FREQB&, CNTSREF&, DELTAFREQA&, DELTAFREQB&, FREQRATIO#, PREVRATIO#
COMMON SHARED MAXPCT%, PCTTIME%, PCTEXPIRED%
COMMON SHARED MICSECS#, STARTTIME#, ENDTIME#, FIRSTIME#, MILSEC#, CYCLENO#, NOW#

DIM SHARED TEMPERATURE#(1 TO 16)   'EXTRAS INCLUDED
DIM SHARED PRESSURE#(1 TO 6)       'EXTRAS INCLUDED
DIM SHARED VOLTAGE#(1 TO 12)       'EXTRAS INCLUDED
DIM SHARED CURRENT#(1 TO 6)        'EXTRAS INCLUDED
DIM SHARED MISSINGTIME#(1 TO 100)  'FOR MISSED DATA TIME
DIM SHARED MISSINGCYCLE#(1 TO 100) 'FOR MISSED CYCLE NUMBER

'******************************************************************

CALL BKGCOLOR
CALL PARAMETERS     'CONTAINS THE EXPERIMENT PARAMETERS
CALL IOMAP          'DEFINE I/O MAP
CALL DATAPOINTERS   'DEFINE DATA POINTERS FOR COUNTER BOARD
CALL CNTRESET       'RESET COUNTER BOARD
CALL SETMASTER      'SET MASTER MODE FOR BOTH COUNTER CHIPS
CALL INITCOUNTERS   'INITIALIZE COUNTER MODE REGISTERS

CYCLENO# = 0
PREVALVEQUADRANT% = 1
VALVESTOPPED% = 0
VALVESKIPPED% = 0
CALL VALVEPOSITION

LA$ = "0101 1111"
OUT CONT1%, B2I&(LA$)                'LOAD COUNTERS(5F) 1,2,3,4,5
                                        'WITH DATA IN LOAD REGISTERS
LA$ = "0011 1111"
OUT CONT1%, B2I&(LA$)                 'ARM COUNTERS 1,2,3,4,5

'THE FOLLOWING IS CODE TO MAKE ON LONG HKD AND RAW FILES
'DATETIME$ = DATE$ + TIME$
'MONTH$ = MID$(DATETIME$, 1, 2)
'DAY$ = MID$(DATETIME$, 4, 2)
'YEAR$ = MID$(DATETIME$, 9, 2)
'HOUR$ = MID$(DATETIME$, 11, 2)

'FILENAME$ = YEAR$ + MONTH$ + DAY$ + HOUR$

'OPEN "C:\DATA\" + FILENAME$ + ".HKD" FOR APPEND AS #1   'TO WRITE COLUMN HEADINGS
'PRINT #1, "COMP_T       TMPL    TMPD    TMPC    TMPF    TMPPS   TMPB   TMPPC   ";
'PRINT #1, "V5REF   V5PS    V28M   V24D    V_24D    V15B   V_15B    V12L   V5PC   ";
'PRINT #1, "I28V     VQ      VEQ     VSQ     VST     PCT"
'CLOSE #1


'OPEN "C:\DATA\" + FILENAME$ + ".RAW" FOR APPEND AS #2   'TO WRITE COLUMN HEADINGS
'PRINT #2, " CYCLE_No      CNT_A   CNT_B   DELP    PABS    PBAR    AIN     BIN     AOUT    BOUT"
'CLOSE #2

   STARTTIME# = TIMER
 
     CLOSED% = 1
     CYCLENO# = -1
     MAXPCT% = 0
     PCTEXPIRED% = 0
  
'........................TIME LOOP STARTS HERE!

DO
     CYCLENO# = CYCLENO# + 1

     STATUS% = 2                            'SET STATUS OF CNTR 1
     DO UNTIL STATUS% = 0
           STATUS% = INP(CONT1%) AND 2      'CHECK STATUS OF CNTR 1
     LOOP
                                           'START 1 SEC HERE: CNTR=4000

    OUT CONT1%, 190                        '190:SAVE CNTR 2,3,4,5
    OUT CONT1%, 94                         '94:LOAD CNTR 2,3,4,5
                                           
    CLS                                    'CLEAR SCREEN EACH SECOND
 
        IF CYCLENO# MOD 10 = 0 THEN
                 CALL VALVECHG
                 CALL VALVESLIP
        END IF

    ENDTIME# = TIMER
                                         'NOW# = ENDTIME#
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

     OUT CONT1%, HOLD4%
     CNTSLOW& = INP(DATA1%)
     CNTSHI& = INP(DATA1%)
     CNTS2B& = CNTSLOW& + CNTSHI& * 256
     OUT CONT1%, HOLD5%
     CNTSLOW& = INP(DATA1%)
     CNTSHI& = INP(DATA1%)
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
   CALL WRITETOSCREEN

' THE FOLLOWING "IF" AND "OPEN" STATEMENTS ARE USED TO MAKE 1 HOUR RAW AND HKD FILES.
                                'SEE ABOVE FOR LONG FORMAT
      'CODE FOR SHORT FILES ENDS HERE:COMMENT OUT STARTS HERE FOR LONG FORMAT
IF CYCLENO# MOD 3600 = 0 THEN
DATETIME$ = DATE$ + TIME$
MONTH$ = MID$(DATETIME$, 1, 2)
DAY$ = MID$(DATETIME$, 4, 2)
YEAR$ = MID$(DATETIME$, 9, 2)
HOUR$ = MID$(DATETIME$, 11, 2)

FILENAME$ = YEAR$ + MONTH$ + DAY$ + HOUR$

OPEN "C:\DATA\" + FILENAME$ + ".HKD" FOR APPEND AS #1   'TO WRITE COLUMN HEADINGS
PRINT #1, "COMP_T       TMPL    TMPD    TMPC    TMPF    TMPPS   TMPB   TMPPC   ";
PRINT #1, "V5REF   V5PS    V28M   V24D    V_24D    V15B   V_15B    V12L   V5PC   ";
PRINT #1, "I28V     VQ      VEQ     VSQ     VST     PCT"
CLOSE #1

OPEN "C:\DATA\" + FILENAME$ + ".RAW" FOR APPEND AS #2   'TO WRITE COLUMN HEADINGS
PRINT #2, " CYCLE_No      CNT_A   CNT_B   DELP    PABS    PBAR    AIN     BIN     AOUT    BOUT"
CLOSE #2

END IF          'CODE FOR SHORT FILES ENDS HERE:COMMENT OUT FOR LONG FORMAT
  
   OPEN "C:\DATA\" + FILENAME$ + ".HKD" FOR APPEND AS #1
   OPEN "C:\DATA\" + FILENAME$ + ".RAW" FOR APPEND AS #2

   CALL WRITETODISK                      'WRITE TO DISK EVERY SECOND
                 
   CLOSE #1                              'CLOSE FILES EVERY SECOND
   CLOSE #2
        
   PRINT "DATA WRITTEN TO", FILENAME$, ".RAW AND .HKD"

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
SUB BKGCOLOR        'SETS SCREEN BACKGROUND COLOR

     SCREEN 12
     BLUE% = 30: GREEN% = 0: RED% = 0
     VALUE = BLUE% * 65536 + GREEN% * 256 + RED%
     PALETTE 0, VALUE
     VM% = 479
     COLOR 14
     WIDTH , 30

END SUB

'******************************************************************
SUB CNTRESET               'INITIALIZE/RESET COUNTER BOARD AT STARTUP

     OUT DIG1%, 0          'ZERO TTL REGISTERS DIG1 & DIG2
   
                 'INITIALIZE COUNTERS

     OUT CONT1%, &HFF      'CHIP 1 MASTER RESET CODE: FFH
     OUT CONT1%, &H5F      'LOAD ALL CNTRS TO CLEAR TC (TERMINAL COUNT)
                                'STATES:5F
     OUT CONT1%, &H15      'SET DATA POINTER TO COUNTER 5: 15
                                   
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

                'PRINT "ALL DATA POINTERS HAVE BEEN DEFINED"

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
          CALL PRESSUB(300, ADVOLTS#)
          '((2K+1K)/1)*1000/10 = 300
          PRESSURE#(3) = PRES#
     END IF

     IF ADCHANNEL% = 6 THEN
          CALL PRESSUB(257.83, ADVOLTS#)
          PRESSURE#(2) = PRES# - 54.226 'ATMOSPHERIC PRESSURE GAUGE
     END IF

     IF ADCHANNEL% = 7 THEN
         CALL CURRSUB(.2, ADVOLTS#)
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
     CNTSFIRST& = DWNCNTS& - (CNTSLOW& + CNTSHI& * 256)
     MICSECS# = CNTSFIRST& * XTALDIVIDE& / XTALFREQ&
     MICSECS# = MICSECS# * 1000         'EXTRA STEP TO AVOID INTEGER OVERFLOW
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

CNTR% = 1                         'CONTROLS COUNTING INTERVAL (1 SEC NOM)
                                  'REPETITIVELY COUNTS DOWN FROM "HOLD"
MREG$ = "0000 1110 1010 0101"     '0DA5 NO GATE,F1/100,SP,RPT,DN,ACT LO TC
                                'CHANGED TO 0EA5 (F1/1000) TO FIX DBL COUNTS
           'THIS MEANS THE COUNTERS WILL MEASURE FOR 3999/4000=0.99975 SEC

LREG$ = I2B$(DWNCNTS&)            'SET COUNTER GATE INTERVAL
CALL SETMODE(CNTR%, MREG$, LREG$)

                    'SET COUNTERS 2 & 3 CHIP 1 FOR 32 BIT FREQ MEASUREMENTS
CNTR% = 2                         'LOWER BITS FOR CHANNEL A
MREG$ = "1001 0010 0010 1000"     '9228
LREG$ = "0000 0000 0000 0000"     '0000: PRESET TO ZERO
CALL SETMODE(CNTR%, MREG$, LREG$)

CNTR% = 3                          'UPPER BITS FOR CHANNEL A
MREG$ = "0001 0000 0010 1000"      '1028
LREG$ = "0000 0000 0000 0000"      '0000: PRESET TO ZERO
CALL SETMODE(CNTR%, MREG$, LREG$)
                                 
                                  'SET COUNTERS 4 & 5 CHIP 1 FOR 32 BIT FREQ MEASUREMENTS
CNTR% = 4                         'LOWER BITS FOR CHANNEL B
MREG$ = "1001 0100 0010 1000"     '9228
LREG$ = "0000 0000 0000 0000"     '0000: PRESET TO ZERO
CALL SETMODE(CNTR%, MREG$, LREG$)

CNTR% = 5                          'UPPER BITS FOR CHANNEL B
MREG$ = "0001 0000 0010 1000"      '1028
LREG$ = "0000 0000 0000 0000"      '0000: PRESET TO ZERO
CALL SETMODE(CNTR%, MREG$, LREG$)

END SUB

'******************************************************************
SUB IOMAP               'SETS I/O BASE PORT ADDRESSES FOR COUNTER BOARD
DATA1% = CNTRBASEADDR% + 0             'CHIP 1 DATA I/O
CONT1% = CNTRBASEADDR% + 1             'CHIP 1 STATUS (I) CONTROL (O)
DIG1% = CNTRBASEADDR% + 2              'CHIP 1 TTL I/O
INTERRUPT1% = CNTRBASEADDR% + 6        'CHIP 1 INTERRUPT NOT USED

END SUB

'******************************************************************
SUB PARAMETERS                  'SETS VARIOUS PARAMETERS

CLS
CNTRBASEADDR% = &H380               'SET COUNTER BASEADDRESS
DIFFBASEADDR% = &H280               'SET DIFF A/D  BASEADDRESS ON PROMETHEUS
SINGBASEADDR% = &H340               'SET SING A/D  BASEADDRESS ON PROMETHEUS

XTALFREQ& = 4000                    'OSC FREQ IN KHZ
XTALDIVIDE& = 1000                  'DIVIDER OF XTAL OSCILLATOR
                                    'CHANGED FROM 100 TO FIX DOUBLE COUNTS
CNTINTERVAL& = 10                   'SET COUNTER INTERVAL IN TENTHS OF SECOND
DWNCNTS& = CNTINTERVAL& / 10 * XTALFREQ& * 1000 / XTALDIVIDE&
                                 'SET DOWN CNTS FOR GATE COUNTER
DEC& = DWNCNTS&                  'DWNCNTS&=40K WORKS; 65535 LIMIT
                                 'CHANGED TO DWNCNTS&=4K TO FIX DBL COUNTS
END SUB

'******************************************************************
SUB PRESSUB (MBARCAL!, ADVOLTS#)
                    'CALC PRESSURE FOR PRESSURE SENSOR
                    'MBARCAL IS NUMBER OF MILLIBARS/VOLT ON SENSOR
                    'ADVOLTS IS VOLTAGE MEASURED BY A/D
     PRES# = ADVOLTS# * MBARCAL!

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

END SUB

'******************************************************************
SUB SETMODE (CNTR%, MREG$, LREG$)    'FOR EACH COUNTER

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
     CHIP% = 1
     CONT% = CONT1%
     DAT% = DATA1%

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
          CALL VOLTSUB(20, 10, ADVOLTS#)
           VOLTAGE#(ADCHANNEL% - 6) = VOLTS#
            END IF
                                                 'FOR -15 VOLT POWER SUPPLY
     IF ADCHANNEL% = 13 THEN
                CALL VOLTSUB(20, 10, ADVOLTS#)
           VOLTAGE#(ADCHANNEL% - 6) = VOLTS#
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
      PRINT #2, USING "########"; CYCLENO#; FREQA&; FREQB&;
      PRINT #2, USING "#####.##"; PRESSURE#(1); PRESSURE#(2); PRESSURE#(3); TEMPERATURE#(1); TEMPERATURE#(2); TEMPERATURE#(3); TEMPERATURE#(4)

      PRINT #1, TIMER;
      PRINT #1, USING "####.##"; TEMPERATURE#(5); TEMPERATURE#(6); TEMPERATURE#(7); TEMPERATURE#(8); TEMPERATURE#(9); TEMPERATURE#(10); TEMPERATURE#(11);
      PRINT #1, USING "####.##"; VOLTAGE#(1); VOLTAGE#(2); VOLTAGE#(3); VOLTAGE#(4); VOLTAGE#(5); VOLTAGE#(6); VOLTAGE#(7); VOLTAGE#(8); VOLTAGE#(9);
      PRINT #1, USING "#####"; CURRENT#(1);
       PRINT #1, USING "#######"; VALVEQUADRANT%; EXPECTQUADRANT%; VALVESKIPPED%; VALVESTOPPED%;
       CALL FINETIMER
       PCTEXPIRED% = 100 * MICSECS# / 1000000
      PRINT #1, USING "###.#"; PCTEXPIRED%

END SUB

'******************************************************************
SUB WRITETOSCREEN
      PRINT "COUNTER A", FREQA&, DELTAFREQA&, "TIME =",
      PRINT USING "######.#"; ENDTIME# - STARTTIME#
      PRINT "COUNTER B", FREQB&, DELTAFREQB&, "CYCLE NO. =", CYCLENO#
      PRINT "RATIO A/B ",
      PRINT USING "###.######"; FREQRATIO#; PREVRATIO# - FREQRATIO#
      PRINT "CHAMBER TEMPS         ",
      PRINT USING "#####.##"; TEMPERATURE#(1); TEMPERATURE#(2); TEMPERATURE#(3); TEMPERATURE#(4)
      PRINT "LAMP, DET, CELL",
      PRINT USING "#####.##"; TEMPERATURE#(5); TEMPERATURE#(6); TEMPERATURE#(7)
      PRINT "FAN, PS PANEL, BOX, PROC",
      PRINT USING "#####.##"; TEMPERATURE#(8); TEMPERATURE#(9); TEMPERATURE#(10); TEMPERATURE#(11)
      PRINT "5V REF, 5V PS, 5V PC, +12V",
      PRINT USING "#####.##"; VOLTAGE#(1); VOLTAGE#(2); VOLTAGE#(9); VOLTAGE#(8);
      PRINT "BARATRON +15V,  -15V ",
     
      PRINT USING "#####.##"; VOLTAGE#(6); VOLTAGE#(7)
      PRINT "+24V, -24V, +28V, 28V I",
      PRINT USING "#####.##"; VOLTAGE#(4); VOLTAGE#(5); VOLTAGE#(3); CURRENT#(1)
      PRINT "DEL PRES, PRES, BARA",
      PRINT USING "#####.##"; PRESSURE#(1); PRESSURE#(2); PRESSURE#(3)
      CALL FINETIMER
      PCTTIME% = 100 * MICSECS# / 1000000
      IF PCTEXPIRED% > MAXPCT% THEN MAXPCT% = PCTEXPIRED%
      PRINT "VALQUAD, EXPQUAD, SKIPS, STOPS";
      PRINT USING "######"; VALVEQUADRANT%; EXPECTQUADRANT%; VALVESKIPPED%; VALVESTOPPED%
      PRINT USING "###.#"; PCTTIME%;
      PRINT "%  OF 1 SECOND TIME WAS USED: MAX USED WAS";
      PRINT USING "###.#"; MAXPCT%;
      PRINT "%"

END SUB

