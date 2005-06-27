/*
-------------------------------------------------------------------------
OBJECT NAME:	tape.h

DESCRIPTION:	Header file tape API package

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-8
-------------------------------------------------------------------------
*/

#ifndef TAPE_H
#define TAPE_H

#ifndef NULL
#define NULL		(0)
#endif

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

#include <header.h>


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
	GetNumberItems(long *result),
	get_lrlen(long *result),
	get_lrppr(long *result),
	get_thdrlen(long *result),
	GetADStype(char **result),
	GetPrimaryAddr(const char vn[], long *result),
	GetSecondaryAddr(const char vn[], long *result),
	GetCalCoeff(const char vn[], float **result),
	GetReadAddr1(const char vn[], long *result),
	GetReadAddr2(const char vn[], long *result),
	GetWriteAddr(const char vn[], long *result),
	GetChannelOffset(const char vn[], long *result),
	GetChannelGain(const char vn[], long *result),
	GetConversionOffset(const char vn[], long *result),
	GetHouseKeepingNames(const char vn[], char **result),
	GetInterfaceNumber(const char vn[], long *result),
	GetInterfaceChannel(const char vn[], long *result),
	GetItemLength(const char vn[], long *result),
	GetItemType(const char vn[], char **result),
	GetLocation(const char vn[], char **result),
	GetDSMlocation(const char vn[], char **result),
	GetLRLength(const char vn[], long *result),
	GetLRPPR(const char vn[], long *result),
	GetName(const char vn[], char **result),
	GetSampleOffset(const char vn[], long *result),
	GetOrder(const char vn[], long *result),
	GetParticleSpacingStart(const char vn[], long *result),
	GetParticleSpacingLength(const char vn[], long *result),
	GetParticleSpacingGate(const char vn[], float *result),
	GetRate(const char vn[], long *result),
	GetResolution(const char vn[], short *result),
	GetBaudRate(const char vn[], long *result),
	GetPortNumber(const char vn[], long *result),
	GetParity(const char vn[], long *result),
	GetFlowControl(const char vn[], long *result),
	GetNumberBins(const char vn[], long *result),
	GetNumberDataBits(const char vn[], long *result),
	GetNumberStopBits(const char vn[], float *result),
	GetDataType(const char vn[], long *result),
	GetTerminationString(const char vn[], char **result),
	GetAlignmentString(const char vn[], char **result),
	GetSerialNumber(const char vn[], char **result),
	GetStart(const char vn[], long *result),
	GetLength(const char vn[], long *result),
	GetType(const char vn[], char **result),
	GetConversionFactor(const char vn[], float *result);
#ifdef __cplusplus
}
#endif

#endif

/* END TAPE.H */
