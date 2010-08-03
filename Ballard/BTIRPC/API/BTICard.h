/**
*
*  BTICard Linux Driver  Version 1.5.4  (10/23/2009)
*  Copyright (c) 2004-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   BTICard.H -- Linux
*                       BTICard Driver Include Header File.
*
**/

/**
*
*  This file defines the procedures provided by the Linux
*  Driver Library for Ballard Technology interface cards.
*  Applications using the BTICard Driver Library must
*  incorporate this include file using the preprocessor directive
*  #include. If this file is in the current working directory,
*  the form would be:
*
*  #include "BTICard.H"
*
**/

/**
*
*  Conditional block to prevent multiple defines.
*
**/

#ifndef __BTICard_H
#define __BTICard_H

/**
*
*  Typedefs used by the BTICard Driver.
*
**/

#ifndef BTICardAPI
#define BTICardAPI
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

#ifndef ULONG_PTR
#define ULONG_PTR unsigned long
#endif

#ifndef LPULONG_PTR
#define LPULONG_PTR unsigned long *
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

#ifndef CHAR
#define CHAR char
#endif

#ifndef BOOL
#define BOOL int
#endif

#ifndef HCARD
#define HCARD long
#endif

#ifndef LPHCARD
#define LPHCARD long *
#endif

#ifndef HCORE
#define HCORE long
#endif

#ifndef LPHCORE
#define LPHCORE long *
#endif

#ifndef HRPC
#define HRPC long
#endif

#ifndef LPHRPC
#define LPHRPC long *
#endif

#ifndef ERRVAL
#define ERRVAL int
#endif

#ifndef SCHNDX
#define SCHNDX int
#endif

/**
*
*  Structs used by the BTICard Driver.
*
**/

#ifndef SEQRECORD1553
typedef struct
{
	UINT16 type;               //Valid in all versions
	UINT16 count;              //Valid in all versions
	UINT32 timestamp;          //Valid in all versions
	UINT16 activity;           //Valid in all versions
	UINT16 error;              //Valid in all versions
	UINT16 cwd1;               //Valid in all versions
	UINT16 cwd2;               //Valid in all versions
	UINT16 swd1;               //Valid in all versions
	UINT16 swd2;               //Valid in all versions
	UINT16 datacount;          //Valid in all versions
	UINT16 data[40];           //Variable length (don't exceed data[datacount-1])
} SEQRECORD1553;
#endif

#ifndef LPSEQRECORD1553
typedef SEQRECORD1553 * LPSEQRECORD1553;
#endif

#ifndef SEQRECORDMORE1553
typedef struct
{
	UINT32 timestamph;         //Valid if version of base record (SEQRECORD1553) >= 1
	UINT16 resptime1;          //Valid if version of base record (SEQRECORD1553) >= 1
	UINT16 resptime2;          //Valid if version of base record (SEQRECORD1553) >= 1
} SEQRECORDMORE1553;
#endif

#ifndef LPSEQRECORDMORE1553
typedef SEQRECORDMORE1553 * LPSEQRECORDMORE1553;
#endif

#ifndef SEQRECORD429
typedef struct
{
	UINT16 type;               //Valid in all versions
	UINT16 count;              //Valid in all versions
	UINT32 timestamp;          //Valid in all versions
	UINT16 activity;           //Valid in all versions
	UINT16 decgap;             //Valid if version >= 1
	UINT32 data;               //Valid in all versions
	UINT32 timestamph;         //Valid if version >= 1
} SEQRECORD429;
#endif

#ifndef LPSEQRECORD429
typedef SEQRECORD429 * LPSEQRECORD429;
#endif

#ifndef SEQRECORD717
typedef struct
{
	UINT16 type;               //Valid in all versions
	UINT16 count;              //Valid in all versions
	UINT32 timestamp;          //Valid in all versions
	UINT16 activity;           //Valid in all versions
	UINT16 wordnum;            //Valid in all versions
	UINT16 subframe;           //Valid in all versions
	UINT16 superframe;         //Valid in all versions
	UINT16 data;               //Valid in all versions
	UINT16 rsvd9;              //Valid if version >= 1
	UINT32 timestamph;         //Valid if version >= 1
} SEQRECORD717;
#endif

#ifndef LPSEQRECORD717
typedef SEQRECORD717 * LPSEQRECORD717;
#endif

#ifndef SEQRECORD708
typedef struct
{
	UINT16 type;               //Valid in all versions
	UINT16 count;              //Valid in all versions
	UINT32 timestamp;          //Valid in all versions
	UINT16 activity;           //Valid in all versions
	UINT16 datacount;          //Valid in all versions
	UINT16 data[100];          //Valid in all versions
	UINT16 extra[16];          //Valid if version >= 1
	UINT16 bitcount;           //Valid if version >= 1
	UINT16 rsvd123;            //Valid if version >= 1
	UINT32 timestamph;         //Valid if version >= 1
} SEQRECORD708;
#endif

#ifndef LPSEQRECORD708
typedef SEQRECORD708 * LPSEQRECORD708;
#endif

#ifndef SEQRECORDCSDB
typedef struct
{
	UINT16 type;               //Valid in all versions
	UINT16 count;              //Valid in all versions
	UINT32 timestamp;          //Valid in all versions
	UINT32 timestamph;         //Valid in all versions
	UINT16 activity;           //Valid in all versions
	UINT16 datacount;          //Valid in all versions
	UINT16 data[32];           //Valid in all versions
} SEQRECORDCSDB;
#endif

#ifndef LPSEQRECORDCSDB
typedef SEQRECORDCSDB * LPSEQRECORDCSDB;
#endif

#ifndef SEQRECORDDIO
typedef struct
{
	UINT16 type;               //Valid in all versions
	UINT16 count;              //Valid in all versions
	UINT16 bank;               //Valid in all versions
	UINT16 state;              //Valid in version 0
	UINT32 timestamp;          //Valid in all versions
	UINT32 timestamph;         //Valid in all versions
	UINT16 change;             //Valid in version 1
	UINT16 value;              //Valid in version 1
} SEQRECORDDIO;
#endif

#ifndef LPSEQRECORDDIO
typedef SEQRECORDDIO * LPSEQRECORDDIO;
#endif

#ifndef SEQFINDINFO
typedef struct
{
	LPUINT16 pRecFirst;
	LPUINT16 pRecNext;
	LPUINT16 pRecLast;
} SEQFINDINFO;
#endif

#ifndef LPSEQFINDINFO
typedef SEQFINDINFO * LPSEQFINDINFO;
#endif

#ifndef BTIIRIGTIME
typedef struct
{
	UINT16  days;
	UINT16  hours;
	UINT16  min;
	UINT16  sec;
	UINT16  msec;
	UINT16  usec;
} BTIIRIGTIME;
#endif

#ifndef LPBTIIRIGTIME
typedef BTIIRIGTIME * LPBTIIRIGTIME;
#endif

#ifndef BTIIDENTIFY
typedef struct
{
	CHAR mac_address[20];
	CHAR serial_number[8];
	CHAR card_string[32];
	CHAR type_string[32];
	CHAR user_string[64];
} BTIIDENTIFY;
#define BTIIDENTIFY BTIIDENTIFY
#endif

#ifndef LPBTIIDENTIFY
typedef BTIIDENTIFY * LPBTIIDENTIFY;
#define LPBTIIDENTIFY LPBTIIDENTIFY
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
*  BTICard Driver functions.
*
**/

