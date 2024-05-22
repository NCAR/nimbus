/*
-------------------------------------------------------------------------
OBJECT NAME:	sdi.c

ENTRY POINTS:	ssdiFlag()

DESCRIPTION:    SDI flag variable for data quality. one = good, zero = bad	

COPYRIGHT:	University Corporation for Atmospheric Research, 2024
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void ssdiFlag(DERTBL *varp)
{
  NR_TYPE	mflow, setpt, flag;

  mflow = GetSample(varp, 0);
  setpt = GetSample(varp, 1);

  flag = (mflow <= setpt*1.02 && mflow >= setpt*0.94);
  // check if mflow is within -6% to +2% of setpt 
  // limits determined for CAESAR conditions

  PutSample(varp, flag);

}	/* END SDIFLAG*/

/* END SDI.C */
