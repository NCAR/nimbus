/* AnaAuxTable.cc
   Class for managing the auxiliary analog channel sample table.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/
#include <AnaAuxTable.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
AnaAuxTable::AnaAuxTable () 
 
// Class constructor.
{
  memset ((char*)&addrs, 0, sizeof (addrs));
  memset ((char*)&gains, 0, sizeof (gains));

  start_25 = 0;
  start_5 = 0;

  len_25 = 0;
  len_5 = 0;

  offset_25 = 0;
  offset_5 = 0;

  cnt_25 = 0;
  cnt_5 = 0;
  tot_cnt = 0;

  cur_rate = RATE_25;
  ch_idx = 0;
}
/*****************************************************************************/

void AnaAuxTable::enter (int adr, int rate, int ch_gain, int start, 
                            int offset, int length)

// Enters channel information into the sample table.
{
// Verify the correct ordering of the sample rates, highest to lowest.
  if (rate > cur_rate) {
    fprintf (stderr, 
"AnaAuxTable: Out of sequence sample rate of %4d hz for channel 0x%04X.\n",
      rate, adr);
    exit (ERROR);
  }

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
      return;					// just ignore

    default:
      fprintf (stderr,
        "AnaAuxTable: Invalid sample rate of %4d hz for channel 0x%04X.\n",
        rate, adr);
      exit (ERROR);
  }

// Enter the channel address and gain. 
  addrs[tot_cnt] = adr;
  gains[tot_cnt] = ch_gain;

// Increment the total channel counter.
  tot_cnt++;		
}
/*****************************************************************************/

void AnaAuxTable::displayTable ()

// Display the table contents.
{
  int j;
/***
  printf ("tot_cnt = %d, cnt25 = %d, cnt5 = %d\n", tot_cnt, cnt_25, cnt_5);

  printf ("Addresses\n");
  for (j = 0; j < tot_cnt; j++) 
    printf ("%04X ", addrs[j]);

  printf ("Gains\n");
  for (j = 0; j < tot_cnt; j++) 
    printf ("%04X ", gains[j]);

  printf ("\nstart25 = %d, start5 = %d\n", start_25, start_5);
  printf ("len25 = %d, len5 = %d\n", len_25, len_5);
  printf ("offset25 = %d, offset5 = %d\n", offset_25, offset_5);
***/
}
/*****************************************************************************/

int AnaAuxTable::nextChannel ()
 
// Selects the next channel in the setup table
{
  if (++ch_idx < tot_cnt) 
    return TRUE;

  ch_idx--;				// end of the table
  return FALSE;
}
/*****************************************************************************/

int AnaAuxTable::selectByAddress (int adr)
 
// Selects a channel in the setup table by address.
{
  int stat;

  for (stat = firstChannel(); stat && (adr != address()); stat = nextChannel());
  return stat;
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

