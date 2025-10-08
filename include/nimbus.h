/* -*- mode: C++; -*-
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

// Turn off legacy zero bin for size-distributions.
//#define ZERO_BIN	1

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
	char	*Name;		// Default name
	char	var[NAMELEN];	// Variable which uses this default
	bool	Used;		// Was it requested by amlib
	bool	Dirty;		// value changed via GUI
	std::vector<float> Values;
	std::string text;
	} DEFAULT;

// Informations for BlankOuts and SetValue files. Refactored Oct/2024
typedef struct
	{
	time_t	start;		// Start time epoch
	time_t  end;		// End time epoch

	// Value to substitute (BlankOuts: nan for RAWTBL, MISS_VAL for DERTBL)
	float	value;
	} SETVAL;

class Metadata
{
  public:
    Metadata(std::string name, std::string value)
	: _attr_name(name), _attr_str(value) { }
    Metadata(std::string name, std::vector<int> values)
	: _attr_name(name), _attr_int(values) { }
    Metadata(std::string name, std::vector<float> values)
	: _attr_name(name), _attr_flt(values) { }

    bool isString() const { return (_attr_str.size() > 0); }
    bool isFloat() const { return (_attr_flt.size() > 0); }
    bool isInt() const { return (_attr_int.size() > 0); }

//  private:
    std::string _attr_name;

    std::string _attr_str;
    std::vector<int> _attr_int;
    std::vector<float> _attr_flt;
};


// Base for the RAWTBL & DERTBL.  Refactored Jan/05.
class var_base
{
public:
  var_base(const char s[]);
  virtual ~var_base();

  void addToMetadata(const char attr_name[], const char attr[]);
  void addToMetadata(const char attr_name[], std::string attr);
  void addToMetadata(const char attr_name[], int value);
  void addToMetadata(const char attr_name[], float value);
  void addToMetadata(const char attr_name[], std::vector<int> values);
  void addToMetadata(const char attr_name[], std::vector<float> values);


  char name[NAMELEN];	// Variable name

  std::string SerialNumber;	// Probe Serial Number

  std::string Units() const;
  std::string AltUnits() const;	// Alternate units.
  std::string LongName() const;

  std::vector<std::string> CategoryList;

  int varid;		// NetCDF variable ID
  int LRstart;		// Start indx into AveragedData
  int SRstart;		// Start indx into SampledData
  int HRstart;		// Start indx into HighRateData

  size_t TimeLength;	// number of measurements in Time dimension
  size_t SampleRate;	// Sampled rate
  size_t Length;	// Histogram length, if histogram
  int coord_varid;	// varid for coordinate variable (size_distribution vars only)
  int bounds_varid;	// varid for coordinate bounds variable (size_distribution vars only)

  size_t ProbeType;	// Is this a probe & which one
  size_t ProbeCount;	// For mulitple identicle probes
			// Used by AMLIB
  bool DependedUpon;	// Is this variable depended upon?

  const MOD *Modulo;	// Pointer for modulo data, if any

  bool Dirty;		// Was variable modified by user
  bool Output;		// Is this going into the output file?
  bool Transmit;	// Is this variable going to the ground, real-time only.
  size_t OutputRate;	// Rate of data in the output [netCDF] file.
  const char *DataQuality;	// Prelim, QC'd, Bad, etc

  std::vector<SETVAL> set_value;	// Fixed values for raw data for time periods.

  std::vector<float> OutputData;      // vector to hold output data for this var

  float min, max;	// Min and max for this variable over course run.

  // Misc additional metadata to add to netCDF file.  Might replace the Defaults stuff...
  std::vector<Metadata> metadata;
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

  // ADS2 only.
  int32_t ADSstart;
  int32_t ADSoffset;	// Offset between samples
  char type[4];		// Analog, Digital or Counter

  ushort dsmID;		// ADS3/nids A/D temp compensation.

  void (*Initializer)(void *); // Function to initialize xlate
  void (*xlate)(RAWTBL *, const void *, NR_TYPE *); // Function to translate data
  void (*Average)(...);	// Routine to use to average/sum data

  int32_t convertOffset;	// A/D offset
  float	convertFactor;	// A/D slope
  std::vector<NR_TYPE> cof;

  // Index into nidas syncrecord of time tag offset of first sample of second.
  int TTindx;		// ads3 only, @see dsm::SyncRecordVariable
  size_t badLagCntr;

  int nidasLag;		// Static lag from nidas.  Here as metadata to go in netCDF
  int StaticLag;	// Static lag in ms to shift data
  int DynamicLag;	// Dynamic lag in ms
  float SpikeSlope;	// Slope for spike detection

  // This could be turned into a generic Payload pattern.
  std::vector<int> channelThresholds; // PCASP/SPP probes

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
  size_t nDependencies;			// # dependencies
  char depend[MAXDEPEND][NAMELEN];	// Dependencies
};


// Global Variables
extern const NR_TYPE MISSING_VALUE, floatNAN;
extern const int MAX_COF;

// Syslog.  Used for onboard / real-time.
extern nidas::util::Logger * logger;

/**
 * A vector subclass for variable pointers which deletes the pointers upon
 * destruction.  It is a simple class to clean up memory.  A better
 * alternative would be boost ptr_vector once someone wants to introduce a
 * boost dependency in nimbus.  See the implementation in globals.cc.
 **/
template <typename T>
class variable_vector : public std::vector<T*>
{
public:
  ~variable_vector();
};

extern variable_vector<RAWTBL> raw;
extern variable_vector<DERTBL> derived;
extern std::vector<DERTBL*> ComputeOrder;

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
        LogMessage(std::string msg),
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
