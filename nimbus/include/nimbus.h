/*
-------------------------------------------------------------------------
OBJECT NAME:	nimbus.h

DESCRIPTION:	Header File declaring Variable and associated processing
		functions.
-------------------------------------------------------------------------
*/

#ifndef NIMBUS_H
#define NIMBUS_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include "constants.h"


#define NAMELEN		NAMLEN

#define MAX_SDI		400
#define MAX_RAW		400
#define MAX_DERIVE	400

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
#define MAX_TIME_SLICES	5


enum SYNTHTYPE {file,constant,function,none};

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
	int	nValues;
	NR_TYPE	*Value;
	} DEFAULT;


/* Next 3 structs are main structures for each variable.  3 of them for the
 * 3 variable catagories we have, Analog/Digital, Block probes, and Derived.
 */

/* Struct for SDI raw variables
 */
typedef struct
	{
	char	name[NAMELEN];	/* Variable name			*/
	int	varid;		/* NetCDF variable ID			*/
	int	LRstart;	/* Start indx into AveragedData		*/
	int	SRstart;	/* Start indx into SampledData		*/
	int	HRstart;	/* Start indx into HighRateData		*/
	int	SampleRate;	/* Sampled rate				*/
	MOD	*Modulo;	/* Pointer for modulo data, if any	*/
	bool	Dirty;		/* Was variable modified by user	*/
	bool	Output;		/* Is this going into the output file?	*/
	bool	Broadcast;	/* Real-time, ether broadcast?		*/
	bool	DependedUpon;	/* Is this variable depended upon?	*/
	int	OutputRate;
	int	StaticLag;	/* Static lag in ms			*/
	NR_TYPE	SpikeSlope;	/* Slope for spike detection		*/
	char	*DataQuality;	/* Prelim, QC'd, Bad, etc		*/

	long	ADSstart;	/* Start offset of variable in block	*/
	long	ADSoffset;	/* Offset between samples		*/
	char	type[4];	/* Analog, Digital or Counter		*/

	void	(*Average)(NR_TYPE *, NR_TYPE *, void *);	/* Routine to use to average/sum data	*/

	long	convertOffset;
	float	convertFactor;
	long	order;
	float	cof[MAX_COF];
        SYNTHTYPE synthtype; 
	} SDITBL;


/* Struct for raw variables (mostly block probes)
 */
typedef struct
	{
	char	name[NAMELEN];
	int	varid;		/* NetCDF variable ID			*/
	int	LRstart;	/* Start indx into AveragedData		*/
	int	SRstart;	/* Start indx into SampledData		*/
	int	HRstart;	/* Start indx into HighRateData		*/
	int	SampleRate;
	MOD	*Modulo;	/* !NULL means 0 - 360 or -180 - 180	*/
	bool	Dirty;		/* Was variable modified by user	*/
	bool	Output;		/* Is this going into the output file?	*/
	bool	Broadcast;	/* Real-time, ether broadcast?		*/
	bool	DependedUpon;	/* Is this variable depended upon?	*/
	int	OutputRate;
	int	Length;		/* Vector length (used by PMS1D)	*/
	int	StaticLag;	/* Static lag in ms to shift data	*/
	NR_TYPE	SpikeSlope;	/* Slope for spike detection		*/
	char	SerialNumber[8];	/* Probe Serial Number		*/
	int	ProbeCount;	/* For mulitple identicle probes	*/
	int	ProbeType;	/* Is this a probe & which one		*/
	char	*DataQuality;	/* Prelim, QC'd, Bad, etc		*/

	long	ADSstart;
	long	ADSoffset;	/* Offset between samples		*/
	int	DynamicLag;	/* Dynamic lag (for IRS & GPS)		*/

	void	(*Initializer)(void *); /* Function to initialize xlate	*/
	void	(*xlate)(void *, void *, float *);
				/* Function to translate data		*/
	void	(*Average)(...);	/* Routine to use to average/sum data	*/

	long	convertOffset;	/* These 4 fields are used by a few	*/
	float	convertFactor;	/* variables only.			*/
	long	order;		/* (PSFD, HGM, HGME).			*/
	float	cof[MAX_COF];
        SYNTHTYPE synthtype;
	} RAWTBL;


