/* nats.h
   
   NATS specific definitions. Any modifications to this file must be C++ as
   well as C compatible.
*/

#define NATSIZE 0x2000                  /* NATS data buffer size */
#define NATS_SMP_INTVL	5		/* Default smp data tx interval */
#define NATS_PORT_NAME  "/dev/ttyb"	/* ground display serial port name */

#define NATS_SMP_TYPE	1		/* NATS sampled data type id */
#define NATS_MSG_TYPE	2		/* NATS message type id */
#define NATS_P2D_TYPE	3		/* NATS pms 2d data type id */

#define NATS_MX_EXP	5		/* max # expansion table entries */

/* Define the NATS block header structure. */

struct NatsHdr {
  int flag;		/* block start flag */
  int type;		/* data type */
  int length;		/* length of the data section */
  int hour;		/* packet creation time */
  int minute;		/* packet creation time */
  int sec;		/* packet creation time */
  str8 source;		/* data source */
  str8 dest;		/* data destination */
};

struct NatsExpEntry {
  strnam name;		/* data block name */
  int offset;		/* offset in expanded shared memory block */
};

struct NatsExpTbl {
  int size;		/* # of entries */
  struct NatsExpEntry entry[NATS_MX_EXP];
};

