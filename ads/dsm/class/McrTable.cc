/* McrTable.cc
   Class for managing the Honeywell irs interface sample table.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <McrTable.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
McrTable::McrTable () 
 
// Class constructor.
{
  mcrout_start = 0;
  mcrstat_start = 0;
}
/*****************************************************************************/

void McrTable::enterMcrOut (TapeHeader &dsm_hdr)

// Make a table entry for the MCR_OUT variable.
{
  mcrout_start = dsm_hdr.start();
  mcrout_adr = dsm_hdr.adsaddr();
}
/*****************************************************************************/

void McrTable::enterMcrStat (TapeHeader &dsm_hdr)
 
// Make a table entry for the MCR_STAT variable.
{
  mcrstat_start = dsm_hdr.start();
}
/*****************************************************************************/

void McrTable::displayTable ()

// Display the table contents.
{
//  printf ("mcrout_adr = 0x%X, mcrout_start = %d, mcrstat_start = %d\n", 
//          mcrout_adr, mcrout_start, mcrstat_start);
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

