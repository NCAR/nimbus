/* sun_natscom.c

   Handles all serial data communications between the nats ground display sun 
   computer and the nats ground radio. This task runs in place of sun_ethcom
   on the nats ground display.
*/
#include <sys/types.h>
#include <sys/param.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <termio.h>

#include <header.h>
#include <adscmd.h>
#include <constants.h>
#include <eth_shm.h>
#include <files.h>
#include <lengths.h>
#include <mxasync.h>
#include <nats.h>
#include <pms2d.h>
#include <shmem.h>

/* Program functions */

void read_nats();			/* reads nats data */
void read_smp_data();			/* read sampled data */
void read_p2d_data();			/* read pms 2d data */
void read_msg_data();			/* read message data */
int expand_smp_data();			/* expand data from nats format */
void expand_irs();			/* expand nats irs blocks */
void expand_ins();			/* expand nats ins blocks */
void write_nats();			/* writes nats data */
void catch_sigusr1();   		/* SIGUSR1 hndler,write requests*/
void cleanup();		    		/* SIGUSR2 handler */
void catchint();   			/* SIGINT hangler */
int init_nats_port();			/* initialize nats serial port */
void read_tape_hdr();			/* read the tape header from disk */
void bld_expansion_tbl();		/* build irs & ins expansion table */
void sort_expansion_tbl();		/* sort the nats expansion table */
int get_desc_length();			/* get length of a tape hdr desc */
void init_shm();			/* initialize shared memory */

/* External variables */

struct SUN_ETH_SHM *eth_shm;		/* ethernet shared mem adr */
struct NatsExpTbl exp_tbl;		/* --> expansion table */

main (argc, argv)
int argc;
char *argv[];
{
  int nats_port;
  int eth_shm_id;
  static unsigned char header[MX_PHYS];	 /* tape header storage */


/* Set up the shared memory for other tasks to interface with */
  if ((eth_shm_id = getmem ("NATSCOM",ETHKEY, sizeof(struct SUN_ETH_SHM))) == ERROR) {
    (void)fprintf (stderr,"sun_natscom: FATAL error on getmem ETHKEY\n");
    exit(ERROR);
  }
  if ((eth_shm = (struct SUN_ETH_SHM*)memaddr(eth_shm_id)) == NULL) {
   (void)fprintf(stderr,"sun_natscom: FATAL error on memaddr for eth_shm_id\n");
    exit(ERROR);
  }

/* Read the tape header from disk. */
  read_tape_hdr(argv[1], header);  

/* Parse the header to get the irs and ins expansion offsets. */
  bld_expansion_tbl (header, &exp_tbl);
 

/* Clear the shared memory handshake parameters */
  init_shm();
  eth_shm->sun_ethcom_pid = getpid(); 	/* get process id */

/* Signal handler to terminate cleanly */
  signal (SIGUSR2, cleanup);

/* Signal handler to handle write requests. */
  signal (SIGUSR1, catch_sigusr1);

/* Signal handler to handle SIGINT, usually passed down from debugger. */
  signal (SIGINT, catchint);

  nats_port = init_nats_port();
  printf ("nats_port = %d\n", nats_port);

  while (1) {
    read_nats(nats_port);		/* Must be reading data first */
    write_nats(nats_port);
  } 
}
/*****************************************************************************/

void read_nats(port)
int port;

/* Reads data from the nats serial port into the SUN_ETH_SHM struct. */
{
  static int len = 0;
  struct NatsHdr nats_hdr;

/* Read the nats header block first. */
  for (len = 0; len < sizeof(struct NatsHdr); ) {
    len += read (port, (char*)&nats_hdr+len, sizeof(struct NatsHdr)-len);
  }
/* Check the packet alignment. */
  if (nats_hdr.flag != PKT_FLG) {
    (void)fprintf (stderr,
      "WARNING: sun_natscom - misaligned nats packet header received.\n");
    tcflush(port, TCIFLUSH);
    return;
  }
/* Determine the data type being received, and read the data into the proper
   shared memory buffer. */
  switch (nats_hdr.type) {
    case NATS_SMP_TYPE:
        read_smp_data (port, nats_hdr.length);
      break;
    case NATS_MSG_TYPE:
        read_msg_data (port, nats_hdr.length);
      break;
    case NATS_P2D_TYPE:
        read_p2d_data (port, nats_hdr.length);
      break;
  }
}
/*****************************************************************************/

