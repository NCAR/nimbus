/* cksynchro.cc 

   This program checks operation of the synchro-to-digital interface of the
   Electra radar altimeter.
   mvme 162 version

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/
#include <mv162.h>

#include <vxWorks.h>
#include <stdioLib.h>
#include <taskLib.h>

#include <dsmVmeDefs.h>
#include <dsmctl.h>
#include <ipicDefs.h>

#include <Synchro.h>

// Edit NUM_CHANs and FIRST_CHANs for desired rates and channels
#define NUM_25HZ_CHANS          2               // number of 25 hz channels
#define FIRST_25HZ_CHAN         0               // first 25 hz channel to sample

// Class declarations.
static Synchro *synchro;
static SynchroTable *table;

// Program functions.
void buildSynchroTable();                        // build a test table

int cksynchro ()
{

// Init the classes.

  table = new SynchroTable();

// Initialize the SynchroTable class.
  synchro = new Synchro ((char*)IP_b_IO_BASE, *table);
  buildSynchroTable ();

  while (TRUE) {
    taskDelay(sysClkRateGet());
    synchro->test();
  }
}
/*****************************************************************************/
 
void buildSynchroTable ()
 
// Creates a test synchro table
{
  int j;
 
  for (j = 0; j < NUM_25HZ_CHANS; j++)
    table->enter (j + FIRST_25HZ_CHAN, RATE_25, j*2, NUM_25HZ_CHANS * 2, 2);
 
}
