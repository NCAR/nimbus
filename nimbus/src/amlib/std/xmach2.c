/*
-------------------------------------------------------------------------
OBJECT NAME:	xmach2.c

FULL NAME:	Mach Number

DESCRIPTION:	Versions of nimbus prior to 2013 used XMACH2 or mach number
		squared.  We now produce a mach number for each QC probe and
		have a MACHX as the blessed reference.

		Projects for ADS3 and later have all been retrofitted with
		corrected depend tables.

COPYRIGHT:	University Corporation for Atmospheric Research, 1990-2013
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void sxmach2(DERTBL *varp)
{
  NR_TYPE	qcxc, psxc;

  qcxc = GetSample(varp, 0);
  psxc = GetSample(varp, 1);

  PutSample(varp, XMAC2(qcxc / psxc));
}
