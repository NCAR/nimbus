/* =========================================================== */
/* SccClass.h                                                  */
/*                                                             */
/* Class for interfacing to a serial port on MV162             */
/*                                                             */
/* Written by Keith Barr                                       */
/* Copyright 1994                                              */
/* National Center for                                         */
/* Atmospheric Research                                        */
/* =========================================================== */

#ifndef SCCCLASS_H
#define SCCCLASS_H
 
#include  "mv162.h"
#include  <vxWorks.h>
#include    <ioLib.h>
#include    <stdio.h>
#include <string.h>

/* necessary defines */
/* Z85230 SCC Registers */
# define PORT_A 0xfff45005
# define PORT_B 0xfff45001

#define SCC_PORT_A	0		// open port A
#define SCC_PORT_B	1		// open port B

/* set up some stuff for the baud rate generator */
# define BAUD_RATE 2400
# define BRG_CLOCK_FREQ 10000000



/* taken from ******************************************************
 *
 * z85230.h
 *
 * Copyright (c)1992 by ISTS Space Geodynamics Laboratory.
 * Written by Georg Feil.
 * Redistribution and use in source and binary forms is allowed provided
 * that prior permission is obtained and this copyright notice is not removed.
 */

/* z85230.h - Zilog 85230 Enhanced Serial Communications Controller driver */

/*
modification history
--------------------
xxx,10dec91,ghf  added enhancements for Zilog 85230. Changed '8530' to '85230'
                   and 'SCC' to 'ESCC'. New features are tagged *new*.
01c,25jul88,gae  fixed file name from scc8530.h.
01b,08nov87,dnw  fixed definition of SCC_WR4_SDLC (was 0x10, should be 0x20).
01a,06jul,llk+jlf   written
*/

/* bit values for write register 0 */
/* command register */

#define WR0        0x00
#define WR1        0x01
#define WR2        0x02
#define WR3        0x03
#define WR4        0x04
#define WR5        0x05
#define WR6        0x06
#define WR7        0x07
#define WR8        0x08
#define WR9        0x09
#define WR10       0x0a
#define WR11       0x0b
#define WR12       0x0c
#define WR13       0x0d
#define WR14       0x0e
#define WR15       0x0f

#define RR2        0x02

/* bit values for write register 1 */
/* command register */
#define WR0_NULL_CODE      0x00
#define WR0_RST_INT        0x10
#define WR0_SEND_ABORT     0x18
#define WR0_EN_INT_RX      0x20
#define WR0_RST_TX_INT     0x28
#define WR0_ERR_RST        0x30
#define WR0_RST_HI_IUS     0x38
#define WR0_RST_RX_CRC     0x40
#define WR0_RST_TX_CRC     0x80
#define WR0_RST_TX_UND     0xc0

/* bit values for write register 1 */
/* tx/rx interrupt and data transfer mode definition */

#define WR1_EXT_INT_EN     0x01
#define WR1_TX_INT_EN      0x02
#define WR1_PARITY         0x04
#define WR1_RX_INT_DIS     0x00
#define WR1_RX_INT_FIR     0x08
#define WR1_INT_ALL_RX     0x10
#define WR1_RX_INT_SPE     0x18
#define WR1_RDMA_RECTR     0x20
#define WR1_RDMA_FUNC      0x40
#define WR1_RDMA_EN        0x80

/* write register 2 */
/* interrupt vector */

/* bit values for write register 3 */
/* receive parameters and control */

#define WR3_RX_EN          0x01
#define WR3_SYNC_CHAR      0x02
#define WR3_ADR_SEARCH     0x04
#define WR3_RX_CRC_EN      0x08
#define WR3_ENTER_HUNT     0x10
#define WR3_AUTO_EN        0x20
#define WR3_RX_5_BITS      0x00
#define WR3_RX_7_BITS      0x40
#define WR3_RX_6_BITS      0x80
#define WR3_RX_8_BITS      0xc0

/* bit values for write register 4 */
/* tx/rx misc parameters and modes */

#define WR4_PAR_EN         0x01
#define WR4_PAR_EVEN       0x02
#define WR4_SYNC_EN        0x00
#define WR4_1_STOP         0x04
#define WR4_2_STOP         0x0c
#define WR4_8_SYNC         0x00
#define WR4_16_SYNC        0x10
#define WR4_SDLC           0x20
#define WR4_EXT_SYNC       0x30
#define WR4_1_CLOCK        0x00
#define WR4_16_CLOCK       0x40
#define WR4_32_CLOCK       0x80
#define WR4_64_CLOCK       0xc0

/* bit values for write register 5 */
/* transmit parameter and controls */

#define WR5_TX_CRC_EN      0x01
#define WR5_RTS            0x02
#define WR5_SDLC           0x04
#define WR5_TX_EN          0x08
#define WR5_SEND_BRK       0x10
#define WR5_TX_5_BITS      0x00
#define WR5_TX_7_BITS      0x20
#define WR5_TX_6_BITS      0x40
#define WR5_TX_8_BITS      0x60
#define WR5_DTR            0x80

/* write register 6 */
/* sync chars or sdlc address field */

/* write register 7 */
/* sync char or sdlc flag */

/* write register 7 prime */
/* enhanced features control (write zeroes to obtain old SCC functionality) */

#define WR7P_AUTO_TX_SDLC  0x01    /* new */
#define WR7P_AUTO_EOM_RES  0x02    /* new */
#define WR7P_AUTO_RTS_RES  0x04    /* new */
#define WR7P_RX_FIFO_INT   0x08    /* new */
#define WR7P_DTR_REQ_MODE  0x10    /* new */
#define WR7P_TX_FIFO_INT   0x20    /* new */
#define WR7P_EXT_RD_EN     0x40    /* new */
/* (bit 7 is unused, must be 0) */

