/* HwIrsTable.cc
   Class for managing the Honeywell irs interface sample table.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <HwIrsTable.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
HwIrsTable::HwIrsTable () 
 
// Class constructor.
{
  memset ((char*)hw_start, 0, sizeof (hw_start));
  memset ((char*)hw_length, 0, sizeof (hw_length));
  memset ((char*)hw_name, 0, sizeof (hw_name));

  hw_idx = 0;
  hw_count = 0;
}
/*****************************************************************************/

void HwIrsTable::enter (TapeHeader &dsm_hdr)

// Make a table entry. dsm_hdr is passed in with the descriptor selected for
// the channel to be entered.
{
  hw_start[hw_count] = dsm_hdr.start();
  hw_length[hw_count] = dsm_hdr.length();

// Build the name string.
  strncpy (hw_name[hw_count], IRS_STR, 4);
  strcat (hw_name[hw_count], "_");
  strcat (hw_name[hw_count], dsm_hdr.locn());

  hw_count++;
}
/*****************************************************************************/

int HwIrsTable::nextEntry ()
 
// Select the next channel entry.
{
  if ((hw_idx + 1) < hw_count) {
    hw_idx++;
    return TRUE;
  }
  return FALSE;
}
/*****************************************************************************/

void HwIrsTable::displayTable ()

// Display the table contents.
{
  int j;
/**
  for (j = 0; j < hw_count; j++) {
    printf ("index %1d, rxchan %1d, start %4d length %3d, name %s\n",
            j, j, hw_start[j], hw_length[j], hw_name[j]);
  }
**/
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

