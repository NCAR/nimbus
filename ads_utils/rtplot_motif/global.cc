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
	FlashMode;

char	buffer[BUFFSIZE], DataPath[1024], *ProjectName, FlightNumber[32];

SDITBL	*sdi[MAX_SDI];			/* SDI (aka raw) variable list	*/
RAWTBL	*raw[MAX_RAW];			/* Alphabeticly sorted pointers	*/


int	nsdi, nraw, nVariables;
VARTBL	Variable[MAXDATASETS];


/* Vars related to data sets	*/
int	NumberDataSets, NumberSeconds, NumberElements[MAXDATASETS],
	CurrentDataSet, Aircraft, Units;

long	nFloats;

/* Data record pointers
 */
char	*ADSrecord, *AVAPSrecord[4];
NR_TYPE	*bits, *volts;
NR_TYPE	*SampledData, *AveragedData;
NR_TYPE	*plotData[MAXDATASETS];

float	HDRversion;

int	FlightDate[3];	// Dummy.  Here to shut linker up.

/* Parameter File Variables	*/
string	title, subtitle, xlabel, ylabel;

/* Scaling & tic mark stuff	*/
NR_TYPE	ymin, ymax, smallest_y, biggest_y;
int	numtics;

/* Error messages	*/
char	*o_mem = "Out of Memory.";

/* X vars	*/
Widget	varList;

instanceRec	iv;

/* END GLOBAL.CC */
