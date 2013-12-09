/**
*
*  BTI429 RPC DRIVER  Version 1.4.0  (10/23/2009)
*  Copyright (c) 2004-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   BTI429.H -- RPC Driver
*                      BTI429 Driver Include Header File.
*
**/

/**
*
*  This file defines the procedures provided by the RPC
*  Driver Library for Ballard Technology ARINC 429 cards.
*  Applications using the BTI429 Driver Library must
*  incorporate this include file using the preprocessor
*  directive #include. If this file is in the current working
*  directory, the form would be:
*
*  #include "BTI429.H"
*
**/

/**
*
*  Conditional block to prevent multiple defines.
*
**/

#ifndef __BTI429_H
#define __BTI429_H

/**
*
*  Typedefs used by the BTI429 Driver.
*
**/

#ifndef BTI429API
#define BTI429API
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
*  Structs used by the BTI429 Driver.
*
**/

#ifndef MSGSTRUCT429

typedef struct
{
	MSGADDR	addr;
	UINT32	data;
} MSGSTRUCT429;

#endif

#ifndef LPMSGSTRUCT429
typedef MSGSTRUCT429 * LPMSGSTRUCT429;
#endif

#ifndef MSGFIELDS429

typedef struct
{
    UINT16 msgopt;              //Driver writes message options
    UINT16 msgact;              //Card writes message activity
    union {
        UINT32 msgdata;         //Card/User writes message data (Rcv/Xmt)
        UINT32 listptr;         //Driver writes list buffer pointer
    };
    union {
        UINT32 timetag;         //Card writes timetag
        UINT32 hitcount;        //Card writes hit count
    };
    union {
        UINT32 maxtime;         //Card writes max time
        UINT32 elapsetime;      //Card writes elapsed time
    };
    UINT32 mintime;             //Card writes min time
    UINT32 userptr;             //Driver writes user code pointer
    UINT32 timetagh;            //Card writes extended timetag
    UINT16 decgap;	            //Card writes measured decoder gap time (rsvd for xmt msg)
    UINT16 rsvd;	            //Reserved
} MSGFIELDS429;

#endif

#ifndef LPMSGFIELDS429
typedef MSGFIELDS429 * LPMSGFIELDS429;
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
*  BTI429 Driver functions.
*
**/

