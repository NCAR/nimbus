/* natsDefs.h
   NATS specific definitions. 

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
*/

#ifndef NATSDEFS_H
#define NATSDEFS_H

#include <header.h>

#define NATS_PORT_NAME		"/dev/ttya"	/* Sun serial port name */
#define NATS_SMP_INTVL	2			/* default smp data interval */

#define NATS_PACKET_FLAG        0x55555555      /* packet start flag */
#define NATS_FLAG_BYTE		0x55		/* one byte of the flag */
#define NATS_SMP_TYPE           1               /* NATS sampled data type id */
#define NATS_MSG_TYPE           2               /* NATS message type id */
#define NATS_P2D_TYPE           3               /* NATS pms 2d data type id */
 
#define NATS_BAUD               2400            /* serial port baud rate */
#define NATS_DATA_BITS          8               /* serial port data bits */
#define NATS_PARITY             SER_NONE        /* serial port parity */
#define NATS_STOP_BITS          1.0             /* serial port stop bits */
#define NATS_HW_HANDSHAKE       FALSE           /* serial port hw handshake */
 
/* Define the NATS packet header structure. */
 
typedef struct {
  int flag;                             /* block start flag */
  int type;                             /* data type */
  int seq;                              /* sequence number */
  int final_seq;                        /* final sequence number */
  int length;                           /* length of the data section */
} NatsPacketHdr;

#endif

