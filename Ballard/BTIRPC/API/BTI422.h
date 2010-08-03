/**
*
*  BTI422 PowerPC Linux Driver  Version 1.0.4  (10/23/2009)
*  Copyright (c) 2004-2009
*  Ballard Technology, Inc.
*  www.ballardtech.com
*  support@ballardtech.com
*  ALL RIGHTS RESERVED
*
*  NAME:   BTI422.H -- PowerPC Linux
*                      BTI422 Driver Include Header File.
*
**/

/**
*
*  This file defines the procedures provided by the PowerPC Linux
*  Driver Library for Ballard Technology RS-422 interface
*  cards.  Applications using the BTI422 Driver Library must
*  incorporate this include file using the preprocessor
*  directive #include. If this file is in the current working
*  directory, the form would be:
*
*  #include "BTI422.H"
*
**/

/**
*
*  Conditional block to prevent multiple defines.
*
**/

#ifndef __BTI422_H
#define __BTI422_H

/**
*
*  Typedefs used by the BTI422 Driver.
*
**/

#ifndef BTI422API
#define BTI422API
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
*  Structs used by the BTI422 Driver.
*
**/

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
*  BTI422 Driver functions.
*
**/

BTI422API INT BTI422_FIFOCheckRx(INT portnum,HCARD handleval);
BTI422API INT BTI422_FIFOCheckTx(INT portnum,HCARD handleval);
BTI422API ERRVAL BTI422_FIFOConfigRx(UINT32 configval,INT threshold,INT portnum,HCARD handleval);
BTI422API ERRVAL BTI422_FIFOConfigTx(UINT32 configval,INT threshold,INT portnum,HCARD handleval);
BTI422API INT BTI422_FIFODataRd(LPBYTE buf,INT count,LPUINT32 status,INT portnum,HCARD handleval);
BTI422API INT BTI422_FIFODataWr(LPBYTE buf,INT count,UINT32 ctrlflags,INT portnum,HCARD handleval);
BTI422API ERRVAL BTI422_FIFOFlushRx(INT portnum,HCARD handleval);
BTI422API ERRVAL BTI422_FIFOFlushTx(INT portnum,HCARD handleval);
BTI422API ERRVAL BTI422_FIFOGapWr(UINT16 gapcount,UINT32 ctrlflags,INT portnum,HCARD handleval);
BTI422API ERRVAL BTI422_ParamAmplitudeConfig(UINT32 configval,UINT16 dacval,INT portnum,HCARD handleval);
BTI422API ERRVAL BTI422_ParamAmplitudeGet(LPUINT16 dacval,INT portnum,HCARD handleval);
BTI422API ERRVAL BTI422_PortConfig(UINT32 configval,UINT16 bitrateflag,INT portnum,HCARD handleval);
BTI422API VOID BTI422_PortGetCount(LPINT portcount,HCARD handleval);
BTI422API UINT32 BTI422_PortGetInfo(UINT16 infotype,INT portnum,HCARD handleval);
BTI422API BOOL BTI422_PortIs422(INT portnum,HCARD handleval);
BTI422API BOOL BTI422_PortStart(INT portnum,HCARD handleval);
BTI422API BOOL BTI422_PortStop(INT portnum,HCARD handleval);

/**
*
*  Internal functions used by the BTI422 Driver.
*
**/

BTI422API INT BTI422_DllUsageCount(VOID);
BTI422API BASEADDR BTI422_PortCreate(INT portnum,HCARD handleval);
BTI422API ERRVAL BTI422_PortReset(INT portnum,HCARD handleval);
BTI422API ERRVAL BTI422_TestProtocol(HCARD handleval);

#ifdef __cplusplus
}
#endif

/**
*
*  Port number constants
*
**/

