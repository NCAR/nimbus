/*
-------------------------------------------------------------------------
OBJECT NAME:	decode.h

DESCRIPTION:	Header File declaring Variable and associated processing
		functions.
-------------------------------------------------------------------------
*/

#ifndef DECODE_H
#define DECODE_H

#define GAP_FOUND	TRUE	/* For CheckForTimeGap()	*/

/* Used for defining time segments that start/end at one end of the tape */
#define BEG_OF_TAPE	(-1)
#define END_OF_TAPE	(-1)


/* QueryWhatToDo() answers	*/
#define	Q_EXIT		0
#define Q_CONTINUE	1
#define Q_NEXTTAPE	2


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
#define PROBE_MASP	0x00008000
#define PROBE_2DC	0x00010000
#define PROBE_2DP	0x00020000
#define PROBE_HVPS	0x00040000
#define PROBE_RDMA	0x00080000
#define PROBE_CLMT	0x00100000


/* Input module config files
 */
extern const std::string PROJ_NAME, DEPENDTBL, DEFAULTS, GROUPS, BLANKVARS,
	LAGS, BROADCAST, SPIKE, DEFDEROR, VARDB, BROADCASTVARS, XMIT_VARS,
	AIRCRAFT, MODVARS, SUMVARS, FILTERS, RAWNAMES, DERIVEDNAMES, INSNAMES,
	IRSNAMES, USERNAMES, OPHIR3NAMES, LANDMARKS, META_DATA;

#define HdrBlkTimeToSeconds(hdr) \
	(ntohs( ((struct Hdr_blk *)hdr)->second ) + \
	ntohs( ((struct Hdr_blk *)hdr)->minute ) * 60 + \
	ntohs( ((struct Hdr_blk *)hdr)->hour ) * 3600)

#define SecondsSinceMidnite(t) (t[0] * 3600 + t[1] * 60 + t[2])

/* External definitions
 */
extern bool	LITTON51_present, AsyncFileEnabled, AVAPS;
extern size_t	nLRfloats, nSRfloats, nHRfloats, LITTON51_start;

extern const std::string DSMSERVER;
extern const int DSMSERVERPORT;


/*      Function Prototypes
 */
void	SetLookupSuffix(char *new_suffix),
	AddToPMS1DprobeList(char probe[], char location[], char serial_num[],
								int type),
	AddVariableToRAWlagList(RAWTBL *varp),
	AddVariableToRAWdespikeList(RAWTBL *varp),
	ReadDefaultDerivedOutputRate(), ReadSumVariables(),
	ReadDefaultsFile(), ReadStaticLags(), ReadModuloVariables(),
	ReadGroundVarsFile(), ReadBlankOuts(),
	RunAMLIBinitializers(), GetUserTimeIntervals(),
	SetUpDependencies(), GenerateComputeOrder(),
	CleanOutUnwantedVariables(),
	CreateNetCDF(const char file_name[]),
	CheckAndAddAtts(int, int, char *),
	InitMRFilters(), ClearMRFilters(),
	DecodeADSrecord(short lr[], NR_TYPE nlr[]),
	AverageSampledData(),
	ApplyCalCoes(NR_TYPE *record),
	ComputeLowRateDerived(), ComputeHighRateDerived(),
	WriteNetCDF(), WriteNetCDF_MRF(), SyncNetCDF(), CloseNetCDF(),
	UpdateTime(NR_TYPE *record),
	FormatTimeSegmentsForOutputFile(char *buff);

void	Sum(NR_TYPE *in_data, NR_TYPE *out_data, size_t n, size_t l, MOD *mp),
	SumVector(NR_TYPE *in_data, NR_TYPE *out_data, size_t n, size_t l),
	AverageVector(NR_TYPE *in_data, NR_TYPE *out_data, size_t n, size_t l),
	Average(NR_TYPE *in_data, NR_TYPE *out_data, size_t n, size_t l, MOD *mp);

void	AllocateDataArrays(), FreeDataArrays(), ResetTimeGapper(),
	processTimeADS3(NR_TYPE *output, time_t ut);

int	SearchDERIVEFTNS(const char target[]),
	DecodeHeader(const char header_file[]),
	DecodeHeader3(const char header_file[]),
	DependIndexLookup(DERTBL *dp, int which_dep),
	NextTimeInterval(long *start, long *end),
	CloseADSfile(),
	LowRateLoop(long starttime, long endtime),
	HighRateLoop(long starttime, long endtime);

long	FindFirstLogicalADS2(char record[], long starttime),
	FindNextLogicalADS2(char record[], long endtime),
	FindFirstLogicalADS3(char record[], long starttime),
	FindNextLogicalADS3(char record[], long endtime);

extern long
	(*FindFirstLogicalRecord)(char record[], long starttime),
	(*FindNextLogicalRecord)(char record[], long endtime);

#endif

/* END DECODE.H */
