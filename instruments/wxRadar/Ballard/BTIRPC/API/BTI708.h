/**
*
*  BTI708 RPC DRIVER  Version 1.1.0  (10/23/2009)
*  Copyright (c) 2004-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   BTI708.H -- RPC Driver
*                      BTI708 Driver Include Header File.
*
**/

/**
*
*  This file defines the procedures provided by the RPC
*  Driver Library for Ballard Technology ARINC 708 cards.
*  Applications using the BTI708 Driver Library must
*  incorporate this include file using the preprocessor
*  directive #include. If this file is in the current working
*  directory, the form would be:
*
*  #include "BTI708.H"
*
**/

/**
*
*  Conditional block to prevent multiple defines.
*
**/

#ifndef __BTI708_H
#define __BTI708_H

/**
*
*  Typedefs used by the BTI708 Driver.
*
**/

#ifndef BTI708API
#define BTI708API
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

#ifndef FLOAT
#define FLOAT float
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

#ifndef MSGNDX
#define MSGNDX int
#endif

/**
*
*  Structs used by the BTI708 Driver.
*
**/

#ifndef MSGFIELDS708

typedef struct
{
    UINT16 msgopt;              //Driver writes message options
    UINT16 msgact;              //Card writes message activity
    union {
        UINT32 timetag;         //Card writes time tag
        UINT32 hitcount;        //Card writes hit count
    };
    union {
        UINT32 elapsetime;      //Card writes elapsed time
        UINT32 maxtime;         //Card writes max time
        UINT32 mintime;         //Card writes min time
    };
    UINT32 userptr;             //Driver writes user code pointer
    UINT16 framenum;            //Frame Number
    UINT16 msgcount;            //Card / Driver writes message bit count (used in variable bit length mode)
    UINT32 timetagh;            //Card writes extended timetag
    UINT16 extflag;             //Driver writes external flags
    UINT16 rsvd5;               //Reserved
    UINT16 rsvd6;               //Reserved
    UINT16 rsvd7;               //Reserved
    UINT16 data[100];           //Card / User writes message data
    UINT16 extra[16];           //Card / User writes extra data (used in variable bit length mode)
} MSGFIELDS708;

#endif

#ifndef LPMSGFIELDS708
typedef MSGFIELDS708 * LPMSGFIELDS708;
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
*  BTI708 Driver functions.
*
**/

