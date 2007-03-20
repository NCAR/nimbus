/*
-------------------------------------------------------------------------
OBJECT NAME:	globals.c

FULL NAME:	Global Variable Definitions

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include "define.h"

char	buffer[4096];		/* Generic, volatile string space	*/
char	*ProjectNumber, *ProjectName, *TimeInterval;
char	*Aircraft, *FlightNumber, *FlightDate;

VARTBL  *Variable[MAX_VARIABLES];

int	nVariables, AverageRate, mvThreshold;
bool	PauseFlag, Interactive;
int	InputFile, OutputFile, PauseWhatToDo;

/* END GLOBALS.C */
