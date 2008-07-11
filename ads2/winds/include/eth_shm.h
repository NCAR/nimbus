/* eth_shm.h
   Definitions for the ethernet connections between the ADS and the sun, 
   and the ethernet communications shared memory struct. 
*/
#ifndef ETHSHM_H
#define ETHSHM_H

#include <header.h>

#define PORT_AVAIL	0		/* port is available	*/
#define PORT_LOCK	-1		/* port is in use	*/

#define TWOSIZE		0x2000		/* 2D receive data shm buffer size */
#define AVAPSIZE	0x0080		/* AVAPS shm buffer size	*/
#define CMDSIZE		0x0100		/* message/status shm buffer size  */
 
struct SOCK_PORT {
  int rseq;				/* read sequence #		*/
  int rlen;				/* completed read length	*/
  int rrqlen;				/* request read length		*/
  int wrqlen;				/* request write length		*/
};

/* Sun global ethernet communications shared memory structure
 * windComm receives data and fills this structure.  winput then
 * processes the new data.
 */

struct SUN_ETH_SHM  {
  int comm_pid;				/* network communication task's pid */
  struct SOCK_PORT sync;		/* smp data control struct	*/
  struct SOCK_PORT grey;		/* grey scale data control struct */
  struct SOCK_PORT two;			/* grey & 2D data ctl struct	*/
  struct SOCK_PORT avaps[MAX_AVAPS];	/* AVAPS data ctl struct	*/
  struct SOCK_PORT cmd;			/* command control struct	*/
  unsigned char syncBuf[MX_PHYS]; 	/* rcvd raw data buffer		*/
  unsigned char greyBuf[MX_PHYS]; 	/* rcvd grey scale buffer	*/
  unsigned char twoBuf[TWOSIZE]; 	/* rcvd 2D data buffer		*/
  unsigned char avapsBuf[MAX_AVAPS][AVAPSIZE]; 	/* rcvd AVAPS data buff */
  unsigned char cmdBuf[CMDSIZE]; 	/* send commands data buffer	*/
};

#endif