/* write register 8 */
/* transmit buffer */

/* bit values for write register 9 */
/* master interrupt control */

#define WR9_VIS            0x01
#define WR9_NV             0x02
#define WR9_DLC            0x04
#define WR9_MIE            0x08
#define WR9_STATUS_HI      0x10
#define WR9_SW_INTACK_EN   0x20    /* new */
#define WR9_NO_RST         0x00
#define WR9_CH_B_RST       0x40
#define WR9_CH_A_RST       0x80
#define WR9_HDWR_RST       0xc0

/* bit values for write register 10 */
/* misc tx/rx control bits */

#define WR10_6_BIT_SYNC    0x01
#define WR10_LOOP_MODE     0x02
#define WR10_ABORT_UND     0x04
#define WR10_MARK_IDLE     0x08
#define WR10_ACT_POLL      0x10
#define WR10_NRZ           0x00
#define WR10_NRZI          0x20
#define WR10_FM1           0x40
#define WR10_FM0           0x60
#define WR10_CRC_PRESET    0x80

/* bit values for write register 11 */
/* clock mode control */

#define WR11_OUT_XTAL      0x00
#define WR11_OUT_TX_CLK    0x01
#define WR11_OUT_BR_GEN    0x02
#define WR11_OUT_DPLL      0x03
#define WR11_TRXC_OI       0x04
#define WR11_TX_RTXC       0x00
#define WR11_TX_TRXC       0x08
#define WR11_TX_BR_GEN     0x10
#define WR11_TX_DPLL       0x18
#define WR11_RX_RTXC       0x00
#define WR11_RX_TRXC       0x20
#define WR11_RX_BR_GEN     0x40
#define WR11_RX_DPLL       0x60
#define WR11_RTXC_XTAL     0x80

/* write register 12 */
/* lower byte of baud rate generator time constant */

/* write register 13 */
/* upper byte of baud rate generator time constant */

/* bit values for write register 14 */
/* misc control bits */

#define WR14_BR_EN         0x01
#define WR14_BR_SRC        0x02
#define WR14_DTR_FUNC      0x04
#define WR14_AUTO_ECHO     0x08
#define WR14_LCL_LOOP      0x10
#define WR14_NULL          0x00
#define WR14_SEARCH        0x20
#define WR14_RST_CLK       0x40
#define WR14_DIS_DPLL      0x60
#define WR14_SRC_BR        0x80
#define WR14_SRC_RTXC      0xa0
#define WR14_FM_MODE       0xc0
#define WR14_NRZI          0xe0

/* bit values for write register 15 */
/* external/status interrupt control */

#define WR15_7PRIME_EN     0x01       /* new */
#define WR15_ZERO_CNT      0x02
#define WR15_FIFO_EN       0x04       /* new */
#define WR15_CD_IE         0x08
#define WR15_SYNC_IE       0x10
#define WR15_CTS_IE        0x20
#define WR15_TX_UND_IE     0x40
#define WR15_BREAK_IE      0x80

/* bit values for read register 0 */
/* tx/rx buffer status and external status  */

#define RR0_RX_AVAIL       0x01
#define RR0_ZERO_CNT       0x02
#define RR0_TX_EMPTY       0x04
#define RR0_CD             0x08
#define RR0_SYNC           0x10
#define RR0_CTS            0x20
#define RR0_TX_UND         0x40
#define RR0_BREAK          0x80

/* bit values for read register 1 */

#define RR1_ALL_SENT       0x01
#define RR1_RES_CD_2       0x02
#define RR1_RES_CD_1       0x01
#define RR1_RES_CD_0       0x08
#define RR1_PAR_ERR        0x10
#define RR1_RX_OV_ERR      0x20
#define RR1_CRC_ERR        0x40
#define RR1_END_FRAME      0x80

/* read register 2 */
/* interrupt vector (unmodified in ch A, modified with status in ch B) */

/* bit values for read register 3 */
/* interrupt pending register (channel A only) */

#define RR3_B_EXT_IP       0x01
#define RR3_B_TX_IP        0x02
#define RR3_B_RX_IP        0x04
#define RR3_A_EXT_IP       0x08
#define RR3_A_TX_IP        0x10
#define RR3_A_RX_IP        0x20

/* read register 8 */
/* receive data register */

/* bit values for read register 10 */
/* misc status bits */

#define RR10_ON_LOOP       0x02
#define RR10_LOOP_SEND     0x10
#define RR10_2_CLK_MIS     0x40
#define RR10_1_CLK_MIS     0x80

/* read register 12 */
/* lower byte of BRG time constant */

/* read register 13 */
/* upper byte of BRG time constant */

/* bit values for read register 15 */
/* external/status ie bits */

#define RR15_ZERO_CNT      0x02
#define RR15_CD_IE         0x08
#define RR15_SYNC_IE       0x10
#define RR15_CTS_IE        0x20
#define RR15_TX_UND_IE     0x40
#define RR15_BREAK_IE      0x80

#define MAX_BYTES          256
/* use this structure to access the SCC */
//typedef struct{
	//char cntl;
	//char filler;
	//char data;
//}SCC;

class SccClass
{
 public:
  SccClass(int port);
  int writePort(char *str, int len);
  int readPort(char *str, int len);
  int readString (char *str);
  int writeString (char *str);

 private:
  //SCC *spb;
  //unsigned short tc;      /* Time constant for BRG */
  int port_id;
};

#endif //SCCCLASS_H
