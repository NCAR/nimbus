/* GreyVmeTable.cc
   Class for managing the grey scale interface sample table.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <GreyVmeTable.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
GreyVmeTable::GreyVmeTable () 
 
// Class constructor.
{
  memset ((char*)p_name, 0, sizeof (p_name));
  index = 0;
  count = 0;
}
/*****************************************************************************/

void GreyVmeTable::enter (TapeHeader &dsm_hdr)

// Make a table entry. dsm_hdr is passed in with the descriptor selected for
// the probe to be entered.
{
  intfc[count] = count / GREY_NUM_PROBES;
  chan[count] = count % GREY_NUM_PROBES;

// Probe names are G1, G2, ..., in the order that they are entered.
  sprintf (p_name[count], "G%1d", count + 1);
  res[count] = dsm_hdr.pms2Resolution();

// If this is a GA1 probe, enable charge sampling by default.
  if (*(dsm_hdr.pms2SerialNumber() + 4) == '1') {
    p_type[count] = GA1;
    charge_spc[count] = GREY_DEFAULT_CHARGE_SPACING;
    charge_locn[count] = GREY_DEFAULT_CHARGE_LOCATION;
  }
  else {
    p_type[count] = GA2;
    charge_spc[count] = 0;
    charge_locn[count] = 0;
  }
  count++;
}
/*****************************************************************************/

int GreyVmeTable::nextEntry ()
 
// Select the next probe entry.
{
  if ((index + 1) < count) {
    index++;
    return TRUE;
  }
  return FALSE;
}
/*****************************************************************************/
 
int GreyVmeTable::selectByName (char *pname)
 
// Selects a config entry by the matching location.
{
  int idx;
 
  for (idx = 0; (idx < count) && strcmp (p_name[idx], pname); idx++);
 
  if (idx < count) {
    index = idx;
    return TRUE;
  }
  return FALSE;
}
/*****************************************************************************/

void GreyVmeTable::displayTable ()

// Display the table contents.
{
  int j;
  char tstr[4];

//  printf ("Grey Scale Probes:\n");
/*  printf (
    "Interface  Channel  Name  Type  Resolution  Chg_Spacing Chg_Locn\n");
  printf (
    "________________________________________________________________\n");
*/
  for (j = 0; j < count; j++) {
    if (p_type[j] == GA1)
      strcpy (tstr, "GA1");
    else
      strcpy (tstr, "GA2");

//    printf ("%5d %10d     %2s    %3s %8d %12d %10d\n", intfc[j], chan[j], 
//            p_name[j], tstr, res[j], charge_spc[j], charge_locn[j]);
  }
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

