/* SynchroTable.cc
   Class for managing the synchro sample table.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/
#include <SynchroTable.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
SynchroTable::SynchroTable () 
 
// Class constructor.
{

  start_25 = 0;
  start_5 = 0;
  start_1 = 0;

  len_25 = 0;
  len_5 = 0;
  len_1 = 0;

  offset_25 = 0;
  offset_5 = 0;
  offset_1 = 0;

  cnt_25 = 0;
  cnt_5 = 0;
  cnt_1 = 0;
  tot_cnt = 0;

  cur_rate = RATE_25;
  ch_idx = 0;
}
/*****************************************************************************/

void SynchroTable::enter ( int adr, int rate, int start, int offset, int length)

// Enters channel information into the sample table.
{
// Verify the correct ordering of the sample rates, highest to lowest.

// Increment the channel information for this rate.
  switch (cur_rate = rate) {

    case RATE_25:
      if (!len_25) {			
        start_25 = start;		// first 25 hz chan
        offset_25 = offset;
      }
      len_25 += length;			// increment sample block len
      cnt_25++;				// incr dsp table count
      break;

    case RATE_5:
      if (!len_5) {			
        start_5 = start;		// first 5 hz chan
        offset_5 = offset;
      }
      len_5 += length;			// increment sample block len
      cnt_5++;				// incr count
      break;

    case RATE_1:				// only for a dummy variable
      if (!len_1) {			
        start_1 = start;		// first 5 hz chan
        offset_1 = offset;
      }
      len_1 += length;			// increment sample block len
      cnt_1++;
      break;					// just ignore

    default:
      fprintf (stderr,
        "SynchroTable: Invalid sample rate of %4d hz for channel 0x%04X.\n",
        rate, adr);
      exit (ERROR);
  }
// Enter the channel address.
  addrs[tot_cnt] = adr;

// Increment the total channel counter.
  tot_cnt++;		
}
/*****************************************************************************/

void SynchroTable::displayTable ()

// Display the table contents.
{
  int j;

  printf ("tot_cnt = %d, cnt25 = %d, cnt5 = %d\n", tot_cnt, cnt_25, cnt_5);

  printf ("\nstart25 = %d, start5 = %d\n", start_25, start_5);
  printf ("len25 = %d, len5 = %d\n", len_25, len_5);
  printf ("offset25 = %d, offset5 = %d\n", offset_25, offset_5);
}
/*****************************************************************************/

int SynchroTable::nextChannel ()
 
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