BTICardAPI VOID BTICard_Add64(LPUINT32 resulth,LPUINT32 resultl,UINT32 valah,UINT32 valal,UINT32 valbh,UINT32 valbl);
BTICardAPI UINT32 BTICard_AddrDSP(UINT32 addr,HCARD handleval);
BTICardAPI UINT32 BTICard_AddrHost(UINT32 addr,HCARD handleval);
BTICardAPI ERRVAL BTICard_AsciiToMant(LPCSTR str,LPUINT32 mant,LPINT exp);
BTICardAPI UINT32 BTICard_BCDToBin(UINT32 bcdval,INT msb,INT lsb);
BTICardAPI UINT32 BTICard_BinToBCD(UINT32 oldbcdval,UINT32 binval,INT msb,INT lsb);
BTICardAPI ERRVAL BTICard_CardClose(HCARD handleval);
BTICardAPI ERRVAL BTICard_CardCloseAll(VOID);
BTICardAPI UINT32 BTICard_CardGetInfo(UINT16 infotype,INT channum,HCARD handleval);
BTICardAPI ERRVAL BTICard_CardGetInfoEx(LPUINT16 bufmodel,UINT16 bufmodelcount,LPUINT16 buffeature,UINT16 buffeaturecount,HCARD handleval);
BTICardAPI BOOL BTICard_CardIsRunning(HCARD handleval);
BTICardAPI ERRVAL BTICard_CardNop(HCARD handleval);
BTICardAPI ERRVAL BTICard_CardOpen(LPHCARD lpHandle,INT cardnum);
BTICardAPI ERRVAL BTICard_CardOpenStr(LPHCARD lpHandle,LPCSTR cardstr);
BTICardAPI LPCSTR BTICard_CardProductStr(HCARD handleval);
BTICardAPI VOID BTICard_CardReset(HCARD handleval);
BTICardAPI VOID BTICard_CardResetEx(HCARD handleval);
BTICardAPI ERRVAL BTICard_CardResume(HCARD handleval);
BTICardAPI ERRVAL BTICard_CardStart(HCARD handleval);
BTICardAPI BOOL BTICard_CardStop(HCARD handleval);
BTICardAPI VOID BTICard_CardSyncEnable(BOOL enableflag,UINT16 syncmask,UINT16 pinpolarity,HCARD handleval);
BTICardAPI UINT16 BTICard_CardSyncValid(HCARD handleval);
BTICardAPI ERRVAL BTICard_CardTest(UINT16 level,HCARD handleval);
BTICardAPI ERRVAL BTICard_CardTest0(HCARD handleval);
BTICardAPI ERRVAL BTICard_CardTest1(HCARD handleval);
BTICardAPI ERRVAL BTICard_CardTest2(HCARD handleval);
BTICardAPI ERRVAL BTICard_CardTest3(HCARD handleval);
BTICardAPI VOID BTICard_CardTrigger(HCARD handleval);
BTICardAPI VOID BTICard_CardTriggerEnable(BOOL enableflag,HCARD handleval);
BTICardAPI VOID BTICard_CardTriggerEnableEx(BOOL enableflag,UINT16 trigmask,UINT16 pinpolarity,HCARD handleval);
BTICardAPI VOID BTICard_CardTriggerEx(UINT16 trigmask,HCARD handleval);
BTICardAPI UINT16 BTICard_CardTriggerValid(HCARD handleval);
BTICardAPI LPCSTR BTICard_CardTypeStr(HCARD handleval);
BTICardAPI VOID BTICard_ChDARClr(UINT16 maskval,UINT16 addrval,INT channum,HCARD handleval);
BTICardAPI BOOL BTICard_ChDARGet(UINT16 maskval,UINT16 addrval,INT channum,HCARD handleval);
BTICardAPI UINT32 BTICard_ChDARRdL(UINT16 addrval,INT channum,HCARD handleval);
BTICardAPI VOID BTICard_ChDARRdsW(LPUINT16 valueptr,UINT16 addrval,INT countval,INT channum,HCARD handleval);
BTICardAPI UINT16 BTICard_ChDARRdW(UINT16 addrval,INT channum,HCARD handleval);
BTICardAPI VOID BTICard_ChDARSet(UINT16 maskval,UINT16 addrval,INT channum,HCARD handleval);
BTICardAPI VOID BTICard_ChDARWrL(UINT32 value,UINT16 addrval,INT channum,HCARD handleval);
BTICardAPI VOID BTICard_ChDARWrsW(LPUINT16 valueptr,UINT16 addrval,INT countval,INT channum,HCARD handleval);
BTICardAPI VOID BTICard_ChDARWrW(UINT16 value,UINT16 addrval,INT channum,HCARD handleval);
BTICardAPI ERRVAL BTICard_CISRd(LPUINT16 buf,UINT16 bufcount,INT cistype,HCARD handleval);
BTICardAPI ERRVAL BTICard_CISWr(LPUINT16 buf,UINT16 bufcount,INT cistype,HCARD handleval);
BTICardAPI UINT16 BTICard_CommBufRd(UINT16 offset,HCARD handleval);
BTICardAPI VOID BTICard_CommBufWr(UINT16 value,UINT16 offset,HCARD handleval);
BTICardAPI ERRVAL BTICard_CommCall(UINT32 addrval,HCARD handleval);
BTICardAPI ERRVAL BTICard_CommChannelReconfig(UINT32 chmask,HCARD handleval);
BTICardAPI ERRVAL BTICard_CommCheck(HCARD handleval);
BTICardAPI ERRVAL BTICard_CommDisable(UINT16 command,HCARD handleval);
BTICardAPI ERRVAL BTICard_CommDisableEx(UINT16 command,HCARD handleval);
BTICardAPI ERRVAL BTICard_CommEnable(HCARD handleval);
BTICardAPI ERRVAL BTICard_CommExternSRQ(UINT32 chmask,HCARD handleval);
BTICardAPI ERRVAL BTICard_CommFillW(UINT16 value,UINT32 addrval,UINT16 count,HCARD handleval);
BTICardAPI BOOL BTICard_CommProtocolFunc(UINT16 opcode,UINT16 argcount,LPUINT16 argbuf,HCARD handleval);
BTICardAPI UINT32 BTICard_CommRdL(UINT32 addrval,HCARD handleval);
BTICardAPI ERRVAL BTICard_CommRdsW(LPUINT16 valueptr,UINT32 addrval,UINT16 count,HCARD handleval);
BTICardAPI UINT16 BTICard_CommRdW(UINT32 addrval,HCARD handleval);
BTICardAPI ERRVAL BTICard_CommRun(LPUINT16 dataptr,UINT16 datacount,LPUINT16 codeptr,UINT16 codecount,HCARD handleval);
BTICardAPI VOID BTICard_CommWrL(UINT32 value,UINT32 addrval,HCARD handleval);
BTICardAPI ERRVAL BTICard_CommWrsW(LPUINT16 valueptr,UINT32 addrval,UINT16 count,HCARD handleval);
BTICardAPI VOID BTICard_CommWrW(UINT16 value,UINT32 addrval,HCARD handleval);
BTICardAPI ERRVAL BTICard_CoProcGetInfo(LPUINT32 valueptr,UINT16 infotype,HCARD handleval);
BTICardAPI ERRVAL BTICard_CoreOpen(LPHCORE lphCore,INT corenum,HCARD hCard);
BTICardAPI VOID BTICard_DARClr(UINT16 maskval,UINT16 addrval,HCARD handleval);
BTICardAPI BOOL BTICard_DARGet(UINT16 maskval,UINT16 addrval,HCARD handleval);
BTICardAPI UINT32 BTICard_DARRdL(UINT16 addrval,HCARD handleval);
BTICardAPI UINT16 BTICard_DARRdW(UINT16 addrval,HCARD handleval);
BTICardAPI VOID BTICard_DARSet(UINT16 maskval,UINT16 addrval,HCARD handleval);
BTICardAPI VOID BTICard_DARWrL(UINT32 value,UINT16 addrval,HCARD handleval);
BTICardAPI VOID BTICard_DARWrW(UINT16 value,UINT16 addrval,HCARD handleval);
BTICardAPI UINT32 BTICard_Div(UINT32 diva,UINT32 divb);
BTICardAPI VOID BTICard_Div64(LPUINT32 resulth,LPUINT32 resultl,UINT32 valah,UINT32 valal,UINT32 valbh,UINT32 valbl);
BTICardAPI ERRVAL BTICard_DspBioClear(HCARD handleval);
BTICardAPI BOOL BTICard_DspBioRd(HCARD handleval);
BTICardAPI ERRVAL BTICard_DspBioSet(HCARD handleval);
BTICardAPI ERRVAL BTICard_DspIntfClear(UINT16 intmask,HCARD handleval);
BTICardAPI BOOL BTICard_DspIntfRd(UINT16 intmask,HCARD handleval);
BTICardAPI ERRVAL BTICard_DspIntmClear(HCARD handleval);
BTICardAPI ERRVAL BTICard_DspIntmSet(HCARD handleval);
BTICardAPI ERRVAL BTICard_DspXfClear(HCARD handleval);
BTICardAPI BOOL BTICard_DspXfRd(HCARD handleval);
BTICardAPI ERRVAL BTICard_DspXfSet(HCARD handleval);
BTICardAPI LPCSTR BTICard_ErrDesc(ERRVAL errval,HCARD handleval);
BTICardAPI LPCSTR BTICard_ErrDescStr(ERRVAL errval,HCARD handleval);
BTICardAPI LPCSTR BTICard_ErrName(ERRVAL errval,HCARD handleval);
BTICardAPI ERRVAL BTICard_EventLogClear(HCARD handleval);
BTICardAPI ERRVAL BTICard_EventLogConfig(UINT16 configval,UINT16 count,HCARD handleval);
BTICardAPI UINT32 BTICard_EventLogRd(LPUINT16 typeval,LPUINT32 infoval,LPINT channel,HCARD handleval);
BTICardAPI INT BTICard_EventLogStatus(HCARD handleval);
BTICardAPI VOID BTICard_ExpandMant(LPUINT32 mant,LPINT exp);
BTICardAPI BOOL BTICard_ExtDinRd(HCARD handleval);
BTICardAPI VOID BTICard_ExtDinWr(BOOL dinval,HCARD handleval);
BTICardAPI VOID BTICard_ExtDIODirSet(INT dionum,BOOL dirval,HCARD handleval);
BTICardAPI ERRVAL BTICard_ExtDIOMonConfig(UINT16 rise_edge,UINT16 fall_edge,INT banknum,HCARD handleval);
BTICardAPI BOOL BTICard_ExtDIORd(INT dionum,HCARD handleval);
BTICardAPI VOID BTICard_ExtDIOWr(INT dionum,BOOL dioval,HCARD handleval);
BTICardAPI BOOL BTICard_ExtLEDRd(HCARD handleval);
BTICardAPI VOID BTICard_ExtLEDWr(BOOL ledval,HCARD handleval);
BTICardAPI VOID BTICard_ExtStatusLEDRd(LPINT ledon,LPINT ledcolor,HCARD handleval);
BTICardAPI VOID BTICard_ExtStatusLEDWr(BOOL ledon,BOOL ledcolor,HCARD handleval);
BTICardAPI UINT16 BTICard_GetHigh(UINT32 val);
BTICardAPI UINT16 BTICard_GetLow(UINT32 val);
BTICardAPI UINT16 BTICard_GlobalRdW(UINT16 addrval,HCARD handleval);
BTICardAPI VOID BTICard_GlobalWrW(UINT16 value,UINT16 addrval,HCARD handleval);
BTICardAPI ERRVAL BTICard_HandleInfo(LPSTR cardstr,LPINT cardnum,LPUINT32 sizval,LPVOID *vxdptr,HCARD handleval);
BTICardAPI ERRVAL BTICard_HandleInfoEx(LPUINT32 valueptr,UINT32 type,HCARD handleval);
BTICardAPI BOOL BTICard_HandleIsCard(HCARD handleval);
BTICardAPI BOOL BTICard_HandleIsCore(HCORE handleval);
BTICardAPI ERRVAL BTICard_HandleMakeCard(LPHCARD lphCard,LPINT lpcorenum,HCORE hCore);
BTICardAPI ERRVAL BTICard_HandleMakeCore(LPHCORE lphCore,INT corenum,HCARD hCard);
BTICardAPI ERRVAL BTICard_HandleMakeRPC(LPHCARD hCard_Remote,LPHRPC lphRPC,HCARD handleval);
BTICardAPI BOOL BTICard_HandleOkay(HCARD handleval);
BTICardAPI UINT32 BTICard_HeapAlloc(INT section,UINT32 wordcount,HCARD handleval);
BTICardAPI UINT32 BTICard_HeapAllocAll(INT section,LPUINT32 wordcount,HCARD handleval);
BTICardAPI UINT32 BTICard_HeapAllocEx(UINT16 configval,INT section,UINT32 wordcount,HCARD handleval);
BTICardAPI UINT32 BTICard_HeapWipe(INT section,HCARD handleval);
BTICardAPI VOID BTICard_HPIFill(UINT16 value,UINT16 addrval,INT countval,HCARD handleval);
BTICardAPI UINT32 BTICard_HPIRdL(UINT16 addrval,HCARD handleval);
BTICardAPI VOID BTICard_HPIRdsL(LPUINT32 valueptr,UINT16 addrval,INT countval,HCARD handleval);
BTICardAPI VOID BTICard_HPIRdsW(LPUINT16 valueptr,UINT16 addrval,INT countval,HCARD handleval);
BTICardAPI UINT16 BTICard_HPIRdW(UINT16 addrval,HCARD handleval);
BTICardAPI VOID BTICard_HPIWrL(UINT32 value,UINT16 addrval,HCARD handleval);
BTICardAPI VOID BTICard_HPIWrsL(LPUINT32 valueptr,UINT16 addrval,INT countval,HCARD handleval);
BTICardAPI VOID BTICard_HPIWrsW(LPUINT16 valueptr,UINT16 addrval,INT countval,HCARD handleval);
BTICardAPI VOID BTICard_HPIWrW(UINT16 value,UINT16 addrval,HCARD handleval);
BTICardAPI ERRVAL BTICard_Identify(LPBTIIDENTIFY info,HCARD handleval);
BTICardAPI UINT16 BTICard_IDRegRd(INT gate_array_num,HCARD handleval);
BTICardAPI VOID BTICard_IDRegWr(UINT16 value,INT gate_array_num,HCARD handleval);
BTICardAPI VOID BTICard_IntClear(HCARD handleval);
BTICardAPI ERRVAL BTICard_IntDisable(HCARD handleval);
BTICardAPI ERRVAL BTICard_IntEnable(HCARD handleval);
BTICardAPI ERRVAL BTICard_IntEnableCond(HCARD handleval);
BTICardAPI LPVOID BTICard_IntGet(HCARD handleval);
BTICardAPI ERRVAL BTICard_IntInstall(LPVOID hEvent,HCARD handleval);
BTICardAPI VOID BTICard_IntReset(HCARD handleval);
BTICardAPI ERRVAL BTICard_IntUninstall(HCARD handleval);
BTICardAPI UINT32 BTICard_IORdL(INT addrval,HCARD handleval);
BTICardAPI UINT16 BTICard_IORdW(INT addrval,HCARD handleval);
BTICardAPI UINT16 BTICard_IOWINRdW(INT addrval,HCARD handleval);
BTICardAPI VOID BTICard_IOWINWrW(UINT16 value,INT addrval,HCARD handleval);
BTICardAPI VOID BTICard_IOWrL(UINT32 value,INT addrval,HCARD handleval);
BTICardAPI VOID BTICard_IOWrW(UINT16 value,INT addrval,HCARD handleval);
BTICardAPI ERRVAL BTICard_IRIGConfig(UINT32 configval,HCARD handleval);
BTICardAPI UINT32 BTICard_IRIGFieldGetDays(UINT32 irigvalh,UINT32 irigvall);
BTICardAPI UINT32 BTICard_IRIGFieldGetHours(UINT32 irigvalh,UINT32 irigvall);
BTICardAPI UINT32 BTICard_IRIGFieldGetMicrosec(UINT32 irigvalh,UINT32 irigvall);
BTICardAPI UINT32 BTICard_IRIGFieldGetMillisec(UINT32 irigvalh,UINT32 irigvall);
BTICardAPI UINT32 BTICard_IRIGFieldGetMin(UINT32 irigvalh,UINT32 irigvall);
BTICardAPI UINT32 BTICard_IRIGFieldGetSec(UINT32 irigvalh,UINT32 irigvall);
BTICardAPI VOID BTICard_IRIGFieldPutDays(UINT32 value,LPUINT32 irigvalh,LPUINT32 irigvall);
BTICardAPI VOID BTICard_IRIGFieldPutHours(UINT32 value,LPUINT32 irigvalh,LPUINT32 irigvall);
BTICardAPI VOID BTICard_IRIGFieldPutMicrosec(UINT32 value,LPUINT32 irigvalh,LPUINT32 irigvall);
BTICardAPI VOID BTICard_IRIGFieldPutMillisec(UINT32 value,LPUINT32 irigvalh,LPUINT32 irigvall);
BTICardAPI VOID BTICard_IRIGFieldPutMin(UINT32 value,LPUINT32 irigvalh,LPUINT32 irigvall);
BTICardAPI VOID BTICard_IRIGFieldPutSec(UINT32 value,LPUINT32 irigvalh,LPUINT32 irigvall);
BTICardAPI ERRVAL BTICard_IRIGInputThresholdGet(LPUINT16 dacval,HCARD handleval);
BTICardAPI ERRVAL BTICard_IRIGInputThresholdSet(UINT16 dacval,HCARD handleval);
BTICardAPI ERRVAL BTICard_IRIGRd(LPBTIIRIGTIME irigtime,HCARD handleval);
BTICardAPI ERRVAL BTICard_IRIGRdEx(LPUINT16 timebuf,HCARD handleval);
BTICardAPI BOOL BTICard_IRIGSyncStatus(HCARD handleval);
BTICardAPI VOID BTICard_IRIGTimeBCDToBin(LPUINT32 timevalh,LPUINT32 timevall,UINT32 irigvalh,UINT32 irigvall);
BTICardAPI VOID BTICard_IRIGTimeBinToBCD(LPUINT32 irigvalh,LPUINT32 irigvall,UINT32 timevalh,UINT32 timevall);
BTICardAPI ERRVAL BTICard_IRIGWr(LPBTIIRIGTIME irigtime,HCARD handleval);
BTICardAPI ERRVAL BTICard_IRIGWrEx(LPUINT16 timebuf,HCARD handleval);
BTICardAPI UINT32 BTICard_MakeLong(UINT16 valh,UINT16 vall);
BTICardAPI UINT16 BTICard_MakeWord(BYTE valh,BYTE vall);
BTICardAPI LPSTR BTICard_MantToAscii(LPSTR buf,INT mant,INT exp);
BTICardAPI UINT32 BTICard_Mask(UINT32 dataval,UINT16 cntval);
BTICardAPI VOID BTICard_MaxMant(LPUINT32 mant,LPINT exp);
BTICardAPI UINT32 BTICard_Mod(UINT32 moda,UINT32 modb);
BTICardAPI VOID BTICard_Mod64(LPUINT32 resulth,LPUINT32 resultl,UINT32 valah,UINT32 valal,UINT32 valbh,UINT32 valbl);
BTICardAPI UINT32 BTICard_Mul(UINT32 mula,UINT32 mulb);
BTICardAPI VOID BTICard_Mul64(LPUINT32 resulth,LPUINT32 resultl,UINT32 valah,UINT32 valal,UINT32 valbh,UINT32 valbl);
BTICardAPI VOID BTICard_NormalMant(LPUINT32 mant,LPINT exp);
BTICardAPI UINT16 BTICard_PortRd(INT addrval,HCARD handleval);
BTICardAPI VOID BTICard_PortWr(UINT16 value,INT addrval,HCARD handleval);
BTICardAPI UINT16 BTICard_ProgRdW(UINT32 addrval,HCARD handleval);
BTICardAPI VOID BTICard_ProgWrW(UINT16 value,UINT32 addrval,HCARD handleval);
BTICardAPI VOID BTICard_RAMFill(UINT16 value,UINT32 addrval,UINT32 countval,HCARD handleval);
BTICardAPI UINT16 BTICard_RAMRdB(UINT32 addrval,HCARD handleval);
BTICardAPI UINT32 BTICard_RAMRdL(UINT32 addrval,HCARD handleval);
BTICardAPI VOID BTICard_RAMRdmL(LPUINT32 valueptr,LPUINT32 addrptr,INT countval,HCARD handleval);
BTICardAPI VOID BTICard_RAMRdmW(LPUINT16 valueptr,LPUINT32 addrptr,INT countval,HCARD handleval);
BTICardAPI VOID BTICard_RAMRdsL(LPUINT32 valueptr,UINT32 addrval,INT countval,HCARD handleval);
BTICardAPI VOID BTICard_RAMRdsW(LPUINT16 valueptr,UINT32 addrval,INT countval,HCARD handleval);
BTICardAPI UINT16 BTICard_RAMRdW(UINT32 addrval,HCARD handleval);
BTICardAPI VOID BTICard_RAMWipe(HCARD handleval);
BTICardAPI VOID BTICard_RAMWipeEx(UINT16 value,HCARD handleval);
BTICardAPI VOID BTICard_RAMWrB(UINT16 value,UINT32 addrval,HCARD handleval);
BTICardAPI VOID BTICard_RAMWrL(UINT32 value,UINT32 addrval,HCARD handleval);
BTICardAPI VOID BTICard_RAMWrmL(LPUINT32 valueptr,LPUINT32 addrptr,INT countval,HCARD handleval);
BTICardAPI VOID BTICard_RAMWrmW(LPUINT16 valueptr,LPUINT32 addrptr,INT countval,HCARD handleval);
BTICardAPI VOID BTICard_RAMWrsL(LPUINT32 valueptr,UINT32 addrval,INT countval,HCARD handleval);
BTICardAPI VOID BTICard_RAMWrsW(LPUINT16 valueptr,UINT32 addrval,INT countval,HCARD handleval);
BTICardAPI VOID BTICard_RAMWrW(UINT16 value,UINT32 addrval,HCARD handleval);
BTICardAPI UINT32 BTICard_ReverseLong(UINT32 value);
BTICardAPI UINT16 BTICard_ReverseWord(UINT16 value);
BTICardAPI ERRVAL BTICard_ROMProg(UINT16 enableflag,LPUINT16 valueptr,UINT16 count,UINT32 addrval,HCARD handleval);
BTICardAPI UINT32 BTICard_SeqBlkRd(LPUINT16 buf,UINT32 bufcount,LPUINT32 blkcnt,HCARD handleval);
BTICardAPI UINT32 BTICard_SeqBlkRdEx(LPUINT16 buf,UINT32 bufcount,UINT32 maxblkcnt,LPUINT32 blkcnt,HCARD handleval);
BTICardAPI ERRVAL BTICard_SeqClear(HCARD handleval);
BTICardAPI UINT16 BTICard_SeqCommRd(LPUINT16 buf,UINT16 bufcount,HCARD handleval);
BTICardAPI ERRVAL BTICard_SeqConfig(UINT32 configval,HCARD handleval);
BTICardAPI ERRVAL BTICard_SeqConfigEx(UINT32 configval,UINT32 seqcount,UINT16 cardnum,HCARD handleval);
BTICardAPI ERRVAL BTICard_SeqConfigExx(UINT32 configval,UINT32 seqaddr,UINT32 seqcount,UINT16 cardnum,HCARD handleval);
BTICardAPI UINT32 BTICard_SeqDMARd(LPUINT16 buf,UINT32 bufcount,HCARD handleval);
BTICardAPI BOOL BTICard_SeqFindCheckVersion(LPUINT16 pRecord,UINT16 version);
BTICardAPI ERRVAL BTICard_SeqFindInit(LPUINT16 seqbuf,UINT32 seqbufsize,LPSEQFINDINFO sfinfo);
BTICardAPI ERRVAL BTICard_SeqFindMore1553(LPSEQRECORDMORE1553 *pRecMore,LPSEQRECORD1553 pRecBase);
BTICardAPI ERRVAL BTICard_SeqFindMore1553Ex(LPSEQRECORDMORE1553 pRecMore,UINT16 recordsize,LPSEQRECORD1553 pRecBase);
BTICardAPI ERRVAL BTICard_SeqFindNext(LPUINT16 *pRecord,LPUINT16 seqtype,LPSEQFINDINFO sfinfo);
BTICardAPI ERRVAL BTICard_SeqFindNext1553(LPSEQRECORD1553 *pRecord,LPSEQFINDINFO sfinfo);
BTICardAPI ERRVAL BTICard_SeqFindNext1553Ex(LPSEQRECORD1553 pRecord,UINT16 recordsize,LPSEQFINDINFO sfinfo);
BTICardAPI ERRVAL BTICard_SeqFindNext429(LPSEQRECORD429 *pRecord,LPSEQFINDINFO sfinfo);
BTICardAPI ERRVAL BTICard_SeqFindNext429Ex(LPSEQRECORD429 pRecord,UINT16 recordsize,LPSEQFINDINFO sfinfo);
BTICardAPI ERRVAL BTICard_SeqFindNext708(LPSEQRECORD708 *pRecord,LPSEQFINDINFO sfinfo);
BTICardAPI ERRVAL BTICard_SeqFindNext708Ex(LPSEQRECORD708 pRecord,UINT16 recordsize,LPSEQFINDINFO sfinfo);
BTICardAPI ERRVAL BTICard_SeqFindNext717(LPSEQRECORD717 *pRecord,LPSEQFINDINFO sfinfo);
BTICardAPI ERRVAL BTICard_SeqFindNext717Ex(LPSEQRECORD717 pRecord,UINT16 recordsize,LPSEQFINDINFO sfinfo);
BTICardAPI ERRVAL BTICard_SeqFindNextCSDB(LPSEQRECORDCSDB *pRecord,LPSEQFINDINFO sfinfo);
BTICardAPI ERRVAL BTICard_SeqFindNextCSDBEx(LPSEQRECORDCSDB pRecord,UINT16 recordsize,LPSEQFINDINFO sfinfo);
BTICardAPI ERRVAL BTICard_SeqFindNextDIO(LPSEQRECORDDIO *pRecord,LPSEQFINDINFO sfinfo);
BTICardAPI ERRVAL BTICard_SeqFindNextDIOEx(LPSEQRECORDDIO pRecord,UINT16 recordsize,LPSEQFINDINFO sfinfo);
BTICardAPI ERRVAL BTICard_SeqFindNextEx(LPUINT16 pRecord,UINT16 recordcount,LPUINT16 seqtype,LPSEQFINDINFO sfinfo);
BTICardAPI INT BTICard_SeqInterval(INT interval,INT mode,HCARD handleval);
BTICardAPI UINT16 BTICard_SeqIntervalEx(UINT16 shiftval,HCARD handleval);
BTICardAPI BOOL BTICard_SeqIsRunning(HCARD handleval);
BTICardAPI UINT16 BTICard_SeqLogFrequency(UINT16 logfreq,HCARD handleval);
BTICardAPI UINT16 BTICard_SeqRd(LPUINT16 buf,HCARD handleval);
BTICardAPI UINT16 BTICard_SeqRdEx(LPUINT16 buf,UINT16 bufcount,HCARD handleval);
BTICardAPI BOOL BTICard_SeqResume(HCARD handleval);
BTICardAPI BOOL BTICard_SeqStart(HCARD handleval);
BTICardAPI BOOL BTICard_SeqStatus(HCARD handleval);
BTICardAPI BOOL BTICard_SeqStop(HCARD handleval);
BTICardAPI UINT32 BTICard_Shl(UINT32 dataval,UINT16 cntval);
BTICardAPI UINT32 BTICard_Shr(UINT32 dataval,UINT16 cntval);
BTICardAPI UINT16 BTICard_SignMant(LPCSTR str);
BTICardAPI VOID BTICard_Sub64(LPUINT32 resulth,LPUINT32 resultl,UINT32 valah,UINT32 valal,UINT32 valbh,UINT32 valbl);
BTICardAPI VOID BTICard_SwapEndianL(LPUINT32 value);
BTICardAPI VOID BTICard_SwapEndianW(LPUINT16 valuea,LPUINT16 valueb);
BTICardAPI VOID BTICard_SwapIfBigEndianL(LPUINT32 value);
BTICardAPI VOID BTICard_SwapIfBigEndianW(LPUINT16 valuea,LPUINT16 valueb);
BTICardAPI VOID BTICard_SwapIfLittleEndianL(LPUINT32 value);
BTICardAPI VOID BTICard_SwapIfLittleEndianW(LPUINT16 valuea,LPUINT16 valueb);
BTICardAPI INT BTICard_TickTimerStart(INT milliseconds);
BTICardAPI BOOL BTICard_TickTimerValid(INT timer);
BTICardAPI ERRVAL BTICard_Timer64Rd(LPUINT32 valueh,LPUINT32 valuel,HCARD handleval);
BTICardAPI VOID BTICard_Timer64Wr(UINT32 valueh,UINT32 valuel,HCARD handleval);
BTICardAPI VOID BTICard_TimerClear(HCARD handleval);
BTICardAPI UINT32 BTICard_TimerRd(HCARD handleval);
BTICardAPI INT BTICard_TimerResolution(INT timerresol,HCARD handleval);
BTICardAPI UINT16 BTICard_TimerResolutionEx(UINT16 timershift,HCARD handleval);
BTICardAPI INT BTICard_TimerStatus(HCARD handleval);
BTICardAPI VOID BTICard_TimerWr(UINT32 value,HCARD handleval);
BTICardAPI LPSTR BTICard_ValAsciiCat(LPSTR strdest,LPCSTR strsrc);
BTICardAPI INT BTICard_ValAsciiCmpi(LPSTR str1,LPSTR str2);
BTICardAPI LPSTR BTICard_ValAsciiCpy(LPSTR strdest,LPCSTR strsrc,INT count);
BTICardAPI VOID BTICard_ValAsciiTrimLead(LPSTR buf);
BTICardAPI VOID BTICard_ValAsciiTrimTrail(LPSTR buf);
BTICardAPI VOID BTICard_ValAsciiTrimZero(LPSTR buf);
BTICardAPI UINT32 BTICard_ValFromAscii(LPCSTR asciistr,INT radixval);
BTICardAPI UINT32 BTICard_ValGetBits(UINT32 oldvalue,INT startbit,INT endbit);
BTICardAPI LPSTR BTICard_ValIncAscii(LPSTR asciistr);
BTICardAPI LPSTR BTICard_ValInccAscii(LPSTR asciistr);
BTICardAPI BOOL BTICard_ValIsLower(INT value);
BTICardAPI INT BTICard_ValLenAscii(INT numbits,INT radixval);
BTICardAPI UINT32 BTICard_ValPutBits(UINT32 oldvalue,UINT32 newfld,INT startbit,INT endbit);
BTICardAPI LPSTR BTICard_ValToAscii(UINT32 value,LPSTR asciistr,INT numbits,INT radixval);
BTICardAPI INT BTICard_ValToUpper(INT value);
BTICardAPI UINT32 BTICard_VARRdL(UINT16 addrval,HCARD handleval);
BTICardAPI VOID BTICard_VARRdsW(LPUINT16 valueptr,UINT16 addrval,INT countval,HCARD handleval);
BTICardAPI UINT16 BTICard_VARRdW(UINT16 addrval,HCARD handleval);
BTICardAPI VOID BTICard_VARWrL(UINT32 value,UINT16 addrval,HCARD handleval);
BTICardAPI VOID BTICard_VARWrsW(LPUINT16 valueptr,UINT16 addrval,INT countval,HCARD handleval);
BTICardAPI VOID BTICard_VARWrW(UINT16 value,UINT16 addrval,HCARD handleval);

