/*
-------------------------------------------------------------------------
OBJECT NAME:	globals.c

FULL NAME:	Global Variable Definitions

ENTRY POINTS:	none

DESCRIPTION:	

INPUT:		none

OUTPUT:		none

REFERENCES:	none

REFERENCED BY:	none

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2005
-------------------------------------------------------------------------
*/

#include "define.h"

char	buffer[4096];		/* Generic, volatile string space	*/
char	*ProjectNumber, *ProjectName, DefaultFormat[32];
char	*Aircraft, *FlightNumber, *FlightDate, *Defaults;

std::vector<VARTBL *> Variable, outputList;

bool	PauseFlag, Interactive, AmesFormat, PrintUnits;
int	InputFile, PauseWhatToDo, XaxisType;
FILE	*OutputFile;
size_t	nRecords;
long	CurrentInputRecordNumber;

/* END GLOBALS.C */
