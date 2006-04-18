/*
-------------------------------------------------------------------------
OBJECT NAME:	rd_lag.c

FULL NAME:	Read lag file.

ENTRY POINTS:	ReadStaticLags()

DESCRIPTION:	

INPUT:		${PROJ_DIR}/###/lags

OUTPUT:		RAWTBL & SDITBL static lags updated

REFERENCES:	SearchTable(), AccessProjectFile()

REFERENCED BY:	hdr_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "bounds.h"


/* -------------------------------------------------------------------- */
void ReadStaticLags()
{
  int	index;
  int	lag;
  char	*lags[512], target[NAMELEN];

  if (AccessProjectFile(LAGS, "r") == FALSE)
    return;

  ReadTextFile(LAGS, lags);

  for (int i = 0; lags[i]; ++i)
    {
    sscanf(lags[i], "%s %d", target, &lag);

    if (abs(lag) > MaxLag)
      {
      sprintf(buffer, "Lag may not exceed %d ms, igonoring lag for %s.\n",
             MaxLag, target);
      LogMessage(buffer);
      continue;
      }

    if ((index = SearchTable(raw, target)) != ERR)
      raw[index]->StaticLag = lag;
    else
      {
      sprintf(buffer, "ReadStaticLags: non-existent variable %s specified, continuing.\n", target);
      LogMessage(buffer);
      }
    }

  FreeTextFile(lags);

}	/* END READSTATICLAGS */

/* END RD_LAG.C */