/* Struct for derived variables
 */
typedef struct
	{
	char	name[NAMELEN];
	int	varid;		/* NetCDF variable ID			*/
	int	LRstart;	/* Start indx into AveragedData		*/
	int	HRstart;	/* Start indx into HighRateData		*/
	MOD	*Modulo;	/* !NULL means 0 - 360 or -180 - 180	*/
	bool	Dirty;		/* Was variable modified by user	*/
	bool	Output;		/* Is this going into the output file?	*/
	bool	Broadcast;	/* Real-time, ether broadcast?		*/
	bool	DependedUpon;	/* Is this variable depended upon?	*/
	int	OutputRate;
	int	Length;		/* Vector length			*/
	char	SerialNumber[8];	/* Probe Serial Number		*/
	int	ProbeType;	/* Is this a probe & which one		*/
	int	ProbeCount;	/* For mulitple identicle probes	*/
				/* Used by AMLIB			*/

	int	Default_HR_OR;	/* Default OutputRate for HighRate run	*/
	char	*DataQuality;	/* Prelim, QC'd, Bad, etc		*/

	void	(*Initializer)(void *);	/* amlib "constructor"		*/
	void	(*compute)(void *);	/* Function to compute data		*/

	int	ndep;				/* # dependancies	*/
	char	depend[MAXDEPEND][NAMELEN];	/* Depandancies		*/
	int	depend_LRindex[MAXDEPEND];
	int	depend_HRindex[MAXDEPEND];
	} DERTBL;


/* Global Variables	*/
extern char	*ProjectDirectory, *ProjectNumber, *ProjectName, FlightNumber[];
extern SDITBL	*sdi[];
extern RAWTBL	*raw[];
extern DERTBL	*derived[], *ComputeOrder[];
extern int	nderive, nsdi, nraw;
extern bool	LoadProductionSetupFile, ProductionRun, PauseFlag, QCenabled,
		RawData,SynthData;
extern int	ProcessingRate, FlightNumberInt, PauseWhatToDo, Mode;
extern char	buffer[];

extern int timeindex[3];
extern float temptime;
extern int  hr;
extern int mins;
extern int sec;
extern bool SDF;
extern bool SDC;
extern bool SDP;



/*      Function Prototypes
 */
FILE	*OpenProjectFile(char filename[], char mode[], int action);

int	AccessProjectFile(char filename[], char mode[]),
	SearchTable(char *table[], int ntable, char target[]),
	SearchTableSansLocation(char *table[], int ntable, char target[]),
	LinearSearchTable(char **list, int n, char target[]),
	ReadTextFile(char filename[], char **list),
	CheckForTimeGap(struct Hdr_blk *ADShdr, int initMode),
	AccessProjectFile(char filename[], char mode[]);

unsigned long	GetProbeType(char name[]);

char	*SearchList(char **list, char target[]),
	*SearchDataQuality(char target[]),
	**GetProbeList(void);

void	SortTable(char **table, int beg, int end),
	FreeTextFile(char **list),
	Initialize(),
	ProcessArgv(int argc, char **argv),
	LogMessage(char msg[]),
	AddProbeToList(char name[], int type),
	SetBaseTime(struct Hdr_blk *hdr),
	BlankOutBadData(),
        FlushXEvents();

void	SaveDefaults(FILE *fp),
	SetDefaultsValue(char target[], NR_TYPE *new_value),
	CreateEditDefaultsWindow(), StopProcessing(), CreateFlightInfoWindow();

void	HandleError(char []);


#ifdef __cplusplus
extern "C" {
#endif
void	*GetMemory(unsigned nbytes);
char    *strupr(char s[]), *strlower(char s[]);
#ifdef __cplusplus
}
#endif

#endif

// END NIMBUS.H
