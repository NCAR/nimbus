/*
-------------------------------------------------------------------------
OBJECT NAME:	nimbus.h

DESCRIPTION:	Header File declaring Variable and associated processing
		functions.
-------------------------------------------------------------------------
*/

#ifndef NIMBUS_H
#define NIMBUS_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sys/types.h>
#include "constants.h"


#define NAMELEN		NAMLEN

#define MAX_VARIABLES	2000
#define MAX_DEFAULTS	256


/* Modes			*/
#define POST_PROCESSING		0
#define REALTIME		1


/* Processing Rates		*/
#define SAMPLE_RATE		0
#define LOW_RATE                1
#define HIGH_RATE               25


/* Nimbus Record Info		*/
#define NR_TYPE float
#define NR_SIZE sizeof(NR_TYPE)


/* OpenProjectFile action values      */
#define RETURN	0
#define EXIT	1

#define MAX_COF		10
#define MAXDEPEND	12
#define MAX_TIME_SLICES	1


enum SYNTHTYPE {sy_file, sy_constant, sy_function, sy_none};

typedef struct
	{
	int	value[2];	/* Low & High wrap values		*/
	int	diff;		/* value[1] - value[0]			*/
	int	bound[2];	/* 10% bound values			*/
	} MOD;

typedef struct
	{
	char	Name[20];	/* Default name				*/
	char	var[NAMELEN];	/* Variable which uses this default	*/
	bool	Used;		/* Was it requested by amlib		*/
	bool	Dirty;		/* value changed via GUI		*/
	size_t	nValues;
	NR_TYPE	*Value;
	} DEFAULT;


/* Next 3 structs are main structures for each variable.  3 of them for the
 * 3 variable catagories we have, Analog/Digital, Block probes, and Derived.
 */

/* Base for the rest.  Refactored Jan/05.
 */
class var_base
	{
	public:

	var_base(const char s[]);

	char	name[NAMELEN];	/* Variable name			*/
	int	varid;		/* NetCDF variable ID			*/
	size_t	LRstart;	/* Start indx into AveragedData		*/
	size_t	SRstart;	/* Start indx into SampledData		*/
	size_t	HRstart;	/* Start indx into HighRateData		*/

	size_t	SampleRate;	/* Sampled rate				*/
	size_t	Length;		/* Vector length (used by PMS1D)	*/

	bool	DependedUpon;	/* Is this variable depended upon?	*/

	MOD	*Modulo;	/* Pointer for modulo data, if any	*/

	bool	Dirty;		/* Was variable modified by user	*/
	bool	Output;		/* Is this going into the output file?	*/
	bool	Broadcast;	/* Real-time, ether broadcast?		*/
	size_t	OutputRate;
	char	*DataQuality;	/* Prelim, QC'd, Bad, etc		*/
	};


/* Struct for SDI analog variables, this variables only require polynomial cals
 * to be processed.
 */
class SDITBL : public var_base
	{
	public:

	SDITBL(const char s[]);

	int	StaticLag;	/* Static lag in ms			*/
	NR_TYPE	SpikeSlope;	/* Slope for spike detection		*/

	long	ADSstart;	/* Start offset of variable in block	*/
	long	ADSoffset;	/* Offset between samples		*/
	char	type[4];	/* Analog, Digital or Counter		*/

	void	(*Average)(NR_TYPE *, NR_TYPE *, void *);	/* Routine to use to average/sum data	*/

	long	convertOffset;
	float	convertFactor;
	size_t	order;
	float	cof[MAX_COF];
        SYNTHTYPE synthtype; 
	} ;


/* Struct for raw variables (mostly block probes), some analog's come down
 * here for specialized processing (i.e. not polynomial cals).
 */
