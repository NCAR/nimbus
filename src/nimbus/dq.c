/*
-------------------------------------------------------------------------
OBJECT NAME:	dq.c

FULL NAME:	Data Quality

ENTRY POINTS:	SearchDataQuality()

STATIC FNS:	none

DESCRIPTION:	

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 2000-05
-------------------------------------------------------------------------
*/

#include "nimbus.h"


const char *dataQuality[] = { "Preliminary", "Good", "Bad", "Synthetic", NULL };


/* -------------------------------------------------------------------- */
const char *SearchDataQuality(const char target[])
{
  int	i;

  for (i = 0; dataQuality[i]; ++i)
    if (strcmp(target, dataQuality[i]) == 0)
      return(dataQuality[i]);

  return(dataQuality[0]);

}

/* END DQ.C */
