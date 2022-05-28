/*
-------------------------------------------------------------------------
OBJECT NAME:	decode.h

DESCRIPTION:	Header File declaring Variable and associated processing
		functions.
-------------------------------------------------------------------------
*/

#ifndef DECODE_H
#define DECODE_H

#include <ctime>

#define ONE_WORD        sizeof(short)

/* ProbeType field.  Bottom nibble is used for ProbeCount.  Top nibble is
 * Catagory of probe.  This method of implementation will handle up to 24
 * different probes before it needs to be re-coded.
 */
#define ALL_ON		0xFFFFFFF1
#define ALL_OFF		0xFFFFFFF0
#define PROBE_PMS2D	0x10000000
#define PROBE_FLUX	0x20000000
#define PROBE_INERTIAL	0x40000000
#define PROBE_PMS1D	0x80000000
#define PROBE_GPS	0x00000010
#define PROBE_IRS	0x00000020	/* Inertial Systems	*/
#define PROBE_OPHIR3	0x00000040
#define PROBE_GUSTC	0x00000080	/* GPS Corrected winds	*/
#define PROBE_UVHYG	0x00000100
#define PROBE_FSSP	0x00000200
#define PROBE_PCASP	0x00000400
#define PROBE_260X	0x00000800
#define PROBE_200X	0x00001000
#define PROBE_200Y	0x00002000
#define PROBE_F300	0x00004000
#define PROBE_2DC	0x00008000
#define PROBE_2DP	0x00010000
#define PROBE_HVPS	0x00020000
#define PROBE_RDMA	0x00040000
#define PROBE_CLMT	0x00080000
#define PROBE_CDP	0x00100000
#define PROBE_2DS	0x00200000


/* Input module config files
 */
extern const std::string PROJ_NAME, DEPENDTBL, DEFAULTS, GROUPS, BLANKVARS,
	LAGS, BROADCAST, SPIKE, DEFDEROR, VARDB, BROADCASTVARS, XMIT_VARS,
	AIRCRAFT, MODVARS, SUMVARS, FILTERS, RAWNAMES, DERIVEDNAMES, INSNAMES,
	IRSNAMES, USERNAMES, OPHIR3NAMES, LANDMARKS, META_DATA, LOGFILE;

extern const std::string
	InterpKey, Interp_Linear, Interp_Cubic, Interp_Akima,
	TwoD_AreaRatioKey,
	TwoD_MethodKey, TwoD_MethodCenterIn, TwoD_MethodRecon;

#define SecondsSinceMidnite(t) (t[0] * 3600 + t[1] * 60 + t[2])

/* External definitions
 */
extern bool	LITTON51_present, AsyncFileEnabled, AVAPS;
extern size_t	nLRvalues, nSRvalues, nHRvalues, LITTON51_start;

extern const std::string DSMSERVER;
extern const int DSMSERVERPORT;

extern const int MaxLag;

/*      Function Prototypes
 */
void	SetLookupSuffix(const char *new_suffix),
	AddToPMS1DprobeList(	const char probe[], const char location[],
				const char serial_num[], int type),
	AddVariableToRAWlagList(RAWTBL *varp),
	AddVariableToRAWdespikeList(RAWTBL *varp),
	ReadDefaultDerivedOutputRate(), ReadSumVariables(),
	ReadDefaultsFile(), ReadStaticLags(), ReadModuloVariables(),
	ReadGroundVarsFile(), ReadBlankOuts(),
        RunAMLIBinitializers(),
	ReadDependencies(), SetupDependencies(), GenerateComputeOrder(),
	CleanOutUnwantedVariables(),
	CreateNetCDF(const char file_name[]),
	CheckAndAddAtts(int, int, char *),
	InitMRFilters(), ClearMRFilters(),
	DecodeADSrecord(const short lr[], NR_TYPE nlr[]),
	AverageSampledData(),
	ApplyCalCoes(NR_TYPE *record),
	ComputeLowRateDerived(time_t), ComputeHighRateDerived(time_t),
	WriteNetCDF(), WriteNetCDF_MRF(), SyncNetCDF(), CloseNetCDF(),
	UpdateTime(const NR_TYPE *record),
	FormatTimeSegmentsForOutputFile(char *buff);

void	Sum(NR_TYPE *in_data, NR_TYPE *out_data, size_t n, size_t l, MOD *mp),
	SumVector(NR_TYPE *in_data, NR_TYPE *out_data, size_t n, size_t l),
	AverageVector(NR_TYPE *in_data, NR_TYPE *out_data, size_t n, size_t l),
	Average(NR_TYPE *in_data, NR_TYPE *out_data, size_t n, size_t l, MOD *mp);

void	AllocateDataArrays(), FreeDataArrays(),
	processTimeADS3(NR_TYPE *output, time_t ut);

int	SearchDERIVEFTNS(const char target[]),
	DecodeHeader(const char header_file[]),
	DecodeHeader3(const char header_file[]),
	DependIndexLookup(DERTBL *dp, int which_dep, bool init_depends),
	CloseADSfile(),
	LowRateLoop(time_t starttime, time_t endtime),
	HighRateLoop(time_t starttime, time_t endtime);


int32_t	FindFirstLogicalADS2(char record[], time_t starttime),
	FindNextLogicalADS2(char record[], time_t endtime),
	FindFirstLogicalADS3(char record[], time_t starttime),
	FindNextLogicalADS3(char record[], time_t endtime);

extern int32_t
	(*FindFirstLogicalRecord)(char record[], time_t starttime),
	(*FindNextLogicalRecord)(char record[], time_t endtime);

#endif

/* END DECODE.H */