BTI708API ERRVAL BTI708_ChClear(INT channum,HCARD handleval);
BTI708API ERRVAL BTI708_ChConfig(UINT32 configval,INT channum,HCARD handleval);
BTI708API VOID BTI708_ChGetCount(LPINT rcvcount,LPINT xmtcount,HCARD handleval);
BTI708API UINT32 BTI708_ChGetInfo(UINT16 infotype,INT channum,HCARD handleval);
BTI708API BOOL BTI708_ChIs708(INT channum,HCARD handleval);
BTI708API BOOL BTI708_ChIsRcv(INT channum,HCARD handleval);
BTI708API BOOL BTI708_ChIsXmt(INT channum,HCARD handleval);
BTI708API VOID BTI708_ChPause(INT channum,HCARD handleval);
BTI708API INT BTI708_ChPauseCheck(INT channum,HCARD handleval);
BTI708API VOID BTI708_ChResume(INT channum,HCARD handleval);
BTI708API BOOL BTI708_ChStart(INT channum,HCARD handleval);
BTI708API BOOL BTI708_ChStop(INT channum,HCARD handleval);
BTI708API ERRVAL BTI708_ChSyncDefine(BOOL enableflag,UINT16 syncmask,UINT16 pinpolarity,INT channum,HCARD handleval);
BTI708API ERRVAL BTI708_ChTriggerDefine(BOOL enableflag,UINT16 trigmask,UINT16 trigval,UINT16 pinpolarity,INT channum,HCARD handleval);
BTI708API UINT16 BTI708_FieldGetAngle(LPUINT16 buf);
BTI708API FLOAT BTI708_FieldGetAngleFl(LPUINT16 buf);
BTI708API UINT16 BTI708_FieldGetControlAccept(LPUINT16 buf);
BTI708API UINT16 BTI708_FieldGetData(INT binnum,LPUINT16 buf);
BTI708API UINT16 BTI708_FieldGetDataAccept(LPUINT16 buf);
BTI708API UINT16 BTI708_FieldGetFaults(LPUINT16 buf);
BTI708API INT BTI708_FieldGetGain(LPUINT16 buf);
BTI708API UINT16 BTI708_FieldGetLabel(LPUINT16 buf);
BTI708API UINT16 BTI708_FieldGetMode(LPUINT16 buf);
BTI708API INT BTI708_FieldGetRange(LPUINT16 buf);
BTI708API UINT16 BTI708_FieldGetTilt(LPUINT16 buf);
BTI708API FLOAT BTI708_FieldGetTiltFl(LPUINT16 buf);
BTI708API UINT16 BTI708_FieldPutAngle(UINT16 value,LPUINT16 buf);
BTI708API FLOAT BTI708_FieldPutAngleFl(FLOAT angle,LPUINT16 buf);
BTI708API UINT16 BTI708_FieldPutControlAccept(UINT16 value,LPUINT16 buf);
BTI708API UINT16 BTI708_FieldPutData(UINT16 value,INT binnum,LPUINT16 buf);
BTI708API UINT16 BTI708_FieldPutDataAccept(UINT16 value,LPUINT16 buf);
BTI708API UINT16 BTI708_FieldPutFaults(UINT16 value,LPUINT16 buf);
BTI708API INT BTI708_FieldPutGain(INT gain,LPUINT16 buf);
BTI708API UINT16 BTI708_FieldPutLabel(UINT16 value,LPUINT16 buf);
BTI708API UINT16 BTI708_FieldPutMode(UINT16 value,LPUINT16 buf);
BTI708API INT BTI708_FieldPutRange(INT range,LPUINT16 buf);
BTI708API UINT16 BTI708_FieldPutTilt(UINT16 value,LPUINT16 buf);
BTI708API FLOAT BTI708_FieldPutTiltFl(FLOAT tilt,LPUINT16 buf);
BTI708API ERRVAL BTI708_MsgBlockRd(LPMSGFIELDS708 msgfields,MSGNDX msgindex,INT channum,HCARD handleval);
BTI708API ERRVAL BTI708_MsgBlockWr(LPMSGFIELDS708 msgfields,MSGNDX msgindex,INT channum,HCARD handleval);
BTI708API ERRVAL BTI708_MsgDataRd(LPUINT16 buf,MSGNDX msgindex,INT channum,HCARD handleval);
BTI708API ERRVAL BTI708_MsgDataWr(LPUINT16 buf,MSGNDX msgindex,INT channum,HCARD handleval);
BTI708API BOOL BTI708_MsgIsAccessed(MSGNDX msgindex,INT channum,HCARD handleval);
BTI708API ERRVAL BTI708_MsgSyncDefine(BOOL enableflag,UINT16 syncmask,UINT16 pinpolarity,MSGNDX msgindex,INT channum,HCARD handleval);
BTI708API ERRVAL BTI708_MsgTriggerDefine(BOOL enableflag,UINT16 trigmask,UINT16 trigval,UINT16 pinpolarity,MSGNDX msgindex,INT channum,HCARD handleval);
BTI708API ERRVAL BTI708_MsgVarDataRd(LPUINT16 buf,LPINT bitcount,MSGNDX msgindex,INT channum,HCARD handleval);
BTI708API ERRVAL BTI708_MsgVarDataWr(LPUINT16 buf,INT bitcount,MSGNDX msgindex,INT channum,HCARD handleval);
BTI708API ERRVAL BTI708_ParamAmplitudeConfig(UINT32 configval,UINT16 dacval,INT channum,HCARD handleval);
BTI708API ERRVAL BTI708_RcvBuild(UINT32 configval,INT logfreq,INT framecount,INT channum,HCARD handleval);
BTI708API ERRVAL BTI708_XmtBuild(UINT32 configval,UINT16 framefreq,INT logfreq,INT framecount,INT channum,HCARD handleval);

