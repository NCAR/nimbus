/* HwGpsTable.cc
   Class for managing the Honeywell gps interface sample table.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <HwGpsTable.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
HwGpsTable::HwGpsTable () 
 
// Class constructor.
{
  memset ((char*)hw_gps_start, 0, sizeof (hw_gps_start));
  memset ((char*)hw_gps_length, 0, sizeof (hw_gps_length));
  memset ((char*)hw_gps_name, 0, sizeof (hw_gps_name));

  hw_gps_idx = 0;
  hw_gps_count = 0;
}
/*****************************************************************************/

void HwGpsTable::enter (TapeHeader &dsm_hdr)

// Make a table entry. dsm_hdr is passed in with the descriptor selected for
// the channel to be entered.
{
  hw_gps_start[hw_gps_count] = dsm_hdr.start();
  hw_gps_length[hw_gps_count] = dsm_hdr.length();

// Build the name string.
  strncpy (hw_gps_name[hw_gps_count], HW_GPS_STR, 4);
  strcat (hw_gps_name[hw_gps_count], "_");
  strcat (hw_gps_name[hw_gps_count], dsm_hdr.locn());

  hw_gps_count++;
}
/*****************************************************************************/

int HwGpsTable::nextEntry ()
 
// Select the next channel entry.
{
  if ((hw_gps_idx + 1) < hw_gps_count) {
    hw_gps_idx++;
    return TRUE;
  }
  return FALSE;
}
/*****************************************************************************/

void HwGpsTable::displayTable ()

// Display the table contents.
{
  int j;
/**
  for (j = 0; j < hw_gps_count; j++) {
    printf ("index %1d, rxchan %1d, start %4d length %3d, name %s\n",
            j, j, hw_gps_start[j], hw_gps_length[j], hw_gps_name[j]);
  }
**/
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

