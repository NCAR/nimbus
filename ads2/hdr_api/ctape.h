/*
-------------------------------------------------------------------------
OBJECT NAME:	tape.h

DESCRIPTION:	Header file tape API package

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-8
-------------------------------------------------------------------------
*/

#ifndef TAPE_H
#define TAPE_H

#ifndef ERR
#define ERR		(-1)
#endif

#ifndef OK
#define OK		(0)
#endif

#ifndef TRUE
#define TRUE		(1)
#define FALSE		(0)
#endif

#define SPACE		' '

#define LEAVE_OPEN	0
#define CLOSE		1

/* Struct Types, just add new ones on the bottom, it can only support 32
 */
#define THDR	0x00000000L
#define SH	0x00000001L
#define PMS1D	0x00000002L
#define INS	0x00000004L
#define IRS	0x00000008L
#define DME	0x00000010L
#define EVENT	0x00000020L
#define BLK	0x00000040L
#define PMS2D	0x00000080L
#define ASYNC	0x00000100L
#define PMS1V2	0x00000200L
#define UVHYG	0x00000400L
#define SERIAL	0x00000800L
#define PMS2DH	0x00001000L
#define MASP1	0x00002000L
#define PMS1V3	0x00004000L
#define ALL	0xffffffffL


/* Error Messages for 'taperr'		*/

#define NOINIT		200	/* InitFlightHeader was not called	*/
#define BADTYPE         201	/* The var does not contain that field	*/
#define BADVAR          202	/* Var name does not exist in list	*/
#define BADHDR          203	/* This was a bad header file		*/
#define READERR		204	/* fread failure			*/
#define BADVER		205	/* field does not exist for this version*/

#include <raf/header.h>


extern struct Fl	*HAPI_header;
extern int		taperr;


#define get_header()		(HAPI_header)

#ifdef __cplusplus
extern "C" {
#endif
char	*GetFirst(void ), *GetNext(void ), **GetVariableList(void );
void	*GetStructPointer(const char vn[]), ReleaseFlightHeader(void);

int	InitFlightHeader(const char name[], int status),
	GetVersion(char **result),
	GetProjectNumber(char **result),
	GetFlightNumber(char **result),
	GetTapeNumber(char **result),
	GetHeaderDate(char **result),
	GetHeaderTime(char **result),
	GetTimeZone(char **result),
	GetAircraft(char **result),
	GetNumberItems(int32_t *result),
	get_lrlen(int32_t *result),
	get_lrppr(int32_t *result),
	get_thdrlen(int32_t *result),
	GetADStype(char **result),
	GetPrimaryAddr(const char vn[], int32_t *result),
	GetSecondaryAddr(const char vn[], int32_t *result),
	GetCalCoeff(const char vn[], float **result),
	GetReadAddr1(const char vn[], int32_t *result),
	GetReadAddr2(const char vn[], int32_t *result),
	GetWriteAddr(const char vn[], int32_t *result),
	GetChannelOffset(const char vn[], int32_t *result),
	GetChannelGain(const char vn[], int32_t *result),
	GetConversionOffset(const char vn[], int32_t *result),
	GetHouseKeepingNames(const char vn[], char **result),
	GetInterfaceNumber(const char vn[], int32_t *result),
	GetInterfaceChannel(const char vn[], int32_t *result),
	GetItemLength(const char vn[], int32_t *result),
	GetItemType(const char vn[], char **result),
	GetLocation(const char vn[], char **result),
	GetDSMlocation(const char vn[], char **result),
	GetLRLength(const char vn[], int32_t *result),
	GetLRPPR(const char vn[], int32_t *result),
	GetName(const char vn[], char **result),
	GetSampleOffset(const char vn[], int32_t *result),
	GetOrder(const char vn[], int32_t *result),
	GetParticleSpacingStart(const char vn[], int32_t *result),
	GetParticleSpacingLength(const char vn[], int32_t *result),
	GetParticleSpacingGate(const char vn[], float *result),
	GetRate(const char vn[], int32_t *result),
	GetResolution(const char vn[], short *result),
	GetBaudRate(const char vn[], int32_t *result),
	GetPortNumber(const char vn[], int32_t *result),
	GetParity(const char vn[], int32_t *result),
	GetFlowControl(const char vn[], int32_t *result),
	GetNumberBins(const char vn[], int32_t *result),
	GetNumberDataBits(const char vn[], int32_t *result),
	GetNumberStopBits(const char vn[], float *result),
	GetDataType(const char vn[], int32_t *result),
	GetTerminationString(const char vn[], char **result),
	GetAlignmentString(const char vn[], char **result),
	GetSerialNumber(const char vn[], char **result),
	GetStart(const char vn[], int32_t *result),
	GetLength(const char vn[], int32_t *result),
	GetType(const char vn[], char **result),
	GetConversionFactor(const char vn[], float *result);
#ifdef __cplusplus
}
#endif

#endif

/* END TAPE.H */