/**
*
*  Internal functions used by the BTI708 Driver.
*
**/

BTI708API ERRVAL BTI708_ChConfigEx(UINT32 configval,UINT16 count,INT channum,HCARD handleval);
BTI708API UINT16 BTI708_ChDataCountRd(INT channum,HCARD handleval);
BTI708API UINT16 BTI708_ChDataCountWr(UINT16 countval,INT channum,HCARD handleval);
BTI708API INT BTI708_DllUsageCount(VOID);
BTI708API VOID BTI708_GetChanCount(LPINT rcvcount,LPINT xmtcount,HCARD handleval);
BTI708API BOOL BTI708_IsChan(INT channum,HCARD handleval);
BTI708API BOOL BTI708_IsRcvChan(INT channum,HCARD handleval);
BTI708API BOOL BTI708_IsXmtChan(INT channum,HCARD handleval);
BTI708API UINT16 BTI708_MsgActivityRd(MSGNDX msgindex,INT channum,HCARD handleval);
BTI708API VOID BTI708_MsgActivityWr(UINT16 value,MSGNDX msgindex,INT channum,HCARD handleval);
BTI708API MSGADDR BTI708_MsgAddr(MSGNDX msgindex,INT channum,HCARD handleval);
BTI708API ERRVAL BTI708_MsgCommRd(LPMSGFIELDS708 msgfields,MSGNDX msgindex,INT channum,HCARD handleval);
BTI708API ERRVAL BTI708_MsgCommWr(LPMSGFIELDS708 msgfields,MSGNDX msgindex,INT channum,HCARD handleval);
BTI708API ERRVAL BTI708_MsgConfig(UINT32 configval,MSGADDR msgaddr,HCARD handleval);
BTI708API MSGADDR BTI708_MsgCreate(UINT32 configval,HCARD handleval);
BTI708API UINT16 BTI708_MsgOptRd(MSGADDR msgaddr,HCARD handleval);
BTI708API VOID BTI708_MsgOptWr(UINT16 msgopt,MSGADDR msgaddr,HCARD handleval);
BTI708API VOID BTI708_MsgReset(MSGADDR msgaddr,HCARD handleval);
BTI708API BASEADDR BTI708_MsgTableCreate(INT count,HCARD handleval);
BTI708API MSGADDR BTI708_MsgTableRd(MSGNDX msgindex,INT channum,HCARD handleval);
BTI708API VOID BTI708_MsgTableReset(INT channum,HCARD handleval);
BTI708API VOID BTI708_MsgTableWipe(BASEADDR baseaddr,HCARD handleval);
BTI708API ERRVAL BTI708_MsgTableWr(MSGADDR msgaddr,INT channum,HCARD handleval);
BTI708API ERRVAL BTI708_ParamAmplitudeGet(LPUINT16 dacval,INT channum,HCARD handleval);
BTI708API ERRVAL BTI708_ProcSetPost(UINT32 useraddr,INT channum,HCARD handleval);
BTI708API ERRVAL BTI708_ProcSetPre(UINT32 useraddr,INT channum,HCARD handleval);
BTI708API ERRVAL BTI708_TestProtocol(HCARD handleval);

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
*  Channel configuration options
*
**/

