/* Pms2dhTable.cc
   Class for managing the 2-D housekeeping sample table.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/
#include <Pms2dhTable.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
Pms2dhTable::Pms2dhTable () 
 
// Class constructor.
{

  memset ((char*)intf, 0, sizeof (intf));
  memset ((char*)start_5, 0, sizeof (start_5));
  memset ((char*)len_5, 0, sizeof (len_5));

  ch_idx = 0;
  tot_cnt = 0;
}
/*****************************************************************************/

void Pms2dhTable::enter (TapeHeader &dsm_hdr)

// Enters channel information into the sample table.
{

// Increment the channel information for this rate.

  start_5[tot_cnt] = dsm_hdr.start();
  len_5[tot_cnt] = dsm_hdr.length();

  intf[tot_cnt] = tot_cnt / PMS2D_NUM_PROBES;
//  chan[tot_cnt] = tot_cnt % PMS2D_NUM_PROBES;
  chan[tot_cnt] = dsm_hdr.pms2h_intf_chan();

// printf("pms2_table->infc# = %d\n", intf[tot_cnt]);
// Increment the total channel counter.
  tot_cnt++;		
}
/*****************************************************************************/

void Pms2dhTable::displayTable ()

// Display the table contents.
{
  int j;

  printf ("tot_cnt = %d \n", tot_cnt);

  printf ("\nstart5 = %d\n", start_5);
  printf ("len5 = %d\n", len_5);
}
/*****************************************************************************/

int Pms2dhTable::nextChannel ()
 
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

