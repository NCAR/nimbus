/*
-------------------------------------------------------------------------
OBJECT NAME:	windComm.c

FULL NAME:	Winds Communications Program

ENTRY POINTS:	main()

DESCRIPTION:	Data sent by discComm via socket are placed into shared
		memory for winput/WINDS to perform data processing.

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	WINDS starts and stops windComm

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1995-9
-------------------------------------------------------------------------
*/

#include <sys/utsname.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "eth_shm.h"
#include "shmem.h"

// Class include files.
#include "adsIOrt.h"

/* Program functions */

void runDsm();				// interface to the dsm control
void netMessage(int, char*, char*);	// network message handler
void catchPipe(int stat);	// SIGPIPE handler
void catch_sigusr1(int status);	// SIGUSR1 handler,wr cmd_sock
void catchint(int status); 	// SIGINT handler 
void cleanup(int status);	// SIGUSR2 handler 
void init_shm();		// initialize shared memory 


/* External functions */
extern "C" {
extern int SetComputeHost();
extern char *GetComputeHost();
extern int read_static_ids(char *ComputeHost);	
extern int getaddr_static();
extern int getaddr_dynamic();
extern void read_dynamic_ids(char *prog_name, char *ComputeHost);
extern int get_datablock_size();
};

/* External variables */

extern volatile struct SUN_ETH_SHM *eth_shm;	/* ethernet shared mem adr */

#define	ERROR	(-1)

char	phys_rec[0x8000];		// Buffer to read in data records.

str8	proj_number;			// project number
char	*ProjectDirectory;


/* -------------------------------------------------------------------- */
main (int argc, char *argv[])
{
  int pid;

  if ((ProjectDirectory = (char *)getenv ("PROJ_DIR"))== NULL) {
    (void)fprintf(stderr,"FATAL: WINDS environment variable not set\n");
    exit (1);
  }

  strcpy(proj_number, argv[1]);
  SetComputeHost();
printf("WINDCOMM: host = %s\n", GetComputeHost());
/* Set up the shared memory for other tasks to interface with */
  pid = getpid();				/* get process id */
  if (!read_static_ids(GetComputeHost())) {
    (void)fprintf (stderr, "windComm: read_static_ids returned ERROR.\n");
    cleanup(ERROR);
  }

  if (getaddr_static()==ERROR) {
   (void)fprintf (stderr, "windComm: getaddr_static returned ERROR.\n");
   cleanup(ERROR);
  }

  read_dynamic_ids("ETHCOM",GetComputeHost());

  fprintf (stderr, "windComm: dynamic ids acquired.\n");

  if (getaddr_dynamic()==ERROR) {
   (void)fprintf (stderr, "windComm: getaddr_dynamic returned ERROR.\n");
   cleanup(ERROR);
  }

/* Clear the shared memory handshake parameters */
  init_shm();
  eth_shm->comm_pid = pid;

// Set up the SIGPIPE handler.
  if ((int)signal (SIGPIPE, catchPipe) < 0) {
    perror("windComm: sigset SIGPIPE");
    cleanup (ERROR);
  }

/* Signal handler to terminate cleanly */
  if ((int)signal(SIGUSR2, cleanup) < 0) {
    perror("windComm signal cleanup");
    cleanup(ERROR);
  }

/* Signal handler to handle write requests. */
  if ((int)signal(SIGUSR1, catch_sigusr1) < 0) {
    perror("windComm signal catch_sigusr1");
    cleanup(ERROR);
  }

  runDsm();
}
 