enum {	CHCFG708_DEFAULT      = 0x00000000L,      //Select all default settings (XMT & RCV) (default)
		CHCFG708_NOSWAP       = 0x00000000L,      //Select 708 bit order (XMT & RCV) (default)
		CHCFG708_SWAP         = 0x00000001L,      //Select 1553 bit order (XMT & RCV)
		CHCFG708_WORDSYNC     = 0x00000000L,      //Select resync on words (RCV) (default)
		CHCFG708_BITSYNC      = 0x00000002L,      //Select resync on every bit (RCV)
		CHCFG708_SELFTEST     = 0x00000004L,      //Enable internal wraparound (XMT & RCV)
		CHCFG708_SELFTESTOFF  = 0x00000000L,      //Disable internal wraparound (XMT & RCV) (default)
		CHCFG708_SYNCSEL      = 0x00000000L,      //Sync oputput controlled at message level (XMT) (default)
		CHCFG708_SYNCALL      = 0x00000008L,      //Sync output for all messages (XMT & RCV)
		CHCFG708_ERRSYNCOFF   = 0x00000000L,      //Disable pulsing of DOUT for errors (RCV) (default)
		CHCFG708_ERRSYNCON    = 0x00000010L,      //Enable pulsing of DOUT for errors (RCV)
		CHCFG708_EXTTRIG      = 0x00000020L,      //Enable external trigger for all messages (XMT)
		CHCFG708_EXTOFF       = 0x00000000L,      //External trigger is enabled on message level (XMT) (default)
		CHCFG708_TRIGNONE     = 0x00000000L,      //No trigger (XMT) (default)
		CHCFG708_TRIGEXT      = 0x00000040L,      //Trigger external (XMT)
		CHCFG708_TRIGSTART    = 0x00000080L,      //Trigger external start (XMT)
		CHCFG708_ACTIVE       = 0x00000000L,      //Enable channel (XMT & RCV) (default)
		CHCFG708_INACTIVE     = 0x00000100L,      //Disable channel (XMT & RCV)
		CHCFG708_HIT	      = 0x00000200L,      //Hit counter is enabled for all messages (XMT & RCV)
		CHCFG708_NOHIT		  = 0x00000000L,      //Hit counter is enabled on message level (XMT & RCV) (default)
		CHCFG708_TIMETAG      = 0x00000400L,      //Enable time-tag for all message records (XMT & RCV)
		CHCFG708_TIMETAGOFF   = 0x00000000L,      //Time-tag is enabled on message level (XMT & RCV) (default)
		CHCFG708_ELAPSE       = 0x00000800L,      //Enable elapse time for all messages (XMT & RCV)
		CHCFG708_ELAPSEOFF    = 0x00000000L,      //Elapse time is enabled on message level (XMT & RCV) (default)
		CHCFG708_MAX          = 0x00001000L,      //Enable max repetition rate monitoring for all messages (XMT & RCV)
		CHCFG708_MAXOFF       = 0x00000000L,      //Repetition rate monitoring is enabled on message level (XMT & RCV) (default)
		CHCFG708_MIN          = 0x00002000L,      //Enable min repetition rate monitoring for all messages (XMT & RCV)
		CHCFG708_MINOFF       = 0x00000000L,      //Repetition rate monitoring is enabled on message level (XMT & RCV) (default)
		CHCFG708_SEQALL       = 0x00080000L,      //Record entire channel in sequential record (XMT & RCV)
		CHCFG708_SEQSEL       = 0x00000000L,      //Sequential record recording is enabled at message level (XMT & RCV) (default)
		CHCFG708_PAUSE        = 0x01000000L,      //Mark channel as paused (XMT & RCV)
		CHCFG708_UNPAUSE      = 0x00000000L,      //Mark channel as unpaused (XMT & RCV) (default)
		CHCFG708_RCVBUSA      = 0x00000000L,      //Receive channel will operate on Bus A (RCV) (default)
		CHCFG708_RCVBUSB      = 0x00004000L,      //Receive channel will operate on Bus B (RCV)
		CHCFG708_XMTBUSA      = 0x00004000L,      //Transmit channel will operate on Bus A (XMT)
		CHCFG708_XMTBUSB      = 0x00000000L,      //Transmit channel will operate on Bus B (XMT) (default)
		CHCFG708_TERMOFF      = 0x00000000L,      //Direct coupled termination resistance off (XMT & RCV) (default)
		CHCFG708_TERMON       = 0x00008000L,      //Direct coupled termination resistance on (XMT & RCV)
		CHCFG708_FIXBIT       = 0x00000000L,      //Channel operates with fixed bit length (XMT & RCV) (default)
		CHCFG708_VARBIT       = 0x00010000L,      //Channel operates with variable bit length (XMT & RCV)
		CHCFG708_PRESYNCOFF   = 0x00000000L,      //Pre-sync pulses off (XMT) (default)
		CHCFG708_PRESYNCON    = 0x00020000L       //Pre-sync pulses on (XMT)
};

