/* AdcAoaTable.cc
   Class for managing the air data computer aoa interface sample table.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <AdcAoaTable.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
AdcAoaTable::AdcAoaTable () 
 
// Class constructor.
{
  memset ((char*)adc_aoa_start, 0, sizeof (adc_aoa_start));
  memset ((char*)adc_aoa_length, 0, sizeof (adc_aoa_length));
  memset ((char*)adc_aoa_name, 0, sizeof (adc_aoa_name));

  adc_aoa_idx = 0;
  adc_aoa_count = 0;
}
/*****************************************************************************/

void AdcAoaTable::enter (TapeHeader &dsm_hdr)

// Make a table entry. dsm_hdr is passed in with the descriptor selected for
// the channel to be entered.
{
  adc_aoa_start[adc_aoa_count] = dsm_hdr.start();
  adc_aoa_length[adc_aoa_count] = dsm_hdr.length();

// Build the name string.
  strncpy (adc_aoa_name[adc_aoa_count], ADC_AOA_STR, 4);
  strcat (adc_aoa_name[adc_aoa_count], "_");
  strcat (adc_aoa_name[adc_aoa_count], dsm_hdr.locn());

  adc_aoa_count++;
}
/*****************************************************************************/

int AdcAoaTable::nextEntry ()
 
// Select the next channel entry.
{
  if ((adc_aoa_idx + 1) < adc_aoa_count) {
    adc_aoa_idx++;
    return TRUE;
  }
  return FALSE;
}
/*****************************************************************************/

void AdcAoaTable::displayTable ()

// Display the table contents.
{
  int j;

  for (j = 0; j < adc_aoa_count; j++) { 
    printf ("index %1d, rxchan %1d, start %4d length %3d, name %s\n",
            j, j, adc_aoa_start[j], adc_aoa_length[j], adc_aoa_name[j]);
  }
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