class RAWTBL : public var_base
	{
	public:

	RAWTBL(const char s[]);

	int	StaticLag;	/* Static lag in ms to shift data	*/
	NR_TYPE	SpikeSlope;	/* Slope for spike detection		*/
	char	SerialNumber[8];	/* Probe Serial Number		*/
	size_t	ProbeCount;	/* For mulitple identicle probes	*/
	int	ProbeType;	/* Is this a probe & which one		*/

	long	ADSstart;
	long	ADSoffset;	/* Offset between samples		*/
	int	DynamicLag;	/* Dynamic lag (for IRS & GPS)		*/

	void	(*Initializer)(void *); /* Function to initialize xlate	*/
	void	(*xlate)(void *, void *, float *);
				/* Function to translate data		*/
	void	(*Average)(...);	/* Routine to use to average/sum data	*/

	long	convertOffset;	/* These 4 fields are used by a few	*/
	float	convertFactor;	/* variables only.			*/
	size_t	order;		/* (PSFD, HGM, HGME).			*/
	float	cof[MAX_COF];
        SYNTHTYPE synthtype;
	} ;


/* Struct for derived variables
 */
class DERTBL : public var_base
	{
	public:

	DERTBL(const char s[]);

	char	SerialNumber[8];	/* Probe Serial Number		*/
	int	ProbeType;	/* Is this a probe & which one		*/
	size_t	ProbeCount;	/* For mulitple identicle probes	*/
				/* Used by AMLIB			*/

	int	Default_HR_OR;	/* Default OutputRate for HighRate run	*/

	void	(*Initializer)(void *);	/* amlib "constructor"		*/
	void	(*compute)(void *);	/* Function to compute data	*/

	size_t	ndep;				/* # dependancies	*/
	char	depend[MAXDEPEND][NAMELEN];	/* Depandancies		*/
	int	depend_LRindex[MAXDEPEND];
	int	depend_HRindex[MAXDEPEND];
	} ;


/* Global Variables	*/
extern char	*ProjectDirectory, *ProjectNumber, *ProjectName, FlightNumber[];

extern const NR_TYPE MISSING_VALUE;

extern std::vector<SDITBL *> sdi;
extern std::vector<RAWTBL *> raw;
extern std::vector<DERTBL *> derived;
extern std::vector<DERTBL *> ComputeOrder;

extern bool	LoadProductionSetupFile, ProductionRun, PauseFlag, QCenabled,
		RawData,SynthData;
extern size_t	ProcessingRate;
extern int	FlightNumberInt, PauseWhatToDo, Mode;
extern char	buffer[];

extern int timeindex[3];
extern bool SDF;
extern bool SDC;
extern bool SDP;


/*      Function Prototypes
 */
FILE	*OpenProjectFile(char filename[], char mode[], int action);

int	AccessProjectFile(char filename[], char mode[]),
	ReadTextFile(char filename[], char **list),
	CheckForTimeGap(struct Hdr_blk *ADShdr, int initMode),
	AccessProjectFile(char filename[], char mode[]);

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


unsigned long	GetProbeType(std::string&);

char	*SearchList(char **list, char target[]),
	*SearchDataQuality(char target[]);

std::vector<std::string> GetProbeList(void);

void	SortTable(char **table, int beg, int end),
	FreeTextFile(char **list),
	Initialize(),
	ResetProbeList(),
	ProcessArgv(int argc, char **argv),
	LogMessage(char msg[]),
	AddProbeToList(char name[], unsigned long type),
	SetBaseTime(struct Hdr_blk *hdr),
	BlankOutBadData(),
	GetDataDirectory(char buff[]),
        FlushXEvents();

void	SaveDefaults(FILE *fp),
	SetDefaultsValue(char target[], NR_TYPE *new_value),
	CreateEditDefaultsWindow(), StopProcessing(), CreateFlightInfoWindow();

void	HandleError(char []);


#ifdef __cplusplus
extern "C" {
#endif
char    *strupr(char s[]), *strlower(char s[]);
#ifdef __cplusplus
}
#endif

#endif

// END NIMBUS.H
