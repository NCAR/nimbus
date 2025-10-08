/*
-------------------------------------------------------------------------
OBJECT NAME:	gettype.c

DESCRIPTION:	hdr_api routines to return Sample Digital Interface (SDI)
		variable information. is analog,
		digital, or counter.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "hdr_api.h"

#define VALID_VARS	SH

/* --------------------------------------------------------------------
 * Return whether analog, difital, or counter.
 */
int GetType(const char vn[], char **result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ((Sh *)HAPI_var[indx].ptr)->type;

  return(OK);

}	/* END GETTYPE */

/* -------------------------------------------------------------------- */
int GetOrder(const char vn[], int32_t *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Sh *)HAPI_var[indx].ptr)->order);

  return(OK);

}	/* END GETORDER */

/* --------------------------------------------------------------------
 * retrieve analog interleaving factor.
 */
int GetSampleOffset(const char vn[], int32_t *result)
{
  int	indx;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  *result = ntohl(((Sh *)HAPI_var[indx].ptr)->offset);

  return(OK);

}	/* END GETSAMPLEOFFSET */

/* --------------------------------------------------------------------
 * routines to get analog channel gains and offsets (voltage range).
 */
int GetChannelOffset(const char vn[], int32_t *result)
{
  int	indx;

  if (HAPI_version < 3.0)
    {
    taperr = BADVER;
    return(ERR);
    }

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  taperr = 0;

  *result = ntohl(((Sh *)HAPI_var[indx].ptr)->ch_offset);

  return(OK);

}	/* END GETCHANNELOFFSET */

/* -------------------------------------------------------------------- */
int GetChannelGain(const char vn[], int32_t *result)
{
  int	indx;

  if (HAPI_version < 3.0)
    {
    taperr = BADVER;
    return(ERR);
    }

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  taperr = 0;

  *result = ntohl(((Sh *)HAPI_var[indx].ptr)->ch_gain);

  return(OK);

}	/* END GETCHANNELGAIN */

/* END GETGAIN.C */
/*
-------------------------------------------------------------------------
OBJECT NAME:	getxvert.c

ENTRY POINTS:	GetConversionFactor()
		GetConversionOffset()


COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/


/* --------------------------------------------------------------------
 * routines to return analog calibrations (counts to Vdc) as opposed to
 * engineering calibrations (Vdc to engineering).
 */
int GetConversionFactor(const char vn[], float *result)
{
  int	indx;
  int32_t	value;
  float	*p = (float *)&value;

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  value = ntohl( *(int *)&(((Sh *)HAPI_var[indx].ptr)->convert) );
  *result = *p;

  return(OK);

}	/* END GETCONVERSIONFACTOR */

/* -------------------------------------------------------------------- */
int GetConversionOffset(const char vn[], int32_t *result)
{
  int	indx;

  if (HAPI_version < 3.09)
    {
    taperr = BADVER;
    return(ERR);
    }

  if ((indx = HAPI_lookupvar(vn, VALID_VARS)) == ERR)
    return(ERR);

  taperr = 0;

  *result = ntohl(((Sh *)HAPI_var[indx].ptr)->convert_offset);

  return(OK);

}	/* END GETCONVERSIONOFFSET */

/* END GETSDI.C */
