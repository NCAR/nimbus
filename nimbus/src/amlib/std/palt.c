/*
-------------------------------------------------------------------------
OBJECT NAME:	palt.c

FULL NAME:	NACA pressure altitude (M)

ENTRY POINTS:	spalt()
		spaltf()

STATIC FNS:	none

DESCRIPTION:  

INPUT:    

OUTPUT:    

REFERENCES:	none

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-9
-------------------------------------------------------------------------
*/

#include "raf.h"
#include "nimbus.h"
#include "amlib.h"

/*  Value from /home/local/proj/Defaults on 23 April 1998           RLR */
static NR_TYPE ASTG = 1013.246; /* Default altimeter setting (mbar)     */

/*  Value to be used when this part of code is implemented              */
static NR_TYPE SFCT = 288.15; /* Default mean surface temp setting (K)  */

extern int  Aircraft;

/* -------------------------------------------------------------------- */
void initPalt(DERTBL *varp)
{
  NR_TYPE  *tmp;

  if ((tmp = GetDefaultsValue("ASTG", varp->name)) == NULL)
  {
    sprintf(buffer, "ASTG set to %f in AMLIB function initPalt.\n", ASTG);
    LogMessage(buffer);
  }
  else
    ASTG = tmp[0];

  if ((tmp = GetDefaultsValue("SFCT", varp->name)) == NULL)
  {
    sprintf(buffer, "SFCT set to %f in AMLIB function initPalt.\n", SFCT);
    LogMessage(buffer);
  }
  else
    SFCT = tmp[0];

}  /* END INITPALT */

/* -------------------------------------------------------------------- */
void spalt(DERTBL *varp)
{
  NR_TYPE  psxc, palt;

  psxc = GetSample(varp, 0);

  switch (Aircraft)
    {
    case B57:
      palt = psxc / ASTG;

      if (palt >= 0.223198605)
        palt = 44308.0 * (1.0 - pow(palt, 0.190284));
      else
        palt = 11000.0 + 14600.0 * log10(0.223198605 / palt);
      break;

    default:
      palt = 153.77 * SFCT *
      (1.0 - pow((double)psxc / ASTG, 0.190284));
    }

  PutSample(varp, palt);

}  /* END SPALT */

/* -------------------------------------------------------------------- */
void spaltf(DERTBL *varp)
{
  PutSample(varp, GetSample(varp, 0) * 3.2808399);

}  /* END SPALTF */

/* END PALT.C */
