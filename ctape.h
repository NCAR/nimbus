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


/* Top Level Header Macros	*/
#define GetVersion(r)		GetHeaderString(r, HAPI_header->version)
#define GetProjectNumber(r)	GetHeaderString(r, HAPI_header->prnum)
#define GetFlightNumber(r)	GetHeaderString(r, HAPI_header->fltnum)
#define GetTapeNumber(r)	GetHeaderString(r, HAPI_header->tpnum)
#define GetHeaderDate(r)	GetHeaderString(r, HAPI_header->date)
#define GetHeaderTime(r)	GetHeaderString(r, HAPI_header->time)
#define GetTimeZone(r)		GetHeaderString(r, HAPI_header->tzone)
#define GetAircraft(r)		GetHeaderString(r, HAPI_header->acraft)

#define GetNumberItems(r)	GetHeaderLong(r, HAPI_header->n_items)
#define get_header()		(HAPI_header)
#define get_lrlen(r)		GetHeaderLong(r, HAPI_header->lrlen)
#define get_lrppr(r)		GetHeaderLong(r, HAPI_header->lrppr)
#define get_thdrlen(r)		GetHeaderLong(r, HAPI_header->thdrlen)


#ifdef __cplusplus
extern "C" {
#endif
char	*GetFirst(void ), *GetNext(void ), **GetVariableList(void );
long	GetHeaderLong(long *result, long value);
void	*GetStructPointer(char vn[]), ReleaseFlightHeader(void);

int	InitFlightHeader(char name[], int status),
	GetHeaderString(char **result, char *value),
	GetADStype(char **result),
	GetPrimaryAddr(char vn[], long *result),
	GetSecondaryAddr(char vn[], long *result),
	GetCalCoeff(char vn[], float **result),
	GetReadAddr1(char vn[], long *result),
	GetReadAddr2(char vn[], long *result),
	GetWriteAddr(char vn[], long *result),
	GetChannelOffset(char vn[], long *result),
	GetChannelGain(char vn[], long *result),
	GetConversionOffset(char vn[], long *result),
	GetHouseKeepingNames(char vn[], char **result),
	GetInterfaceNumber(char vn[], long *result),
	GetInterfaceChannel(char vn[], long *result),
	GetItemLength(char vn[], long *result),
	GetItemType(char vn[], char **result),
	GetLocation(char vn[], char **result),
	GetDSMlocation(char vn[], char **result),
	GetLRLength(char vn[], long *result),
	GetLRPPR(char vn[], long *result),
	GetName(char vn[], char **result),
	GetSampleOffset(char vn[], long *result),
	GetOrder(char vn[], long *result),
	GetParticleSpacingStart(char vn[], long *result),
	GetParticleSpacingLength(char vn[], long *result),
	GetParticleSpacingGate(char vn[], float *result),
	GetRate(char vn[], long *result),
	GetResolution(char vn[], short *result),
	GetBaudRate(char vn[], long *result),
	GetPortNumber(char vn[], long *result),
	GetParity(char vn[], long *result),
	GetFlowControl(char vn[], long *result),
	GetNumberBins(char vn[], long *result),
	GetNumberDataBits(char vn[], long *result),
	GetNumberStopBits(char vn[], float *result),
	GetDataType(char vn[], long *result),
	GetTerminationString(char vn[], char **result),
	GetAlignmentString(char vn[], char **result),
	GetSerialNumber(char vn[], char **result),
	GetStart(char vn[], long *result),
	GetLength(char vn[], long *result),
	GetType(char vn[], char **result),
	GetConversionFactor(char vn[], float *result);
#ifdef __cplusplus
}
#endif

#endif

/* END TAPE.H */
