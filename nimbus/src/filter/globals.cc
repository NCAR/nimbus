/*
-------------------------------------------------------------------------
OBJECT NAME:	globals.c

FULL NAME:	Global Variable Definitions

NOTE:		Most global Widget definitions exist in Xwin.c.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-05
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "amlib.h"
#include <injectsd.h>

class PostgreSQL;

namespace dsm
{
  class SyncRecordReader;
}

const NR_TYPE floatNAN = nanf("");
const NR_TYPE MISSING_VALUE = -32767.0;
const int MaxLag = 2000;
const int MAX_COF = 6;

const std::string DSMSERVER = "localhost";
const int DSMSERVERPORT = 30001;

const std::string PROJ_NAME     = "%s/%s/%s/ProjectName";
const std::string DEPENDTBL     = "%s/%s/%s/DependTable";
const std::string DEFAULTS      = "%s/%s/%s/Defaults";
const std::string GROUPS        = "%s/%s/%s/Production/FlightGroups";
const std::string BLANKVARS     = "%s/%s/%s/Production/BlankOuts";
const std::string LAGS          = "%s/%s/%s/Lags";
const std::string BROADCAST     = "%s/%s/%s/ascii_parms";
const std::string SPIKE         = "%s/%s/%s/Despike";
const std::string DEFDEROR      = "%s/%s/%s/LowRateVars";
const std::string VARDB         = "%s/%s/%s/VarDB";
const std::string BROADCASTVARS = "%s/%s/%s/BcastVars";
const std::string LANDMARKS     = "%s/%s/%s/landmarks";
const std::string XMIT_VARS     = "%s/%s/%s/groundvars";

const std::string AIRCRAFT	= "AircraftSpecs";
const std::string MODVARS	= "%s/Configuration/raf/ModVars";
const std::string SUMVARS	= "%s/Configuration/raf/SumVars";
const std::string RAWNAMES	= "%s/Configuration/raf/RawNames";
const std::string DERIVEDNAMES	= "%s/Configuration/raf/DerivedNames";
const std::string META_DATA	= "%s/Configuration/raf/MetaDataTable";
const std::string INSNAMES	= "%s/Configuration/raf/ins.names";
const std::string IRSNAMES	= "%s/Configuration/raf/irs.names";

const std::string FILTERS	= "%s/Configuration/raf/filters/%s";

const std::string USERNAMES     = "%s/%s/%s/UserNames";
const std::string OPHIR3NAMES   = "%s/%s/%s/ophir3.names";


char	buffer[8192];		// Generic, volatile string space
char	*ProjectDirectory;

Config cfg;	// Global configuration.

std::vector<RAWTBL *> raw;	// Alphabeticly sorted pointers
std::vector<DERTBL *> derived;	// Alphabeticly sorted pointers
std::vector<DERTBL *> ComputeOrder;	// Compute Order for derived

size_t	nDefaults = 0;
DEFAULT	*Defaults[MAX_DEFAULTS];	// Values from 'Defaults' file

bool	LITTON51_present,	/* hdr_decode.c & adsIO.c		*/
	AVAPS,			/* Is AVAPS dropesonde present.		*/
	PauseFlag,
	LoadProductionSetupFile;

size_t	ProcessingRate,
	SampleOffset;	/* Used by amlib fns, set in hrloop.c	*/

RateFeedBack	FeedBack;

int	PauseWhatToDo, FlightNumberInt;

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

dsm::SyncRecordReader* syncRecReader = 0;

long (*FindFirstLogicalRecord)(char lr[], long starttime);
long (*FindNextLogicalRecord)(char lr[], long endtime);

PostgreSQL *psql;


//***************************synthetic data varables*************************//

int	timeIndex[6];	// array of time
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