#define PORT0  0x0000          //Port 0
#define	PORT1  0x0001          //Port 1
#define	PORT2  0x0002          //Port 2
#define	PORT3  0x0003          //Port 3
#define	PORT4  0x0004          //Port 4
#define	PORT5  0x0005          //Port 5
#define	PORT6  0x0006          //Port 6
#define	PORT7  0x0007          //Port 7
#define	PORT8  0x0008          //Port 8
#define	PORT9  0x0009          //Port 9
#define	PORT10 0x000A          //Port 10
#define	PORT11 0x000B          //Port 11
#define	PORT12 0x000C          //Port 12
#define	PORT13 0x000D          //Port 13
#define	PORT14 0x000E          //Port 14
#define	PORT15 0x000F          //Port 15
#define	PORT16 0x0010          //Port 16
#define	PORT17 0x0011          //Port 17
#define	PORT18 0x0012          //Port 18
#define	PORT19 0x0013          //Port 19
#define	PORT20 0x0014          //Port 20
#define	PORT21 0x0015          //Port 21
#define	PORT22 0x0016          //Port 22
#define	PORT23 0x0017          //Port 23
#define	PORT24 0x0018          //Port 24
#define	PORT25 0x0019          //Port 25
#define	PORT26 0x001A          //Port 26
#define	PORT27 0x001B          //Port 27
#define	PORT28 0x001C          //Port 28
#define	PORT29 0x001D          //Port 29
#define	PORT30 0x001E          //Port 30
#define	PORT31 0x001F          //Port 31

/**
*
*  Bitrate configuration options
*
**/

#define	BITRATE422_CLK1            0x8000      //CLK1, 20 MHz (default)
#define	BITRATE422_CLK2            0x0000      //CLK2, 9.216 MHz (standard option)
#define	BITRATE422_XMTSLOPEHIGH    0x0000      //Transmit with high speed slope (default)
#define	BITRATE422_XMTSLOPELOW     0x4000      //Transmit with low speed slope

// These defines assume an external clock of 9.216 MHz
#define	BITRATE422_921600BPS   0x0000          //External Clock, Div = 0
#define	BITRATE422_460800BPS   0x0001          //External Clock, Div = 1
#define	BITRATE422_230400BPS   0x0003          //External Clock, Div = 3
#define	BITRATE422_115200BPS   0x0007          //External Clock, Div = 7
#define	BITRATE422_57600BPS    0x000F          //External Clock, Div = 15
#define	BITRATE422_38400BPS    0x0017          //External Clock, Div = 23
#define	BITRATE422_28800BPS    0x001F          //External Clock, Div = 31
#define	BITRATE422_19200BPS    0x002F          //External Clock, Div = 47
#define	BITRATE422_14400BPS    0x003F          //External Clock, Div = 63
#define	BITRATE422_9600BPS     0x005F          //External Clock, Div = 95
#define	BITRATE422_7200BPS     0x007F          //External Clock, Div = 127
#define	BITRATE422_4800BPS     0x00BF          //External Clock, Div = 191
#define	BITRATE422_3600BPS     0x00FF          //External Clock, Div = 255
#define	BITRATE422_2400BPS     0x017F          //External Clock, Div = 383
#define	BITRATE422_1800BPS     0x01FF          //External Clock, Div = 511
#define	BITRATE422_1200BPS     0x02FF          //External Clock, Div = 767
#define	BITRATE422_900BPS      0x03FF          //External Clock, Div = 1023
#define	BITRATE422_600BPS      0x05FF          //External Clock, Div = 1535
#define	BITRATE422_300BPS      0x0BFF          //External Clock, Div = 3071

#define	BITRATE422_1000000BPS  0x8001          //Internal Clock, Div = 1
#define	BITRATE422_500000BPS   0x8003          //Internal Clock, Div = 3
#define	BITRATE422_400000BPS   0x8004          //Internal Clock, Div = 4
#define	BITRATE422_250000BPS   0x8007          //Internal Clock, Div = 7
#define	BITRATE422_200000BPS   0x8009          //Internal Clock, Div = 9
#define	BITRATE422_100000BPS   0x8013          //Internal Clock, Div = 19
#define	BITRATE422_80000BPS    0x8018          //Internal Clock, Div = 24
#define	BITRATE422_50000BPS    0x8027          //Internal Clock, Div = 39
#define	BITRATE422_40000BPS    0x8031          //Internal Clock, Div = 49
#define	BITRATE422_25000BPS    0x804F          //Internal Clock, Div = 79
#define	BITRATE422_20000BPS    0x8063          //Internal Clock, Div = 99
#define	BITRATE422_12500BPS    0x809F          //Internal Clock, Div = 159
#define	BITRATE422_10000BPS    0x80C7          //Internal Clock, Div = 199

/**
*
*  Port configuration options
*
**/

