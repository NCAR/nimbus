/*
-------------------------------------------------------------------------
OBJECT NAME:	rdma.c

FULL NAME:	

ENTRY POINTS:	sfunc()

STATIC FNS:	none

DESCRIPTION:	Template for AMLIB std & user directories.

COPYRIGHT:	University Corporation for Atmospheric Research, 2002
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE crdma_cnc[];	// From amlib/xlate/rdma.c

/* -------------------------------------------------------------------- */
void scrdma(DERTBL *varp)
{
  int	i;

  for (i = 0; i < 64; ++i)
    AveragedData[varp->LRstart+i] = crdma_cnc[i];
//    AveragedData[varp->LRstart+i] = floatNAN;

}	/* END SCRDMA */

/* END RDMA.C */
