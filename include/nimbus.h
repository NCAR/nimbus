/*
-------------------------------------------------------------------------
OBJECT NAME:	nimbus.h

DESCRIPTION:	Header File declaring Variable and associated processing
		functions.
-------------------------------------------------------------------------
*/

#ifndef NIMBUS_H
#define NIMBUS_H

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdint.h>
#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <sys/types.h>
#include <raf/constants.h>
#include "config.h"

#include <nidas/util/Logger.h>

#define NAMELEN		32
#define MAX_VARIABLES	2000
#define MAX_DEFAULTS	256
#define MAXDEPEND	12
#define MAX_TIME_SLICES	1


/* Nimbus Record Info		*/
typedef double NR_TYPE;


// OpenProjectFile action values
enum projOpenFailAction { RETURN, EXIT };

enum SYNTHTYPE {sy_file, sy_constant, sy_function, sy_none};

typedef struct
	{
	int	value[2];	// Low & High wrap values
	int	diff;		// value[1] - value[0]
	int	bound[2];	// 10% bound values
	} MOD;

typedef struct
	{
	char	Name[20];	// Default name
	char	var[NAMELEN];	// Variable which uses this default
	bool	Used;		// Was it requested by amlib
	bool	Dirty;		// value changed via GUI
	std::vector<NR_TYPE> Values;
	} DEFAULT;


// Base for the RAWTBL & DERTBL.  Refactored Jan/05.
class var_base
{
public:
  var_base(const char s[]);

  char name[NAMELEN];	// Variable name

  std::string SerialNumber;	// Probe Serial Number

  std::string Units;
  std::string AltUnits;	// Alternate units.
  std::string LongName;
  std::vector<std::string> CategoryList;

  int varid;		// NetCDF variable ID
  int LRstart;		// Start indx into AveragedData
  int SRstart;		// Start indx into SampledData
  int HRstart;		// Start indx into HighRateData
  int LAGstart;		// ads3 only, @see dsm::SyncRecordVariable

  size_t SampleRate;	// Sampled rate
  size_t Length;	// Vector length (used by PMS1D)

  size_t ProbeType;	// Is this a probe & which one
  size_t ProbeCount;	// For mulitple identicle probes
			// Used by AMLIB
  bool DependedUpon;	// Is this variable depended upon?

  MOD *Modulo;		// Pointer for modulo data, if any

  bool Dirty;		// Was variable modified by user
  bool Output;		// Is this going into the output file?
  bool Transmit;	// Is this variable going to the ground, real-time only.
  size_t OutputRate;	// Rate of data in the output [netCDF] file.
  const char *DataQuality;	// Prelim, QC'd, Bad, etc

  std::vector<std::pair<int, int> > blank_out;

  NR_TYPE min, max;	// Min and max for this variable over course run.

  size_t badLagCntr;
};


/* Next 2 structs are main structures for each variable.  2 of them for the
 * 2 variable catagories we have, Raw recorded data, and Derived.
 */

/* Struct for raw variables (mostly block probes), some analog's come down
 * here for specialized processing (i.e. not polynomial cals).
 */
class RAWTBL : public var_base
{
public:
  RAWTBL(const char s[]);

  int32_t ADSstart;
  int32_t ADSoffset;	// Offset between samples
  char type[4];		// Analog, Digital or Counter

  ushort dsmID;		// ADS3/nids A/D temp compensation.

  void (*Initializer)(void *); // Function to initialize xlate
  void (*xlate)(RAWTBL *, void *, NR_TYPE *); // Function to translate data
  void (*Average)(...);	// Routine to use to average/sum data

  int32_t convertOffset;	// A/D offset
  float	convertFactor;	// A/D slope
  std::vector<NR_TYPE> cof;

  int StaticLag;	// Static lag in ms to shift data
  int DynamicLag;	// Dynamic lag
  NR_TYPE SpikeSlope;	// Slope for spike detection

  SYNTHTYPE synthtype;
};


/* Struct for derived variables
 */
class DERTBL : public var_base
{
public:
  DERTBL(const char s[]);

  int Default_HR_OR;		// Default OutputRate for HighRate run

  void (*Initializer)(void *);	// amlib "constructor"
  void (*compute)(void *);	// Function to compute data

  std::vector<var_base *> depends;
  size_t ndep;			// # dependencies
  char depend[MAXDEPEND][NAMELEN];	// Dependencies
};


// Global Variables
extern char sync_server_pipe[];

extern const NR_TYPE MISSING_VALUE, floatNAN;
extern const int MAX_COF;

// Syslog.  Used for onboard / real-time.
extern nidas::util::Logger * logger;

extern std::vector<RAWTBL *> raw;
extern std::vector<DERTBL *> derived;
extern std::vector<DERTBL *> ComputeOrder;

extern bool	PauseFlag, SynthData;
extern int	FlightNumberInt, PauseWhatToDo, Mode;
extern char	buffer[];

extern int timeIndex[];
extern bool SDF;
extern bool SDC;
extern bool SDP;

extern const char *ISO8601_T;


//      Function Prototypes
FILE	*OpenProjectFile(const std::string& filename, const char mode[], int action);

int	AccessProjectFile(const std::string& filename, const char mode[]),
	ReadTextFile(const std::string& filename, char **list),
	CheckForTimeGap(const void *ADShdr, int initMode);


struct var_match_name
{
  var_match_name (const std::string& target);
  bool operator()(var_base*) const;
  std::string _target;
};

struct var_match_name_sans_location
{
  var_match_name_sans_location (const std::string& target);
  bool operator()(var_base*) const;
  std::string _target;
};


template <typename T, typename P>
int
SearchTable_p(std::vector<T *> &table, const P& predicate, int s=0, int e=-1)
{
  typename std::vector<T*>::iterator end;
  end = (e == -1) ? table.end() : table.begin()+e;
  typename std::vector<T*>::iterator it;
  it = std::find_if(table.begin()+s, end, predicate);
  return (it != end) ? it - table.begin() : ERR;
}

template <typename T>
int
SearchTable(std::vector<T *> &table, const char target[])
{
  return SearchTable_p (table, var_match_name(target));
}

template <typename T>
int
SearchTableSansLocation(std::vector<T *> &table, const char target[])
{
  return SearchTable_p (table, var_match_name_sans_location(target));
}  

template <typename T>
int
SearchTable(std::vector<T *> &list, int n, const char target[])
{
  return SearchTable_p (list, var_match_name(target), 0, n);
}

template <typename T>
int
SearchTable(std::vector<T *> &list, int s, int e, const char target[])
{
  return SearchTable_p (list, var_match_name(target), s, e);
}


size_t	GetProbeType(std::string&);

char	*SearchList(char **list, const char target[]);

void	SortTable(char **table, int beg, int end),
	FreeTextFile(char **list),
	Initialize(),
	ResetProbeList(),
	MakeProjectFileName(char file[], const std::string& format),
	ProcessArgv(int argc, char **argv),
	LogMessage(const char msg[]),
	SetBaseTime(const NR_TYPE *record),
	BlankOutBadData(),
	GetDataDirectory(char buff[]),
        FlushXEvents(),
	quit();

void	SaveDefaults(FILE *fp),
	SetDefaultsValue(const char target[], NR_TYPE *new_value),
	CreateEditDefaultsWindow(), StopProcessing(), CreateFlightInfoWindow();

void	HandleFatalError(const char msg[]), HandleError(const char msg[]);

extern Config cfg;

#ifdef __cplusplus
extern "C" {
#endif
char    *strupr(char s[]), *strlower(char s[]);
#ifdef __cplusplus
}
#endif

#endif

// END NIMBUS.H