BTI429API UINT32 BTI429_BCDGetData(UINT32 msg,UINT16 msb,UINT16 lsb);
BTI429API UINT32 BTI429_BCDGetMant(UINT32 msg,UINT16 sigdig);
BTI429API UINT16 BTI429_BCDGetSign(UINT32 msg);
BTI429API UINT16 BTI429_BCDGetSSM(UINT32 msg);
BTI429API VOID BTI429_BCDGetVal(LPSTR buf,UINT32 msg,UINT16 sigdig,LPCSTR resolstr);
BTI429API UINT32 BTI429_BCDPutData(UINT32 msg,UINT32 value,UINT16 msb,UINT16 lsb);
BTI429API UINT32 BTI429_BCDPutMant(UINT32 msg,UINT32 value,UINT16 sigdig,UINT16 sign);
BTI429API UINT32 BTI429_BCDPutSign(UINT32 msg,UINT16 sign);
BTI429API UINT32 BTI429_BCDPutSSM(UINT32 msg,UINT16 value);
BTI429API UINT32 BTI429_BCDPutVal(LPCSTR buf,UINT32 msg,UINT16 sigdig,LPCSTR resolstr);
BTI429API UINT32 BTI429_BNRGetData(UINT32 msg,UINT16 msb,UINT16 lsb);
BTI429API UINT32 BTI429_BNRGetMant(UINT32 msg,UINT16 sigbit);
BTI429API UINT16 BTI429_BNRGetSign(UINT32 msg);
BTI429API UINT16 BTI429_BNRGetSSM(UINT32 msg);
BTI429API VOID BTI429_BNRGetVal(LPSTR buf,UINT32 msg,UINT16 sigbit,LPCSTR resolstr);
BTI429API UINT32 BTI429_BNRPutData(UINT32 msg,UINT32 value,UINT16 msb,UINT16 lsb);
BTI429API UINT32 BTI429_BNRPutMant(UINT32 msg,UINT32 value,UINT16 sigbit,UINT16 twos);
BTI429API UINT32 BTI429_BNRPutSign(UINT32 msg,UINT16 twos);
BTI429API UINT32 BTI429_BNRPutSSM(UINT32 msg,UINT16 value);
BTI429API UINT32 BTI429_BNRPutVal(LPCSTR buf,UINT32 msg,UINT16 sigbit,LPCSTR resolstr);
BTI429API ERRVAL BTI429_ChClear(INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_ChConfig(UINT32 configval,INT channum,HCARD handleval);
BTI429API VOID BTI429_ChGetCount(LPINT rcvcount,LPINT xmtcount,HCARD handleval);
BTI429API UINT32 BTI429_ChGetInfo(UINT16 infotype,INT channum,HCARD handleval);
BTI429API BOOL BTI429_ChIs429(INT channum,HCARD handleval);
BTI429API BOOL BTI429_ChIsRcv(INT channum,HCARD handleval);
BTI429API BOOL BTI429_ChIsXmt(INT channum,HCARD handleval);
BTI429API VOID BTI429_ChPause(INT channum,HCARD handleval);
BTI429API INT BTI429_ChPauseCheck(INT channum,HCARD handleval);
BTI429API VOID BTI429_ChResume(INT channum,HCARD handleval);
BTI429API BOOL BTI429_ChStart(INT channum,HCARD handleval);
BTI429API BOOL BTI429_ChStop(INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_ChSyncDefine(BOOL enableflag,UINT16 syncmask,UINT16 pinpolarity,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_ChTriggerDefine(BOOL enableflag,UINT16 trigmask,UINT16 trigval,UINT16 pinpolarity,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_FilterClear(BASEADDR baseaddr,HCARD handleval);
BTI429API MSGADDR BTI429_FilterDefault(UINT32 configval,INT channum,HCARD handleval);
BTI429API MSGADDR BTI429_FilterRd(INT labelval,INT sdival,INT channum,HCARD handleval);
BTI429API MSGADDR BTI429_FilterSet(UINT32 configval,INT labelval,INT sdimask,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_FilterWr(MSGADDR msgaddr,INT labelval,INT sdival,INT channum,HCARD handleval);
BTI429API UINT32 BTI429_FldGetData(UINT32 msgval);
BTI429API UINT16 BTI429_FldGetLabel(UINT32 msgval);
BTI429API UINT16 BTI429_FldGetParity(UINT32 msgval);
BTI429API UINT16 BTI429_FldGetSDI(UINT32 msgval);
BTI429API UINT32 BTI429_FldGetValue(UINT32 msgval,UINT16 startbit,UINT16 endbit);
BTI429API UINT32 BTI429_FldPutData(UINT32 msgval,UINT32 data);
BTI429API UINT32 BTI429_FldPutLabel(UINT32 msgval,UINT16 label);
BTI429API UINT32 BTI429_FldPutSDI(UINT32 msgval,UINT16 sdi);
BTI429API UINT32 BTI429_FldPutValue(UINT32 msgval,UINT32 dataval,UINT16 startbit,UINT16 endbit);
BTI429API LISTADDR BTI429_ListAsyncCreate(UINT32 listconfigval,INT count,INT channum,HCARD handleval);
BTI429API UINT32 BTI429_ListDataRd(LISTADDR listaddr,HCARD handleval);
BTI429API BOOL BTI429_ListDataWr(UINT32 value,LISTADDR listaddr,HCARD handleval);
BTI429API LISTADDR BTI429_ListRcvCreate(UINT32 listconfigval,INT count,MSGADDR msgaddr,HCARD handleval);
BTI429API INT BTI429_ListStatus(LISTADDR listaddr,HCARD handleval);
BTI429API LISTADDR BTI429_ListXmtCreate(UINT32 listconfigval,INT count,MSGADDR msgaddr,HCARD handleval);
BTI429API MSGADDR BTI429_MsgBlockRd(LPMSGFIELDS429 msgfields,MSGADDR msgaddr,HCARD handleval);
BTI429API MSGADDR BTI429_MsgBlockWr(LPMSGFIELDS429 msgfields,MSGADDR msgaddr,HCARD handleval);
BTI429API MSGADDR BTI429_MsgCreate(UINT32 configval,HCARD handleval);
BTI429API UINT32 BTI429_MsgDataRd(MSGADDR msgaddr,HCARD handleval);
BTI429API VOID BTI429_MsgDataWr(UINT32 value,MSGADDR msgaddr,HCARD handleval);
BTI429API VOID BTI429_MsgGroupBlockRd(LPMSGFIELDS429 msgfields,LPMSGADDR msgaddrptr,INT nummsgs,HCARD handleval);
BTI429API VOID BTI429_MsgGroupBlockWr(LPMSGFIELDS429 msgfields,LPMSGADDR msgaddrptr,INT nummsgs,HCARD handleval);
BTI429API VOID BTI429_MsgGroupRd(LPUINT32 msgdataptr,LPMSGADDR msgaddrptr,INT nummsgs,HCARD handleval);
BTI429API VOID BTI429_MsgGroupWr(LPUINT32 msgdataptr,LPMSGADDR msgaddrptr,INT nummsgs,HCARD handleval);
BTI429API BOOL BTI429_MsgIsAccessed(MSGADDR msgaddr,HCARD handleval);
BTI429API BOOL BTI429_MsgSkipRd(MSGADDR msgaddr,HCARD handleval);
BTI429API VOID BTI429_MsgSkipWr(BOOL skip,MSGADDR msgaddr,HCARD handleval);
BTI429API ERRVAL BTI429_MsgSyncDefine(BOOL enableflag,UINT16 syncmask,UINT16 pinpolarity,MSGADDR msgaddr,HCARD handleval);
BTI429API ERRVAL BTI429_MsgTriggerDefine(BOOL enableflag,UINT16 trigmask,UINT16 trigval,UINT16 pinpolarity,MSGADDR msgaddr,HCARD handleval);
BTI429API ERRVAL BTI429_ParamAmplitudeConfig(UINT32 configval,UINT16 dacval,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_ParamAmplitudeGet(LPUINT16 dacval,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_ParamBitRateConfig(UINT32 configval,UINT16 divval,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_ParamBitRateGet(LPUINT16 divval,INT channum,HCARD handleval);
BTI429API SCHNDX BTI429_SchedBranch(UINT32 condition,SCHNDX destindex,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_SchedBranchUpdate(SCHNDX destindex,SCHNDX opcodeindex,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_SchedBuild(INT nummsgs,LPMSGADDR msgaddr,LPINT minperiod,LPINT maxperiod,INT channum,HCARD handleval);
BTI429API SCHNDX BTI429_SchedCall(UINT32 condition,SCHNDX destindex,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_SchedCallUpdate(SCHNDX destindex,SCHNDX opcodeindex,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_SchedMode(UINT32 modeval);
BTI429API SCHNDX BTI429_SchedEntry(INT channum,HCARD handleval);
BTI429API SCHNDX BTI429_SchedGap(UINT16 gapval,INT channum,HCARD handleval);
BTI429API SCHNDX BTI429_SchedGapFixed(UINT16 gapval,INT channum,HCARD handleval);
BTI429API SCHNDX BTI429_SchedGapList(UINT16 gapval,LISTADDR listaddr,INT channum,HCARD handleval);
BTI429API SCHNDX BTI429_SchedHalt(INT channum,HCARD handleval);
BTI429API SCHNDX BTI429_SchedLog(UINT32 condition,UINT16 tagval,INT channum,HCARD handleval);
BTI429API SCHNDX BTI429_SchedMsg(MSGADDR msgaddr,INT channum,HCARD handleval);
BTI429API SCHNDX BTI429_SchedMsgEx(MSGADDR msgaddr,UINT16 gapval,INT channum,HCARD handleval);
BTI429API SCHNDX BTI429_SchedPause(INT channum,HCARD handleval);
BTI429API SCHNDX BTI429_SchedPulse(INT dionum,INT channum,HCARD handleval);
BTI429API SCHNDX BTI429_SchedRestart(INT channum,HCARD handleval);
BTI429API SCHNDX BTI429_SchedReturn(INT channum,HCARD handleval);

/**
*
*  Internal functions used by the BTI429 Driver.
*
**/

BTI429API ERRVAL BTI429_ChConfigEx(UINT32 configval,UINT16 count,INT channum,HCARD handleval);
BTI429API UINT32 BTI429_CmdAddr(SCHNDX index,INT channum,HCARD handleval);
BTI429API UINT32 BTI429_CmdAlloc(INT count,HCARD handleval);
BTI429API UINT32 BTI429_CmdAllocEx(UINT32 configval,INT count,HCARD handleval);
BTI429API UINT32 BTI429_CmdBaseRd(INT channum,HCARD handleval);
BTI429API VOID BTI429_CmdBaseWr(UINT32 addrval,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_CmdClear(INT channum,HCARD handleval);
BTI429API UINT16 BTI429_CmdCountRd(INT channum,HCARD handleval);
BTI429API VOID BTI429_CmdCountWr(UINT16 countval,INT channum,HCARD handleval);
BTI429API UINT16 BTI429_CmdCtrlRd(SCHNDX index,INT channum,HCARD handleval);
BTI429API VOID BTI429_CmdCtrlWr(UINT16 ctrlval,SCHNDX index,INT channum,HCARD handleval);
BTI429API UINT16 BTI429_CmdCurrRd(INT channum,HCARD handleval);
BTI429API VOID BTI429_CmdCurrWr(UINT16 addrval,INT channum,HCARD handleval);
BTI429API VOID BTI429_CmdInit(UINT16 opcode,LPUINT16 block);
BTI429API SCHNDX BTI429_CmdInsert(LPUINT16 block,INT channum,HCARD handleval);
BTI429API UINT16 BTI429_CmdMaxLoopRd(INT channum,HCARD handleval);
BTI429API VOID BTI429_CmdMaxLoopWr(UINT16 countval,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_CmdRd(LPUINT16 block,UINT32 addrval,HCARD handleval);
BTI429API VOID BTI429_CmdRestart(INT channum,HCARD handleval);
BTI429API BOOL BTI429_CmdShotRd(SCHNDX index,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_CmdShotWr(BOOL value,SCHNDX index,INT channum,HCARD handleval);
BTI429API BOOL BTI429_CmdSkipRd(SCHNDX index,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_CmdSkipWr(BOOL value,SCHNDX index,INT channum,HCARD handleval);
BTI429API VOID BTI429_CmdStackClr(INT channum,HCARD handleval);
BTI429API UINT16 BTI429_CmdStartRd(INT channum,HCARD handleval);
BTI429API VOID BTI429_CmdStartWr(UINT16 addrval,INT channum,HCARD handleval);
BTI429API BOOL BTI429_CmdStepRd(SCHNDX index,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_CmdStepWr(BOOL value,SCHNDX index,INT channum,HCARD handleval);
BTI429API INT BTI429_CmdTotalRd(INT channum,HCARD handleval);
BTI429API VOID BTI429_CmdTotalWr(INT countval,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_CmdWr(LPUINT16 block,UINT32 addrval,HCARD handleval);
BTI429API UINT32 BTI429_ConvFromArinc(UINT32 msgval);
BTI429API UINT32 BTI429_ConvToArinc(UINT32 msgval);
BTI429API INT BTI429_DllUsageCount(VOID);
BTI429API UINT32 BTI429_FilterAddr(INT labelval,INT sdival,INT channum,HCARD handleval);
BTI429API BASEADDR BTI429_FilterCreate(HCARD handleval);
BTI429API ERRVAL BTI429_FilterFill(MSGADDR msgaddr,BASEADDR baseaddr,HCARD handleval);
BTI429API UINT16 BTI429_FldGetParBit(UINT32 msgval);
BTI429API UINT32 BTI429_FldPutParBit(UINT32 msgval);
BTI429API VOID BTI429_GetChanCount(LPINT rcvcount,LPINT xmtcount,HCARD handleval);
BTI429API BOOL BTI429_IsChan(INT channum,HCARD handleval);
BTI429API BOOL BTI429_IsRcvChan(INT channum,HCARD handleval);
BTI429API BOOL BTI429_IsXmtChan(INT channum,HCARD handleval);
BTI429API UINT32 BTI429_LabelReverse(UINT32 msgval);
BTI429API UINT32 BTI429_ListAddr(INT index,LISTADDR listaddr,HCARD handleval);
BTI429API UINT32 BTI429_ListAddrEx(INT index,LISTADDR listaddr,LPUINT16 header,HCARD handleval);
BTI429API ERRVAL BTI429_ListAdvance(LISTADDR listaddr,HCARD handleval);
BTI429API ERRVAL BTI429_ListClear(LISTADDR listaddr,HCARD handleval);
BTI429API LISTADDR BTI429_ListConfig(UINT32 configval,INT count,HCARD handleval);
BTI429API LISTADDR BTI429_ListConfigEx(UINT32 configval,INT count,HCARD handleval);
BTI429API BOOL BTI429_ListDataBlkRd(LPUINT32 dataptr,LPUINT16 datacountptr,LISTADDR listaddr,HCARD handleval);
BTI429API BOOL BTI429_ListDataBlkWr(LPUINT32 dataptr,UINT16 datacount,LISTADDR listaddr,HCARD handleval);
BTI429API UINT32 BTI429_ListDataRdEx(INT index,LISTADDR listaddr,HCARD handleval);
BTI429API VOID BTI429_ListDataWrEx(UINT32 value,INT index,LISTADDR listaddr,HCARD handleval);
BTI429API ERRVAL BTI429_ListInit(LISTADDR listaddr,MSGADDR msgaddr,HCARD handleval);
BTI429API UINT32 BTI429_ListNextRd(UINT32 listaddr,LPUINT16 header,HCARD handleval);
BTI429API UINT32 BTI429_ListNextWr(UINT32 listaddr,LPUINT16 header,HCARD handleval);
BTI429API VOID BTI429_ListPostRd(LISTADDR listaddr,LPUINT16 header,HCARD handleval);
BTI429API VOID BTI429_ListPostWr(LISTADDR listaddr,LPUINT16 header,HCARD handleval);
BTI429API VOID BTI429_ListPreRd(LISTADDR listaddr,LPUINT16 header,HCARD handleval);
BTI429API VOID BTI429_ListPreWr(LISTADDR listaddr,LPUINT16 header,HCARD handleval);
BTI429API UINT16 BTI429_MsgActivityRd(MSGADDR msgaddr,HCARD handleval);
BTI429API VOID BTI429_MsgActivityWr(UINT16 value,MSGADDR msgaddr,HCARD handleval);
BTI429API UINT16 BTI429_MsgCheck(MSGADDR msgaddr,HCARD handleval);
BTI429API MSGADDR BTI429_MsgCommRd(LPMSGFIELDS429 msgfields,MSGADDR msgaddr,HCARD handleval);
BTI429API MSGADDR BTI429_MsgCommWr(LPMSGFIELDS429 msgfields,MSGADDR msgaddr,HCARD handleval);
BTI429API ERRVAL BTI429_MsgConfig(UINT32 configval,MSGADDR msgaddr,HCARD handleval);
BTI429API UINT16 BTI429_MsgOptRd(MSGADDR msgaddr,HCARD handleval);
BTI429API VOID BTI429_MsgOptWr(UINT16 msgopt,MSGADDR msgaddr,HCARD handleval);
BTI429API MSGADDR BTI429_MsgReset(MSGADDR msgaddr,HCARD handleval);
BTI429API ERRVAL BTI429_ProcSetPost(UINT32 useraddr,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_ProcSetPre(UINT32 useraddr,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_Sched1Build(INT basemsgnum,INT nummsgs,LPMSGADDR msgaddr,LPINT minperiodIn,LPINT maxperiodIn,BOOL speed,BOOL microflag,INT channum,BOOL debug,HCARD handleval);
BTI429API ERRVAL BTI429_Sched2AdvanceMessage(INT entries,INT panicindex,INT nummsgs,LPVOID infoptr,LPINT nextdeltaptr,INT ulMasterTimer,LPMSGADDR msgaddrptr,BOOL debugflag,BOOL tryflag,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_Sched2AdvanceNoMessage(INT nummsgs,LPVOID infoptr,LPINT nextdeltaptr,INT ulMasterTimer,LPMSGADDR msgaddrptr,BOOL debugflag,BOOL tryflag,INT channum,HCARD handleval);
BTI429API VOID BTI429_Sched2Analyze(INT nummsgs,LPVOID infoptr);
BTI429API ERRVAL BTI429_Sched2Build(INT nummsgs,LPMSGADDR msgaddrptr,LPINT minlist,LPINT maxlist,BOOL speedflag,BOOL microflag,BOOL debugflag,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_Sched2BuildEx(LPINT styleptr,INT nummsgs,LPINT minlist,LPINT maxlist,BOOL speedflag,BOOL microflag);
BTI429API ERRVAL BTI429_Sched2BuildExx(INT style,BOOL speedflag,BOOL microflag,BOOL debugflag,BOOL tryflag,INT nummsgs,LPINT minlist,LPINT maxlist,LPINT entriesptr,LPINT maxentriesptr,LPVOID infoptr,LPMSGADDR msgaddrptr,INT channum,HCARD handleval);
BTI429API UINT32 BTI429_SchedAdvance(LPVOID infoptr);
BTI429API SCHNDX BTI429_SchedAgain(SCHNDX index,INT channum,HCARD handleval);
BTI429API SCHNDX BTI429_SchedBranchEx(UINT16 condition1,UINT16 condition2,SCHNDX destindex,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_SchedBuildEx(INT nummsgs,LPMSGADDR msgaddr,LPINT minperiod,LPINT maxperiod,BOOL speed,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_SchedBuildExx(INT basemsgnum,INT nummsgs,LPMSGADDR msgaddr,LPINT minperiod,LPINT maxperiod,BOOL speed,LPVOID buf,UINT32 bufsize,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_SchedBuildExxx(INT nummsgs,LPMSGADDR msgaddr,LPINT minperiod,LPINT maxperiod,BOOL speed,INT channum,BOOL debug,BOOL microflag,INT algorithm,HCARD handleval);
BTI429API UINT32 BTI429_SchedCalc(LPVOID infoptr);
BTI429API SCHNDX BTI429_SchedCallEx(UINT16 condition1,UINT16 condition2,SCHNDX destindex,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_SchedCheck(LPINT minperiod,LPINT maxperiod,LPVOID infoptr);
BTI429API VOID BTI429_SchedCheckEnd();
BTI429API VOID BTI429_SchedCheckMiddle(UINT32 msgnum,UINT32 minlen,UINT32 midlen,UINT32 maxlen);
BTI429API VOID BTI429_SchedCheckReal(UINT32 msgnum,UINT32 minlen,UINT32 midlen,UINT32 maxlen);
BTI429API VOID BTI429_SchedCheckStart();
BTI429API UINT32 BTI429_SchedConflicts(LPVOID infoptr);
BTI429API VOID BTI429_SchedDisplay(UINT32 nummsgs,LPUINT32 count,LPUINT32 *data);
BTI429API ERRVAL BTI429_SchedDivide(LPVOID infoptr);
BTI429API UINT32 BTI429_SchedFix(LPVOID infoptr);
BTI429API VOID BTI429_SchedFixEnd();
BTI429API VOID BTI429_SchedFixMiddle(UINT32 conflicts);
BTI429API VOID BTI429_SchedFixStart();
BTI429API VOID BTI429_SchedHook(INT index,LPVOID ptr);
BTI429API ERRVAL BTI429_SchedInsert(UINT32 events,UINT32 index,UINT32 msgnum,LPMSGADDR msgaddr,UINT32 gapval,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_SchedInsert2(UINT32 events,UINT32 index,INT msgnum,LPMSGADDR msgaddr,UINT32 gapval,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_SchedInsertEnd(UINT32 events,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_SchedInsertStart(UINT32 events,INT channum,HCARD handleval);
BTI429API VOID BTI429_SchedLayout(LPVOID infoptr);
BTI429API SCHNDX BTI429_SchedLogEx(UINT16 condition1,UINT16 condition2,UINT16 tagval,INT channum,HCARD handleval);
BTI429API UINT32 BTI429_SchedMaxval(LPUINT32 array,UINT32 arraycount);
BTI429API UINT32 BTI429_SchedMinval(LPUINT32 array,UINT32 arraycount);
BTI429API SCHNDX BTI429_SchedNop(INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_SchedParse(INT basemsgnum,LPMSGADDR msgaddr,INT channum,LPVOID infoptr,HCARD handleval);
BTI429API VOID BTI429_SchedPrint(LPSTR str,...);
BTI429API VOID BTI429_SchedPrintEx(INT type,LPSTR str,...);
BTI429API SCHNDX BTI429_SchedPulse0(INT dionum,INT channum,HCARD handleval);
BTI429API SCHNDX BTI429_SchedPulse1(INT dionum,INT channum,HCARD handleval);
BTI429API ERRVAL BTI429_SchedReset(LPVOID infoptr);
BTI429API UINT32 BTI429_SchedRotate(LPVOID infoptr);
BTI429API INT BTI429_SchedShift(INT msgnum,INT value,LPVOID infoptr);
BTI429API ERRVAL BTI429_SchedSlide(LPVOID infoptr);
BTI429API VOID BTI429_SchedSlideEnd();
BTI429API VOID BTI429_SchedSlideMiddle(UINT32 index,UINT32 count,UINT32 conflicts);
BTI429API VOID BTI429_SchedSlideStart(UINT32 factor);
BTI429API VOID BTI429_SchedSort(LPVOID infoptr);
BTI429API UINT32 BTI429_SchedToBit(UINT32 value,LPVOID infoptr);
BTI429API UINT32 BTI429_SchedToMsec(UINT32 value,LPVOID infoptr);
BTI429API VOID BTI429_SchedUnsort(LPVOID infoptr);
BTI429API SCHNDX BTI429_SchedUser(UINT32 useraddr,INT channum,HCARD handleval);
BTI429API UINT32 BTI429_SchedWalk(LPVOID infoptr);
BTI429API ERRVAL BTI429_TestProtocol(HCARD handleval);
BTI429API UINT16 BTI429_Transpose12(UINT16 val);

#ifdef __cplusplus
}
#endif

/**
*
*  Channel number constants
*
**/

#define CH0  0x0000           //Channel 0
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
*  SDI selection constants
*
**/

enum {	SDI00                 = 0x0001,           //Enable SDI00
		SDI01                 = 0x0002,           //Enable SDI01
		SDI10                 = 0x0004,           //Enable SDI10
		SDI11                 = 0x0008,           //Enable SDI11
		SDIALL                = 0x000F            //Enable all SDIs
};

/**
*
*  Channel configuration options
*
**/

enum {	CHCFG429_DEFAULT      = 0x00000000L,      //Select all default settings (XMT & RCV) (default)
		CHCFG429_HIGHSPEED    = 0x00000001L,      //Select high speed (XMT & RCV)
		CHCFG429_AUTOSPEED    = 0x00000002L,      //Select auto speed detection (RCV)
		CHCFG429_LOWSPEED     = 0x00000000L,      //Select low speed (XMT & RCV) (default)
		CHCFG429_SELFTEST     = 0x00000004L,      //Enable internal wraparound (XMT & RCV)
		CHCFG429_SELFTESTOFF  = 0x00000000L,      //Disable internal wraparound (XMT & RCV) (default)
		CHCFG429_SYNC         = 0x00000008L,      //Sync Enable (XMT & RCV)
		CHCFG429_SYNCOFF      = 0x00000000L,      //Disable sync output (XMT & RCV) (default)
		CHCFG429_PARODD       = 0x00000000L,      //Parity odd (XMT & RCV) (default)
		CHCFG429_PAREVEN      = 0x00000010L,      //Parity even (XMT & RCV)
		CHCFG429_PARDATA      = 0x00000020L,      //Parity bit as data (XMT & RCV)
		CHCFG429_ACTIVE       = 0x00000000L,      //Enable channel activity (XMT & RCV) (default)
		CHCFG429_INACTIVE     = 0x00000040L,      //Disable channel activity (XMT & RCV)
		CHCFG429_EXTTRIG      = 0x00000080L,      //Enable external trigger for all messages (XMT)
		CHCFG429_EXTOFF       = 0x00000000L,      //External trigger is enabled on message level (XMT) (default)
		CHCFG429_PARERR       = 0x00000100L,      //Enable parity error for all messages (XMT)
		CHCFG429_NOERR        = 0x00000000L,      //Errors are enabled on message level (XMT) (default)
		CHCFG429_HIT	      = 0x00000200L,      //Hit counter is enabled for all messages
		CHCFG429_NOHIT		  = 0x00000000L,      //Hit counter is enabled on message level (default)
		CHCFG429_TIMETAG      = 0x00000400L,      //Enable time-tag for all message records
		CHCFG429_TIMETAGOFF   = 0x00000000L,      //Time-tag is enabled on message level (default)
		CHCFG429_ELAPSE       = 0x00000800L,      //Enable elapse time for all messages
		CHCFG429_ELAPSEOFF    = 0x00000000L,      //Elapse time is enabled on message level (default)
		CHCFG429_MAX          = 0x00001000L,      //Enable max repetition rate monitoring for all messages
		CHCFG429_MIN          = 0x00002000L,      //Enable min repetition rate monitoring for all messages
		CHCFG429_MAXMIN       = 0x00003000L,      //Enable max and min repetition rate monitoring for all messages
		CHCFG429_MAXMINOFF    = 0x00000000L,      //Repetition rate monitoring is enabled on message level (default)
		CHCFG429_SEQALL       = 0x00080000L,      //Record entire channel in sequential record
		CHCFG429_SEQSEL       = 0x00000000L,      //Sequential record recording is enabled at message level (default)
		CHCFG429_LOOPMAX      = 0x00100000L,      //Enable schedule maximum loop count
		CHCFG429_NOLOOPMAX    = 0x00000000L,      //Disable schedule maximum loop count (default)
		CHCFG429_LOGHALT      = 0x00200000L,      //Enable event log on schedule halt
		CHCFG429_NOLOGHALT    = 0x00000000L,      //No event log on schedule halt (default)
		CHCFG429_LOGPAUSE     = 0x00400000L,      //Enable event log on schedule pause
		CHCFG429_NOLOGPAUSE   = 0x00000000L,      //No event log on schedule pause (default)
		CHCFG429_LOGERR       = 0x00800000L,      //Enable event log on decoder errors.
		CHCFG429_NOLOGERR	  = 0x00000000L,      //No event log on decoder errors (default)
		CHCFG429_PAUSE        = 0x01000000L,      //Mark channel as paused
		CHCFG429_UNPAUSE      = 0x00000000L       //Mark channel as unpaused (default)
};

/**
*
*  Message configuration options
*
**/

enum {	MSGCRT429_DEFAULT     = 0x00000000L,      //Default settings
		MSGCRT429_NOSEQ       = 0x00000000L,      //Message will not be recorded in sequential record (default)
		MSGCRT429_SEQ         = 0x00000001L,      //Message will be recorded in sequential record
		MSGCRT429_NOLOG       = 0x00000000L,      //Message will not generate event log (default)
		MSGCRT429_LOG         = 0x00000002L,      //Message will generate event log
		MSGCRT429_NOSKIP      = 0x00000000L,      //Message will not be skipped (default)
		MSGCRT429_SKIP        = 0x00000008L,      //Message will be skipped
		MSGCRT429_NOTIMETAG   = 0x00000000L,      //Message will not record time-tag (default)
		MSGCRT429_TIMETAG     = 0x00000010L,      //Message will record time-tag
		MSGCRT429_NOELAPSE    = 0x00000000L,      //Message will not record elapse time (default)
		MSGCRT429_ELAPSE      = 0x00000020L,      //Message will record elapse time
		MSGCRT429_NOMAXMIN    = 0x00000000L,      //Message will not record min/max time (default)
		MSGCRT429_MAX         = 0x00000040L,      //Message will record max time
		MSGCRT429_MIN         = 0x00000080L,      //Message will record min time
		MSGCRT429_MAXMIN      = 0x000000C0L,      //Message will record min/max time
		MSGCRT429_NOSYNC      = 0x00000000L,      //No sync will be generated for message (default)
		MSGCRT429_SYNC        = 0x00000100L,      //Sync will be generated for message
		MSGCRT429_NOERR       = 0x00000000L,      //No error will be generated for message (default)
		MSGCRT429_PARERR      = 0x00000200L,      //Parity error will be generated for message
		MSGCRT429_NOHIT       = 0x00000000L,      //Message will not record hit count (default)
		MSGCRT429_HIT         = 0x00000400L,      //Message will record hit count
		MSGCRT429_NOEXTRIG    = 0x00000000L,      //Message will not be externally triggered (default)
		MSGCRT429_EXTRIG      = 0x00001000L,      //Message will be externally triggered
		MSGCRT429_WIPE        = 0x00000000L,      //Enables message clear (default)
		MSGCRT429_NOWIPE      = 0x80000000L,      //Disables message clear
		MSGCRT429_WIPE0       = 0x00000000L,      //Initialize data with zeros (default)
		MSGCRT429_WIPE1       = 0x40000000L       //Initialize data with ones
};

/**
*
*  List buffer options
*
**/

enum {	LISTCRT429_DEFAULT    = 0x00000000L,      //Select all default settings
		LISTCRT429_FIFO       = 0x00000000L,      //Enable FIFO mode (default)
		LISTCRT429_PINGPONG   = 0x00000001L,      //Enable ping-pong mode
		LISTCRT429_CIRCULAR   = 0x00000002L,      //Enable circular mode
		LISTCRT429_RCV	      = 0x00000010L,      //User will read from list buffer
		LISTCRT429_XMT        = 0x00000020L,      //User will write to list buffer
		LISTCRT429_ASYNC      = 0x00000040L,      //Asynchronous mode
		LISTCRT429_NOLOG      = 0x00000000L,      //Do not generate event log when list buffer empty/full (default)
		LISTCRT429_LOG        = 0x00000100L       //Generate event log when list buffer empty/full
};

/**
*
*  Parametric configuration options
*
**/

enum {	PARAMCFG429_DEFAULT    = 0x00000000L,     //Select all default settings (default)
		PARAMCFG429_AMPLON     = 0x00000000L,     //Enables parametric amplitude control (default)
		PARAMCFG429_AMPLOFF    = 0x00000001L,     //Disables parametric amplitude control
		PARAMCFG429_BITRATEON  = 0x00000000L,     //Enables parametric bit rate control (default)
		PARAMCFG429_BITRATEOFF = 0x00000002L      //Disables parametric bit rate control
};

/**
*
*  Scheduling conditions.
*
**/

enum {	COND429_ALWAYS        = 0x00000000L,      //Unconditional
		COND429_DIO1ACT       = 0x00000001L,      //Condition on digital I/O #1 active
		COND429_DIO1NACT      = 0x00000002L,      //Condition on digital I/O #1 not active
		COND429_DIO2ACT       = 0x00000004L,      //Condition on digital I/O #2 active
		COND429_DIO2NACT      = 0x00000008L,      //Condition on digital I/O #2 not active
		COND429_DIO3ACT       = 0x00000010L,      //Condition on digital I/O #3 active
		COND429_DIO3NACT      = 0x00000020L,      //Condition on digital I/O #3 not active
		COND429_DIO4ACT       = 0x00000040L,      //Condition on digital I/O #4 active
		COND429_DIO4NACT      = 0x00000080L,      //Condition on digital I/O #4 not active
		COND429_DIO5ACT       = 0x00000100L,      //Condition on digital I/O #5 active
		COND429_DIO5NACT      = 0x00000200L,      //Condition on digital I/O #5 not active
		COND429_DIO6ACT       = 0x00000400L,      //Condition on digital I/O #6 active
		COND429_DIO6NACT      = 0x00000800L,      //Condition on digital I/O #6 not active
		COND429_DIO7ACT       = 0x00001000L,      //Condition on digital I/O #7 active
		COND429_DIO7NACT      = 0x00002000L,      //Condition on digital I/O #7 not active
		COND429_DIO8ACT       = 0x00004000L,      //Condition on digital I/O #8 active
		COND429_DIO8NACT      = 0x00008000L       //Condition on digital I/O #8 not active
};

/**
*
*  Bit fields of the message activity flag.
*
**/

enum {	MSGACT429_CHMASK      = 0xFF00,           //Channel number mask value
		MSGACT429_CHSHIFT     = 0x0008,           //Channel number shift value
		MSGACT429_SPD         = 0x0080,           //Bus speed
		MSGACT429_ERR         = 0x0040,           //Error bit
		MSGACT429_GAP         = 0x0020,           //Gap error bit
		MSGACT429_PAR         = 0x0010,           //Parity error bit
		MSGACT429_LONG        = 0x0008,           //Long word error bit
		MSGACT429_BIT         = 0x0004,           //Bit time error bit
		MSGACT429_TO          = 0x0002,           //Time out error bit
		MSGACT429_HIT         = 0x0001            //Always set
};

/**
*
*  Channel info fields.
*
**/

enum {	INFO429_PARAM         = 0x0001           //Channel supports parametric control
};

/**
*
*  Scheduling algorithm options
*
**/

enum {
		SCHEDMODE_ALGOR_MASK    = 0x000F,         //Mask for algorithm setting
		SCHEDMODE_DEFAULT_ALGOR = 0x0000,         //Choose the default algorithm
		SCHEDMODE_QUICK_ALGOR   = 0x0001,         //Choose the quick algorithm
		SCHEDMODE_BOTH_ALGOR    = 0x0002,         //Use the quick algorithm first, if fails, then uses the normal algorithm
		SCHEDMODE_MILLISEC      = 0x0000,         //Specify periods in milliseconds
		SCHEDMODE_MICROSEC      = 0x0010,         //Specify periods in microseconds
		SCHEDMODE_REMOTE        = 0x0000,         //Perform calculations remotely, if applicable
		SCHEDMODE_LOCAL         = 0x0020          //Perform calculations locally, if applicable
};

#endif