/* -------------------------------------------------------------------- */
void runDsm()
{ 
  ADS_rtFile	*file;
  Hdr_blk	*h, twod;
  int		len;
  long		cntr = 0;

  file = new ADS_rtFile();
  file->LastSyncRecord(phys_rec, 60);  // Back off 60 seconds.
  file->LastPMS2dRecord((P2d_rec *)phys_rec);

// Main loop.
  while (TRUE)
    {
    if (file->NextSyncRecord((char *)eth_shm->syncBuf))
      {
      eth_shm->sync.rlen = file->hdr->lrLength();
      eth_shm->sync.rseq++;
//      h = (Hdr_blk*)eth_shm->syncBuf;
//      fprintf(stderr, "WINDCOMM: %2X %2d:%2d:%2d\n", ntohs(h->id),
//		ntohs(h->hour),ntohs(h->minute),ntohs(h->second));
      }


    // Check once a second at the last record.
    if (cntr % 10 == 0 && file->LastPMS2dRecord((P2d_rec *)eth_shm->twoBuf))
      {
      if (memcmp((void *)eth_shm->twoBuf, (void *)&twod, 8))
        {
        eth_shm->two.rlen = sizeof(P2d_rec);
        eth_shm->two.rseq++;
        memcpy((void *)&twod, (void *)eth_shm->twoBuf, 8);
//        h = (Hdr_blk*)eth_shm->twoBuf;
//        fprintf(stderr, "WINDCOMM: %2X %02d:%02d:%02d\n", ntohs(h->id),
//		ntohs(h->hour),ntohs(h->minute),ntohs(h->second));
        }
      }

/*
    if (file->NextAvapsRecord(phys_rec))
      {
      int	avapsIndex;

      avapsIndex = atoi(&phys_rec[7]) - 1;
      memcpy((void *)eth_shm->avapsBuf[avapsIndex], (void *)phys_rec, len);
      eth_shm->avaps[avapsIndex].rlen = len;
      eth_shm->avaps[avapsIndex].rseq++;
      }
*/

    for (len = 0; len < 30000 &&
		file->NextGreyRecord((char *)&eth_shm->greyBuf[len]); )
      {
      len += sizeof(GreyParticle) +
	(ntohs(((GreyParticle *)(&eth_shm->greyBuf[len]))->slice_cnt) * 16);
      }
   
    if (len > 0)
      {
fprintf(stderr, "windComm, len=%d\n", len);
      memcpy((char *)&eth_shm->greyBuf[len], "  ", 2);
      eth_shm->grey.rlen = len;
      eth_shm->grey.rseq++;
      }

    usleep(20000);
    ++cntr;
    }

}
 
/* -------------------------------------------------------------------- */
void netMessage(int action, char *name, char *msg_str)
 
// Network message handler.
{
  fprintf(stderr, "windComm: %s", msg_str);
}

/* -------------------------------------------------------------------- */
void catchPipe (int stat)
{

}

/* -------------------------------------------------------------------- */
void catch_sigusr1 (int status)

{
}

/* -------------------------------------------------------------------- */
void cleanup (int status)

/* catch SIGUSR2 signal ==> clean up and exit */
{
/* reset eth_shm's fields, since the old values may remain in shared memory.
   eth_shm may be attached to by some other process in next WINDS session
   before windComm has a chance to initialize fields on startup, and those
   old values can cause confusion.  In particular, the first comparison
   by input process of its sequence number and eth_shm's sequence number
   will be non-zero (implying new data, which there may not be) if this
   exit initialization is not performed.
*/
  init_shm();

  exit (status);
}

/* -------------------------------------------------------------------- */
void init_shm()
{
  int	i;

  eth_shm->comm_pid	= 0;
  eth_shm->sync.rseq	= 0;
  eth_shm->sync.rlen	= 0;
  eth_shm->grey.rseq	= 0;
  eth_shm->grey.rlen	= 0;
  eth_shm->two.rseq	= 0;
  eth_shm->two.rlen	= 0;
  eth_shm->cmd.rseq	= 0;
  eth_shm->cmd.rlen	= 0;
  eth_shm->cmd.rrqlen	= 0;
  eth_shm->cmd.wrqlen	= PORT_LOCK;

  for (i = 0; i < MAX_AVAPS; ++i)
    {
    eth_shm->avaps[i].rseq = 0;
    eth_shm->avaps[i].rlen = 0;
    }

}	/* END INIT_SHM */

/* END WINDCOMM.CC */
