/* MaspTable.cc
   Class for managing the Masp sample table.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <MaspTable.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
MaspTable::MaspTable () 
 
// Class constructor.
{
  index = 0;
  count = 0;
}
/*****************************************************************************/

void MaspTable::enter (TapeHeader &dsm_hdr)

// Make a table entry. dsm_hdr is passed in with the descriptor selected for
// the channel to be entered.
{
  masp_start = dsm_hdr.start();
  masp_length = dsm_hdr.length();
  ++count;
}
/*****************************************************************************/

int MaspTable::nextEntry ()
 
// Select the next probe entry.
{
  if ((index + 1) < count) {
    index++;
    return TRUE;
  }
  return FALSE;
}
/*****************************************************************************/

void MaspTable::displayTable ()

// Display the table contents.
{
  int j;
/**
  for (j = 0; j < count; j++) 
    printf ("name %s\n", dev_name[j]);
**/
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