#ifdef __cplusplus
}
#endif

/**
*
*  Core number constants
*
**/

enum {	COREA              = 0x0000,               //Selects Core A
		COREB              = 0x0001,               //Selects Core B
		COREC              = 0x0002,               //Selects Core C
		CORED              = 0x0003               //Selects Core D
};

/**
*
*  Sequential Record configuration options
*
**/

enum {	SEQCFG_DEFAULT     = 0x00000000L,          //Select all default settings
		SEQCFG_FILLHALT    = 0x00000000L,          //Enable sequential record in fill and halt mode (default)
		SEQCFG_DISABLE     = 0x00000001L,          //Disable sequential record
		SEQCFG_CONTINUOUS  = 0x00000002L,          //Enable sequential record in continuous mode
		SEQCFG_DMA         = 0x00000004L,          //Enable monitor in dma mode
		SEQCFG_FREE        = 0x00000008L,          //Enable sequential record in free mode
		SEQCFG_DELTA       = 0x00000010L,          //Enable sequential record in delta mode
		SEQCFG_INTERVAL    = 0x00000020L,          //Enable sequential record in interval mode
		SEQCFG_NOLOGFULL   = 0x00000000L,          //Do not generate event log when sequential record is full (default)
		SEQCFG_LOGFULL     = 0x00001000L,          //Generate event log when sequential record is full
		SEQCFG_NOLOGFREQ   = 0x00000000L,          //Do not generate event logs at a user specified frequency (default)
		SEQCFG_LOGFREQ     = 0x00002000L,          //Generate event logs at user specified frequency
		SEQCFG_16K         = 0x00000000L,          //Allocate a 16K sequential record buffer (default)
		SEQCFG_ALLAVAIL    = 0x01000000L,          //Allocate all available memory to a sequential record buffer
		SEQCFG_32K         = 0x02000000L,          //Allocate a 32K sequential record buffer
		SEQCFG_64K         = 0x04000000L,          //Allocate a 64K sequential record buffer
		SEQCFG_128K        = 0x08000000L           //Allocate a 128K sequential record buffer
};

