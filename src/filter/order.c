/*
-------------------------------------------------------------------------
OBJECT NAME:	order.c

FULL NAME:	Create Compute Order

ENTRY POINTS:	GenerateComputeOrder()

STATIC FNS:	none

DESCRIPTION:	Sort ComputeOrder[] into compute order based on dependencies.

INPUT:		none

OUTPUT:		sorted *ComputeOrder[]

REFERENCES:	bsearch.c

REFERENCED BY:	cb_main.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"


/* -------------------------------------------------------------------- */
void GenerateComputeOrder()
{
  int		i, j;
  int		index;
  bool		found;
  DERTBL	*ihold;

{
  /* Copy over the list of Derived variables, placing the
   * ones with 0 dependencies at the beginning and the rest
   * at the end.  This will speed up processing.
   */
  int	bpos = 0,
  epos = nderive-1;

  for (i = 0; i < nderive; ++i)
    if (derived[i]->ndep == 0)
      ComputeOrder[bpos++] = derived[i];
    else
      ComputeOrder[epos--] = derived[i];

  i = bpos;
}

  for (; i < nderive; )
    {
    found = true;

    for (j = 0; j < ComputeOrder[i]->ndep; j++)
      {
      if (SearchTable((char **)sdi, nsdi, ComputeOrder[i]->depend[j]) == ERR
          && SearchTable((char **)raw, nraw, ComputeOrder[i]->depend[j]) == ERR
          && LinearSearchTable((char **)ComputeOrder, i, ComputeOrder[i]->depend[j]) == ERR)
        {
        found = false;
        break;
        }
      }


    if (found)
      ++i;
    else
      {
      /* Start the search after the current position and
       * compensate by adding i+1
       */
      index = LinearSearchTable((char **)&ComputeOrder[i+1],
		nderive - i-1, ComputeOrder[i]->depend[j]) + i+1;

      ihold = ComputeOrder[i];
      ComputeOrder[i] = ComputeOrder[index];
      ComputeOrder[index] = ihold;
      }
    }

}	/* END GENERATECOMPUTEORDER */

/* END ORDER.C */
