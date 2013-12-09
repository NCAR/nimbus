/**
*
*  BTIDIO RPC DRIVER  Version 1.0.4  (10/23/2009)
*  Copyright (c) 2005-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   BTIDIO.H -- RPC Driver
*                      BTIDIO Driver Include Header File.
*
**/

/**
*
*  This file defines the procedures provided by the RPC
*  Driver Library for Ballard Technology Discrete interface
*  cards.  Applications using the BTIDIO Driver Library must
*  incorporate this include file using the preprocessor
*  directive #include. If this file is in the current working
*  directory, the form would be:
*
*  #include "BTIDIO.H"
*
**/

/**
*
*  Conditional block to prevent multiple defines.
*
**/

#ifndef __BTIDIO_H
#define __BTIDIO_H

/**
*
*  Typedefs used by the BTIDIO Driver.
*
**/

#ifndef BTIDIOAPI
#define BTIDIOAPI
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
*  BTIDIO Driver functions.
*
**/

BTIDIOAPI ERRVAL BTIDIO_BankConfig(UINT32 configval,UINT16 threshold,UINT16 samplerate,INT banknum,HCARD handleval);
BTIDIOAPI VOID BTIDIO_BankFaultClr(BYTE mask,INT banknum,HCARD handleval);
BTIDIOAPI BYTE BTIDIO_BankFaultRd(INT banknum,HCARD handleval);
BTIDIOAPI BOOL BTIDIO_BankIsDIO(INT banknum,HCARD handleval);
BTIDIOAPI BYTE BTIDIO_BankRd(INT banknum,HCARD handleval);
BTIDIOAPI BOOL BTIDIO_BankStart(INT banknum,HCARD handleval);
BTIDIOAPI BOOL BTIDIO_BankStop(INT banknum,HCARD handleval);
BTIDIOAPI ERRVAL BTIDIO_BankWr(BYTE value,INT banknum,HCARD handleval);
BTIDIOAPI ERRVAL BTIDIO_BankWrMask(BYTE value,BYTE mask,INT banknum,HCARD handleval);
BTIDIOAPI ERRVAL BTIDIO_DiscreteConfig(UINT32 configval,INT ionum,INT banknum,HCARD handleval);
BTIDIOAPI BOOL BTIDIO_DiscreteRd(INT ionum,INT banknum,HCARD handleval);
BTIDIOAPI ERRVAL BTIDIO_DiscreteWr(BOOL value,INT ionum,INT banknum,HCARD handleval);

/**
*
*  Internal functions used by the BTIDIO Driver.
*
**/

BTIDIOAPI ERRVAL BTIDIO_BankClear(INT banknum,HCARD handleval);
BTIDIOAPI INT BTIDIO_BankGetCount(HCARD handleval);
BTIDIOAPI UINT32 BTIDIO_BankGetInfo(UINT16 infotype,INT banknum,HCARD handleval);
BTIDIOAPI UINT16 BTIDIO_BankGetSampleRate(INT banknum,HCARD handleval);
BTIDIOAPI UINT16 BTIDIO_BankGetThreshold(INT banknum,HCARD handleval);
BTIDIOAPI UINT16 BTIDIO_BankSetSampleRate(UINT16 samplerate,INT banknum,HCARD handleval);
BTIDIOAPI UINT16 BTIDIO_BankSetThreshold(UINT16 threshold,INT banknum,HCARD handleval);
BTIDIOAPI INT BTIDIO_DiscreteGetCount(INT banknum,HCARD handleval);
BTIDIOAPI INT BTIDIO_DllUsageCount(VOID);
BTIDIOAPI INT BTIDIO_GetBankCount(HCARD handleval);
BTIDIOAPI BOOL BTIDIO_IsBank(INT banknum,HCARD handleval);
BTIDIOAPI ERRVAL BTIDIO_TestProtocol(HCARD handleval);

#ifdef __cplusplus
}
#endif

/**
*
*  Bank number constants
*
**/

#define BANKA  0x0000           //Bank A
#define BANKB  0x0001           //Bank B
#define BANKC  0x0002           //Bank C
#define BANKD  0x0003           //Bank D

/**
*
*  Bank configuration options
*
**/

enum {
		BANKCFGDIO_DEFAULT       = 0x00000000L,      //Select all default settings (in & out) (default)
		BANKCFGDIO_INPUT         = 0x00000000L,      //Select input mode (in) (default)
		BANKCFGDIO_OUTPUT        = 0x00000001L,      //Select output mode (out)
		BANKCFGDIO_INOUT         = 0x00000002L,      //Select concurrent input and output mode (in & out)
		BANKCFGDIO_SERIES        = 0x00000000L,      //Select series mode (in) (default)
		BANKCFGDIO_SHUNT         = 0x00000004L,      //Select shunt mode (in)
		BANKCFGDIO_POLIN_INVOFF  = 0x00000000L,      //Disable polarity inversion for bank input (in) (default)
		BANKCFGDIO_POLIN_INV     = 0x00000100L,      //Enable polarity inversion for bank input (in)
		BANKCFGDIO_POLOUT_INVOFF = 0x00000000L,      //Disable polarity inversion for bank output (out) (default)
		BANKCFGDIO_POLOUT_INV    = 0x00000200L,      //Enable polarity inversion for bank output (out)
		BANKCFGDIO_ACTIVE        = 0x00000000L,      //Enable bank activity (in & out) (default)
		BANKCFGDIO_INACTIVE      = 0x00000010L,      //Disables bank activity (in & out)
		BANKCFGDIO_SEQSEL        = 0x00000000L,      //Sequential monitoring of selected transitions (in & out) (default)
		BANKCFGDIO_SEQALL        = 0x00000020L,      //Every transition will be recorded in the Sequential Record (in & out)
		BANKCFGDIO_NOLOGFAULT    = 0x00000000L,      //No entry will be made in the Event Log List when an output detects a fault (out) (default)
		BANKCFGDIO_LOGFAULT      = 0x00000040L,      //An entry will be made in the Event Log List when an output detects a fault (out)
		BANKCFGDIO_NOLOGEDGE     = 0x00000000L,      //No entry will be made in the Event Log List when an input detects an edge (in) (default)
		BANKCFGDIO_LOGEDGE       = 0x00000080L,      //An entry will be made in the Event Log List when an input detects an edge (in)
		BANKCFGDIO_LOGALL        = 0x000000C0L       //An entry will be made in the Event Log List when an input detects an edge and when an output detects a fault (in & out)
};

