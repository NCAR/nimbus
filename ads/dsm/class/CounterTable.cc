/* CounterTable.cc

   Class for managing the IP daughter Counter Table.

   Original Author: Chris Webster
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <CounterTable.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
/* -------------------------------------------------------------------- */
CounterTable::CounterTable () 
{
  start_50	= start_25	= start_5 = 0;
  len_50	= len_25	= len_5 = 0;
  offset_50	= offset_25	= offset_5 = 0;
  cnt_50	= cnt_25	= cnt_5 = 0;

  tot_cnt = 0;
  cur_idx = 0;

  cur_rate = RATE_50;

}

/* -------------------------------------------------------------------- */
void CounterTable::enter (TapeHeader &dsm_hdr)

// Make a table entry. dsm_hdr is passed in with the descriptor selected for
// the channel to be entered.
{
/***
  printf ("item= %s,", hdr_des->item_type);
  printf (" start= %4d,", hdr_des->start);
  printf (" len= %1d,", hdr_des->length);
  printf (" rate= %3d,", hdr_des->rate);
  printf (" addr= %4X,", hdr_des->adsaddr);
  printf (" name= %s,", hdr_des->name);
  printf (" type= %s\n", hdr_des->type);
***/

// Verify the correct ordering of the sample rates, highest to lowest.
  if (dsm_hdr.rate() > cur_rate) {
    fprintf (stderr, 
    "CounterTable: Out of sequence sample rate of %4d hz for channel 0x%04X.\n",
      dsm_hdr.rate(), dsm_hdr.adsaddr());
    exit (ERROR);
  }

// Determine the port size, based on the variable name.  The default port size
// is 16 bits, but some instruments need other sizes.
  if (!strcmp(dsm_hdr.type(), "C24"))
    portType[tot_cnt] = CTR_PORT_32;
  else
    portType[tot_cnt] = CTR_PORT_16;

// Increment the channel information for this rate.
  switch (cur_rate = dsm_hdr.rate()) {

    case RATE_50:
      if (!start_50) {
        start_50 = dsm_hdr.start();		// first 50 hz chan
        offset_50 = dsm_hdr.offset();
      }
      len_50 += dsm_hdr.length();               // increment sample block len
      adr[tot_cnt++] = dsm_hdr.adsaddr();	// enter address
      cnt_50++;
      break;

    case RATE_25:
      if (!start_25) {
        start_25 = dsm_hdr.start();		// first 25 hz chan
        offset_25 = dsm_hdr.offset();
      }
      len_25 += dsm_hdr.length();               // increment sample block len
      adr[tot_cnt++] = dsm_hdr.adsaddr();	// enter address
      cnt_25++;
      break;

    case RATE_5:
      if (!start_5) {
        start_5 = dsm_hdr.start();              // first 5 hz chan
        offset_5 = dsm_hdr.offset();
      }
      len_5 += dsm_hdr.length();                // increment sample block len
      adr[tot_cnt++] = dsm_hdr.adsaddr();	// enter address
      cnt_5++;
      break;

    default:
      fprintf (stderr,
        "CounterTable: Invalid sample rate of %4d hz for channel 0x%04X.\n",
        dsm_hdr.rate(), dsm_hdr.adsaddr());
      exit (ERROR);
  }
}

/* -------------------------------------------------------------------- */
void CounterTable::displayTable ()

// Display the table contents.
{
  int j;

//  printf ("Addresses\n");
  for (j = 0; j < tot_cnt; j++) 
    printf ("%04X ", adr[j]);

//  printf ("\nPort Types\n");
  for (j = 0; j < tot_cnt; j++) 
    printf ("%04X ", portType[j]);

/*  printf ("\ncnt_50 = %d, cnt_25 = %d, cnt_5 = %d, tot_cnt = %d\n", 
          cnt_50, cnt_25, cnt_5, tot_cnt);
  printf ("start_50 = %d, start_25 = %d, start_5 = %d\n", 
          start_50, start_25, start_5);
  printf ("len_50 = %d, len_25 = %d, len_5 = %d\n", len_50, len_25, len_5);
*/
}
