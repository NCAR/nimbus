/* Pms1VmeTable.cc
   Class for managing the analog channel sample table.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <Pms1VmeTable.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
Pms1VmeTable::Pms1VmeTable () 
 
// Class constructor.
{
  memset ((char*)p1_intf, 0, sizeof (p1_intf));
  memset ((char*)p1_chan, 0, sizeof (p1_chan));
  memset ((char*)p1_start, 0, sizeof (p1_start));
  memset ((char*)p1_length, 0, sizeof (p1_length));
  memset ((char*)ps_start, 0, sizeof (ps_start));
  memset ((char*)ps_length, 0, sizeof (ps_length));
  memset ((char*)p1_rate, 0, sizeof (p1_rate));
  memset ((char*)p1_bins, 0, sizeof (p1_bins));
  memset ((char*)p1_range, 0, sizeof (p1_range));
  memset ((char*)p1_name, 0, sizeof (p1_name));

  p1_idx = 0;
  p1_count = 0;
}
/*****************************************************************************/

void Pms1VmeTable::enter (TapeHeader &dsm_hdr)

// Make a table entry. dsm_hdr is passed in with the descriptor selected for
// the channel to be entered.
{
  p1_intf[p1_count] = dsm_hdr.intf_num();
  p1_chan[p1_count] = dsm_hdr.intf_chan();
  p1_start[p1_count] = dsm_hdr.start();
  p1_length[p1_count] = dsm_hdr.length();
  ps_start[p1_count] = dsm_hdr.ps_start();
  ps_length[p1_count] = dsm_hdr.ps_length();
  p1_rate[p1_count] = dsm_hdr.rate();

  if (!strncmp(FSSP_STR, dsm_hdr.name(), 4) ||
      !strncmp(ASAS_STR, dsm_hdr.name(), 4) ||
      !strncmp(PCAS_STR, dsm_hdr.name(), 4)) {
    p1_bins[p1_count] = BINS_16;
    p1_range[p1_count] = RANGE_0;
  }
  else if (!strncmp(X200_STR, dsm_hdr.name(), 4) ||
           !strncmp(Y200_STR, dsm_hdr.name(), 4)) {
    p1_bins[p1_count] = BINS_16;
    p1_range[p1_count] = RANGE_NONE;
  }
  else if (!strncmp (F300_STR, dsm_hdr.name(), 4)) {
    p1_bins[p1_count] = BINS_32;
    p1_range[p1_count] = RANGE_NONE;
  }
  else if (!strncmp (X260_STR, dsm_hdr.name(), 4)) {
    p1_bins[p1_count] = BINS_64;
    p1_range[p1_count] = RANGE_NONE;
  }
  else {
    printf ("Pms1VmeTable: Unknown VME Pms 1d probe name, %s\n", 
            dsm_hdr.name());
    exit (ERROR);
  }

  strncpy (p1_name[p1_count], dsm_hdr.name(), 4);
  strcat (p1_name[p1_count], "_");
  strcat (p1_name[p1_count], dsm_hdr.locn());
  p1_count++;
}
/*****************************************************************************/

int Pms1VmeTable::nextChan ()
 
// Select the next channel entry.
{
  if ((p1_idx + 1) < p1_count) {
    p1_idx++;
    return TRUE;
  }
  return FALSE;
}
/*****************************************************************************/

void Pms1VmeTable::displayTable ()

// Display the table contents.
{
  int j;

  for (j = 0; j < p1_count; j++) {
/**
    printf ("intf %1d, chan %1d, rate %2d, name %s\n", 
            p1_intf[j], p1_chan[j], p1_rate[j], p1_name[j]);
    printf ("start %4d, length %3d, ps_start %4d, ps_length %3d\n",
            p1_start[j], p1_length[j], ps_start[j], ps_length[j]); 
**/
  }
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