/**
*
*  Sequential Record type fields
*
**/

enum {	SEQTYPE_MASK       = 0x00FF,               //Sequential record type mask value
		SEQTYPE_429        = 0x0001,               //Sequential record type is ARINC 429
		SEQTYPE_717        = 0x0002,               //Sequential record type is ARINC 717
		SEQTYPE_1553       = 0x0003,               //Sequential record type is MIL-STD-1553
		SEQTYPE_708        = 0x0004,               //Sequential record type is ARINC 708
		SEQTYPE_USER       = 0x0005,               //Sequential record type is User Defined
		SEQTYPE_CSDB       = 0x0006,               //Sequential record type is CSDB
		SEQTYPE_DIO        = 0x0007,               //Sequential record type is DIO
		SEQTYPE_RESTART    = 0x00FF                //Sequential record type is Restart
};

/**
*
*  Sequential Record version fields
*
**/

enum {	SEQVER_MASK        = 0xFF00,               //Sequential record version mask value
		SEQVER_0           = 0x0000,               //Sequential record version number is 0
		SEQVER_1           = 0x0100                //Sequential record version number is 1
};

/**
*
*  Event log list configuration options
*
**/

enum {	LOGCFG_DEFAULT     = 0x00000000L,          //Select all default settings
		LOGCFG_ENABLE      = 0x00000000L,          //Enable event log list (default)
		LOGCFG_DISABLE     = 0x00000001L           //Disable event log list
};

