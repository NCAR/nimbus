
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

#include "/h/9/forbes/nimbus/include/nimbus.h"
#include "decode.h"
#include "header.h"
#include <string.h>
#include <vector.h>
#include <injectsd.h>


char	buffer[4096];		/* Generic, volatile string space	*/
char	*ProjectDirectory, *ProjectNumber, *ProjectName, FlightNumber[12];

SDITBL  *sdi[MAX_SDI];		/* SDI (aka raw) variable list		*/
RAWTBL	*raw[MAX_RAW];		/* Alphabeticly sorted pointers		*/
DERTBL	*derived[MAX_DERIVE];	/* Alphabeticly sorted pointers		*/
DERTBL	*ComputeOrder[MAX_DERIVE];	/* Compute Order for derived	*/
DEFAULT	*Defaults[MAX_DEFAULTS];	/* Values from 'Defaults' file	*/
std::string functions[25];                   //these are the functions that the synthetic data injector can modify
int	nsdi, nraw, nderive, nDefaults;

bool	LITTON51_present,	/* hdr_decode.c & adsIO.c		*/
	AVAPS,			/* Is AVAPS dropesonde present.		*/
	PauseFlag,
	AsyncFileEnabled,
	LoadProductionSetupFile,
	Interactive,
	ProductionRun,
	RawData,		/* No despiking or phase_shifting	*/
	QCenabled;		/* QC Library, currently not enabled.	*/



int	Mode,		/* RealTime or PostProcessing (winput or nimbus)*/
	ProcessingRate,
	FeedBack, SampleOffset,	/* Used by amlib fns, set in hrloop.c	*/
	PauseWhatToDo,
	Aircraft, FlightNumberInt;

/* Data record pointers
 */
char	*ADSrecord;		/* Raw ADS record as read from tape.	*/
char	*AVAPSrecord[MAX_AVAPS];/* Raw AVAPS data as read from tape.	*/
ushort	*bits;			/* Realtime (winput) only.		*/
NR_TYPE	*volts;			/* Realtime (winput) only.		*/
NR_TYPE	*SampledData, *AveragedData, *HighRateData;

float	HDRversion;

long	nFloats;	/* Contains number of floats used in SampledData */
long	LITTON51_start;		/* hdr_decode.c & adsIO.c		*/


//***********************************synthetic data varables**************************///

int timeindex[3]; // array of time
float temptime;           //temporary time  holder
int hr,sec,mins;      // store the hours, minutes and seconds 
SyntheticData sd;
char * func[19];

bool    SynthData=false;              //bool value that tells wheter or not synthetic data is being used.
bool    SDF=false;                    //bool value that tells wheter or not synthetic data is being inserted from a file
bool    SDC=false;                    //bool value that tells whether or not a constant value is being inserted for a variable
bool    SDP=false;                    //bool value that tells wheter or not a variable is being modified by a function

//************************************************************************************//

/* END GLOBALS.C */
