/**
*
*  BTI717 RPC DRIVER  Version 1.0.4  (10/23/2009)
*  Copyright (c) 2004-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   BTI717.H -- RPC Driver
*                      BTI717 Driver Include Header File.
*
**/

/**
*
*  This file defines the procedures provided by the RPC
*  Driver Library for Ballard Technology ARINC 717 cards.
*  Applications using the BTI717 Driver Library must
*  incorporate this include file using the preprocessor
*  directive #include. If this file is in the current working
*  directory, the form would be:
*
*  #include "BTI717.H"
*
**/

/**
*
*  Conditional block to prevent multiple defines.
*
**/

#ifndef __BTI717_H
#define __BTI717_H

/**
*
*  Typedefs used by the BTI717 Driver.
*
**/

#ifndef BTI717API
#define BTI717API
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

#ifndef SUBFRMADDR
#define SUBFRMADDR unsigned int
#endif

#ifndef SUPERFRMADDR
#define SUPERFRMADDR unsigned int
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

/**
*
*  Structs used by the BTI717 Driver.
*
**/

#ifndef SUBFRMFIELDS717

typedef struct
{
    UINT16 subfrmopt;           //Driver writes message options
    UINT16 subfrmact;           //Card writes message activity
    UINT16 subfrmnum;           //Card writes subframe number
    UINT16 subfrmwps;           //Card writes WPS
    union {
        UINT32 timestamp;       //Card writes timestamp
        UINT32 hitcount;        //Card writes hit count
    };
    UINT32 spfptr;              //Card writes superframe pointer
    UINT16 data[8192];          //Card/User writes subframe data. Valid from 0 -> WPS-1. (Rcv/Xmt)
    UINT32 timestamph;          //Card writes extended timestamp
    UINT16 extflag;             //Driver writes external trigger/sync control flags
    UINT16 rsvd[13];            //Reserved
} SUBFRMFIELDS717;

#endif

#ifndef LPSUBFRMFIELDS717
typedef SUBFRMFIELDS717 * LPSUBFRMFIELDS717;
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
*  BTI717 Driver functions.
*
**/