enum {
		THRESHOLDDIO_TTL      = 0x00F0,           //Corresponds to a 1.4V switching threshold (TTL)
		THRESHOLDDIO_CMOS     = 0x01B0,           //Corresponds to a 2.5V switching threshold (CMOS)
		THRESHOLDDIO_1_4V     = 0x00F0,           //Corresponds to a 1.4V switching threshold
		THRESHOLDDIO_2_5V     = 0x01B0,           //Corresponds to a 2.5V switching threshold
		THRESHOLDDIO_5V       = 0x0370,           //Corresponds to a 5V switching threshold
		THRESHOLDDIO_7V       = 0x04D0,           //Corresponds to a 7V switching threshold
		THRESHOLDDIO_12V      = 0x0850,           //Corresponds to a 12V switching threshold
		THRESHOLDDIO_16V      = 0x0B10,           //Corresponds to a 16V switching threshold
		THRESHOLDDIO_10KOHM   = 0x00F0,           //Corresponds to approx. 10kOhm switching threshold (Shunt)
		THRESHOLDDIO_25KOHM   = 0x01B0,           //Corresponds to approx. 25kOhm switching threshold (Shunt)
		THRESHOLDDIO_100KOHM  = 0x0300            //Corresponds to approx. 100kOhm switching threshold (Shunt)
};

enum {
		SAMPLERATEDIO_US      = 0x0000,           //Corresponds to a 1us base sample clock
		SAMPLERATEDIO_MS      = 0x8000            //Corresponds to a 1ms base sample clock
};

/**
*
*  Discrete configuration options
*
**/

enum {
		DISCCFGDIO_DEFAULT       = 0x00000000L,      //Select all default settings (in & out) (default)
		DISCCFGDIO_INPUT         = 0x00000000L,      //Select input mode (in) (default)
		DISCCFGDIO_OUTPUT        = 0x00000001L,      //Select output mode (out)
		DISCCFGDIO_INOUT         = 0x00000002L,      //Select concurrent input and output mode (in & out)
		DISCCFGDIO_POLIN_INVOFF  = 0x00000000L,      //Disable polarity inversion for input (in) (default)
		DISCCFGDIO_POLIN_INV     = 0x00000004L,      //Enable polarity inversion for input (in)
		DISCCFGDIO_POLOUT_INVOFF = 0x00000000L,      //Disable polarity inversion for output (out) (default)
		DISCCFGDIO_POLOUT_INV    = 0x00000008L,      //Enable polarity inversion for output (out)
		DISCCFGDIO_SEQNONE       = 0x00000000L,      //No transitions for this input will be recorded in the Sequential Record (in & out) (default)
		DISCCFGDIO_SEQPOSEDGE    = 0x00000010L,      //Positive transitions for this input will be recorded in the Sequential Record (in & out)
		DISCCFGDIO_SEQNEGEDGE    = 0x00000020L,      //Negative transitions for this input will be recorded in the Sequential Record (in & out)
		DISCCFGDIO_SEQANYEDGE    = 0x00000030L,      //Every transition for this input will be recorded in the Sequential Record (in & out)
		DISCCFGDIO_NOLOGEDGE     = 0x00000000L,      //No transitions for this input will create an entry in the Event Log List (in & out) (default)
		DISCCFGDIO_LOGPOSEDGE    = 0x00000040L,      //Positive transitions for this input will create an entry in the Event Log List (in & out)
		DISCCFGDIO_LOGNEGEDGE    = 0x00000080L,      //Negative transitions for this input will create an entry in the Event Log List (in & out)
		DISCCFGDIO_LOGANYEDGE    = 0x000000C0L,      //Every transition for this input will create an entry in the Event Log List (in & out)
		DISCCFGDIO_NOLOGFAULT    = 0x00000000L,      //No faults for this output will create an entry in the Event Log List (out) (default)
		DISCCFGDIO_LOGFAULT      = 0x00000100L,      //Every fault for this output will create an entry in the Event Log List (out)
		DISCCFGDIO_LOGALL        = 0x000001C0L       //Every transition for this input and every fault for this output will create an entry in the Event Log Lit (in/out)
};

/**
*
*  Bank info fields.
*
**/

enum {	INFODIO_INPUT          = 0x0001,          //Bank is input capable
		INFODIO_OUTPUT         = 0x0002,          //Bank is output capable
		INFODIO_SERIES         = 0x0004,          //Bank is series capable
		INFODIO_SHUNT          = 0x0008           //Bank is shunt capable
};

#endif
