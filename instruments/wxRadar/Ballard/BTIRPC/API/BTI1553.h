/**
*
*  BTI1553 RPC DRIVER  Version 1.1.4  (10/23/2009)
*  Copyright (c) 2004-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   BTI1553.H -- RPC Driver
*                       BTI1553 Driver Include Header File.
*
**/

/**
*
*  This file defines the procedures provided by the RPC
*  Driver Library for Ballard Technology MIL-STD-1553 cards.
*  Applications using the BTI1553 Driver Library must
*  incorporate this include file using the preprocessor directive
*  #include.  If this file is in the current working directory,
*  the form would be:
*
*  #include "BTI1553.H"
*
**/

/**
*
*  Conditional block to prevent multiple defines.
*
**/

#ifndef __BTI1553_H
#define __BTI1553_H

/**
*
*  Typedefs used by the BTI1553 Driver.
*
**/

#ifndef BTI1553API
#define BTI1553API
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
*  Structs used by the BTI1553 Driver.
*
**/

#ifndef XMITFIELDS1553

typedef struct
{
    UINT32 ctrlflags;           //User writes message configuration options
    UINT16 flag1;               //Card writes message flag 1
    UINT16 flag2;               //Card writes message flag 2
    UINT16 errflags;            //Card writes error flags
    UINT16 actflags;            //Card writes activity flags
    UINT16 resptime1;           //Card writes response time 1
    UINT16 resptime2;           //Card writes response time 2
    UINT16 datacount;           //Card writes data count
    UINT16 extflag;             //Driver writes external flags
    UINT32 timetag;             //Card writes time tag
    UINT32 elapsetime;          //Card writes elapsed time
	UINT32 preaddr;             //Driver writes pre-proc address
	UINT32 postaddr;            //Driver writes post-proc address
	UINT32 timetagh;            //Card writes extended timetag
    UINT16 resv18;              //Reserved field
    UINT16 resv19;              //Reserved field
    UINT16 cwd1;                //User writes command word 1
    UINT16 cwd2;                //User writes command word 2
    UINT16 swd1;                //Card writes status word 1
    UINT16 swd2;                //Card writes status word 2
    UINT16 data[32];            //User writes data words
    UINT16 extra[8];            //User writes extra words
} XMITFIELDS1553;

#endif

#ifndef LPXMITFIELDS1553
typedef XMITFIELDS1553 * LPXMITFIELDS1553;
#endif

#ifndef MSGFIELDS1553

typedef struct
{
    UINT16 msgflag;             //Driver writes message flags
    UINT16 msgopt;              //Driver writes message options
    UINT16 msgerr;              //Card writes message errors
    UINT16 msgact;              //Card writes message activity
    UINT16 resptime1;           //Card writes response time 1
    UINT16 resptime2;           //Card writes response time 2
    UINT16 datacount;           //Card writes data count
    UINT16 extflag;             //Driver writes external flags
    union {
		UINT32 timetag;         //Card writes time tag
		UINT32 hitcount;        //Card writes hit count
	};
	union {
		UINT32 elapsetime;      //Card writes elapsed time
		UINT32 mintime;         //Card writes min time
		UINT32 maxtime;         //Card writes max time
    };
	UINT32 preaddr;             //Driver writes pre-proc address
	UINT32 postaddr;            //Driver writes post-proc address
	UINT32 timetagh;            //Card writes extended timetag
    UINT16 resv18;              //Reserved field
    UINT16 resv19;              //Reserved field
    UINT16 cwd1;                //Card/User writes command word 1 (R/T)
    UINT16 cwd2;                //Card/User writes command word 2 (R/T)
    UINT16 swd1;                //Card writes status word 1
    UINT16 swd2;                //Card writes status word 2
    UINT16 data[32];            //User/Card writes data words (R/T)
    UINT16 extra[8];            //User/Card writes extra words (R/T)
} MSGFIELDS1553;

#endif

#ifndef LPMSGFIELDS1553
typedef MSGFIELDS1553 * LPMSGFIELDS1553;
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
*  BTI1553 Driver functions.
*
**/