/**
*
*  IRIG timer configuration options
*
**/

enum {	IRIGCFG_DEFAULT    = 0x00000000L,          //Select all default settings
		IRIGCFG_ENABLE     = 0x00000000L,          //Enable IRIG timer (default)
		IRIGCFG_DISABLE    = 0x00000001L,          //Disable IRIG timer
		IRIGCFG_SPEEDB     = 0x00000000L,          //Bit rate is IRIGB (default)
		IRIGCFG_SPEEDA     = 0x00000002L,          //Bit rate is IRIGA
		IRIGCFG_INTERNAL   = 0x00000000L,          //IRIG timer operates internally (default)
		IRIGCFG_EXTERNAL   = 0x00000004L,          //IRIG timer operates externally
		IRIGCFG_SLAVE      = 0x00000000L,          //IRIG timer is a slave  / receiver (default)
		IRIGCFG_MASTER     = 0x00000008L,          //IRIG timer is a master / transmitter
		IRIGCFG_PPS        = 0x00000010L,          //IRIG timer operates in PPS mode (rcv/xmt)
		IRIGCFG_PWM        = 0x00000000L,          //IRIG timer uses pulse width modulated signaling (rcv/xmt) (default)
		IRIGCFG_AM         = 0x00000020L           //IRIG timer uses amplitude modulated signaling (rcv only, hw dependant)
};

