/* GreyVmehTable.cc
   Class for managing the Grey housekeeping sample table.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/
#include <GreyVmehTable.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
GreyVmehTable::GreyVmehTable () 
 
// Class constructor.
{

  memset ((char*)intf, 0, sizeof (intf));
  memset ((char*)start_1, 0, sizeof (start_1));
  memset ((char*)len_1, 0, sizeof (len_1));

  ch_idx = 0;
  tot_cnt = 0;
}
/*****************************************************************************/

void GreyVmehTable::enter (TapeHeader &dsm_hdr)

// Enters channel information into the sample table.
{

// Increment the channel information for this rate.

  start_1[tot_cnt] = dsm_hdr.start();
  len_1[tot_cnt] = dsm_hdr.length();

  intf[tot_cnt] = tot_cnt / GREY_NUM_PROBES;
  chan[tot_cnt] = tot_cnt % GREY_NUM_PROBES;

// Increment the total channel counter.
  tot_cnt++;		
}
/*****************************************************************************/

void GreyVmehTable::displayTable ()

// Display the table contents.
{
  int j;

  printf ("tot_cnt = %d \n", tot_cnt);

  printf ("\nstart1 = %d\n", start_1);
  printf ("len1 = %d\n", len_1);
}
/*****************************************************************************/

int GreyVmehTable::nextChannel ()
 
// Selects the next channel in the setup table
{
  if (++ch_idx < tot_cnt) 
    return TRUE;

  ch_idx--;				// end of the table
  return FALSE;
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

