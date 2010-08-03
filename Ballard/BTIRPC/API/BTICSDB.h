/**
*
*  BTICSDB RPC Driver  Version 1.0.4  (10/23/2009)
*  Copyright (c) 2005-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   BTICSDB.H -- BTICSDB RPC Driver Include Header File.
*
**/

/**
*
*  This file defines the procedures provided by the PowerPC Linux
*  Driver Library for Ballard Technology CSDB interface
*  cards.  Applications using the BTICSDB Driver Library must
*  incorporate this include file using the preprocessor
*  directive #include. If this file is in the current working
*  directory, the form would be:
*
*  #include "BTICSDB.H"
*
**/

/**
*
*  Conditional block to prevent multiple defines.
*
**/

#ifndef __BTICSDB_H
#define __BTICSDB_H

/**
*
*  Typedefs used by the BTICSDB Driver.
*
**/

#ifndef BTICSDBAPI
#define BTICSDBAPI
#endif

#ifndef VOID
#define VOID void
#endif

#ifndef LPVOID
#define LPVOID void *
#endif

#ifndef INT
#define INT int
#endif

#ifndef LPINT
#define LPINT int *
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef LPBYTE
#define LPBYTE unsigned char *
#endif

#ifndef CHAR
#define CHAR char
#endif

#ifndef UINT16
#define UINT16 unsigned short
#endif

#ifndef LPUINT16
#define LPUINT16 unsigned short *
#endif

#ifndef UINT32
#define UINT32 unsigned int
#endif

#ifndef LPUINT32
#define LPUINT32 unsigned int *
#endif

#ifndef MSGADDR
#define MSGADDR unsigned int
#endif

#ifndef BASEADDR
#define BASEADDR unsigned int
#endif

#ifndef LISTADDR
#define LISTADDR unsigned int
#endif

#ifndef LPMSGADDR
#define LPMSGADDR unsigned int *
#endif

#ifndef LPCSTR
#define LPCSTR const char *
#endif

#ifndef LPSTR
#define LPSTR char *
#endif

#ifndef BOOL
#define BOOL int
#endif

#ifndef HCARD
#define HCARD long
#endif

#ifndef ERRVAL
#define ERRVAL int
#endif

#ifndef SCHNDX
#define SCHNDX int
#endif

/**
*
*  Structs used by the BTICSDB Driver.
*
**/
#ifndef MSGFIELDSCSDB

typedef struct
{
    UINT16 msgopt;             //Driver writes message options
    UINT16 msgoptext;          //Non-continous, Valid, and current burst count (FW)
    UINT16 burstcount;         //Burst count (API)
    UINT16 reserved;           //reserved
    UINT16 msgact;             //Card writes message activity
    UINT16 datacount;          //Number of valid bytes in data[] buffer
    UINT32 listptr;            //Driver writes list buffer pointer
    union {
        UINT32 timetag;        //Card writes time-tag low value (32 bits)
        UINT32 hitcount;       //Card writes hit count
    };
    union {
        UINT32 maxtime;        //Card writes max time
        UINT32 elapsetime;     //Card writes elapsed time
    };
    UINT32 mintime;            //Card writes min time
    UINT32 userptr;            //Driver writes user code pointer
    UINT32 miscptr;            //reserved
    UINT32 timetagh;           //Card writes extended timetag (32 bits)
    UINT16 data[32];           //Card writes data block
} MSGFIELDSCSDB;

#endif

#ifndef LPMSGFIELDSCSDB
typedef MSGFIELDSCSDB * LPMSGFIELDSCSDB;
#endif

/**
*
*  "C" block if compiling a C++ file.
*
**/