/**
*
*  IRIG timer field definitions
*
**/

enum {	IRIGFIELD_USECLSB  = 0x0000,               //Microseconds LSB in timestamp field
		IRIGFIELD_USECMSB  = 0x000B,               //Microseconds MSB in timestamp field
		IRIGFIELD_MSECLSB  = 0x000C,               //Milliseconds LSB in timestamp field
		IRIGFIELD_MSECMSB  = 0x0017,               //Milliseconds MSB in timestamp field
		IRIGFIELD_SECLSB   = 0x0018,               //Seconds LSB in timestamp field
		IRIGFIELD_SECMSB   = 0x001F,               //Seconds MSB in timestamp field

		IRIGFIELD_MINLSB   = 0x0000,               //Minutes LSB in timestamph field
		IRIGFIELD_MINMSB   = 0x0007,               //Minutes MSB in timestamph field
		IRIGFIELD_HRSLSB   = 0x0008,               //Hours LSB in timestamph field
		IRIGFIELD_HRSMSB   = 0x000F,               //Hours MSB in timestamph field
		IRIGFIELD_DAYLSB   = 0x0010,               //Days LSB in timestamph field
		IRIGFIELD_DAYMSB   = 0x001B                //Days MSB in timestamph field
};

/**
*
*  C54x Global Registers
*
**/

enum {	C54_GREG_IMR       = 0x0000,               //Interrupt mask register
		C54_GREG_IFR       = 0x0001,               //Interrupt flag register
		C54_GREG_ST0       = 0x0006,               //Status register 0
		C54_GREG_ST1       = 0x0007,               //Status register 1
		C54_GREG_AL        = 0x0008,               //Accumulator A low word (bits 15-00)
		C54_GREG_AH        = 0x0009,               //Accumulator A high word (bits 31-16)
		C54_GREG_AG        = 0x000A,               //Accumulator A guars bits (bits 39-32)
		C54_GREG_BL        = 0x000B,               //Accumulator B low word (bits 15-00)
		C54_GREG_BH        = 0x000C,               //Accumulator B high word (bits 31-16)
		C54_GREG_BG        = 0x000D,               //Accumulator B guard bits (bits 39-32
		C54_GREG_T         = 0x000E,               //Temporary register
		C54_GREG_TRN       = 0x000F,               //Transition register
		C54_GREG_AR0       = 0x0010,               //Auxilliary register 0
		C54_GREG_AR1       = 0x0011,               //Auxilliary register 1
		C54_GREG_AR2       = 0x0012,               //Auxilliary register 2
		C54_GREG_AR3       = 0x0013,               //Auxilliary register 3
		C54_GREG_AR4       = 0x0014,               //Auxilliary register 4
		C54_GREG_AR5       = 0x0015,               //Auxilliary register 5
		C54_GREG_AR6       = 0x0016,               //Auxilliary register 6
		C54_GREG_AR7       = 0x0017,               //Auxilliary register 7
		C54_GREG_SP        = 0x0018,               //Stack pointer
		C54_GREG_BK        = 0x0019,               //Circular-buffer size register
		C54_GREG_BRC       = 0x001A,               //Block-repeat counter
		C54_GREG_RSA       = 0x001B,               //Block-repeat start address
		C54_GREG_REA       = 0x001C,               //Block-repeat end address
		C54_GREG_PMST      = 0x001D,               //Processor mode status register
		C54_GREG_XPC       = 0x001E,               //Program counter extension register
		C54_GREG_DRR20     = 0x0020,               //McBSP0 data receive register high
		C54_GREG_DRR10     = 0x0021,               //McBSP0 data receive register low
		C54_GREG_DXR20     = 0x0022,               //McBSP0 data transmit register high
		C54_GREG_DXR10     = 0x0023,               //McBSP0 data transmit register low
		C54_GREG_TIM       = 0x0024,               //Timer count register
		C54_GREG_PRD       = 0x0025,               //Timer period register
		C54_GREG_TCR       = 0x0026,               //Timer control register
		C54_GREG_SWWSR     = 0x0028,               //External interface software wait-state register
		C54_GREG_BSCR      = 0x0029,               //External interface bank-switching control register
		C54_GREG_SWCR      = 0x002B,               //Software wait-state control register
		C54_GREG_HPIC      = 0x002C,               //Host port interface control register
		C54_GREG_DRR22     = 0x0030,               //McBSP2 data receive register high
		C54_GREG_DRR12     = 0x0031,               //McBSP2 data receive register low
		C54_GREG_DXR22     = 0x0032,               //McBSP2 data transmit register high
		C54_GREG_DXR12     = 0x0033,               //McBSP2 data transmit register low
		C54_GREG_SPSA2     = 0x0034,               //McBSP2 sub-address register
		C54_GREG_SPDR2     = 0x0035,               //McBSP2 sub-address data register
		C54_GREG_SPSA0     = 0x0038,               //McBSP0 sub-address register
		C54_GREG_SPDR0     = 0x0039,               //McBSP0 sub-address data register
		C54_GREG_DRR21     = 0x0040,               //McBSP1 data receive register high
		C54_GREG_DRR11     = 0x0041,               //McBSP1 data receive register low
		C54_GREG_DXR21     = 0x0042,               //McBSP1 data transmit register high
		C54_GREG_DXR11     = 0x0043,               //McBSP1 data transmit register low
		C54_GREG_SPSA1     = 0x0048,               //McBSP1 sub-address register
		C54_GREG_SPDR1     = 0x0049,               //McBSP1 sub-address data register
		C54_GREG_DMPREC    = 0x0054,               //DMA channel priority and enable control
		C54_GREG_DMSBAR    = 0x0055,               //DMA channel sub-address register
		C54_GREG_DMADI     = 0x0056,               //DMA channel sub-address data with increment
		C54_GREG_DMADN     = 0x0057,               //DMA channel sub-address data without increment
		C54_GREG_CLKMD     = 0x0058                //Clock-mode register
};

