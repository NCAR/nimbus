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

NOTE:		Globals also exist in nimbus.c, memalloc.c & sh_mem.c.
		Most global Widget definitions exist in Xwin.c.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "amlib.h"
#include <injectsd.h>

const NR_TYPE MISSING_VALUE = -32767;
const int MAX_COF = 10;

char	buffer[4096];		/* Generic, volatile string space	*/
char	*ProjectDirectory, *ProjectNumber, *ProjectName, FlightNumber[12];

std::vector<SDITBL *> sdi;	/* SDI (aka raw) variable list		*/
std::vector<RAWTBL *> raw;	/* Alphabeticly sorted pointers		*/
std::vector<DERTBL *> derived;	/* Alphabeticly sorted pointers		*/
std::vector<DERTBL *> ComputeOrder;	/* Compute Order for derived	*/

size_t	nDefaults = 0;
DEFAULT	*Defaults[MAX_DEFAULTS];	/* Values from 'Defaults' file	*/

Config cfg;

bool	LITTON51_present,	/* hdr_decode.c & adsIO.c		*/
	AVAPS,			/* Is AVAPS dropesonde present.		*/
	PauseFlag,
	LoadProductionSetupFile;

size_t	ProcessingRate,
	SampleOffset;	/* Used by amlib fns, set in hrloop.c	*/

RateFeedBack	FeedBack;

int	PauseWhatToDo, Aircraft, FlightNumberInt;

/* Data record pointers
 */
char	*ADSrecord = 0;		/* Raw ADS record as read from tape.	*/
char	*AVAPSrecord[MAX_AVAPS];/* Raw AVAPS data as read from tape.	*/
ushort	*bits = 0;		/* Realtime (winput) only.		*/
NR_TYPE	*volts = 0;		/* Realtime (winput) only.		*/
NR_TYPE	*SRTvolts = 0;		/* Realtime (aeros) only.		*/
NR_TYPE	*SampledData = 0, *AveragedData = 0, *HighRateData = 0;

float	HDRversion;

size_t	nLRfloats = 0,
	nSRfloats = 0,	// Contains number of floats used in SampledData.
	nHRfloats = 0;	// Contains number of floats used in HighRateData.
size_t	LITTON51_start;	// hdr_decode.c & adsIO.c


//***************************synthetic data varables*************************//

int	timeindex[3];	// array of time
float	temptime;	//temporary time  holder
int	hr,sec,mins;	// store the hours, minutes and seconds 
SyntheticData sd;
char	*func[19];

bool    SynthData=false; // wheter or not synthetic data is being used.
bool    SDF=false;      // wheter or not synthetic data is being inserted from a file
bool    SDC=false;      // whether or not a constant value is being inserted for a variable
bool    SDP=false;      // wheter or not a variable is being modified by a function

//these are the functions that the synthetic data injector can modify
std::string functions[25];

//***************************************************************************//

/* END GLOBALS.C */
