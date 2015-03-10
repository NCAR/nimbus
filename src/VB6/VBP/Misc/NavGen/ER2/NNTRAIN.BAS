DECLARE FUNCTION BTEMP! (ELANGLE!)
'TRAINING FILE GENERATOR
'FOR PDNNET
'930210 RFD
'
'INIT SECTION
'ATMOSPHERIC LAYERS=5
'ELEVATION ANGLES=5
DIM SHARED PROFILE, AL, EL
 AL = 4: NPROFS = AL: EL = 4' 0 TO N =N+1 POINTS
DIM SHARED T(NPROFS, AL), TB(NPROFS, EL), ELANGLE(EL), TBC(EL)
GOSUB SETNOMINALS
FOR I = 0 TO AL: ELANGLE(I) = (I * 15) + ((I ^ 2 / 16) * 15): NEXT
'

PRINT "MONITOR WHICH PROFILE (0 - "; AL; ")"; : INPUT MP
PRINT : PRINT

L0:
        PRINT USING "PROFILE #  PATTERN ###                          0   1   2   3   4"; MP; PAT
        PRINT "                     LAST PHYSICAL T'S = ";
        FOR I = 0 TO AL
                PRINT USING " .##"; T(PROFILE, I);
        NEXT
        PRINT

        PRINT "               LAST   TB(PROFILE,EL)'S = ";
        FOR I = 0 TO AL
                TB(PROFILE, I) = BTEMP(ELANGLE(I))
                PRINT USING " .##"; TB(PROFILE, I);
        NEXT

GOSUB SETNOMINALS
        PRINT
        PRINT "           STARTING   TB(PROFILE,EL)'S = ";
        FOR I = 0 TO AL
                TB(PROFILE, I) = BTEMP(ELANGLE(I))
                PRINT USING " .##"; TB(PROFILE, I);
        NEXT
        PRINT

PRINT "INPUT LAYER (0 -"; AL; ")"; ", TEMP(0 - 1)    (-LAYER TO END)": INPUT L, T
T(PROFILE, L) = T
PAT = PAT + 1
GOTO L0
END



SETNOMINALS:
PROFILE = 0
                FOR I = 0 TO AL
                T(PROFILE, I) = .5 - (.05 * I)'LINEAR LAPSE ABOVE A/C
                NEXT

PROFILE = 1
                FOR I = 0 TO AL
                T(PROFILE, I) = .5 - (.025 * I)'LINEAR LAPSE ABOVE A/C
                NEXT

PROFILE = 2
                FOR I = 0 TO AL
                T(PROFILE, I) = .5 ' ISOTHERMAL ABOVE A/C
                NEXT
PROFILE = 3
                FOR I = 0 TO AL
                T(PROFILE, I) = .5 + (.025 * I)'LINEAR INCREASE ABOVE A/C
                NEXT

PROFILE = 4
                FOR I = 0 TO AL
                T(PROFILE, I) = .5 + (.05 * I)'LINEAR INCREASE ABOVE A/C
                NEXT



RETURN

FUNCTION BTEMP (ELANGLE)
ELRADIANS = ELANGLE / 57.3
NLV = INT(SIN(ELRADIANS) * 4 + .5)
FRAC = NLV / AL
TEMP = 0

IF NLV = 0 THEN TEMP = T(PROFILE, 0): GOTO BT
FOR I = 0 TO NLV
        TEMP = (TEMP + T(PROFILE, I))
NEXT I
        TEMP = TEMP / (NLV + 1)

BT:
BTEMP = TEMP
END FUNCTION