/**
*
*  Event types.
*
**/

enum {	EVENTTYPE_1553MSG        = 0x0001,         //MIL-STD-1553 message
		EVENTTYPE_1553OPCODE     = 0x0002,         //MIL-STD-1553 event log opcode
		EVENTTYPE_1553HALT       = 0x0003,         //MIL-STD-1553 schedule halt
		EVENTTYPE_1553PAUSE      = 0x0004,         //MIL-STD-1553 schedule pause
		EVENTTYPE_1553LIST       = 0x0005,         //MIL-STD-1553 list buffer empty/full
		EVENTTYPE_1553SERIAL     = 0x0006,         //MIL-STD-1553 serial empty

		EVENTTYPE_429MSG         = 0x0011,         //ARINC 429 message
		EVENTTYPE_429OPCODE      = 0x0012,         //ARINC 429 event log opcode
		EVENTTYPE_429HALT        = 0x0013,         //ARINC 429 schedule halt
		EVENTTYPE_429PAUSE       = 0x0014,         //ARINC 429 schedule pause
		EVENTTYPE_429LIST        = 0x0015,         //ARINC 429 list buffer empty/full
		EVENTTYPE_429ERR         = 0x0016,         //ARINC 429 decoder error detected

		EVENTTYPE_717WORD        = 0x0021,         //ARINC 717 word received
		EVENTTYPE_717SUBFRM      = 0x0022,         //ARINC 717 sub frame completed
		EVENTTYPE_717SYNCERR     = 0x0023,         //ARINC 717 receive channel lost synchronization

		EVENTTYPE_708MSG         = 0x0031,         //ARINC 708 message

		EVENTTYPE_SEQFULL        = 0x0041,         //Sequential record full
		EVENTTYPE_SEQFREQ        = 0x0042,         //Sequential record frequency

		EVENTTYPE_422TXTHRESHOLD = 0x0051,         //RS-422 TX under threshold
		EVENTTYPE_422TXFIFO      = 0x0052,         //RS-422 TX underflow
		EVENTTYPE_422RXTHRESHOLD = 0x0053,         //RS-422 RX over threshold
		EVENTTYPE_422RXFIFO      = 0x0054,         //RS-422 RX overflow
		EVENTTYPE_422RXERROR     = 0x0055,         //RS-422 RX error

		EVENTTYPE_CSDBMSG        = 0x0058,         //CSDB message
		EVENTTYPE_CSDBOPCODE     = 0x0059,         //CSDB event log opcode
		EVENTTYPE_CSDBHALT       = 0x005A,         //CSDB schedule halt
		EVENTTYPE_CSDBPAUSE      = 0x005B,         //CSDB schedule pause
		EVENTTYPE_CSDBLIST       = 0x005C,         //CSDB list buffer empty/full
		EVENTTYPE_CSDBERR        = 0x005D,         //CSDB decoder error detected
		EVENTTYPE_CSDBSYNCERR    = 0x005E,         //CSDB receive channel lost synchronization

		EVENTTYPE_DIOEDGE        = 0x0060,         //DIO edge event
		EVENTTYPE_DIOFAULT       = 0x0061          //DIO fault event
};

/**
*
*  Card Info types
*
**/

enum {	INFOTYPE_PLAT      = 0x0001,               //Returns the platform type
		INFOTYPE_PROD      = 0x0002,               //Returns the product type
		INFOTYPE_GEN       = 0x0003,               //Returns the generation number
		INFOTYPE_1553COUNT = 0x0004,               //Returns the 1553 channel count
		INFOTYPE_1553SIZE  = 0x0005,               //Returns the 1553 channel size
		INFOTYPE_429COUNT  = 0x0006,               //Returns the 429 channel count
		INFOTYPE_429SIZE   = 0x0007,               //Returns the 429 channel size
		INFOTYPE_717COUNT  = 0x0008,               //Returns the 717 channel count
		INFOTYPE_717SIZE   = 0x0009,               //Returns the 717 channel size
		INFOTYPE_708COUNT  = 0x000A,               //Returns the 708 channel count
		INFOTYPE_708SIZE   = 0x000B,               //Returns the 708 channel size
		INFOTYPE_VERSION   = 0x000C,               //Returns the version number
		INFOTYPE_DATE      = 0x000D,               //Returns the version date
		INFOTYPE_CHINFO    = 0x000E,               //Returns the channel info
		INFOTYPE_422COUNT  = 0x000F,               //Returns the 422 port count
		INFOTYPE_422SIZE   = 0x0010,               //Returns the 422 port size
		INFOTYPE_CSDBCOUNT = 0x0011,               //Returns the CSDB channel count
		INFOTYPE_CSDBSIZE  = 0x0012,               //Returns the CSDB channel size
		INFOTYPE_DIOCOUNT  = 0x0013,               //Returns the DIO bank count
		INFOTYPE_DIOSIZE   = 0x0014                //Returns the DIO bank size
};

/**
*
*  Co-Processor Info types
*
**/

enum {	COPROCINFO_PLAT      = 0x0001,             //Returns the platform type
		COPROCINFO_PROD      = 0x0002,             //Returns the product type
		COPROCINFO_GEN       = 0x0003,             //Returns the generation number
		COPROCINFO_VERSION   = 0x0004,             //Returns the version number (major.minor)
		COPROCINFO_DATE      = 0x0005,             //Returns the version date
		COPROCINFO_CISADDR   = 0x0006,             //Returns the CIS address
		COPROCINFO_DMA       = 0x0007,             //Returns whether or not CoProc supports DMA mode
		COPROCINFO_VERSIONEX = 0x0009              //Returns the version number (major.minor.minorminor)
};

/**
*
*  CIS types
*
**/

enum {	CISTYPE_CARD         = 0x0001,             //Select card CIS
		CISTYPE_IOMODULE     = 0x0002,             //Select I/O module CIS
		CISTYPE_PLX          = 0x0003,             //Select PLX EEPROM
		CISTYPE_INFO         = 0x0004              //Select Info module CIS
};

/**
*
*  Handle Info types
*
**/

enum {	HANDINFO_CORENUM   = 0x0001                //Returns the Handle Core number
};

/**
*
*  Trigger flags
*
**/

enum {	TRIGMASK_TRIGA     = 0x0001,               //Selects trigger line A
		TRIGMASK_TRIGB     = 0x0002,               //Selects trigger line B
		TRIGMASK_TRIGC     = 0x0004,               //Selects trigger line C

		TRIGVAL_TRIGAOFF   = 0x0000,               //Tests for trigger line A inactive
		TRIGVAL_TRIGAON    = 0x0001,               //Tests for trigger line A active
		TRIGVAL_TRIGBOFF   = 0x0000,               //Tests for trigger line B inactive
		TRIGVAL_TRIGBON    = 0x0002,               //Tests for trigger line B active
		TRIGVAL_TRIGCOFF   = 0x0000,               //Tests for trigger line C inactive
		TRIGVAL_TRIGCON    = 0x0004,               //Tests for trigger line C active

		TRIGPOL_TRIGAL     = 0x0000,               //Sets active low polarity for trigger line A
		TRIGPOL_TRIGAH     = 0x0001,               //Sets active high polarity for trigger line A
		TRIGPOL_TRIGBL     = 0x0000,               //Sets active low polarity for trigger line B
		TRIGPOL_TRIGBH     = 0x0002,               //Sets active high polarity for trigger line B
		TRIGPOL_TRIGCL     = 0x0000,               //Sets active low polarity for trigger line C
		TRIGPOL_TRIGCH     = 0x0004                //Sets active high polarity for trigger line C
};

/**
*
*  Sync flags
*
**/

enum {	SYNCMASK_SYNCA     = 0x0001,              //Selects sync line A
		SYNCMASK_SYNCB     = 0x0002,              //Selects sync line B
		SYNCMASK_SYNCC     = 0x0004,              //Selects sync line C