BTI1553API ERRVAL BTI1553_BCConfig(UINT32 configval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BCConfigEx(UINT32 configval,UINT16 count,INT channum,HCARD handleval);
BTI1553API MSGADDR BTI1553_BCConfigMsg(UINT32 configval,MSGADDR msgaddr,HCARD handleval);
BTI1553API LISTADDR BTI1553_BCCreateList(UINT32 listconfigval,INT count,UINT32 msgconfigval,UINT16 cwd1,UINT16 cwd2,LPUINT16 data,HCARD handleval);
BTI1553API MSGADDR BTI1553_BCCreateMsg(UINT32 configval,UINT16 cwd1,UINT16 cwd2,LPUINT16 data,HCARD handleval);
BTI1553API VOID BTI1553_BCPause(INT channum,HCARD handleval);
BTI1553API INT BTI1553_BCPauseCheck(INT channum,HCARD handleval);
BTI1553API VOID BTI1553_BCResume(INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedAgain(SCHNDX index,INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedBranch(UINT32 condition,SCHNDX destindex,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BCSchedBranchUpdate(SCHNDX destindex,SCHNDX opcodeindex,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BCSchedBuild(UINT16 nummsgs,LPMSGADDR msgaddr,LPUINT16 freq,INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedCall(UINT32 condition,SCHNDX destindex,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BCSchedCallUpdate(SCHNDX destindex,SCHNDX opcodeindex,INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedEntry(INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedFrame(UINT32 timeval,INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedFrameEnd(INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedFrameStart(UINT32 timeval,INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedGap(UINT16 gapval,INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedHalt(INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedLog(UINT32 condition,UINT16 tagval,INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedMsg(MSGADDR msgaddr,INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedPause(INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedPulse(INT dionum,INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedPulse0(INT dionum,INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedPulse1(INT dionum,INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedRestart(INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedRetry(UINT32 condition,UINT16 retries,INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedReturn(INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BCSetDefaultGap(INT gapval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BCSetTimeout(UINT16 timeoutval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BCSyncDefine(BOOL enableflag,UINT16 syncmask,UINT16 pinpolarity,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BCTransmitMsg(LPXMITFIELDS1553 xmitfields,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BCTriggerDefine(BOOL enableflag,UINT16 trigmask,UINT16 trigval,UINT16 pinpolarity,INT channum,HCARD handleval);
BTI1553API VOID BTI1553_ChGetCount(LPINT a_count,LPINT b4_count,LPINT b32_count,LPINT c_count,HCARD handleval);
BTI1553API UINT32 BTI1553_ChGetInfo(UINT16 infotype,INT channum,HCARD handleval);
BTI1553API BOOL BTI1553_ChIs1553(INT channum,HCARD handleval);
BTI1553API BOOL BTI1553_ChIsA(INT channum,HCARD handleval);
BTI1553API BOOL BTI1553_ChIsB32(INT channum,HCARD handleval);
BTI1553API BOOL BTI1553_ChIsB4(INT channum,HCARD handleval);
BTI1553API BOOL BTI1553_ChIsC(INT channum,HCARD handleval);
BTI1553API BOOL BTI1553_ChIsD(INT channum,HCARD handleval);
BTI1553API BOOL BTI1553_ChIsM(INT channum,HCARD handleval);
BTI1553API BOOL BTI1553_ChIsS(INT channum,HCARD handleval);
BTI1553API BOOL BTI1553_ChIsX(INT channum,HCARD handleval);
BTI1553API BOOL BTI1553_ChStart(INT channum,HCARD handleval);
BTI1553API BOOL BTI1553_ChStop(INT channum,HCARD handleval);
BTI1553API UINT16 BTI1553_CmdMaxLoopRd(INT channum,HCARD handleval);
BTI1553API VOID BTI1553_CmdMaxLoopWr(UINT16 countval,INT channum,HCARD handleval);
BTI1553API BOOL BTI1553_CmdShotRd(SCHNDX index,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_CmdShotWr(BOOL value,SCHNDX index,INT channum,HCARD handleval);
BTI1553API BOOL BTI1553_CmdSkipRd(SCHNDX index,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_CmdSkipWr(BOOL value,SCHNDX index,INT channum,HCARD handleval);
BTI1553API BOOL BTI1553_CmdStepRd(SCHNDX index,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_CmdStepWr(BOOL value,SCHNDX index,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_ErrorCtrl(UINT32 ctrlval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_ErrorDefine(UINT32 defineval,UINT16 errvalue,INT countval,UINT16 wordpos,UINT16 bitpos,INT channum,HCARD handleval);
BTI1553API BOOL BTI1553_ErrorSent(INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_ErrorTagBC(BOOL tagval,MSGADDR msgaddr,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_ErrorTagRT(BOOL tagval,BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_ErrorTriggerDefine(BOOL enableflag,UINT16 trigmask,UINT16 trigval,UINT16 pinpolarity,INT channum,HCARD handleval);
BTI1553API MSGADDR BTI1553_ListBlockRd(LPUINT16 buf,LISTADDR listaddr,HCARD handleval);
BTI1553API MSGADDR BTI1553_ListBlockWr(LPUINT16 buf,LISTADDR listaddr,HCARD handleval);
BTI1553API INT BTI1553_ListDataRd(LPUINT16 buf,INT count,LISTADDR listaddr,HCARD handleval);
BTI1553API INT BTI1553_ListDataWr(LPUINT16 buf,INT count,LISTADDR listaddr,HCARD handleval);
BTI1553API BOOL BTI1553_ListMultiBlockRd(LPUINT16 buf,LPINT blkcountptr,LISTADDR listaddr,HCARD handleval);
BTI1553API BOOL BTI1553_ListMultiBlockWr(LPUINT16 buf,INT blkcount,LISTADDR listaddr,HCARD handleval);
BTI1553API ERRVAL BTI1553_MonConfig(UINT32 configval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_MonFilterSA(INT taval,UINT32 rcvsamask,UINT32 xmtsamask,UINT32 rcvmcmask,UINT32 xmtmcmask,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_MonFilterTA(UINT32 tamask,INT channum,HCARD handleval);
BTI1553API MSGADDR BTI1553_MsgBlockRd(LPMSGFIELDS1553 buf,MSGADDR msgaddr,HCARD handleval);
BTI1553API MSGADDR BTI1553_MsgBlockWr(LPMSGFIELDS1553 buf,MSGADDR msgaddr,HCARD handleval);
BTI1553API VOID BTI1553_MsgDataRd(LPUINT16 buf,INT count,MSGADDR msgaddr,HCARD handleval);
BTI1553API VOID BTI1553_MsgDataWr(LPUINT16 buf,INT count,MSGADDR msgaddr,HCARD handleval);
BTI1553API UINT32 BTI1553_MsgFieldRd(UINT16 fieldtype,MSGADDR msgaddr,HCARD handleval);
BTI1553API UINT32 BTI1553_MsgFieldWr(UINT32 fieldval,UINT16 fieldtype,MSGADDR msgaddr,HCARD handleval);
BTI1553API VOID BTI1553_MsgGroupDataRd(INT nummsgs,UINT16 databufs[][32],LPMSGADDR msgaddrptr,HCARD handleval);
BTI1553API VOID BTI1553_MsgGroupDataWr(INT nummsgs,UINT16 databufs[][32],LPMSGADDR msgaddrptr,HCARD handleval);
BTI1553API VOID BTI1553_MsgGroupRd(INT nummsgs,LPMSGFIELDS1553 msgflds,LPMSGADDR msgaddrptr,HCARD handleval);
BTI1553API VOID BTI1553_MsgGroupWr(INT nummsgs,LPMSGFIELDS1553 msgflds,LPMSGADDR msgaddrptr,HCARD handleval);
BTI1553API BOOL BTI1553_MsgSkipRd(MSGADDR msgaddr,HCARD handleval);
BTI1553API VOID BTI1553_MsgSkipWr(BOOL skip,MSGADDR msgaddr,HCARD handleval);
BTI1553API ERRVAL BTI1553_MsgSyncDefine(BOOL enableflag,UINT16 syncmask,UINT16 pinpolarity,MSGADDR msgaddr,HCARD handleval);
BTI1553API ERRVAL BTI1553_MsgTriggerDefine(BOOL enableflag,UINT16 trigmask,UINT16 trigval,UINT16 pinpolarity,MSGADDR msgaddr,HCARD handleval);
BTI1553API ERRVAL BTI1553_ParamAmplitudeConfig(UINT32 configval,UINT16 dacval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_ParamAmplitudeGet(LPUINT16 dacval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_PlayConfig(UINT32 configval,UINT32 tamask,INT channum,HCARD handleval);
BTI1553API INT BTI1553_PlayStatus(INT channum,HCARD handleval);
BTI1553API UINT16 BTI1553_PlayWr(LPUINT16 buf,UINT16 bufcount,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_RTConfig(UINT32 configval,INT taval,INT channum,HCARD handleval);
BTI1553API LISTADDR BTI1553_RTCreateList(UINT32 listconfigval,INT count,UINT32 msgconfigval,BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCARD handleval);
BTI1553API MSGADDR BTI1553_RTCreateMsg(UINT32 configval,BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCARD handleval);
BTI1553API MSGADDR BTI1553_RTGetMsg(BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_RTReset(INT taval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_RTResponseTimeSet(INT resptime,INT taval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_RTSetMode(UINT32 configval,INT taval,INT channum,HCARD handleval);
BTI1553API UINT16 BTI1553_RTSWDRd(INT taval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_RTSWDWr(UINT16 swdval,INT taval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_RTSyncDefine(BOOL enableflag,UINT16 syncmask,UINT16 pinpolarity,INT taval,UINT32 rcvsamask,UINT32 xmtsamask,UINT32 rcvmcmask,UINT32 xmtmcmask,INT channum,HCARD handleval);
BTI1553API UINT16 BTI1553_ValPackCWD(INT TAval,INT TRflag,INT SAval,INT WCval);
BTI1553API VOID BTI1553_ValUnpackCWD(UINT16 CWDval,LPINT TAval,LPINT TRflag,LPINT SAval,LPINT WCval);

/**
*
*  Internal functions used by the BTI1553 Driver.
*
**/

BTI1553API LISTADDR BTI1553_BCCreateListEx(UINT32 listconfigval,INT skipval,INT count,UINT32 msgconfigval,UINT16 cwd1,UINT16 cwd2,LPUINT16 data,HCARD handleval);
BTI1553API LISTADDR BTI1553_BCCreateListExx(UINT32 listconfigval,INT skipval,INT count,UINT32 msgconfigval,UINT16 cwd1,UINT16 cwd2,LPUINT16 data,HCARD handleval);
BTI1553API MSGADDR BTI1553_BCGetMsg(INT index,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BCIMGapRd(LPUINT16 gapval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BCIMGapWr(UINT16 gapval,INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedBranchEx(UINT16 condition1,UINT16 condition2,UINT16 condition3,UINT16 opcode,SCHNDX destindex,INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedCallEx(UINT16 condition1,UINT16 condition2,UINT16 condition3,UINT16 opcode,SCHNDX destindex,INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedLogEx(UINT16 condition1,UINT16 condition2,UINT16 condition3,UINT16 opcode,UINT16 tagval,INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedNop(INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedRetryEx(UINT16 condition1,UINT16 condition2,UINT16 condition3,UINT16 retryflag,UINT16 retries,INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSchedUser(UINT32 useraddr,INT channum,HCARD handleval);
BTI1553API SCHNDX BTI1553_BCSetEntry(INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BCTransmitMsgBlkEx(BOOL writeflag,INT count,LPXMITFIELDS1553 xmitfields,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BCTransmitMsgBlkInit(INT maxcount,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BCTransmitMsgBlkRd(INT count,LPXMITFIELDS1553 xmitfields,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BCTransmitMsgBlkWr(INT count,LPXMITFIELDS1553 xmitfields,INT channum,HCARD handleval);
BTI1553API UINT16 BTI1553_BlkAddr(INT taval,INT channum,HCARD handleval);
BTI1553API UINT16 BTI1553_BlkOptRd(INT taval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BlkOptWr(UINT16 rtopt,INT taval,INT channum,HCARD handleval);
BTI1553API UINT32 BTI1553_BlkPtrRd(INT taval,INT channum,HCARD handleval);
BTI1553API VOID BTI1553_BlkPtrWr(UINT32 descaddr,INT taval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BlkRd(LPUINT16 block,INT taval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BlkReset(INT taval,INT channum,HCARD handleval);
BTI1553API UINT16 BTI1553_BlkSWDRd(INT taval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BlkSWDWr(UINT16 swdval,INT taval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BlkWipe(INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_BlkWr(LPUINT16 block,INT taval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_ChConfig(BOOL enableflag,INT type,UINT16 taval,INT channum,HCARD handleval);
BTI1553API BOOL BTI1553_ChIsRunning(INT channum,HCARD handleval);
BTI1553API UINT32 BTI1553_CmdAddr(SCHNDX index,INT channum,HCARD handleval);
BTI1553API UINT32 BTI1553_CmdAlloc(INT count,HCARD handleval);
BTI1553API UINT32 BTI1553_CmdAllocEx(UINT32 configval,INT count,HCARD handleval);
BTI1553API UINT32 BTI1553_CmdBaseRd(INT channum,HCARD handleval);
BTI1553API VOID BTI1553_CmdBaseWr(UINT32 addrval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_CmdClear(INT channum,HCARD handleval);
BTI1553API UINT16 BTI1553_CmdCountRd(INT channum,HCARD handleval);
BTI1553API VOID BTI1553_CmdCountWr(UINT16 countval,INT channum,HCARD handleval);
BTI1553API UINT16 BTI1553_CmdCtrlRd(SCHNDX index,INT channum,HCARD handleval);
BTI1553API VOID BTI1553_CmdCtrlWr(UINT16 ctrlval,SCHNDX index,INT channum,HCARD handleval);
BTI1553API UINT16 BTI1553_CmdCurrRd(INT channum,HCARD handleval);
BTI1553API VOID BTI1553_CmdCurrWr(UINT16 addrval,INT channum,HCARD handleval);
BTI1553API VOID BTI1553_CmdInit(UINT16 opcode,LPUINT16 block);
BTI1553API SCHNDX BTI1553_CmdInsert(LPUINT16 block,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_CmdRd(LPUINT16 block,UINT32 addrval,HCARD handleval);
BTI1553API VOID BTI1553_CmdRestart(INT channum,HCARD handleval);
BTI1553API VOID BTI1553_CmdStackClr(INT channum,HCARD handleval);
BTI1553API UINT16 BTI1553_CmdStartRd(INT channum,HCARD handleval);
BTI1553API VOID BTI1553_CmdStartWr(UINT16 addrval,INT channum,HCARD handleval);
BTI1553API INT BTI1553_CmdTotalRd(INT channum,HCARD handleval);
BTI1553API VOID BTI1553_CmdTotalWr(INT countval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_CmdWr(LPUINT16 block,UINT32 addrval,HCARD handleval);
BTI1553API UINT32 BTI1553_DescAddr(BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCARD handleval);
BTI1553API UINT32 BTI1553_DescAlloc(HCARD handleval);
BTI1553API ERRVAL BTI1553_DescConfig(UINT32 configval,BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_DescConfigAll(UINT32 configval,INT taval,INT channum,HCARD handleval);
BTI1553API UINT16 BTI1553_DescOptRd(BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCARD handleval);
BTI1553API VOID BTI1553_DescOptWr(UINT16 descopt,BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCARD handleval);
BTI1553API MSGADDR BTI1553_DescPtrRd(BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCARD handleval);
BTI1553API UINT32 BTI1553_DescPtrWr(MSGADDR msgaddr,BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCARD handleval);
BTI1553API UINT32 BTI1553_DescRd(BOOL mcflag,INT taval,BOOL trflag,INT saval,LPUINT16 desc,INT channum,HCARD handleval);
BTI1553API UINT32 BTI1553_DescWr(BOOL mcflag,INT taval,BOOL trflag,INT saval,LPUINT16 desc,INT channum,HCARD handleval);
BTI1553API INT BTI1553_DllUsageCount(VOID);
BTI1553API BOOL BTI1553_ErrorIsTaggedBC(MSGADDR msgaddr,INT channum,HCARD handleval);
BTI1553API BOOL BTI1553_ErrorIsTaggedRT(BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_ErrorResponse(UINT16 responseval,INT channum,HCARD handleval);
BTI1553API VOID BTI1553_ErrorSync(BOOL value,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_ErrorTimeout(UINT16 timeoutval,INT channum,HCARD handleval);
BTI1553API BOOL BTI1553_IsChan(INT channum,HCARD handleval);
BTI1553API MSGADDR BTI1553_ListAddr(INT index,LISTADDR listaddr,HCARD handleval);
BTI1553API MSGADDR BTI1553_ListAddrEx(INT index,LISTADDR listaddr,LPUINT16 header,HCARD handleval);
BTI1553API MSGADDR BTI1553_ListBlockRdEx(LPUINT16 buf,INT index,LISTADDR listaddr,HCARD handleval);
BTI1553API MSGADDR BTI1553_ListBlockWrEx(LPUINT16 buf,INT index,LISTADDR listaddr,HCARD handleval);
BTI1553API ERRVAL BTI1553_ListClear(LISTADDR listaddr,HCARD handleval);
BTI1553API LISTADDR BTI1553_ListConfig(UINT32 configval,INT count,UINT16 skipval,HCARD handleval);
BTI1553API LISTADDR BTI1553_ListConfigEx(UINT32 configval,INT count,UINT16 skipval,HCARD handleval);
BTI1553API INT BTI1553_ListDataRdEx(LPUINT16 buf,INT count,INT index,LISTADDR listaddr,HCARD handleval);
BTI1553API INT BTI1553_ListDataWrEx(LPUINT16 buf,INT count,INT index,LISTADDR listaddr,HCARD handleval);
BTI1553API ERRVAL BTI1553_ListInit(LISTADDR listaddr,MSGADDR msgaddr,HCARD handleval);
BTI1553API MSGADDR BTI1553_ListNextRd(UINT32 listaddr,LPUINT16 header,HCARD handleval);
BTI1553API MSGADDR BTI1553_ListNextWr(UINT32 listaddr,LPUINT16 header,HCARD handleval);
BTI1553API VOID BTI1553_ListPostRd(LISTADDR listaddr,LPUINT16 header,HCARD handleval);
BTI1553API VOID BTI1553_ListPostWr(LISTADDR listaddr,LPUINT16 header,HCARD handleval);
BTI1553API VOID BTI1553_ListPreRd(LISTADDR listaddr,LPUINT16 header,HCARD handleval);
BTI1553API VOID BTI1553_ListPreWr(LISTADDR listaddr,LPUINT16 header,HCARD handleval);
BTI1553API INT BTI1553_ListStatus(LISTADDR listaddr,HCARD handleval);
BTI1553API ERRVAL BTI1553_MonConfigEx(UINT32 configval,UINT32 moncount,UINT16 cardnum,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_MonFilterSAEx(INT taval,UINT32 rcvsamask,UINT32 xmtsamask,UINT32 rcvmcmask,UINT32 xmtmcmask,INT channum,HCARD handleval);
BTI1553API UINT16 BTI1553_MsgCheck(MSGADDR msgaddr,HCARD handleval);
BTI1553API MSGADDR BTI1553_MsgCommRd(LPMSGFIELDS1553 buf,MSGADDR msgaddr,HCARD handleval);
BTI1553API MSGADDR BTI1553_MsgCommWr(LPMSGFIELDS1553 buf,MSGADDR msgaddr,HCARD handleval);
BTI1553API MSGADDR BTI1553_MsgConfig(UINT32 configval,HCARD handleval);
BTI1553API MSGADDR BTI1553_MsgConfigEx(UINT32 configval,UINT16 cwd,HCARD handleval);
BTI1553API UINT16 BTI1553_MsgCWD1Rd(MSGADDR msgaddr,HCARD handleval);
BTI1553API VOID BTI1553_MsgCWD1Wr(UINT16 cwd1,MSGADDR msgaddr,HCARD handleval);
BTI1553API UINT16 BTI1553_MsgCWD2Rd(MSGADDR msgaddr,HCARD handleval);
BTI1553API VOID BTI1553_MsgCWD2Wr(UINT16 cwd2,MSGADDR msgaddr,HCARD handleval);
BTI1553API UINT16 BTI1553_MsgErrorRd(MSGADDR msgaddr,HCARD handleval);
BTI1553API VOID BTI1553_MsgErrorWr(UINT16 value,MSGADDR msgaddr,HCARD handleval);
BTI1553API UINT16 BTI1553_MsgFlagRd(MSGADDR msgaddr,HCARD handleval);
BTI1553API VOID BTI1553_MsgFlagWr(UINT16 msgflag,MSGADDR msgaddr,HCARD handleval);
BTI1553API BOOL BTI1553_MsgIsAccessed(MSGADDR msgaddr,HCARD handleval);
BTI1553API UINT16 BTI1553_MsgOptRd(MSGADDR msgaddr,HCARD handleval);
BTI1553API VOID BTI1553_MsgOptWr(UINT16 msgopt,MSGADDR msgaddr,HCARD handleval);
BTI1553API MSGADDR BTI1553_MsgReset(MSGADDR msgaddr,HCARD handleval);
BTI1553API MSGADDR BTI1553_MsgResetEx(MSGADDR msgaddr,HCARD handleval);
BTI1553API UINT16 BTI1553_MsgSWD1Rd(MSGADDR msgaddr,HCARD handleval);
BTI1553API UINT16 BTI1553_MsgSWD2Rd(MSGADDR msgaddr,HCARD handleval);
BTI1553API ERRVAL BTI1553_PlayClear(INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_PlayConfigEx(UINT32 configval,UINT32 playcount,UINT32 tamask,INT channum,HCARD handleval);
BTI1553API UINT32 BTI1553_PlayCounterWr(UINT32 totalcount,INT channum,HCARD handleval);
BTI1553API INT BTI1553_PlayStatusEx(LPUINT32 totalcount,INT channum,HCARD handleval);
BTI1553API UINT16 BTI1553_PlayWrEx(LPUINT16 buf,UINT16 bufcount,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_ProcSetPost(UINT32 useraddr,UINT32 msgaddr,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_ProcSetPre(UINT32 useraddr,UINT32 msgaddr,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_RTConfigEx(UINT32 configval,INT taval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_RTConfigExx(UINT32 configval,INT taval,INT channum,HCARD handleval);
BTI1553API LISTADDR BTI1553_RTCreateListEx(UINT32 listconfigval,INT skipval,INT count,UINT32 msgconfigval,BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCARD handleval);
BTI1553API LISTADDR BTI1553_RTCreateListExx(UINT32 listconfigval,INT skipval,INT count,UINT32 msgconfigval,BOOL mcflag,INT taval,BOOL trflag,INT saval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_RTResponseTimeRd(LPUINT16 timeval,INT taval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_RTResponseTimeWr(UINT16 timeval,INT taval,INT channum,HCARD handleval);
BTI1553API UINT16 BTI1553_RTStatus(INT taval,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_SchedBuild(INT nummsgs,LPMSGADDR msgaddr,LPINT freq,INT channum,HCARD handleval);
BTI1553API ERRVAL BTI1553_TestProtocol(HCARD handleval);
BTI1553API UINT16 BTI1553_ValPackRTCWD(INT MCflag,INT TAval,INT TRflag,INT SAval,INT count);
BTI1553API VOID BTI1553_ValUnpackRTCWD(UINT16 cwd,LPINT BCASTflag,LPINT MCflag,LPINT TAval,LPINT TRflag,LPINT SAval,LPINT count);

#ifdef __cplusplus
}
#endif

/**
*
*  Channel number constants
*
**/

#define CH0 0x0000           //Channel 0
#define CH1 0x0001           //Channel 1
#define CH2 0x0002           //Channel 2
#define CH3 0x0003           //Channel 3
#define CH4 0x0004           //Channel 4
#define CH5 0x0005           //Channel 5
#define CH6 0x0006           //Channel 6
#define CH7 0x0007           //Channel 7

/**
*
*  Bus controller configuration options
*
**/

enum {	BCCFG1553_DEFAULT      = 0x00000000L,      //Select all default settings
		BCCFG1553_ENABLE       = 0x00000000L,      //Enable bus controller (default)
		BCCFG1553_DISABLE      = 0x00000001L,      //Disable bus controller
		BCCFG1553_TRIGNONE     = 0x00000000L,      //No trigger (default)
		BCCFG1553_TRIGEXT      = 0x00000010L,      //Trigger external
		BCCFG1553_TRIGSTART    = 0x00000020L,      //Trigger external start
		BCCFG1553_NOLOGHALT    = 0x00000000L,      //Do not generate event log on halt (default)
		BCCFG1553_LOGHALT      = 0x00000100L,      //Generate event log on halt
		BCCFG1553_NOLOGPAUSE   = 0x00000000L,      //Do not generate event log on pause (default)
		BCCFG1553_LOGPAUSE     = 0x00000200L,      //Generate event log on pause
		BCCFG1553_NOLOOPMAX    = 0x00000000L,      //Disable maximum loop count (default)
		BCCFG1553_LOOPMAX      = 0x00001000L,      //Enable maximum loop count
		BCCFG1553_NOSTEP       = 0x00000000L,      //Disable single stepping (default)
		BCCFG1553_STEP         = 0x00002000L,      //Enable single stepping
		BCCFG1553_MC01         = 0x00000000L,      //Select SA=00000 or SA=11111 for mode codes (default)
		BCCFG1553_MC1          = 0x01000000L,      //Select SA=11111 for mode codes
		BCCFG1553_MC0          = 0x02000000L,      //Select SA=00000 for mode codes
		BCCFG1553_MCNONE       = 0x03000000L,      //Disable mode codes
		BCCFG1553_TERMOFF      = 0x00000000L,      //Direct coupled termination resistance off (Bus A & B) (default)
		BCCFG1553_TERMONA      = 0x04000000L,      //Direct coupled termination resistance on  (Bus A)
		BCCFG1553_TERMONB      = 0x08000000L,      //Direct coupled termination resistance on  (Bus B)
		BCCFG1553_TERMONAB     = 0x0C000000L,      //Direct coupled termination resistance on  (Bus A & B)
		BCCFG1553_SYNCSEL      = 0x00000000L,      //Selective sync (default)
		BCCFG1553_SYNCALL      = 0x40000000L,      //Sync on all messages
		BCCFG1553_PAUSE        = 0x00000400L,      //Mark channel as paused
		BCCFG1553_UNPAUSE      = 0x00000000L,      //Mark channel as unpaused (default)
		BCCFG1553_SELFTEST     = 0x00800000L,      //This channel will transmit/receive on the internal self-test bus
		BCCFG1553_SELFTESTOFF  = 0x00000000L       //This channel will transmit/receive on the operational bus (default)
};

/**
*
*  Monitor configuration options
*
**/

enum {	MONCFG1553_DEFAULT     = 0x00000000L,      //Select all default settings
		MONCFG1553_ENABLE      = 0x00000000L,      //Enable monitor
		MONCFG1553_DISABLE     = 0x00000001L,      //Disable monitor
		MONCFG1553_NOBCAST     = 0x00000000L,      //Disable broadcast (default)
		MONCFG1553_BCAST       = 0x00000100L,      //Enable broadcast
		MONCFG1553_COMPLETE    = 0x00000000L,      //Enable receiving complete words (default)
		MONCFG1553_INCOMPLETE  = 0x00010000L,      //Enable receiving incomplete words
		MONCFG1553_MC01        = 0x00000000L,      //Select SA=00000 or SA=11111 for mode codes (default)
		MONCFG1553_MC1         = 0x00100000L,      //Select SA=11111 for mode codes
		MONCFG1553_MC0         = 0x00200000L,      //Select SA=00000 for mode codes
		MONCFG1553_MCNONE      = 0x00300000L,      //Disable mode codes
		MONCFG1553_TERMOFF     = 0x00000000L,      //Direct coupled termination resistance off (Bus A & B) (default)
		MONCFG1553_TERMONA     = 0x04000000L,      //Direct coupled termination resistance on  (Bus A)
		MONCFG1553_TERMONB     = 0x08000000L,      //Direct coupled termination resistance on  (Bus B)
		MONCFG1553_TERMONAB    = 0x0C000000L,      //Direct coupled termination resistance on  (Bus A & B)
		MONCFG1553_SELFTEST    = 0x00800000L,      //This channel will transmit/receive on the internal self-test bus
		MONCFG1553_SELFTESTOFF = 0x00000000L       //This channel will transmit/receive on the operational bus (default)
};

/**
*
*  Remote terminal configuration options
*
**/

enum {	RTCFG1553_DEFAULT      = 0x00000000L,      //Select all default settings
		RTCFG1553_SIMULATE     = 0x00000000L,      //Enable RT simulation (default)
		RTCFG1553_DISABLE      = 0x00000001L,      //Disable RT
		RTCFG1553_MONITOR      = 0x00000002L,      //Enable RT monitor
		RTCFG1553_NOBCAST      = 0x00000000L,      //Disable broadcast (default)
		RTCFG1553_BCAST        = 0x00000100L,      //Enable broadcast
		RTCFG1553_NOAUTOBUSY   = 0x00000000L,      //Disable auto busy (default)
		RTCFG1553_AUTOBUSY     = 0x00000200L,      //Enable auto busy
		RTCFG1553_BUILD        = 0x00000000L,      //Enable auto building (default)
		RTCFG1553_NOBUILD      = 0x00000400L,      //Disable auto building
		RTCFG1553_STDB         = 0x00000000L,      //Use MIL-STD-1553B standard (default)
		RTCFG1553_STDA         = 0x00400000L,      //Use MIL-STD-1553A standard
		RTCFG1553_NODYNBC      = 0x00000000L,      //Do not respond to dynamic BC mode code (default)
		RTCFG1553_DYNBC        = 0x00001000L,      //Respond to dynamic BC mode code
		RTCFG1553_NOIMMCLR     = 0x00000000L,      //Do not clear status word bits (default)
		RTCFG1553_IMMCLR       = 0x00002000L,      //Status word bits are cleared immediately
		RTCFG1553_NOBCASTADDR  = 0x00000000L,      //Disable broadcast handling for address (default)
		RTCFG1553_BCASTADDR    = 0x00004000L,      //Enable broadcast handling for address
		RTCFG1553_CHANAB       = 0x00000000L,      //Respond to both channels (default)
		RTCFG1553_CHANA        = 0x00020000L,      //Respond to channel A
		RTCFG1553_CHANB        = 0x00010000L,      //Respond to channel B
		RTCFG1553_CHANNONE     = 0x00030000L,      //Respond to neither channel A nor B
		RTCFG1553_MC01         = 0x00000000L,      //Select SA=00000 or SA=11111 for mode codes (default)
		RTCFG1553_MC1          = 0x01000000L,      //Select SA=11111 for mode codes
		RTCFG1553_MC0          = 0x02000000L,      //Select SA=00000 for mode codes
		RTCFG1553_MCNONE       = 0x03000000L,      //Disable mode codes
		RTCFG1553_TERMOFF      = 0x00000000L,      //Direct coupled termination resistance off (Bus A & B) (default)
		RTCFG1553_TERMONA      = 0x04000000L,      //Direct coupled termination resistance on  (Bus A)
		RTCFG1553_TERMONB      = 0x08000000L,      //Direct coupled termination resistance on  (Bus B)
		RTCFG1553_TERMONAB     = 0x0C000000L,      //Direct coupled termination resistance on  (Bus A & B)
		RTCFG1553_SYNCSEL      = 0x00000000L,      //Selective sync (default)
		RTCFG1553_SYNCALL      = 0x40000000L,      //Sync on all messages
		RTCFG1553_WIPE         = 0x00000000L,      //Enables message clear (default)
		RTCFG1553_NOWIPE       = 0x80000000L,      //Disables message clear
		RTCFG1553_WIPE0        = 0x00000000L,      //Initialize data with zeros (default)
		RTCFG1553_WIPE123      = 0x10000000L,      //Initialize data with incrementing values
		RTCFG1553_WIPECWD      = 0x20000000L,      //Initialize data with command word
		RTCFG1553_RESPONSEB    = 0x00000000L,      //RT responds using MIL-STD-1553B response time
		RTCFG1553_RESPONSEA    = 0x00400000L,      //RT responds using MIL-STD-1553A response time
		RTCFG1553_SELFTEST     = 0x00800000L,      //This channel will transmit/receive on the internal self-test bus
		RTCFG1553_SELFTESTOFF  = 0x00000000L       //This channel will transmit/receive on the operational bus (default)
};

/**
*
*  Playback configuration options
*
**/

enum {	PLAYCFG1553_DEFAULT     = 0x00000000L,      //Select all default settings
		PLAYCFG1553_ENABLE      = 0x00000000L,      //Enable Playback mode
		PLAYCFG1553_DISABLE     = 0x00000001L,      //Disable Playback mode
		PLAYCFG1553_16K         = 0x00000000L,      //Allocate a 16K playback buffer (default)
		PLAYCFG1553_ALLAVAIL    = 0x00000010L,      //Allocate all available memory to playback buffer
		PLAYCFG1553_32K         = 0x00000020L,      //Allocate a 32K playback buffer
		PLAYCFG1553_64K         = 0x00000040L,      //Allocate a 64K playback buffer
		PLAYCFG1553_128K        = 0x00000080L,      //Allocate a 128K playback buffer
		PLAYCFG1553_MC01        = 0x00000000L,      //Select SA=00000 or SA=11111 for mode codes (default)
		PLAYCFG1553_MC1         = 0x01000000L,      //Select SA=11111 for mode codes
		PLAYCFG1553_MC0         = 0x02000000L,      //Select SA=00000 for mode codes
		PLAYCFG1553_MCNONE      = 0x03000000L,      //Disable mode codes
		PLAYCFG1553_TERMOFF     = 0x00000000L,      //Direct coupled termination resistance off (Bus A & B) (default)
		PLAYCFG1553_TERMONA     = 0x04000000L,      //Direct coupled termination resistance on  (Bus A)
		PLAYCFG1553_TERMONB     = 0x08000000L,      //Direct coupled termination resistance on  (Bus B)
		PLAYCFG1553_TERMONAB    = 0x0C000000L       //Direct coupled termination resistance on  (Bus A & B)
};

/**
*
*  Serial configuration options
*
**/

enum {	SERCFG1553_DEFAULT     = 0x00000000L,      //Select all default settings
		SERCFG1553_CONTINUOUS  = 0x00000000L,      //Enable continuous mode (default)
		SERCFG1553_DISABLE     = 0x00000001L,      //Disable free mode
		SERCFG1553_FREE        = 0x00000002L,      //Enable free mode
		SERCFG1553_NOLOGEMPTY  = 0x00000000L,      //Do not generate event log when empty (default)
		SERCFG1553_LOGEMPTY    = 0x00000010L       //Generate event log when empty
};

/**
*
*  Error configuration options
*
**/

enum {	ERRDEF1553_NONE        = 0x00000000L,      //Disable error generation (default)
		ERRDEF1553_GAP         = 0x00010000L,      //Generate preceeding gap error
		ERRDEF1553_CNTWRDABS   = 0x00020000L,      //Generate an absolute word count error
		ERRDEF1553_CNTWRDREL   = 0x00040000L,      //Generate a relative count error
		ERRDEF1553_CNTBIT      = 0x00080000L,      //Generate a bit count error
		ERRDEF1553_MAN1        = 0x00100000L,      //Generate manchester error on first half of bit
		ERRDEF1553_MAN2        = 0x00200000L,      //Generate manchester error on second half of bit
		ERRDEF1553_SYNC        = 0x00400000L,      //Generate a sync error
		ERRDEF1553_PAR         = 0x00800000L,      //Generate a parity error
		ERRDEF1553_ZEROX1      = 0x01000000L,      //Generate zero crossing error on leading zero crossing (if exists)
		ERRDEF1553_ZEROX2      = 0x02000000L,      //Generate zero crossing error on mid-bit zero crossing

		ERRDEF1553_DWD00       = 0x0000,           //Error will be generated in a data word  0
		ERRDEF1553_DWD01       = 0x0001,           //Error will be generated in a data word  1
		ERRDEF1553_DWD02       = 0x0002,           //Error will be generated in a data word  2
		ERRDEF1553_DWD03       = 0x0003,           //Error will be generated in a data word  3
		ERRDEF1553_DWD04       = 0x0004,           //Error will be generated in a data word  4
		ERRDEF1553_DWD05       = 0x0005,           //Error will be generated in a data word  5
		ERRDEF1553_DWD06       = 0x0006,           //Error will be generated in a data word  6
		ERRDEF1553_DWD07       = 0x0007,           //Error will be generated in a data word  7
		ERRDEF1553_DWD08       = 0x0008,           //Error will be generated in a data word  8
		ERRDEF1553_DWD09       = 0x0009,           //Error will be generated in a data word  9
		ERRDEF1553_DWD10       = 0x000A,           //Error will be generated in a data word 10
		ERRDEF1553_DWD11       = 0x000B,           //Error will be generated in a data word 11
		ERRDEF1553_DWD12       = 0x000C,           //Error will be generated in a data word 12
		ERRDEF1553_DWD13       = 0x000D,           //Error will be generated in a data word 13
		ERRDEF1553_DWD14       = 0x000E,           //Error will be generated in a data word 14
		ERRDEF1553_DWD15       = 0x000F,           //Error will be generated in a data word 15
		ERRDEF1553_DWD16       = 0x0010,           //Error will be generated in a data word 16
		ERRDEF1553_DWD17       = 0x0011,           //Error will be generated in a data word 17
		ERRDEF1553_DWD18       = 0x0012,           //Error will be generated in a data word 18
		ERRDEF1553_DWD19       = 0x0013,           //Error will be generated in a data word 19
		ERRDEF1553_DWD20       = 0x0014,           //Error will be generated in a data word 20
		ERRDEF1553_DWD21       = 0x0015,           //Error will be generated in a data word 21
		ERRDEF1553_DWD22       = 0x0016,           //Error will be generated in a data word 22
		ERRDEF1553_DWD23       = 0x0017,           //Error will be generated in a data word 23
		ERRDEF1553_DWD24       = 0x0018,           //Error will be generated in a data word 24
		ERRDEF1553_DWD25       = 0x0019,           //Error will be generated in a data word 25
		ERRDEF1553_DWD26       = 0x001A,           //Error will be generated in a data word 26
		ERRDEF1553_DWD27       = 0x001B,           //Error will be generated in a data word 27
		ERRDEF1553_DWD28       = 0x001C,           //Error will be generated in a data word 28
		ERRDEF1553_DWD29       = 0x001D,           //Error will be generated in a data word 29
		ERRDEF1553_DWD30       = 0x001E,           //Error will be generated in a data word 30
		ERRDEF1553_DWD31       = 0x001F,           //Error will be generated in a data word 31
		ERRDEF1553_CWD1        = 0x0100,           //Error will be generated in CWD1
		ERRDEF1553_CWD2        = 0x0101,           //Error will be generated in CWD2
		ERRDEF1553_SWD         = 0x0102,           //Error will be generated in SWD1
		ERRDEF1553_SWD1        = 0x0102,           //Error will be generated in SWD1
		ERRDEF1553_SWD2        = 0x0102            //Error will be generated in SWD2
};

/**
*
*  Error control options
*
**/

enum {	ERRCTRL1553_OFF        = 0x00000000L,      //Disable error generation
		ERRCTRL1553_ON         = 0x00000001L,      //Enable error generation
		ERRCTRL1553_ONCE       = 0x00000002L,      //Enable error generation once
		ERRCTRL1553_EXTERNAL   = 0x00000003L,      //Enable externally triggered error generation
		ERRCTRL1553_TAGMSG     = 0x00000000L,      //Generate errors on tagged messages
		ERRCTRL1553_ANYMSG     = 0x80000000L       //Generate errors on any message
};

/**
*
*  Message configuration options
*
**/

enum {	MSGCRT1553_DEFAULT     = 0x00000000L,      //Default settings
		MSGCRT1553_ENABLE      = 0x00000000L,      //Enable subaddress (RT) (default)
		MSGCRT1553_DISABLE     = 0x00001000L,      //Disable subaddress (RT)
		MSGCRT1553_RESP        = 0x00000000L,      //Enable response (RT) (default)
		MSGCRT1553_NORESP      = 0x00002000L,      //Disable response (RT)
		MSGCRT1553_NOWRAP      = 0x00000000L,      //Disable data wrap (RT) (default)
		MSGCRT1553_WRAP        = 0x00004000L,      //Enable data wrap (RT)
		MSGCRT1553_NOLOG       = 0x00000000L,      //Message will not generate event log (default)
		MSGCRT1553_LOG         = 0x00000001L,      //Message will generate event log
		MSGCRT1553_NOERR       = 0x00000000L,      //No message error will be generated for message (default)
		MSGCRT1553_ERR         = 0x00000002L,      //A message error will be generated for message
		MSGCRT1553_BUSA        = 0x00000000L,      //Message will be transmitted on bus A (BC) (default)
		MSGCRT1553_BUSB        = 0x00000004L,      //Message will be transmitted on bus B (BC)
		MSGCRT1553_BCRT        = 0x00000000L,      //Message is a BC-RT transfer (BC) (default)
		MSGCRT1553_RTRT        = 0x00000008L,      //Message is an RT-RT transfer (BC)
		MSGCRT1553_NOMON       = 0x00000000L,      //Message will not be monitored (default)
		MSGCRT1553_MON         = 0x00000010L,      //Message will be monitored
		MSGCRT1553_NOTIMETAG   = 0x00000000L,      //Message will not record time-tag (default)
		MSGCRT1553_TIMETAG     = 0x00000040L,      //Message will record time-tag
		MSGCRT1553_NOELAPSE    = 0x00000000L,      //Message will not record elapse time (default)
		MSGCRT1553_ELAPSE      = 0x00000080L,      //Message will record elapse time
		MSGCRT1553_NOMIN       = 0x00000000L,      //Message will not record min time (default)
		MSGCRT1553_MIN         = 0x00000100L,      //Message will record min time
		MSGCRT1553_NOMAX       = 0x00000000L,      //Message will not record max time (default)
		MSGCRT1553_MAX         = 0x00000200L,      //Message will record max time
		MSGCRT1553_NOHIT       = 0x00000000L,      //Message will not record hit count (default)
		MSGCRT1553_HIT         = 0x00000400L,      //Message will record hit count
		MSGCRT1553_NOSYNC      = 0x00000000L,      //No sync will be generated for message (default)
		MSGCRT1553_SYNC        = 0x40000000L,      //Sync will be generated for message
		MSGCRT1553_WIPE        = 0x00000000L,      //Enables message clear (default)
		MSGCRT1553_NOWIPE      = 0x80000000L,      //Disables message clear
		MSGCRT1553_WIPE0       = 0x00000000L,      //Initialize data with zeros (default)
		MSGCRT1553_WIPE123     = 0x01000000L,      //Initialize data with incrementing values
		MSGCRT1553_WIPECWD     = 0x02000000L       //Initialize data with command word
};

/**
*
*  List buffer options
*
**/

enum {	LISTCRT1553_DEFAULT    = 0x00000000L,      //Select all default settings
		LISTCRT1553_PINGPONG   = 0x00000000L,      //Enable ping-pong mode (default)
		LISTCRT1553_FIFO       = 0x00000001L,      //Enable FIFO mode
		LISTCRT1553_CIRCULAR   = 0x00000002L,      //Enable circular mode
		LISTCRT1553_TRBIT      = 0x00000000L,      //User will read or write depending upon T/R bit
		LISTCRT1553_READ       = 0x00000010L,      //User will read from list buffer
		LISTCRT1553_WRITE      = 0x00000020L,      //User will write to list buffer
		LISTCRT1553_NOTSKIP    = 0x00000000L,      //Will not skip messages
		LISTCRT1553_SKIP       = 0x00000040L,      //Enable message skipping
		LISTCRT1553_ASYNC      = 0x00000080L,      //Asynchronous mode
		LISTCRT1553_NOLOG      = 0x00000000L,      //Do not generate event log when list buffer empty/full (default)
		LISTCRT1553_LOG        = 0x00000100L,      //Generate event log when list buffer empty/full
		LISTCRT1553_NOHIT      = 0x00000000L,      //Do not generate event log when list is hit (default)
		LISTCRT1553_HIT        = 0x00000200L,      //Generate event log when list is hit
		LISTCRT1553_NOLOGHALF  = 0x00000000L,      //Do not generate event log on middle and last entries (default)
		LISTCRT1553_LOGHALF    = 0x00000400L       //Generate event log on middle and last entries
};

/**
*
*  Condition flags.
*
**/

enum {	COND1553_ALWAYS        = 0x00000000L,      //Unconditional
		COND1553_FAIL          = 0x00000001L,      //Condition when all retries have failed
		COND1553_SRQ           = 0x00000002L,      //Condition on Service Request bit set in status word
		COND1553_INS           = 0x00000004L,      //Condition on Instrumentation bit set in status word
		COND1553_SSF           = 0x00000008L,      //Condition on Subsystem Fail bit set in status word
		COND1553_TF            = 0x00000010L,      //Condition on Terminal Flag bit set in status word
		COND1553_BUSY          = 0x00000020L,      //Condition on Busy bit set in status word
		COND1553_ME            = 0x00000040L,      //Condition on Message Error bit set in status word
		COND1553_RESPERR       = 0x00000080L,      //Condition on response error
		COND1553_NORESP        = 0x00000100L,      //Condition on no response error
		COND1553_ALTBUS        = 0x00000200L,      //Retries are performed on alternate bus
		COND1553_DIO1ACT       = 0x00001000L,      //Condition on digital I/O #1 active
		COND1553_DIO1NACT      = 0x00002000L,      //Condition on digital I/O #1 not active
		COND1553_DIO2ACT       = 0x00004000L,      //Condition on digital I/O #2 active
		COND1553_DIO2NACT      = 0x00008000L,      //Condition on digital I/O #2 active
		COND1553_DIO3ACT       = 0x00010000L,      //Condition on digital I/O #3 active
		COND1553_DIO3NACT      = 0x00020000L,      //Condition on digital I/O #3 active
		COND1553_DIO4ACT       = 0x00040000L,      //Condition on digital I/O #4 active
		COND1553_DIO4NACT      = 0x00080000L       //Condition on digital I/O #4 active
};

/**
*
*  Bit fields of the message error flag
*
**/

enum {	MSGERR1553_NORESP      = 0x8000,           //Set when we timeout waiting for response
		MSGERR1553_ANYERR      = 0x4000,           //Set when any error bit is set
		MSGERR1553_PROTOCOL    = 0x2000,           //General protocol error
		MSGERR1553_SYNC        = 0x1000,           //Set when we have the wrong sync
		MSGERR1553_DATACOUNT   = 0x0800,           //Set when improper number of data words
		MSGERR1553_MANCH       = 0x0020,           //Set when manchester error detected
		MSGERR1553_PARITY      = 0x0010,           //Set when parity error detected
		MSGERR1553_WORD        = 0x0008,           //Set when word error detected
		MSGERR1553_RETRY       = 0x0004,           //All retries failed
		MSGERR1553_SYSTEM      = 0x0002,           //An internal system error occurred
		MSGERR1553_HIT         = 0x0001            //Always set
};

/**
*
*  Bit fields of the message activity flag.
*
**/

enum {	MSGACT1553_CHMASK      = 0xF000,           //Channel number mask value
		MSGACT1553_CHSHIFT     = 0x000C,           //Channel number shift value
		MSGACT1553_XMTCWD1     = 0x0800,           //Transmitted CWD1
		MSGACT1553_XMTCWD2     = 0x0400,           //Transmitted CWD2
		MSGACT1553_XMTSWD1     = 0x0200,           //Transmitted SWD1
		MSGACT1553_XMTSWD2     = 0x0100,           //Transmitted SWD2
		MSGACT1553_RCVCWD1     = 0x0080,           //Received CWD1
		MSGACT1553_RCVCWD2     = 0x0040,           //Received CWD2
		MSGACT1553_RCVSWD1     = 0x0020,           //Received SWD1
		MSGACT1553_RCVSWD2     = 0x0010,           //Received SWD2
		MSGACT1553_XMTDWD      = 0x0008,           //Transmitted DWD
		MSGACT1553_RCVDWD      = 0x0004,           //Received DWD
		MSGACT1553_BUS         = 0x0001            //Bus A/B bit
};

/**
*
*  Field types.
*
**/

enum {	FIELD1553_CWD1         = 0x0000,           //CWD1 field
		FIELD1553_CWD2         = 0x0001,           //CWD2 field
		FIELD1553_SWD1         = 0x0002,           //SWD1 field
		FIELD1553_SWD2         = 0x0003,           //SWD2 field
		FIELD1553_TTAG         = 0x0004,           //Time tag field
		FIELD1553_ELAPSE       = 0x0005,           //Elapse time field
		FIELD1553_ERROR        = 0x0006,           //Error field
		FIELD1553_ACT          = 0x0007,           //Activity field
		FIELD1553_RESP1        = 0x0008,           //Response time 1 field
		FIELD1553_RESP2        = 0x0009,           //Response time 2 field
		FIELD1553_COUNT        = 0x000A,           //Data count field
		FIELD1553_FLAG1        = 0x000B,           //Flag 1 field
		FIELD1553_FLAG2        = 0x000C,           //Flag 2 field
		FIELD1553_EXTFLAG      = 0x000D,           //External flag field
		FIELD1553_TTAGH        = 0x000E            //Extended time tag field
};

/**
*
*  Channel info fields.
*
**/

enum {	INFO1553_MODEL         = 0x0001,            //Channel model type
		INFO1553_MAXCOUNT      = 0x0002,            //Channel terminal count
		INFO1553_MULTIMODE     = 0x0003,            //Channel supports multi-mode
		INFO1553_ERRORGEN      = 0x0004,            //Channel supports error generation
		INFO1553_CONCURMON     = 0x0005,            //Channel supports concuurent monitor
		INFO1553_MONFILTER     = 0x0006,            //Channel supports filtering of monitor
		INFO1553_PARAM         = 0x0007,            //Channel supports parametric control
		INFO1553_RTRESPTIME    = 0x0008,            //Channel supports programmable RT response time
		INFO1553_PLAYBACK      = 0x0009,            //Channel supports hardware playback mode
		INFO1553_VER2CHOBJ     = 0x000A,            //Channel supports version 2 channel object
		INFO1553_BCDFLTGAP     = 0x000B             //Channel supports programmable default gap time
};

/**
*
*  Parametric configuration options
*
**/

enum {	PARAMCFG1553_DEFAULT   = 0x00000000L,       //Select all default settings (default)
		PARAMCFG1553_AMPLON    = 0x00000000L,       //Enables parametric amplitude control (default)
		PARAMCFG1553_AMPLOFF   = 0x00000001L        //Disables parametric amplitude control
};

/**
*
*  Other flags.
*
**/

enum {	SUBADDRESS             = 0x0000,            //Selects Subaddress messages
		MODECODE               = 0x0001             //Selects Modecode messages
};

#endif