/**
*
*  Message configuration options
*
**/

enum {	MSGCRT708_DEFAULT     = 0x00000000L,      //Default settings
		MSGCRT708_NOSEQ       = 0x00000000L,      //Message will not be recorded in sequential record (default)
		MSGCRT708_SEQ         = 0x00000001L,      //Message will be recorded in sequential record
		MSGCRT708_NOLOG       = 0x00000000L,      //Message will not generate event log (default)
		MSGCRT708_LOG         = 0x00000002L,      //Message will generate event log
		MSGCRT708_NOSKIP      = 0x00000000L,      //Message will not be skipped (default)
		MSGCRT708_SKIP        = 0x00000008L,      //Message will be skipped
		MSGCRT708_NOTIMETAG   = 0x00000000L,      //Message will not record time-tag (default)
		MSGCRT708_TIMETAG     = 0x00000010L,      //Message will record time-tag
		MSGCRT708_NOELAPSE    = 0x00000000L,      //Message will not record elapse time (default)
		MSGCRT708_ELAPSE      = 0x00000020L,      //Message will record elapse time
		MSGCRT708_NOMAX       = 0x00000000L,      //Message will not record max time (default)
		MSGCRT708_MAX         = 0x00000040L,      //Message will record max time
		MSGCRT708_NOMIN       = 0x00000000L,      //Message will not record min time (default)
		MSGCRT708_MIN         = 0x00000080L,      //Message will record min time
		MSGCRT708_MAXMIN      = 0x000000C0L,      //Message will record min/max time
		MSGCRT708_NOSYNC      = 0x00000000L,      //Message will not generate a syncout pulse (default)
		MSGCRT708_SYNC        = 0x00000100L,      //Message will generate a syncout pulse
		MSGCRT708_NOHIT       = 0x00000000L,      //Message will not record hit count (default)
		MSGCRT708_HIT         = 0x00000400L,      //Message will record hit count
		MSGCRT708_NOEXTRIG    = 0x00000000L,      //Message will not be externally triggered (default)
		MSGCRT708_EXTRIG      = 0x00001000L       //Message will be externally triggered
};

/**
*
*  Parametric configuration options
*
**/

enum {	PARAMCFG708_DEFAULT   = 0x00000000L,      //Select all default settings (default)
		PARAMCFG708_AMPLON    = 0x00000000L,      //Enables parametric amplitude control (default)
		PARAMCFG708_AMPLOFF   = 0x00000001L       //Disables parametric amplitude control
};

/**
*
*  Bit fields of the message activity flag.
*
**/

enum {	MSGACT708_CHMASK      = 0xFF00,           //Channel number mask value
		MSGACT708_CHSHIFT     = 0x0008,           //Channel number shift value
		MSGACT708_ERR         = 0x0080,           //Error bit
		MSGACT708_SHORT       = 0x0040,           //Short word error bit
		MSGACT708_LONG        = 0x0020,           //Long word error bit
		MSGACT708_MANCH       = 0x0010,           //Manchester error bit
		MSGACT708_DSYNC       = 0x0008,           //Data sync error bit
		MSGACT708_WORD        = 0x0004,           //Word error bit
		MSGACT708_TO          = 0x0002,           //Time out error bit
		MSGACT708_HIT         = 0x0001            //Always set
};

/**
*
*  Channel info fields.
*
**/

enum {	INFO708_PARAM	      = 0x0001,           //Channel supports parametric control
		INFO708_VARBIT        = 0x0002            //Channel supports variable bit length mode
};

#endif
