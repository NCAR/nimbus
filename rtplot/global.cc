/*
-------------------------------------------------------------------------
OBJECT NAME:	global.c

FULL NAME:	Global Variable Definitions

DESCRIPTION:	extern.h should look just like this.
-------------------------------------------------------------------------
*/

#include "define.h"

bool	Grid,				/* Overlay grid on graph?	*/
	Frozen,
	ProductionRun,
	FlashMode;

char	buffer[BUFFSIZE], DataPath[1024], *ProjectName, FlightNumber[32],
	OutputFileName[256];

std::vector<SDITBL *> sdi;	/* SDI (aka raw) variable list	*/
std::vector<RAWTBL *> raw;	/* Alphabeticly sorted pointers	*/

int	nVariables;
VARTBL	Variable[MAXDATASETS];


/* Vars related to data sets	*/
int	NumberDataSets, NumberSeconds, NumberElements[MAXDATASETS],
	CurrentDataSet, Aircraft;

long	nFloats;

/* Data record pointers
 */
char	*ADSrecord, *AVAPSrecord[4];
NR_TYPE	*bits, *volts;
NR_TYPE	*SampledData, *AveragedData;
double	*plotData[MAXDATASETS], *xData[MAXDATASETS];

float	HDRversion;

int	FlightDate[3];	// Dummy.  Here to shut linker up.

std::string	title;

/* END GLOBAL.CC */
