/* PmsVme2dTable.cc
   Class for managing the Pms 2d sample table.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <PmsVme2dTable.h>

/******************************************************************************
** Public Functions
******************************************************************************/
PmsVme2dTable::PmsVme2dTable () 
 
// Class constructor.
{
  memset ((char*)p_name, 0, sizeof (p_name));
  index = 0;
  count = 0;
}

/*****************************************************************************/
void PmsVme2dTable::enter (TapeHeader &dsm_hdr)

// Make a table entry. dsm_hdr is passed in with the descriptor selected for
// the channel to be entered.
{
  intfc[count] = count / PMS2D_NUM_PROBES;
//  chan[count] = count % PMS2D_NUM_PROBES;
  chan[count] = dsm_hdr.pms2_intf_chan();
  res[count] = dsm_hdr.pms2Resolution();
  strcpy (p_name[count], dsm_hdr.name());

  switch (p_name[count][3])
    {
    case 'C':
      p_type[count] = 1;
      break;

    case 'P':
      p_type[count] = 2;
      break;

    case 'H':
      p_type[count] = 3;
      break;
    }

  ++count;
}

/*****************************************************************************/
int PmsVme2dTable::nextEntry ()
 
// Select the next probe entry.
{
  if ((index + 1) < count) {
    index++;
    return TRUE;
  }
  return FALSE;
}

/*****************************************************************************/
void PmsVme2dTable::displayTable ()

// Display the table contents.
{
  int j;
/**
  for (j = 0; j < count; j++) 
    printf ("name %s\n", dev_name[j]);
**/
}