BTI717API ERRVAL BTI717_ChClear(INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_ChConfig(UINT32 configval,INT channum,HCARD handleval);
BTI717API VOID BTI717_ChGetCount(LPINT rcvcount,LPINT xmtcount,HCARD handleval);
BTI717API UINT32 BTI717_ChGetInfo(UINT16 infotype,INT channum,HCARD handleval);
BTI717API BOOL BTI717_ChIs717(INT channum,HCARD handleval);
BTI717API BOOL BTI717_ChIsBiPhase(INT channum,HCARD handleval);
BTI717API BOOL BTI717_ChIsRcv(INT channum,HCARD handleval);
BTI717API BOOL BTI717_ChIsSelEnc(INT channum,HCARD handleval);
BTI717API BOOL BTI717_ChIsType(UINT32 configval,INT channum,HCARD handleval);
BTI717API BOOL BTI717_ChIsXmt(INT channum,HCARD handleval);
BTI717API VOID BTI717_ChPause(INT channum,HCARD handleval);
BTI717API INT BTI717_ChPauseCheck(INT channum,HCARD handleval);
BTI717API VOID BTI717_ChResume(INT channum,HCARD handleval);
BTI717API BOOL BTI717_ChStart(INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_ChStatus(LPUINT16 buf,INT channum,HCARD handleval);
BTI717API BOOL BTI717_ChStop(INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_ChSyncDefine(BOOL enableflag,UINT16 syncmask,UINT16 pinpolarity,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_ChTriggerDefine(BOOL enableflag,UINT16 trigmask,UINT16 trigval,UINT16 pinpolarity,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_ParamAmplitudeConfig(UINT32 configval,UINT16 dacval,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_ParamAmplitudeGet(LPUINT16 dacval,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SubFrmBlkRd(LPUINT16 databuf,INT subframe,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SubFrmBlkWr(LPUINT16 databuf,INT subframe,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SubFrmStatus(LPUINT16 buf,INT subframe,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SubFrmSyncDefine(BOOL enableflag,UINT16 syncmask,UINT16 pinpolarity,INT subframe,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SubFrmTriggerDefine(BOOL enableflag,UINT16 trigmask,UINT16 trigval,UINT16 pinpolarity,INT subframe,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SubFrmWordConfig(UINT32 configval,INT subframe,INT word,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SubFrmWordRd(LPUINT16 value,INT subframe,INT word,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SubFrmWordWr(UINT16 value,INT subframe,INT word,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SuperFrmConfig(INT count,INT framespersuperframe,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SuperFrmCounterPos(INT subframe,INT word,INT channum,HCARD handleval);
BTI717API SUPERFRMADDR BTI717_SuperFrmDefine(INT subframe,INT word,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SuperFrmWordConfig(UINT32 configval,INT superfrmnum,SUPERFRMADDR superfrmaddr,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SuperFrmWordRd(LPUINT16 value,INT superfrmnum,SUPERFRMADDR superfrmaddr,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SuperFrmWordWr(UINT16 value,INT superfrmnum,SUPERFRMADDR superfrmaddr,INT channum,HCARD handleval);
BTI717API BOOL BTI717_SyncTest(INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SyncValRd(LPINT syncword1,LPINT syncword2,LPINT syncword3,LPINT syncword4,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SyncValWr(INT syncword1,INT syncword2,INT syncword3,INT syncword4,INT channum,HCARD handleval);

/**
*
*  Internal functions used by the BTI717 Driver.
*
**/

BTI717API BASEADDR BTI717_ChCreate(INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_ChReset(INT channum,HCARD handleval);
BTI717API INT BTI717_DllUsageCount(VOID);
BTI717API VOID BTI717_GetChanCount(LPINT rcvcount,LPINT xmtcount,HCARD handleval);
BTI717API BOOL BTI717_IsBiPhaseChan(INT channum,HCARD handleval);
BTI717API BOOL BTI717_IsChan(INT channum,HCARD handleval);
BTI717API BOOL BTI717_IsRcvChan(INT channum,HCARD handleval);
BTI717API BOOL BTI717_IsXmtChan(INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_ProcSetPost(UINT32 useraddr,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_ProcSetPre(UINT32 useraddr,INT channum,HCARD handleval);
BTI717API SUBFRMADDR BTI717_SubFrmAddr(INT subframe,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SubFrmBlkRdEx(LPSUBFRMFIELDS717 subfrmdata,INT subframe,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SubFrmBlkWrEx(LPSUBFRMFIELDS717 subfrmdata,INT subframe,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SubFrmCreate(UINT32 configval,INT subframe,BASEADDR chanbase,HCARD handleval);
BTI717API ERRVAL BTI717_SubFrmReset(INT subframe,INT channum,HCARD handleval);
BTI717API BASEADDR BTI717_SuperFrmBaseAddr(INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SuperFrmConfigEx(UINT32 configval,INT count,INT framespersuperframe,INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SuperFrmReset(INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_SyncValPtrInit(INT channum,HCARD handleval);
BTI717API ERRVAL BTI717_TestProtocol(HCARD handleval);

#ifdef __cplusplus
}
#endif

/**
*
*  Channel number constants
*
**/

#define CH0  0x0000          //Channel 0
#define	CH1  0x0001          //Channel 1
#define	CH2  0x0002          //Channel 2
#define	CH3  0x0003          //Channel 3
#define	CH4  0x0004          //Channel 4
#define	CH5  0x0005          //Channel 5
#define	CH6  0x0006          //Channel 6
#define	CH7  0x0007          //Channel 7
#define	CH8  0x0008          //Channel 8
#define	CH9  0x0009          //Channel 9
#define	CH10 0x000A          //Channel 10
#define	CH11 0x000B          //Channel 11
#define	CH12 0x000C          //Channel 12
#define	CH13 0x000D          //Channel 13
#define	CH14 0x000E          //Channel 14
#define	CH15 0x000F          //Channel 15
#define	CH16 0x0010          //Channel 16
#define	CH17 0x0011          //Channel 17
#define	CH18 0x0012          //Channel 18
#define	CH19 0x0013          //Channel 19
#define	CH20 0x0014          //Channel 20
#define	CH21 0x0015          //Channel 21
#define	CH22 0x0016          //Channel 22
#define	CH23 0x0017          //Channel 23
#define	CH24 0x0018          //Channel 24
#define	CH25 0x0019          //Channel 25
#define	CH26 0x001A          //Channel 26
#define	CH27 0x001B          //Channel 27
#define	CH28 0x001C          //Channel 28
#define	CH29 0x001D          //Channel 29
#define	CH30 0x001E          //Channel 30
#define	CH31 0x001F          //Channel 31

/**
*
*  Subframe constants
*
**/

enum {	SUBFRM1                = 0x0001,          //Sub Frame 1
		SUBFRM2                = 0x0002,          //Sub Frame 2
		SUBFRM3                = 0x0003,          //Sub Frame 3
		SUBFRM4                = 0x0004           //Sub Frame 4
};

/**
*
*  Channel configuration options
*
**/


enum {	CHCFG717_DEFAULT       = 0x00000000L,     //Select all default settings (XMT & RCV) (default)
		CHCFG717_AUTOSPEED     = 0x00000002L,     //Select auto speed detection (RCV)
		CHCFG717_64WPS         = 0x00004000L,     //Select speed of 64WPS (XMT & RCV)
		CHCFG717_128WPS        = 0x00008000L,     //Select speed of 128WPS (XMT & RCV)
		CHCFG717_256WPS        = 0x00000000L,     //Select speed of 256WPS (XMT & RCV) (default)
		CHCFG717_512WPS        = 0x00010000L,     //Select speed of 512WPS (XMT & RCV)
		CHCFG717_1024WPS       = 0x00020000L,     //Select speed of 1024WPS (XMT & RCV)
		CHCFG717_2048WPS       = 0x00040000L,     //Select speed of 2048WPS (XMT & RCV)
		CHCFG717_4096WPS       = 0x02000000L,     //Select speed of 4096WPS (XMT & RCV)
		CHCFG717_8192WPS       = 0x04000000L,     //Select speed of 8192WPS (XMT & RCV)
		CHCFG717_BIPHASERCV    = 0x00000000L,     //Select Bi-Phase decoding for receiver (Bi-Phase or RCV) (default)
		CHCFG717_BIPHASEXMT    = 0x08000000L,     //Select Bi-Phase encoding for transmitter (Bi-Phase or XMT)
		CHCFG717_BIPOLARRCV    = 0x20000000L,     //Select Bi-Polar decoding for receiver (Bi-Polar or RCV)
		CHCFG717_BIPOLARXMT    = 0x60000000L,     //Select Bi-Polar encoding for transmitter (Bi-Polar or XMT)
		CHCFG717_BIPOLARHSPD   = 0x08000000L,     //Select high speed slope for Bi-Polar transmitter (Bi-Polar XMT)
		CHCFG717_SELFTEST      = 0x00000004L,     //Enable internal wraparound (XMT & RCV)
		CHCFG717_SELFTESTOFF   = 0x00000000L,     //Disable internal wraparound (XMT & RCV) (default)
		CHCFG717_SYNC          = 0x00000008L,     //Sync Enable (XMT & RCV)
		CHCFG717_SYNCOFF       = 0x00000000L,     //Disable sync output (XMT & RCV) (default)
		CHCFG717_ACTIVE        = 0x00000000L,     //Enable ehannel activity (XMT & RCV) (default)
		CHCFG717_INACTIVE      = 0x00000040L,     //Disable channel activity (XMT & RCV)
		CHCFG717_EXTSTART      = 0x00000100L,     //Enable external trigger for first message only (XMT)
		CHCFG717_EXTTRIG       = 0x00000080L,     //Enable external trigger for all messages (XMT)
		CHCFG717_EXTOFF        = 0x00000000L,     //External trigger is enabled on message level (XMT) (default)
		CHCFG717_HIT           = 0x00000200L,     //Hit counter is enabled for all sub frames (XMT & RCV)
		CHCFG717_NOHIT         = 0x00000000L,     //Hit counter is disabled (XMT & RCV) (default)
		CHCFG717_TIMETAG       = 0x00000400L,     //Enable time-tag for all sub frames (XMT & RCV)
		CHCFG717_TIMETAGOFF    = 0x00000000L,     //Time-tag is disabled (XMT & RCV) (default)
		CHCFG717_SEQALL        = 0x00080000L,     //Record entire channel in sequential record (XMT & RCV)
		CHCFG717_SEQSEL        = 0x00000000L,     //Sequential record recording is enabled at word level (XMT & RCV) (default)
		CHCFG717_LOGERR        = 0x00800000L,     //Enable event log on Out of Sync Error (RCV)
		CHCFG717_NOLOGERR      = 0x00000000L,     //No event log on Out of Sync Error (RCV) (default)
		CHCFG717_PAUSE         = 0x01000000L,     //Mark channel as paused (XMT & RCV)
		CHCFG717_UNPAUSE       = 0x00000000L,     //Mark channel as unpaused (XMT & RCV) (default)
		CHCFG717_XMTSLOPELOW   = 0x00000000L,     //Select low speed slope for transmitter (XMT) (default)
		CHCFG717_XMTSLOPEHIGH  = 0x10000000L      //Select high speed slope for transmitter (XMT)
};

/**
*
*  Sub Frame word configuration options
*
**/

enum {	WRDCFG717_DEFAULT      = 0x00000000L,     //Select all default settings
		WRDCFG717_SFLOG        = 0x00000001L,     //Enable subframe event log
		WRDCFG717_SFLOGOFF     = 0x00000000L,     //Disable subframe event log (default)
		WRDCFG717_WRDLOG       = 0x00000002L,     //Enable word event log
		WRDCFG717_WRDLOGOFF    = 0x00000000L,     //Disable word event log (default)
		WRDCFG717_SEQ          = 0x00000004L,     //Enable sequential record
		WRDCFG717_SEQOFF       = 0x00000000L      //Disable sequential record (default)
};

/**
*
*  Parametric configuration options
*
**/

enum {	PARAMCFG717_DEFAULT    = 0x00000000L,     //Select all default settings (default)
		PARAMCFG717_AMPLON     = 0x00000000L,     //Enables parametric amplitude control (default)
		PARAMCFG717_AMPLOFF    = 0x00000001L      //Disables parametric amplitude control
};

/**
*
*  Bit fields of the message activity flag.
*
**/

enum {	MSGACT717_CHMASK       = 0xFF00,          //Channel number mask value
		MSGACT717_CHSHIFT      = 0x0008,          //Channel number shift value
		MSGACT717_SPDMASK      = 0x00E0,          //Current speed mask value
		MSGACT717_64WPS        = 0x0000,          //Current speed is 64   wps
		MSGACT717_128WPS       = 0x0020,          //Current speed is 128  wps
		MSGACT717_256WPS       = 0x0040,          //Current speed is 256  wps
		MSGACT717_512WPS       = 0x0060,          //Current speed is 512  wps
		MSGACT717_1024WPS      = 0x0080,          //Current speed is 1024 wps
		MSGACT717_2048WPS      = 0x00A0,          //Current speed is 2048 wps
		MSGACT717_4096WPS      = 0x00C0,          //Current speed is 4096 wps
		MSGACT717_8192WPS      = 0x00E0,          //Current speed is 8192 wps
		MSGACT717_TO           = 0x0002,          //Time out error bit
		MSGACT717_HIT          = 0x0001           //Always set
};

/**
*
*  Channel info fields.
*
**/

enum {	INFO717_PARAM         = 0x0001           //Channel supports parametric control
};

#endif