void read_smp_data (port, blk_len)
int port;
int blk_len;

/* Reads sampled data from the nats port into sampled data shared memory. */
{
  int len;
  unsigned char tbuf[MX_PHYS];		/* temp data buffer */

/* Read the block. */
  for (len = 0; len < blk_len; ) {
    len += read (port, &tbuf[len], blk_len - len);
  }
/* Check block alignment. */
  if (*(unsigned short*)tbuf == (unsigned short)SDID) {
    eth_shm->smp.rlen = expand_smp_data (tbuf, eth_shm->smpbuf, blk_len);
    eth_shm->smp.rseq++;
    printf ("rlen = %d, rseq = %d\n", eth_shm->smp.rlen, eth_shm->smp.rseq);
  }
  else { 
    (void)fprintf (stderr, 
      "WARNING: sun_natscom - misaligned sampled data block received.\n");
    tcflush(port, TCIFLUSH);
  }
}
/*****************************************************************************/
 
void read_p2d_data (port, blk_len)
int port;
int blk_len;
 
/* Reads pms 2d data from the nats port into pms2d data shared memory. */
{
  int len;
 
/* Read the block. */
  for (len = 0; len < blk_len; ) {
    len += read (port, &eth_shm->twobuf[len], blk_len - len);
  } 
/* Check block alignment. */
  switch (((struct P2d_rec*)eth_shm->twobuf)->id) {
    case (short)C1 :
    case (short)P1 :
    case (short)C2 :
    case (short)P2 :
      eth_shm->two.rlen = blk_len;
      eth_shm->two.rseq++;
      break;
    default :
      (void)fprintf (stderr,
        "WARNING: sun_natscom - misaligned pms 2d data block received.\n");
  }
}
/*****************************************************************************/   
void read_msg_data (port, blk_len) 
int port;
int blk_len;
  
/* Reads message data from the nats port into message data shared memory. */ 
{ 
}
/*****************************************************************************/

int expand_smp_data (src, dest, len)
unsigned char *src;			/* source buffer */
unsigned char *dest;			/* destination buffer */
int len;				/* source buffer length */

/* Expands nats sampled data blocks from the src buffer into the destination
   buffer.
*/
{
  int j;
  int corr; 
  int idx;

  for (j = 0, idx = 0, corr = 0; j < exp_tbl.size; j++) {

/* Copy any previous data before an irs or ins block. */
    memcpy ( &dest[idx], &src[idx - corr], exp_tbl.entry[j].offset - idx); 
    idx = exp_tbl.entry[j].offset;

/* Expand the irs or ins block from nats format to sampled data format. */
    if (!strcmp (exp_tbl.entry[j].name, IRS_STR)) {
      expand_irs((struct Irs_blk_nats*)&src[idx - corr],
                 (struct Irs_blk*)&dest[idx]);
      idx += sizeof(struct Irs_blk);
      corr += (sizeof(struct Irs_blk) - sizeof(struct Irs_blk_nats));
    }
    else if (!strcmp (exp_tbl.entry[j].name, INS_STR)) {
      expand_ins((struct Ins_blk_nats*)&src[idx - corr],
                 (struct Ins_blk*)&dest[idx]);
      idx += sizeof(struct Ins_blk);
      corr += (sizeof(struct Ins_blk) - sizeof(struct Ins_blk_nats));
    }
  }
/* Copy the tail end of the block. */
  memcpy ( &dest[idx], &src[idx - corr], len - (idx - corr)); 
  return len + corr;			/* length of expanded block */
}
/*****************************************************************************/

void expand_irs (nats_irs, smp_irs)
struct Irs_blk_nats *nats_irs;
struct Irs_blk *smp_irs;

