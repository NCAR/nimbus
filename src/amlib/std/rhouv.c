/*
-------------------------------------------------------------------------
OBJECT NAME:	rhouv.c

FULL NAME:	Absolute humidity (RAF fast) (g/m3)

ENTRY POINTS:	srhouv()

STATIC FNS:	none

DESCRIPTION:  

COPYRIGHT:	University Corporation for Atmospheric Research, 2007
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static NR_TYPE cx_1[4] = { 16.91, -196.13, 967.85, -1653.8 };
static NR_TYPE  *CX_1;

/* -------------------------------------------------------------------- */
void rhouvInit(var_base *varp)
{
  NR_TYPE	*tmp;

  if ((tmp = GetDefaultsValue("RHOUV_CAL", varp->name)) == NULL)
  {
    CX_1 = cx_1;
    sprintf(buffer, "Values returned = %f, %f, %f in AMLIB function RHOUV_CAL.\n", CX_1[0], CX_1[1], CX_1[2]);
    LogMessage(buffer);
  }
  else
    CX_1 = tmp;
}

/* -------------------------------------------------------------------- */
void srhouv(DERTBL *varp)
{
  NR_TYPE xuvi = GetSample(varp, 0);
  NR_TYPE rhouv;

   rhouv = (CX_1[0] + CX_1[1]*xuvi + CX_1[2]*xuvi*xuvi + CX_1[3]*xuvi*xuvi*xuvi);

   if (rhouv < 0.5)
     rhouv = 0.5;

  PutSample(varp, rhouv);

}