#ifdef __cplusplus
extern "C" {
#endif

/**
*
*  BTICSDB Driver functions.
*
**/

BTICSDBAPI ERRVAL BTICSDB_ChClear(INT channum,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_ChConfig(UINT32 configval,UINT16 bitrateflag,UINT16 bytesperblock,UINT16 framespersecond,UINT16 blocksperframe,INT channum,HCARD handleval);
BTICSDBAPI VOID BTICSDB_ChGetCount(LPINT rcvcount,LPINT xmtcount,HCARD handleval);
BTICSDBAPI UINT32 BTICSDB_ChGetInfo(UINT16 infotype,INT channum,HCARD handleval);
BTICSDBAPI BOOL BTICSDB_ChIsCSDB(INT channum,HCARD handleval);
BTICSDBAPI BOOL BTICSDB_ChIsRcv(INT channum,HCARD handleval);
BTICSDBAPI BOOL BTICSDB_ChIsXmt(INT channum,HCARD handleval);
BTICSDBAPI VOID BTICSDB_ChPause(INT channum,HCARD handleval);
BTICSDBAPI INT BTICSDB_ChPauseCheck(INT channum,HCARD handleval);
BTICSDBAPI VOID BTICSDB_ChResume(INT channum,HCARD handleval);
BTICSDBAPI BOOL BTICSDB_ChStart(INT channum,HCARD handleval);
BTICSDBAPI BOOL BTICSDB_ChStop(INT channum,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_FilterClear(BASEADDR baseaddr,HCARD handleval);
BTICSDBAPI MSGADDR BTICSDB_FilterDefault(UINT32 configval,INT channum,HCARD handleval);
BTICSDBAPI MSGADDR BTICSDB_FilterRd(INT addrval,INT sival,INT channum,HCARD handleval);
BTICSDBAPI MSGADDR BTICSDB_FilterSet(UINT32 configval,INT addrval,INT simask,INT channum,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_FilterWr(MSGADDR msgaddr,INT addrval,INT sival,INT channum,HCARD handleval);
BTICSDBAPI INT BTICSDB_ListDataRd(LPBYTE buf,INT count,LISTADDR listaddr,HCARD handleval);
BTICSDBAPI INT BTICSDB_ListDataWr(LPBYTE buf,INT count,LISTADDR listaddr,HCARD handleval);
BTICSDBAPI LISTADDR BTICSDB_ListRcvCreate(UINT32 listconfigval,INT count,MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI INT BTICSDB_ListStatus(LISTADDR listaddr,HCARD handleval);
BTICSDBAPI LISTADDR BTICSDB_ListXmtCreate(UINT32 listconfigval,INT count,MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI MSGADDR BTICSDB_MsgBlockRd(LPMSGFIELDSCSDB msgfields,MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI MSGADDR BTICSDB_MsgBlockWr(LPMSGFIELDSCSDB msgfields,MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI MSGADDR BTICSDB_MsgCreate(UINT32 configval,HCARD handleval);
BTICSDBAPI BYTE BTICSDB_MsgDataByteRd(INT index,MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI INT BTICSDB_MsgDataByteWr(BYTE data,INT index,MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI INT BTICSDB_MsgDataRd(LPBYTE buf,INT count,MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI INT BTICSDB_MsgDataWr(LPBYTE buf,INT count,MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI BOOL BTICSDB_MsgIsAccessed(MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI UINT16 BTICSDB_MsgValidSet(MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_ParamAmplitudeConfig(UINT32 configval,UINT16 dacval,INT channum,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_ParamAmplitudeGet(LPUINT16 dacval,INT channum,HCARD handleval);
BTICSDBAPI SCHNDX BTICSDB_SchedBranch(UINT32 condition,SCHNDX destindex,INT channum,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_SchedBuild(INT nummsgs,LPMSGADDR msgs,LPINT freq,INT interblockgap,INT rsvdblocks,INT channum,HCARD handleval);
BTICSDBAPI SCHNDX BTICSDB_SchedCall(UINT32 condition,SCHNDX destindex,INT channum,HCARD handleval);
BTICSDBAPI SCHNDX BTICSDB_SchedEntry(INT channum,HCARD handleval);
BTICSDBAPI SCHNDX BTICSDB_SchedGap(UINT16 gapval,INT channum,HCARD handleval);
BTICSDBAPI SCHNDX BTICSDB_SchedHalt(INT channum,HCARD handleval);
BTICSDBAPI SCHNDX BTICSDB_SchedLog(UINT32 condition,UINT16 tagval,INT channum,HCARD handleval);
BTICSDBAPI SCHNDX BTICSDB_SchedMsg(MSGADDR msgaddr,INT channum,HCARD handleval);
BTICSDBAPI SCHNDX BTICSDB_SchedPause(INT channum,HCARD handleval);
BTICSDBAPI SCHNDX BTICSDB_SchedPulse(INT dionum,INT channum,HCARD handleval);
BTICSDBAPI SCHNDX BTICSDB_SchedRestart(INT channum,HCARD handleval);
BTICSDBAPI SCHNDX BTICSDB_SchedReturn(INT channum,HCARD handleval);

/**
*
*  Internal functions used by the BTICSDB Driver.
*
**/

BTICSDBAPI ERRVAL BTICSDB_ChConfigEx(UINT32 configval,UINT16 bitrateflag,UINT16 bytesperblock,UINT16 framespersecond,UINT16 blocksperframe,UINT16 interbytegap,INT channum,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_ChConfigExx(UINT32 configval,UINT16 count,UINT16 bitrateflag,UINT16 bytesperblock,UINT16 framespersecond,UINT16 blocksperframe,UINT16 interbytegap,INT channum,HCARD handleval);
BTICSDBAPI UINT32 BTICSDB_CmdAddr(SCHNDX index,INT channum,HCARD handleval);
BTICSDBAPI UINT32 BTICSDB_CmdAlloc(INT count,HCARD handleval);
BTICSDBAPI UINT32 BTICSDB_CmdAllocEx(UINT32 configval,INT count,HCARD handleval);
BTICSDBAPI UINT32 BTICSDB_CmdBaseRd(INT channum,HCARD handleval);
BTICSDBAPI VOID BTICSDB_CmdBaseWr(UINT32 addrval,INT channum,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_CmdClear(INT channum,HCARD handleval);
BTICSDBAPI UINT16 BTICSDB_CmdCountRd(INT channum,HCARD handleval);
BTICSDBAPI VOID BTICSDB_CmdCountWr(UINT16 countval,INT channum,HCARD handleval);
BTICSDBAPI UINT16 BTICSDB_CmdCtrlRd(SCHNDX index,INT channum,HCARD handleval);
BTICSDBAPI VOID BTICSDB_CmdCtrlWr(UINT16 ctrlval,SCHNDX index,INT channum,HCARD handleval);
BTICSDBAPI UINT16 BTICSDB_CmdCurrRd(INT channum,HCARD handleval);
BTICSDBAPI VOID BTICSDB_CmdCurrWr(UINT16 addrval,INT channum,HCARD handleval);
BTICSDBAPI SCHNDX BTICSDB_CmdInsert(LPUINT16 block,INT channum,HCARD handleval);
BTICSDBAPI UINT16 BTICSDB_CmdMaxLoopRd(INT channum,HCARD handleval);
BTICSDBAPI VOID BTICSDB_CmdMaxLoopWr(UINT16 countval,INT channum,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_CmdRd(LPUINT16 block,UINT32 addrval,HCARD handleval);
BTICSDBAPI VOID BTICSDB_CmdRestart(INT channum,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_CmdShotWr(BOOL value,SCHNDX index,INT channum,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_CmdSkipWr(BOOL value,SCHNDX index,INT channum,HCARD handleval);
BTICSDBAPI VOID BTICSDB_CmdStackClr(INT channum,HCARD handleval);
BTICSDBAPI UINT16 BTICSDB_CmdStartRd(INT channum,HCARD handleval);
BTICSDBAPI VOID BTICSDB_CmdStartWr(UINT16 addrval,INT channum,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_CmdStepWr(BOOL value,SCHNDX index,INT channum,HCARD handleval);
BTICSDBAPI INT BTICSDB_CmdTotalRd(INT channum,HCARD handleval);
BTICSDBAPI VOID BTICSDB_CmdTotalWr(INT countval,INT channum,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_CmdWr(LPUINT16 block,UINT32 addrval,HCARD handleval);
BTICSDBAPI INT BTICSDB_DllUsageCount(VOID);
BTICSDBAPI UINT32 BTICSDB_FilterAddr(INT addrval,INT sival,INT channum,HCARD handleval);
BTICSDBAPI BASEADDR BTICSDB_FilterCreate(HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_FilterFill(MSGADDR msgaddr,BASEADDR baseaddr,HCARD handleval);
BTICSDBAPI VOID BTICSDB_GetChanCount(LPINT rcvcount,LPINT xmtcount,HCARD handleval);
BTICSDBAPI BOOL BTICSDB_IsChan(INT channum,HCARD handleval);
BTICSDBAPI BOOL BTICSDB_IsRcvChan(INT channum,HCARD handleval);
BTICSDBAPI BOOL BTICSDB_IsXmtChan(INT channum,HCARD handleval);
BTICSDBAPI UINT32 BTICSDB_ListAddr(INT index,LISTADDR listaddr,HCARD handleval);
BTICSDBAPI UINT32 BTICSDB_ListAddrEx(INT index,LISTADDR listaddr,LPUINT16 header,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_ListAdvance(LISTADDR listaddr,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_ListClear(LISTADDR listaddr,HCARD handleval);
BTICSDBAPI LISTADDR BTICSDB_ListConfig(UINT32 configval,INT count,HCARD handleval);
BTICSDBAPI LISTADDR BTICSDB_ListConfigEx(UINT32 configval,INT count,HCARD handleval);
BTICSDBAPI INT BTICSDB_ListDataRdEx(LPBYTE buf,INT count,INT index,LISTADDR listaddr,HCARD handleval);
BTICSDBAPI INT BTICSDB_ListDataWrEx(LPBYTE buf,INT count,INT index,LISTADDR listaddr,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_ListInit(LISTADDR listaddr,MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI UINT32 BTICSDB_ListNextRd(UINT32 listaddr,LPUINT16 header,HCARD handleval);
BTICSDBAPI UINT32 BTICSDB_ListNextWr(UINT32 listaddr,LPUINT16 header,HCARD handleval);
BTICSDBAPI VOID BTICSDB_ListPostRd(LISTADDR listaddr,LPUINT16 header,HCARD handleval);
BTICSDBAPI VOID BTICSDB_ListPostWr(LISTADDR listaddr,LPUINT16 header,HCARD handleval);
BTICSDBAPI VOID BTICSDB_ListPreRd(LISTADDR listaddr,LPUINT16 header,HCARD handleval);
BTICSDBAPI VOID BTICSDB_ListPreWr(LISTADDR listaddr,LPUINT16 header,HCARD handleval);
BTICSDBAPI UINT16 BTICSDB_MsgActivityRd(MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI VOID BTICSDB_MsgActivityWr(UINT16 value,MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI UINT16 BTICSDB_MsgCheck(MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI MSGADDR BTICSDB_MsgCommRd(LPMSGFIELDSCSDB msgfields,MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI MSGADDR BTICSDB_MsgCommWr(LPMSGFIELDSCSDB msgfields,MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_MsgConfig(UINT32 configval,INT count,MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI MSGADDR BTICSDB_MsgCreateEx(UINT32 configval,INT count,HCARD handleval);
BTICSDBAPI UINT32 BTICSDB_MsgFieldRd(INT fieldtype,MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI UINT32 BTICSDB_MsgFieldWr(UINT32 fieldval,INT fieldtype,MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI UINT16 BTICSDB_MsgOptRd(MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI VOID BTICSDB_MsgOptWr(UINT16 msgopt,MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI MSGADDR BTICSDB_MsgReset(MSGADDR msgaddr,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_ProcSetPost(UINT32 useraddr,INT channum,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_ProcSetPre(UINT32 useraddr,INT channum,HCARD handleval);
BTICSDBAPI SCHNDX BTICSDB_SchedAgain(SCHNDX index,INT channum,HCARD handleval);
BTICSDBAPI SCHNDX BTICSDB_SchedBranchEx(UINT16 condition1,UINT16 condition2,SCHNDX destindex,INT channum,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_SchedBuildEx(INT nummsgs,LPMSGADDR msgs,LPINT freq,INT interblockgap,INT rsvdblocks,INT bitrate,INT channum,HCARD handleval);
BTICSDBAPI SCHNDX BTICSDB_SchedCallEx(UINT16 condition1,UINT16 condition2,SCHNDX destindex,INT channum,HCARD handleval);
BTICSDBAPI SCHNDX BTICSDB_SchedLogEx(UINT16 condition1,UINT16 condition2,UINT16 tagval,INT channum,HCARD handleval);
BTICSDBAPI SCHNDX BTICSDB_SchedNop(INT channum,HCARD handleval);
BTICSDBAPI SCHNDX BTICSDB_SchedPulse0(INT dionum,INT channum,HCARD handleval);
BTICSDBAPI SCHNDX BTICSDB_SchedPulse1(INT dionum,INT channum,HCARD handleval);
BTICSDBAPI SCHNDX BTICSDB_SchedUser(UINT32 useraddr,INT channum,HCARD handleval);
BTICSDBAPI ERRVAL BTICSDB_TestProtocol(HCARD handleval);

#ifdef __cplusplus
}
#endif

/**
*
*  Channel number constants
*
**/

#define	CH0  0x0000           //Channel 0
#define	CH1  0x0001           //Channel 1
#define	CH2  0x0002           //Channel 2
#define	CH3  0x0003           //Channel 3
#define	CH4  0x0004           //Channel 4
#define	CH5  0x0005           //Channel 5
#define	CH6  0x0006           //Channel 6
#define	CH7  0x0007           //Channel 7
#define	CH8  0x0008           //Channel 8
#define	CH9  0x0009           //Channel 9
#define	CH10 0x000A           //Channel 10
#define	CH11 0x000B           //Channel 11
#define	CH12 0x000C           //Channel 12
#define	CH13 0x000D           //Channel 13
#define	CH14 0x000E           //Channel 14
#define	CH15 0x000F           //Channel 15
#define	CH16 0x0010           //Channel 16
#define	CH17 0x0011           //Channel 17
#define	CH18 0x0012           //Channel 18
#define	CH19 0x0013           //Channel 19
#define	CH20 0x0014           //Channel 20
#define	CH21 0x0015           //Channel 21
#define	CH22 0x0016           //Channel 22
#define	CH23 0x0017           //Channel 23
#define	CH24 0x0018           //Channel 24
#define	CH25 0x0019           //Channel 25
#define	CH26 0x001A           //Channel 26
#define	CH27 0x001B           //Channel 27
#define	CH28 0x001C           //Channel 28
#define	CH29 0x001D           //Channel 29
#define	CH30 0x001E           //Channel 30
#define	CH31 0x001F           //Channel 31

/**
*
*  Bitrate configuration options
*
**/

#define	BITRATECSDB_CLK1            0x8000      //CLK1, 20 MHz (default)
#define	BITRATECSDB_CLK2            0x0000      //CLK2, 9.216 MHz (standard option)
#define	BITRATECSDB_XMTSLOPEHIGH    0x0000      //Transmit with high speed slope (default)
#define	BITRATECSDB_XMTSLOPELOW     0x4000      //Transmit with low speed slope

#define	BITRATECSDB_921600BPS   0x0000          //External Clock, Div = 0
#define	BITRATECSDB_460800BPS   0x0001          //External Clock, Div = 1
#define	BITRATECSDB_230400BPS   0x0003          //External Clock, Div = 3
#define	BITRATECSDB_115200BPS   0x0007          //External Clock, Div = 7
#define	BITRATECSDB_57600BPS    0x000F          //External Clock, Div = 15
#define	BITRATECSDB_38400BPS    0x0017          //External Clock, Div = 23
#define	BITRATECSDB_28800BPS    0x001F          //External Clock, Div = 31
#define	BITRATECSDB_19200BPS    0x002F          //External Clock, Div = 47
#define	BITRATECSDB_14400BPS    0x003F          //External Clock, Div = 63
#define	BITRATECSDB_9600BPS     0x005F          //External Clock, Div = 95
#define	BITRATECSDB_7200BPS     0x007F          //External Clock, Div = 127
#define	BITRATECSDB_4800BPS     0x00BF          //External Clock, Div = 191
#define	BITRATECSDB_3600BPS     0x00FF          //External Clock, Div = 255
#define	BITRATECSDB_2400BPS     0x017F          //External Clock, Div = 383
#define	BITRATECSDB_1800BPS     0x01FF          //External Clock, Div = 511
#define	BITRATECSDB_1200BPS     0x02FF          //External Clock, Div = 767
#define	BITRATECSDB_900BPS      0x03FF          //External Clock, Div = 1023
#define	BITRATECSDB_600BPS      0x05FF          //External Clock, Div = 1535
#define	BITRATECSDB_300BPS      0x0BFF          //External Clock, Div = 3071

#define	BITRATECSDB_500000BPS   0x8003          //Internal Clock, Div = 3
#define	BITRATECSDB_400000BPS   0x8004          //Internal Clock, Div = 4
#define	BITRATECSDB_250000BPS   0x8007          //Internal Clock, Div = 7
#define	BITRATECSDB_200000BPS   0x8009          //Internal Clock, Div = 9
#define	BITRATECSDB_100000BPS   0x8013          //Internal Clock, Div = 19
#define	BITRATECSDB_80000BPS    0x8018          //Internal Clock, Div = 24
#define	BITRATECSDB_50000BPS    0x8027          //Internal Clock, Div = 39
#define	BITRATECSDB_40000BPS    0x8031          //Internal Clock, Div = 49
#define	BITRATECSDB_25000BPS    0x804F          //Internal Clock, Div = 79
#define	BITRATECSDB_20000BPS    0x8063          //Internal Clock, Div = 99
#define	BITRATECSDB_12500BPS    0x809F          //Internal Clock, Div = 159
#define	BITRATECSDB_10000BPS    0x80C7          //Internal Clock, Div = 199

#define	BITRATECSDB_HIGHSPEED   0xC027          //Internal Clock, Div = 39
#define	BITRATECSDB_LOWSPEED    0xC09F          //Internal Clock, Div = 159

/**
*
*  SDI selection constants
*
**/

enum {	SIALL                 = 0x00FF,           //Enable all SIs
		SIx00                 = 0x0011,           //Enable SIx00
		SIx01                 = 0x0022,           //Enable SIx01
		SIx10                 = 0x0044,           //Enable SIx10
		SIx11                 = 0x0088,           //Enable SIx11
		SI000                 = 0x0001,           //Enable SI000
		SI001                 = 0x0002,           //Enable SI001
		SI010                 = 0x0004,           //Enable SI010
		SI011                 = 0x0008,           //Enable SI011
		SI100                 = 0x0010,           //Enable SI100
		SI101                 = 0x0020,           //Enable SI101
		SI110                 = 0x0040,           //Enable SI110
		SI111                 = 0x0080            //Enable SI111
};

/**
*
*  Channel configuration options
*
**/


enum {	CHCFGCSDB_DEFAULT      = 0x00000000L,      //Select all default settings (XMT & RCV) (default)
		CHCFGCSDB_SELFTESTOFF  = 0x00000000L,      //Disable internal wraparound (XMT & RCV) (default)
		CHCFGCSDB_SELFTEST     = 0x00000004L,      //Enable internal wraparound (XMT & RCV)
		CHCFGCSDB_PARITYODD    = 0x00000000L,      //Enable odd parity (XMT & RCV) (default)
		CHCFGCSDB_PARITYNONE   = 0x80000000L,      //Disable parity (XMT & RCV)
		CHCFGCSDB_PARITYEVEN   = 0x40000000L,      //Enable even parity (XMT & RCV)
		CHCFGCSDB_PARITYMARK   = 0x20000000L,      //Force set parity (XMT & RCV)
		CHCFGCSDB_PARITYSPACE  = 0x10000000L,      //Force clear parity (XMT & RCV)
		CHCFGCSDB_ACTIVE       = 0x00000000L,      //Enable ehannel activity (XMT & RCV) (default)
		CHCFGCSDB_INACTIVE     = 0x00000040L,      //Disable channel activity (XMT & RCV)
		CHCFGCSDB_SEQSEL       = 0x00000000L,      //Sequential record recording is enabled at message level (default)
		CHCFGCSDB_SEQALL       = 0x00080000L,      //Record entire channel in sequential record
		CHCFGCSDB_NOLOGHALT    = 0x00000000L,      //No event log on schedule halt (default)
		CHCFGCSDB_LOGHALT      = 0x00200000L,      //Enable event log on schedule halt
		CHCFGCSDB_NOLOGPAUSE   = 0x00000000L,      //No event log on schedule pause (default)
		CHCFGCSDB_LOGPAUSE     = 0x00400000L,      //Enable event log on schedule pause
		CHCFGCSDB_NOLOGERR     = 0x00000000L,      //No event log on decoder errors (default)
		CHCFGCSDB_LOGERR       = 0x00800000L,      //Enable event log on decoder errors and sync errors.
		CHCFGCSDB_TIMETAGOFF   = 0x00000000L,      //Time-tag is enabled on message level (default)
		CHCFGCSDB_TIMETAG      = 0x00000400L,      //Enable time-tag for all message records
		CHCFGCSDB_NOHIT        = 0x00000000L,      //Hit counter is enabled on message level (default)
		CHCFGCSDB_HIT          = 0x00000200L,      //Hit counter is enabled for all messages
		CHCFGCSDB_ELAPSEOFF    = 0x00000000L,      //Elapse time is enabled on message level (default)
		CHCFGCSDB_ELAPSE       = 0x00000800L,      //Enable elapse time for all messages
		CHCFGCSDB_MAXMINOFF    = 0x00000000L,      //Repetition rate monitoring is enabled on message level (default)
		CHCFGCSDB_MAX          = 0x00001000L,      //Enable max repetition rate monitoring for all messages
		CHCFGCSDB_MIN          = 0x00002000L,      //Enable min repetition rate monitoring for all messages
		CHCFGCSDB_MAXMIN       = 0x00003000L,      //Enable max and min repetition rate monitoring for all messages
		CHCFGCSDB_NOLOOPMAX    = 0x00000000L,      //Disable schedule maximum loop count (default)
		CHCFGCSDB_LOOPMAX      = 0x00100000L,      //Enable schedule maximum loop count
		CHCFGCSDB_UNPAUSE      = 0x00000000L,      //Mark channel as unpaused (default)
		CHCFGCSDB_PAUSE        = 0x01000000L,      //Mark channel as paused
		CHCFGCSDB_BUSINVERTOFF = 0x00000000L,      //No bus inversion (default)
		CHCFGCSDB_BUSINVERT    = 0x00000001L,      //Enable bus inversion
		CHCFGCSDB_MODE422      = 0x00000000L,      //Enable RS-422 mode (balanced) (XMT) (default)
		CHCFGCSDB_MODE232      = 0x08000000L,      //Enable RS-232 / RS-423 mode (unbalanced) (XMT)
		CHCFGCSDB_SYNCMODE     = 0x00000000L,      //Synchronous, scheduled mode (CSDB) (default)
		CHCFGCSDB_ASYNCMODE    = 0x00000002L       //Asynchrounous mode (general purpose serial)
};

/**
*
*  Message configuration options
*
**/

enum {	MSGCRTCSDB_DEFAULT     = 0x00000000L,      //Default settings
		MSGCRTCSDB_NOSEQ       = 0x00000000L,      //Message will not be recorded in sequential record (default)
		MSGCRTCSDB_SEQ         = 0x00000001L,      //Message will be recorded in sequential record
		MSGCRTCSDB_NOLOG       = 0x00000000L,      //Message will not generate event log (default)
		MSGCRTCSDB_LOG         = 0x00000002L,      //Message will generate event log
		MSGCRTCSDB_NOERROR     = 0x00000000L,      //Message will not generate errors (default)
		MSGCRTCSDB_ERROR       = 0x00000004L,      //Message can generate errors
		MSGCRTCSDB_NOSKIP      = 0x00000000L,      //Message will not be skipped (default)
		MSGCRTCSDB_SKIP        = 0x00000008L,      //Message will be skipped
		MSGCRTCSDB_NOTIMETAG   = 0x00000000L,      //Message will not record time-tag (default)
		MSGCRTCSDB_TIMETAG     = 0x00000010L,      //Message will record time-tag
		MSGCRTCSDB_NOELAPSE    = 0x00000000L,      //Message will not record elapse time (default)
		MSGCRTCSDB_ELAPSE      = 0x00000020L,      //Message will record elapse time
		MSGCRTCSDB_NOMAXMIN    = 0x00000000L,      //Message will not record min/max time (default)
		MSGCRTCSDB_MAX         = 0x00000040L,      //Message will record max time
		MSGCRTCSDB_MIN         = 0x00000080L,      //Message will record min time
		MSGCRTCSDB_MAXMIN      = 0x000000C0L,      //Message will record min/max time
		MSGCRTCSDB_NOHIT       = 0x00000000L,      //Message will not record hit count (default)
		MSGCRTCSDB_HIT         = 0x00000800L,      //Message will record hit count
		MSGCRTCSDB_WIPE        = 0x00000000L,      //Enables message clear (default)
		MSGCRTCSDB_NOWIPE      = 0x80000000L,      //Disables message clear
		MSGCRTCSDB_WIPE0       = 0x00000000L,      //Initialize data with zeros (default)
		MSGCRTCSDB_WIPE1       = 0x40000000L,      //Initialize data with ones
		MSGCRTCSDB_WIPESYNC    = 0x20000000L,      //Initialize data with 0xA5
		MSGCRTCSDB_CONT        = 0x00000000L,      //Selects continuous mode (default)
		MSGCRTCSDB_NONCONT     = 0x00002000L,      //Selects non-continuous mode
		MSGCRTCSDB_BURST       = 0x00004000L       //Selects burst mode
};

/**
*
*  List buffer options
*
**/

enum {	LISTCRTCSDB_DEFAULT    = 0x00000000L,      //Select all default settings
		LISTCRTCSDB_FIFO       = 0x00000000L,      //Enable FIFO mode (default)
		LISTCRTCSDB_PINGPONG   = 0x00000001L,      //Enable ping-pong mode
		LISTCRTCSDB_CIRCULAR   = 0x00000002L,      //Enable circular mode
		LISTCRTCSDB_RCV        = 0x00000010L,      //User will read from list buffer
		LISTCRTCSDB_XMT        = 0x00000020L,      //User will write to list buffer
		LISTCRTCSDB_NOLOG      = 0x00000000L,      //Do not generate event log when list buffer empty/full (default)
		LISTCRTCSDB_LOG        = 0x00000100L       //Generate event log when list buffer empty/full
};

/**
*
*  Parametric configuration options
*
**/

enum {	PARAMCFGCSDB_DEFAULT    = 0x00000000L,     //Select all default settings (default)
		PARAMCFGCSDB_AMPLON     = 0x00000000L,     //Enables parametric amplitude control (default)
		PARAMCFGCSDB_AMPLOFF    = 0x00000001L      //Disables parametric amplitude control
};

/**
*
*  Scheduling conditions.
*
**/

enum {	CONDCSDB_ALWAYS        = 0x00000000L,      //Unconditional
		CONDCSDB_DIO1ACT       = 0x00000001L,      //Condition on digital I/O #1 active
		CONDCSDB_DIO1NACT      = 0x00000002L,      //Condition on digital I/O #1 not active
		CONDCSDB_DIO2ACT       = 0x00000004L,      //Condition on digital I/O #2 active
		CONDCSDB_DIO2NACT      = 0x00000008L,      //Condition on digital I/O #2 not active
		CONDCSDB_DIO3ACT       = 0x00000010L,      //Condition on digital I/O #3 active
		CONDCSDB_DIO3NACT      = 0x00000020L,      //Condition on digital I/O #3 not active
		CONDCSDB_DIO4ACT       = 0x00000040L,      //Condition on digital I/O #4 active
		CONDCSDB_DIO4NACT      = 0x00000080L,      //Condition on digital I/O #4 not active
		CONDCSDB_DIO5ACT       = 0x00000100L,      //Condition on digital I/O #5 active
		CONDCSDB_DIO5NACT      = 0x00000200L,      //Condition on digital I/O #5 not active
		CONDCSDB_DIO6ACT       = 0x00000400L,      //Condition on digital I/O #6 active
		CONDCSDB_DIO6NACT      = 0x00000800L,      //Condition on digital I/O #6 not active
		CONDCSDB_DIO7ACT       = 0x00001000L,      //Condition on digital I/O #7 active
		CONDCSDB_DIO7NACT      = 0x00002000L,      //Condition on digital I/O #7 not active
		CONDCSDB_DIO8ACT       = 0x00004000L,      //Condition on digital I/O #8 active
		CONDCSDB_DIO8NACT      = 0x00008000L       //Condition on digital I/O #8 not active
};

/**
*
*  Bit fields of the message activity flag.
*
**/

enum {	MSGACTCSDB_CHMASK      = 0xFF00,           //Channel number mask value
		MSGACTCSDB_CHSHIFT     = 0x0008,           //Channel number shift value
		MSGACTCSDB_ERR         = 0x0080,           //Error bit
		MSGACTCSDB_BIT         = 0x0040,           //Bit time error bit
		MSGACTCSDB_NRZ         = 0x0020,           //NRZ bit error
		MSGACTCSDB_EDGE        = 0x0010,           //Edge bit error
		MSGACTCSDB_PAR         = 0x0008,           //Parity error bit
		MSGACTCSDB_FRAME       = 0x0004,           //Frame error bit
		MSGACTCSDB_HIT         = 0x0001            //Always set
};


/**
*
*  Channel info fields.
*
**/

enum {	INFOCSDB_PARAM         = 0x0001           //Channel supports parametric control
};

#endif