/* Expands the Irs block from the transmitted nats format to the sampled data
   format. */
{
  smp_irs->pitch_angle[0] = nats_irs->pitch_angle;
  smp_irs->roll_angle[0] = nats_irs->roll_angle;
  smp_irs->true_heading[0] = nats_irs->true_heading;
  smp_irs->inrt_vert_speed[0] = nats_irs->inrt_vert_speed;
  smp_irs->ground_speed[0] = nats_irs->ground_speed;
  smp_irs->wind_speed[0] = nats_irs->wind_speed;
  smp_irs->wind_dir_true[0] = nats_irs->wind_dir_true;
  smp_irs->velocity_ns[0] = nats_irs->velocity_ns;
  smp_irs->velocity_ew[0] = nats_irs->velocity_ew;
  smp_irs->present_lat[0] = nats_irs->present_lat;
  smp_irs->present_lon[0] = nats_irs->present_lon;
  smp_irs->lag_50hz_frame = nats_irs->lag_50hz_frame;
  smp_irs->lag_25hz_frame = nats_irs->lag_25hz_frame;
  smp_irs->lag_10hz_frame = nats_irs->lag_10hz_frame;
  smp_irs->lag_5hz_frame = nats_irs->lag_5hz_frame;
}
/*****************************************************************************/
 
void expand_ins (nats_ins, smp_ins) 
struct Ins_blk_nats *nats_ins; 
struct Ins_blk *smp_ins;   
 
/* Expands the Irs block from the transmitted nats format to the sampled data 
   format. */
{ 
  smp_ins->sec = nats_ins->sec;
  smp_ins->t250 = nats_ins->t250;
  smp_ins->lat = nats_ins->lat;
  smp_ins->lon = nats_ins->lon;
  smp_ins->vx1 = nats_ins->vx1;
  smp_ins->vy1 = nats_ins->vy1;
  smp_ins->alpha1 = nats_ins->alpha1;
  smp_ins->truehd1 = nats_ins->truehd1;
  smp_ins->track = nats_ins->track;
  smp_ins->gndspd = nats_ins->gndspd;
}
/*****************************************************************************/
 
void write_nats(port)
int port;
 
/* Writes nats message and command data to the nats serial port. */
{
}
/*****************************************************************************/

void catchint()
/* Handles SIGINT
*/
{
}

/*****************************************************************************/

void catch_sigusr1()

/* Handles write request signals. */
{
}
/*****************************************************************************/

int init_nats_port()

/* Configure the nats serial port for communications with the radios. */
{
  struct termios tio_cfg;
  static char port_name[] = NATS_PORT_NAME;
  int port;
  int tmp;

  if ((port = open(port_name, O_RDWR)) < 0) {
    perror ("sun_natscom - open nats_port");
    cleanup(ERROR);
  }
  memset ((char*)&tio_cfg, 0, sizeof (struct termios));
  printf ("port = %d\n", port);

/* Configure for no post processing, 9600 baud, 8 data bits, 1 stop bit,
   enable receiver, enable parity, odd parity, enable rts/cts flow control. */
  tio_cfg.c_iflag = BRKINT | IGNPAR;
  tio_cfg.c_oflag = 0;
  tio_cfg.c_cflag = B9600 | CS8 | CREAD | CRTSCTS | HUPCL;
  tio_cfg.c_lflag = ISIG | NOFLSH;
  if (ioctl(port, (int)TCSETS, (char*)&tio_cfg) < 0) {
    perror ("ioctl TCSETS");
    cleanup(ERROR);
  }
  return port;
}
/*****************************************************************************/

