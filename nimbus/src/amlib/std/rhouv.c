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

static NR_TYPE cx_1[4] = { 0.28, -5.9761, -6.0135, 0. };
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

  xuvi = xuvi + CX_1[0];
  if (xuvi <= 0.0)
    xuvi = 0.001;

   rhouv = (CX_1[1] + CX_1[2]*log(xuvi));

   if (rhouv < 0.01)
     rhouv = 0.01;

  PutSample(varp, rhouv);

}