enum {	PORTCFG422_DEFAULT       = 0x00000000L,     //Select all default settings (XMT & RCV) (default)
		PORTCFG422_RCVALL        = 0x00000000L,     //Recieve all data (RCV) (default)
		PORTCFG422_RCVGOOD       = 0x40000000L,     //Receive only error free data (RCV)
		PORTCFG422_SELFTEST      = 0x00000004L,     //Enable internal wraparound (XMT & RCV)
		PORTCFG422_SELFTESTOFF   = 0x00000000L,     //Disable internal wraparound (XMT & RCV) (default)
		PORTCFG422_ACTIVE        = 0x00000000L,     //Enable ehannel activity (XMT & RCV) (default)
		PORTCFG422_INACTIVE      = 0x00000040L,     //Disable channel activity (XMT & RCV)
		PORTCFG422_PARITYNONE    = 0x00000000L,     //Disable parity (XMT & RCV) (default)
		PORTCFG422_PARITYODD     = 0x00800000L,     //Enable odd parity (XMT & RCV)
		PORTCFG422_PARITYEVEN    = 0x00400000L,     //Enable even parity (XMT & RCV)
		PORTCFG422_PARITYMARK    = 0x00200000L,     //Force set parity (XMT & RCV)
		PORTCFG422_PARITYSPACE   = 0x00100000L,     //Force clear parity (XMT & RCV)
		PORTCFG422_BUSINVERT     = 0x80000000L,     //Enable bus polarity inversion (XMT & RCV)
		PORTCFG422_BUSINVERTOFF  = 0x00000000L,     //Disable bus polarity inversion (XMT & RCV) (default)
		PORTCFG422_MODE232       = 0x08000000L,     //Enable RS-232 / RS-423 mode (unbalanced) (XMT)
		PORTCFG422_MODE422       = 0x00000000L      //Enable RS-422 mode (balanced) (XMT) (default)
};

/**
*
*  FIFO configuration options
*
**/

enum {	FIFOCFG422_DEFAULT       = 0x00000000L,     //Select all default settings
		FIFOCFG422_NONE          = 0x00000000L,     //Disable FIFO Events (default)
		FIFOCFG422_TXTHRESHOLD   = 0x00000001L,     //Enable Transmit FIFO Threshold Event
		FIFOCFG422_TXEMPTY       = 0x00000002L,     //Enable Transmit FIFO Empty Event
		FIFOCFG422_RXTHRESHOLD	 = 0x00000004L,     //Enable Receive FIFO Threshold Event
		FIFOCFG422_RXFULL        = 0x00000008L,     //Enable Receive FIFO Full Event
		FIFOCFG422_RXERROR       = 0x00000010L      //Enable Receive Error Event
};

/**
*
*  FIFO word configuration options
*
**/

enum {	WRDCFG422_DEFAULT        = 0x00000000L,     //Select all default settings
		WRDCFG422_GAPEN          = 0x00008000L,     //Enable the use of data as gap
		WRDCFG422_GAPENOFF       = 0x00000000L,     //Disable the use of data as gap (default)
		WRDCFG422_FRAMEERR       = 0x00001000L,     //Enable Frame Error
		WRDCFG422_FRAMEERROFF    = 0x00000000L,     //Disable Frame Error (default)
		WRDCFG422_PARERR         = 0x00000800L,     //Enable Parity Error
		WRDCFG422_PARERROFF      = 0x00000000L,     //Disable Parity Error (default)
		WRDCFG422_ATOMIC         = 0x80000000L,     //Enable atomic FIFO access
		WRDCFG422_ATOMICOFF      = 0x00000000L      //Disable atomic FIFO access (default)
};

/**
*
*  FIFO gap configuration options
*
**/

enum {	GAPCFG422_DEFAULT        = 0x00000000L      //Select all default settings
};

/**
*
*  Bit fields of the FIFO status flags.
*
**/

enum {	WRDSTAT422_ANYERR      = 0x0020,          //Any error
		WRDSTAT422_BITERR      = 0x0010,          //Any bit error
		WRDSTAT422_NRZERR      = 0x0008,          //Bit error (NRZ)
		WRDSTAT422_EDGEERR     = 0x0004,          //Bit error (Edge)
		WRDSTAT422_PARERR      = 0x0002,          //Parity error
		WRDSTAT422_FRAMEERR    = 0x0001           //Frame error
};

/**
*
*  Parametric configuration options
*
**/

enum {	PARAMCFG422_DEFAULT    = 0x00000000L,     //Select all default settings (default)
		PARAMCFG422_AMPLON     = 0x00000000L,     //Enables parametric amplitude control (default)
		PARAMCFG422_AMPLOFF    = 0x00000001L      //Disables parametric amplitude control
};

/**
*
*  Channel info fields.
*
**/

enum {	INFO422_PARAM         = 0x0001           //Channel supports parametric control
};

#endif
