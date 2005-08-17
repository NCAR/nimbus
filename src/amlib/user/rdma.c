/*
-------------------------------------------------------------------------
OBJECT NAME:	rdma.c

FULL NAME:	

ENTRY POINTS:	sfunc()

STATIC FNS:	

DESCRIPTION:	Template for AMLIB std & user directories.

INPUT:		

OUTPUT:		

REFERENCES:	

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 2002
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

extern float crdma_cnc[];	// From amlib/xlate/rdma.c

/* -------------------------------------------------------------------- */
void scrdma(DERTBL *varp)
{
  int	i;

  for (i = 0; i < 64; ++i)
    AveragedData[varp->LRstart+i] = crdma_cnc[i];
//    AveragedData[varp->LRstart+i] = floatNAN;

}	/* END SCRDMA */

/* END RDMA.C */
