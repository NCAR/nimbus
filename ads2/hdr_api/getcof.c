/*
-------------------------------------------------------------------------
OBJECT NAME:	getcof.c

ENTRY POINTS:	GetCalCoeff()

DESCRIPTION:	

INPUT:		char *var, float **result

OUTPUT:		OK/ERR

REFERENCES:	_lookupvar()

REFERENCED BY:	User Level Applications

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	(SH | PMS1V2 | PMS2DH | MASP1)


/* -------------------------------------------------------------------- */
int GetCalCoeff(const char vn[], float **result)
{
  int	i, j, indx;
  int32_t  value;
  float *p = (float *)&value;
  static float	sh_cofs[MX_COF];
  static float	cof1d[P1DV2_AUX][P1DV2_COF];
  static float	cof2d[P2D_HOUSE][P1DV2_COF];

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);


  switch (HAPI_var[indx].type)
    {
    case SH:
      for (i = 0; i < MX_COF; ++i)
        {
        value = ntohl( *(int *)&(((Sh *)HAPI_var[indx].ptr)->cof[i]) );
        sh_cofs[i] = *p;
        }

      *result = sh_cofs;
      break;

    case PMS2DH:
      for (i = 0; i < P2D_HOUSE; ++i)
        for (j = 0; j < P1DV2_COF; ++j)
          {
          value = ntohl(*(int *)&(((Pms2h *)HAPI_var[indx].ptr)->hsk_cof[i][j]));
          cof2d[i][j] = *p;
          }

      *result = (float *)cof2d;
      break;

    case MASP1:
      for (i = 0; i < P1DV2_AUX; ++i)
        for (j = 0; j < P1DV2_COF; ++j)
          {
          value = ntohl(*(int *)&(((Masp *)HAPI_var[indx].ptr)->hsk_cof[i][j]));
          cof1d[i][j] = *p;
          }

      *result = (float *)cof1d;
      break;

    case PMS1V2:
      if (HAPI_version >= 2.09)
        {
        for (i = 0; i < P1DV2_AUX; ++i)
          for (j = 0; j < P1DV2_COF; ++j)
            {
            value = ntohl(*(int *)&(((Pms1v2 *)HAPI_var[indx].ptr)->hsk_cof[i][j]));
            cof1d[i][j] = *p;
            }

        *result = (float *)cof1d;
        break;
        }
      else
        {
        taperr = BADVER;
        return(ERR);
        }

    default:
      taperr = BADTYPE;
      return(ERR);
    }

  return(OK);

}	/* END GETCALCOEFF */

/* END GETCOF.C */
