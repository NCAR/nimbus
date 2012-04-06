CFLAGS = -g 
COFLAGS =  -g -c
FOFLAGS =  -g -c
CLIB = -lc -lm 
FLIB =  -lm
WLIB = -L$(HOME)/subs -lwac

LIBORDER = ${FLIB} ${CLIB} ${WLIB}

ROUTINES = SpecificRange
 
for:	${ROUTINES}

AVERAGE.O =  SpecificRange.o

SpecificRange: $(AVERAGE.O) 
	ncargf77 $(CFLAGS) $(AVERAGE.O) $(LIBORDER) -o SpecificRange

SpecificRange.o: SpecificRange.c
	cc -DDEBUG=FALSE $(COFLAGS) SpecificRange.c -o SpecificRange.o

