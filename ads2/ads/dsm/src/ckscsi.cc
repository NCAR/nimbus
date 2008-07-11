/* MODULE: ckscsi.cc
*  PURPOSE: Test routine for Exabyte drive on the MVME162.
*  DATE: 8/26/94
*  AUTHOR: REIF HECK
*
*/
#include "mv162.h" /* mvme162 includes */
#include "vxWorks.h"
#include "scsiLib.h"
#include "ioLib.h"
#include "string.h"
#include "errnoLib.h"
#include "logLib.h"
#include "stdioLib.h"
#include "memLib.h"
#include "sysLib.h"
#include "exbDefs.h"
 
#include <Exb162.h>

Exb162 *exb;

void ckscsi()
{
  int stat,i;
  char data[100];
  char print[100];
  exb = new Exb162();

// Open the drive.
  printf ("Opening the drive in unbuffered mode.\n");
  while (exb->open(4, EXB_OPT_UNBUFFERED | EXB_OPT_VARIABLE_BLOCK_LENGTH | 
         EXB_OPT_UNIT_READY_ON_OPEN, 0) == ERROR) {
    printf ("Waiting for the drive.\n");
    taskDelay (sysClkRateGet());
  }

// Dump the sense block.
  stat = exb->senseDump (0);
  printf ("senseDump: STAT=%d\n", stat);
  taskDelay (sysClkRateGet());

// Load the drive.
  printf ("Loading the drive.\n");
  stat = exb->load (TRUE);       		// TRUE to load
  printf ("load: stat=%d\n",stat);
  taskDelay (sysClkRateGet());

// Write the leader record.
  printf ("Writing the leader record.\n");
  strcpy (data, "ADS DATA TAPE");
  stat = exb->write(data, 20);
  printf ("write: len = %d\n", stat);
  taskDelay (sysClkRateGet());

// Close the drive.
  printf ("Closing the drive.\n");
  stat = exb->close();
  printf ("close: stat=%d\n",stat);
  taskDelay (sysClkRateGet());

// Open the drive in buffered mode.
  printf ("Opening the drive in buffered mode.\n");
  while (exb->open(4, EXB_OPT_VARIABLE_BLOCK_LENGTH | 
         EXB_OPT_UNIT_READY_ON_OPEN, 0) == ERROR) {
    printf ("Waiting for the drive.\n");
    taskDelay (sysClkRateGet());
  }
  printf ("open: stat = %d\n", stat);
  taskDelay (sysClkRateGet());

// Write some records.
  printf ("Writing 1000 records.\n");
  strcpy (data, "HELLO!");
  for(i=0; i<1000; i++) {
    if (i == 999)
      strcpy (data, "GOODBYE!");
    stat = exb->write (data, sizeof(data));
    printf ("write: rec %d, len = %d\n", i, stat);
  }
  taskDelay (sysClkRateGet());

  printf ("Writing filemarks.\n");
  stat = exb->writeFilemarks (2);
  printf ("writeFilemarks: stat = %d\n", stat);
  taskDelay (2 * sysClkRateGet());

// Rewind the drive.
  printf ("Rewinding the drive.\n");
  stat = exb->rewind();
  printf ("rewind: stat=%d\n", stat);
  taskDelay (sysClkRateGet());

// Reload the drive.
  printf ("Loading the drive.\n");
  stat = exb->load (TRUE);		// TRUE to load
  printf("load: stat = %d\n", stat);
  taskDelay (sysClkRateGet());

// Read back the records.
  printf ("Reading records.\n");
  for (i = 0; (stat=exb->read (print, 32768)) > 0; i++) {
    printf ("read: rec %d, len = %d\n", i, stat);
    printf ("%s\n", print);
    print[0] = '\0';
  }
  taskDelay (sysClkRateGet());

  printf ("Unloading the drive.\n");
  stat = exb->load (FALSE);		// FALSE to unload
  printf ("load: stat = %d\n", stat);
  taskDelay (sysClkRateGet());

  printf ("Closing the drive.\n");
  stat = exb->close ();
  printf ("close: stat = %d\n", stat);
}