void read_tape_hdr(fname, header)
char *fname;
unsigned char *header;
{
  int hdr_fd;
  int len;
  int tlen;
  char hdr_file[MAX_FLNM_LNGTH];         /* pathname for header */
  struct Fl *fl;                         /* --> file descriptor struct */
  
  (void)sprintf(hdr_file, "/home/winds/proj/%s/header.nats", fname);

  if ((hdr_fd = open (hdr_file, O_RDONLY, 0)) == ERROR) {
    (void)fprintf (stderr,"sun_natscom: FATAL error opening tape header.\n");
    cleanup(ERROR);
  }

  if (read (hdr_fd, header, sizeof(struct Fl)) != sizeof(struct Fl)) {
    (void)fprintf (stderr,"sun_natscom: FATAL error reading tape header.\n");
    cleanup(ERROR);
  }

  fl = (struct Fl*)header;
  if (strcmp(fl->thdr, TAPEHDR_STR)) {  /* check for a valid id */
    (void)fprintf (stderr,
             "sun_natscom: Fatal error, invalid tape header id string.\n");
    cleanup(ERROR);
  }
 
  for (len = sizeof(struct Fl); (len < fl->thdrlen) &&
    (tlen = read (hdr_fd, &header[len], fl->thdrlen - len)); len += tlen);
 
  if (len != fl->thdrlen) {
    (void)fprintf (stderr,"sun_natscom: FATAL error reading tape header.\n");
    cleanup(ERROR);
  }
}
/*****************************************************************************/

void bld_expansion_tbl(hdr, exp_tbl)
unsigned char *hdr;
struct NatsExpTbl *exp_tbl;		/* --> expansion table */

/* Parses the tape header and creates the irs and ins expansion table. */
{
  char *desc;				/* --> tape header descriptor */
  struct Fl *fl;
  int j;
  int tbl_idx = 0;			/* expansion table index */
  int exp_corr;				/* expansion offset correction */

  fl = (struct Fl*)hdr;
  memset ((char*)exp_tbl, 0, sizeof(struct NatsExpTbl));

  for (j = 0, desc = (char*)(hdr + sizeof(struct Fl)); j < fl->n_items;
       desc += get_desc_size(desc), j++) {
    if (!strcmp (desc, IRS_STR)) {
      (void)strcpy (exp_tbl->entry[tbl_idx].name, desc);
      exp_tbl->entry[tbl_idx++].offset = ((struct Irs*)desc)->start;
    }
    else if (!strcmp (desc, INS_STR)) {
      (void)strcpy (exp_tbl->entry[tbl_idx].name, desc);
      exp_tbl->entry[tbl_idx++].offset = ((struct Ins*)desc)->start;
    }
  } 
  exp_tbl->size = tbl_idx;
  sort_expansion_tbl(exp_tbl); /* sort the table by to offsets */
}
/*****************************************************************************/

void sort_expansion_tbl (tbl)
struct NatsExpTbl *tbl;

/* Sorts the passed in table by it "to" offsets. */
{
}
/*****************************************************************************/

int get_desc_size(desc)
char *desc;

/* Returns the length of the current descriptor.  All tape header descriptors
   have the descriptor length as the 2nd field of the struct.
*/
{
  return ((struct Blk*)desc)->item_len;
}
/*****************************************************************************/

void cleanup(status)
int status;

/* catch SIGINT signal ==> clean up and exit */
{
/* reset eth_shm's fields, since the old values may remain in shared memory.
   eth_shm may be attached to by some other process in next WINDS session
   before sun_ethcom has a chance to initialize fields on startup, and those
   old values can cause confusion.  In particular, the first comparison
   by input process of its sequence number and eth_shm's sequence number
   will be non-zero (implying new data, which there may not be) if this
   exit initialization is not performed.
*/
  init_shm();
  exit (status);
}
/******************************************************************************/

void init_shm()

/* Clear the shared memory handshake parameters */
{
  eth_shm->sun_ethcom_pid = 0;
  eth_shm->smp.rseq = 0;
  eth_shm->smp.rlen = 0;
  eth_shm->two.rseq = 0;
  eth_shm->two.rlen = 0;
  eth_shm->cmd.rseq = 0;
  eth_shm->cmd.rlen = 0;
  eth_shm->cmd.rrqlen = 0;
  eth_shm->cmd.wrqlen = PORT_LOCK;
  eth_shm->cur_tas = TAS2D_AUTO;
}
/******************************************************************************/

