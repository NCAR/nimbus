/* SyncVar.cc

   Sync data variable class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <SyncVar.h>

/******************************************************************************
** Public Functions
******************************************************************************/
SyncVar::SyncVar (TapeHeader *hdr, const char *name)

// Constructor.
{
  int stat;

  for (stat = hdr->firstDesc(); stat; stat = hdr->nextDesc()) {
    if ((hdr->shType() && !strcmp (hdr->name(), name)) ||
        !strcmp(hdr->item_type(), name))
      break;
  }

  if (!stat) {
    fprintf (stderr, "SyncVar: Variable %s not found in the tape header.\n",
             name);
    exit (ERROR);
  }

  strcpy(sv_name, name);
  start = hdr->start();

  if (strcmp(name, DPRES_STR))
    {
    hdr->coefficients(cof);
    convert = hdr->convert();
    convert_offset = hdr->getShConvertOffset();
    offset = hdr->offset();
    order = hdr->order();
    }

}

/*****************************************************************************/
void SyncVar::computeFromSample (short raw)

// Computes the variable value from the raw sample value.
{
  int k;

// Compute the voltage value.
  sv_volts = (float)((short)ntohs(raw) - convert_offset) * convert;

// Compute the engineering units value, using Horner's rule.
  for (k = order - 2, sv_value = cof[order - 1]; k >= 0; k--)
    sv_value = cof[k] + sv_volts * sv_value;

//printf ("convert_offset = %d, convert = %f\n", convert_offset, convert);
//printf ("order = %d, raw = 0x%x, volts = %f, value = %f\n", 
//        order, raw, sv_volts, sv_value);

}
