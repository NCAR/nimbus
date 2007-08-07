/*
-------------------------------------------------------------------------
OBJECT NAME:	rhour.c

FULL NAME:	Absolute humidity (RAF fast) (g/m3)

ENTRY POINTS:	srhour()

STATIC FNS:	none

DESCRIPTION:  

COPYRIGHT:	University Corporation for Atmospheric Research, 2007
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

/*  Values from /home/local/proj/Defaults on 23 April 1998           RLR */
static NR_TYPE cx_1[3] = { 0.1335, 16.272, -10.216 };
static NR_TYPE  *CX_1;

/* -------------------------------------------------------------------- */
void rhourInit(var_base *varp)
{
  NR_TYPE	*tmp;

  if ((tmp = GetDefaultsValue("RHOUR_CAL", varp->name)) == NULL)
  {
    CX_1 = cx_1;
    sprintf(buffer, "Values returned = %f, %f, %f in AMLIB function RHOUR_CAL.\n", CX_1[0], CX_1[1], CX_1[2]);
    LogMessage(buffer);
  }
  else
    CX_1 = tmp;
}

/* -------------------------------------------------------------------- */
void srhour(DERTBL *varp)
{
  NR_TYPE xuvi  = GetSample(varp, 0);

  PutSample(varp, CX_1[0] + CX_1[1] * exp(CX_1[2] * xuvi));
}
