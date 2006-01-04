/*
-------------------------------------------------------------------------
OBJECT NAME:	global.c

FULL NAME:	Global Variable Definitions

DESCRIPTION:	extern.h should look just like this.
-------------------------------------------------------------------------
*/

#include "define.h"
#include "amlib.h"

const NR_TYPE MISSING_VALUE = -32767;

bool	Grid,				/* Overlay grid on graph?	*/
	Frozen,
	ProductionRun,
	FlashMode;

char	buffer[BUFFSIZE], DataPath[1024], *ProjectName, FlightNumber[32],
	OutputFileName[256];

const NR_TYPE floatNAN = nanf("");

std::vector<RAWTBL *> raw;	/* Alphabeticly sorted pointers	*/

size_t	nVariables;
VARTBL	Variable[MAXDATASETS];

size_t  nDefaults = 0;
DEFAULT *Defaults[MAX_DEFAULTS];        // Values from 'Defaults' file

Config cfg;

int     timeIndex[6];

size_t SampleOffset;

/* Vars related to data sets	*/
size_t	NumberDataSets, NumberSeconds, NumberElements[MAXDATASETS],
	CurrentDataSet;

int	Aircraft;

size_t	nSRfloats;

/* Data record pointers
 */
char	*ADSrecord, *AVAPSrecord[4];
NR_TYPE	*bits, *volts, *SRTvolts;
NR_TYPE *SampledData = 0, *AveragedData = 0, *HighRateData = 0;

RateFeedBack    FeedBack;

double	*plotData[MAXDATASETS], *xData[MAXDATASETS];

float	HDRversion;

int	FlightDate[3];	// Dummy.  Here to shut linker up.

std::string	title;

/* END GLOBAL.CC */
