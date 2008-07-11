/* Pms2IOTable.cc
   Class for managing the Pms 2d sample table.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <Pms2IOTable.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
Pms2IOTable::Pms2IOTable () 
 
// Class constructor.
{
  memset ((char*)dev_name, 0, sizeof (dev_name));
  index = 0;
  count = 0;
}
/*****************************************************************************/

void Pms2IOTable::enter (TapeHeader &dsm_hdr)

// Make a table entry. dsm_hdr is passed in with the descriptor selected for
// the channel to be entered.
{
  strcpy (dev_name[count++], dsm_hdr.name());
}
/*****************************************************************************/

int Pms2IOTable::nextEntry ()
 
// Select the next probe entry.
{
  if ((index + 1) < count) {
    index++;
    return TRUE;
  }
  return FALSE;
}
/*****************************************************************************/

void Pms2IOTable::displayTable ()

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

