/* DigitalTable.cc

   Class for managing the digital input channel sample table.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <DigitalTable.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
DigitalTable::DigitalTable () 
 
// Class constructor.
{
  start_55 = 0;
  start_50 = 0;
  start_25 = 0;
  start_5 = 0;
  start_1 = 0;
  len_55 = 0;
  len_50 = 0;
  len_25 = 0;
  len_5 = 0;
  len_1 = 0;
  offset_55 = 0;
  offset_50 = 0;
  offset_25 = 0;
  offset_5 = 0;
  cnt_55 = 0;
  cnt_50 = 0;
  cnt_25 = 0;
  cnt_5 = 0;
  cnt_1 = 0;
  tot_cnt = 0;
  cur_idx = 0;
  cur_rate = RATE_55;
}
/*****************************************************************************/

void DigitalTable::enter (TapeHeader &dsm_hdr)

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
    "DigitalTable: Out of sequence sample rate of %4d hz for channel 0x%04X.\n",
      dsm_hdr.rate(), dsm_hdr.adsaddr());
    exit (ERROR);
  }

// Determine the port size, based on the variable name.  The default port size
// is 8 bits, but some instruments need other sizes.

  if (strcmp(dsm_hdr.type(), "D2") == 0)
    port_type[tot_cnt] = DIG_PORT_2;
  else if (!strcmp (dsm_hdr.name(), RADAR_ALT_NAME))
    port_type[tot_cnt] = DIG_PORT_RADAR_ALT;
  else if (strcmp(dsm_hdr.type(), "D16") == 0)
    port_type[tot_cnt] = DIG_PORT_16;
  else
    port_type[tot_cnt] = DIG_PORT_8;


/*
  if (!strcmp (dsm_hdr.name(), PSFD2_NAME))
  else if (!strcmp (dsm_hdr.name(), PSFD_NAME) ||
           !strcmp (dsm_hdr.name(), MCRSTAT_NAME))
    port_type[tot_cnt] = DIG_PORT_16;
*/


// Increment the channel information for this rate.
  switch (cur_rate = dsm_hdr.rate()) {

    case RATE_55:
      if (!start_55) {
        start_55 = dsm_hdr.start();             // first 55 hz chan
        offset_55 = dsm_hdr.offset();
      }
      len_55 += dsm_hdr.length();               // increment sample block len
      adr[tot_cnt++] = dsm_hdr.adsaddr();       // enter address
      cnt_55++;
      break;

    case RATE_50:
      if (!start_50) {
        start_50 = dsm_hdr.start();             // first 50 hz chan
        offset_50 = dsm_hdr.offset();
      }
      len_50 += dsm_hdr.length();               // increment sample block len
      adr[tot_cnt++] = dsm_hdr.adsaddr();       // enter address
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

    case RATE_1:
      if (!start_1)			
        start_1 = dsm_hdr.start();		// first 1 hz chan
      len_1 += dsm_hdr.length();		// increment sample block len
      adr[tot_cnt++] = dsm_hdr.adsaddr();	// enter address
      cnt_1++;
      break;

    default:
      fprintf (stderr,
        "DigitalTable: Invalid sample rate of %4d hz for channel 0x%04X.\n",
        dsm_hdr.rate(), dsm_hdr.adsaddr());
      exit (ERROR);
  }
}
/*****************************************************************************/

void DigitalTable::displayTable ()

// Display the table contents.
{
  int j;

//  printf ("Addresses\n");
  for (j = 0; j < tot_cnt; j++) 
    printf ("%04X ", adr[j]);

//  printf ("\nPort Types\n");
  for (j = 0; j < tot_cnt; j++) 
    printf ("%04X ", port_type[j]);

/*  printf ("\ncnt_25 = %d, cnt_5 = %d, cnt_1 = %d, tot_cnt = %d\n", 
          cnt_25, cnt_5, cnt_1, tot_cnt);
  printf ("start_25 = %d, start_5 = %d, start_1 = %d\n", 
          start_25, start_5, start_1);
  printf ("len_25 = %d, len_5 = %d, len_1 = %d\n", len_25, len_5, len_1);
*/
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