		SYNCPOL_SYNCAL     = 0x0000,              //Sets active low polarity for sync line A
		SYNCPOL_SYNCAH     = 0x0001,              //Sets active high polarity for sync line A
		SYNCPOL_SYNCBL     = 0x0000,              //Sets active low polarity for sync line B
		SYNCPOL_SYNCBH     = 0x0002,              //Sets active high polarity for sync line B
		SYNCPOL_SYNCCL     = 0x0000,              //Sets active low polarity for sync line C
		SYNCPOL_SYNCCH     = 0x0004               //Sets active high polarity for sync line C
};

/**
*
*  Heap flags.
*
**/

enum {	HEAPCFG_DEFAULT    = 0x0000,               //Block is allocated within page
		HEAPCFG_TEST       = 0x0001,               //Returns address but doesn't make it permanent
		HEAPCFG_NULL       = 0x0002,               //Returns address but doesn't mark it as in use
		HEAPCFG_SPAN       = 0x0004,               //Block can span pages
		HEAPCFG_QUICK      = 0x0008,               //Does a quick allocation
		HEAPCFG_NOALIGN    = 0x0010,               //Relaxes alignment rules to help fit

		HEAPSECT_SRAM      = 0x0000                //Heap section for primary SRAM
};

/**
*
*  Timer resolutions.
*
**/

enum {	TIMERRESOL_1US     = 1,                    //1us timer resolution, 1:11:34 range
		TIMERRESOL_16US    = 2,                    //16us timer resolution, 19:05:19 range
		TIMERRESOL_1024US  = 3                     //1024us timer resolution, 50 day range
};

/**
*
*  Timer status.
*
**/

enum {	TIMETAG_FORMAT_BIN = 0,                    //Timetag will be in binary format
		TIMETAG_FORMAT_BCD = 1                     //Timetag will be in BCD format
};

/**
*
*  Interval calculation modes.
*
**/

enum {	INTERVALMODE_CLOSEST  = 1,                 //Finds interval closest to value specified
		INTERVALMODE_LESS     = 2,                 //Finds closest interval less than value specified
		INTERVALMODE_GREATER  = 3                  //Finds closest interval more than value specified
};

/**
*
*  Test flags.
*
**/

enum {	TEST_LEVEL_0       = 0,                    //Test I/O interface
		TEST_LEVEL_1       = 1,                    //Test memory interface
		TEST_LEVEL_2       = 2,                    //Test communication process
		TEST_LEVEL_3       = 3                     //Test bus transceiver
};

/**
*
*  Status flags.
*
**/

enum {	STAT_EMPTY         = 0,                   //Buffer is empty
		STAT_PARTIAL       = 1,                   //Buffer is partially filled
		STAT_FULL          = 2,                   //Buffer is full
		STAT_OFF           = 3                    //Buffer is off
};

/**
*
*  Other flags.
*
**/

enum {	RCV     = 0,
		XMT     = 1
};

/**
*
*  Error types.
*
**/

enum {	ERR_NONE          =  0,                   //No error
		ERR_UNKNOWN       = -1,                   //An unexpected error occurred
		ERR_BADVER        = -2,                   //A bad version was encountered
		ERR_BADPTR        = -3,                   //A bad pointer was passed
		ERR_NOCORE        = -4,                   //The specified core number doesn't exist
		ERR_BADPARAMS     = -11,                  //CardOpen() called with bad parameters
		ERR_NOHANDLES     = -12,                  //CardOpen() already has allocated too many handles
		ERR_NOCARD        = -13,                  //CardOpen() could not find a L43 card at the specified address
		ERR_NOIO          = -14,                  //CardOpen() could not find the I/O ports
		ERR_NOMEM         = -15,                  //CardOpen() could not find the memory
		ERR_BAD16BIT      = -16,                  //Card is conflicting with another 16-bit card
		ERR_WRONGMODEL    = -17,                  //Card does not support this feature
		ERR_NOSEL         = -18,                  //CardOpen() could not allocate a memory selector
		ERR_LOCK          = -19,                  //The communication process is locked up
		ERR_TOOMANY       = -20,                  //Too many channels have been configured
		ERR_BADHANDLE     = -21,                  //A bad handle was specified
		ERR_GOODHANDLE    = -22,                  //The handle is still valid and should not be destroyed
		ERR_NOTCHAN	      = -23,                  //Not a valid channel
		ERR_NOTXMT        = -24,                  //The Transmitter has not been configured
		ERR_NOTRCV        = -25,                  //The Receiver has not been configured
		ERR_NOTSEQ        = -26,                  //The Sequential Record has not been configured
		ERR_ALLOC         = -27,                  //There is not enough memory to allocate
		ERR_VXD           = -28,                  //An error occurred in the VXD
		ERR_BADLABEL      = -29,                  //The specified label value is not valid
		ERR_BADSDI	      = -30,                  //The specified sdi value is not valid
		ERR_BADMSG        = -31,                  //The specified command block is not a message block
		ERR_BADSCHNDX     = -32,                  //Specified command index is out of range
		ERR_BUFSIZE       = -33,                  //Insufficient space in user buffer
		ERR_NOCONFIG      = -34,                  //The card has not been properly configured
		ERR_CONFLICTS     = -35,                  //Unable to resolve conflicts
		ERR_RANGE         = -36,                  //Schedule is out of range
		ERR_FACTOR        = -37,                  //A bad factor value was specified
		ERR_NOIOBOOT      = -40,                  //Could not talk to IO Boot port of DSP
		ERR_BOOTFULL      = -41,                  //No space to add boot code
		ERR_BOOTNUM       = -42,                  //There is no boot code with the specified number
		ERR_ACCESS        = -43,                  //Unable to write to access register
		ERR_ROMVERIFY     = -44,                  //Unable to verify the value written to the ROM
		ERR_COUNT         = -45,                  //An invalid count was specified
		ERR_CRC           = -46,                  //There was a bad checksum in the HEX file
		ERR_FNAME         = -47,                  //Bad filenames were specified
		ERR_FRDWR         = -48,                  //There was an error reading or writing the HEX file
		ERR_HEX           = -49,                  //There was a bad hex character in the HEX file
		ERR_INDEX         = -51,                  //The command block index was invalid or the schedule is full
		ERR_NOMSGS        = -52,                  //No messages specified
		ERR_TYPE          = -54,                  //There was a bad type value in the HEX file
		ERR_ZEROLEN       = -55,                  //Zero length was specified
		ERR_BADADDRESS	  = -56,                  //A bad address was specified
		ERR_MSGNDX        = -57,                  //A bad message index was specified
		ERR_BADTA         = -60,                  //A bad terminal address was specified
		ERR_BADFRAME      = -61,                  //A bad frame time was specified
		ERR_NOTBC         = -62,                  //The BC has not been configured
		ERR_NOTRT         = -63,                  //The RT has not been configured
		ERR_NOTMON        = -64,                  //The monitor has not been configured
		ERR_SELFIOFAIL    = -71,                  //I/O selftest failed
		ERR_SELFMEMFAIL   = -72,                  //Memory selftest failed
		ERR_SELFCOMMFAIL  = -73,                  //Communication selftest failed
		ERR_SELFXMTFAIL   = -74,                  //Transmit selftest failed
		ERR_PLXBUG        = -75,                  //PLX bug is causing problems
		ERR_NOTSUPPORTED  = -76,                  //Base class does not support feature
		ERR_DLL           = -77,                  //A required DLL is missing
		ERR_SEQTYPE       = -80,                  //Invalid sequential record type value
		ERR_SEQNEXT       = -81,                  //Next sequential record does not exist
		ERR_SEQFINDINFO   = -82,                  //The SEQFINDINFO structure is not valid
		ERR_SEQBASEPTR    = -83,                  //The base pointer passed is invalid
		ERR_SEQMORE       = -84,                  //More (extended) record data does not exist
		ERR_TIMEOUT       = -90,                  //Function timed out waiting for data
		ERR_SUBFRMNUM     = -101,                 //Invalid SubFrame number was specified
		ERR_WORDNUM       = -102,                 //Invalid Word number was specified
		ERR_NOTINSYNC     = -103,                 //Not Synchronized to databus
		ERR_SUPERFRM      = -104,                 //SuperFrame not configured
		ERR_SUPERFRMNUM   = -105                  //Invalid SuperFrame number was specified
};

#endif
