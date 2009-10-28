/*
-------------------------------------------------------------------------
OBJECT NAME:	get.c

FULL NAME:	VarDB_Get* functions

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2006
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <sys/types.h>

#include "vardb.h"
#include <netcdf.h>
#include <raf/portable.h>

static struct var_v2 defaults =
	{
	"", "Unk", "Unk", "No title",
	FLOATING, {0.0, 100.0}, 200.0, 0.0, 0.0, {0.0, 0.0}, 0, 0, 0
	};

static const float default_FillValue = -32767.0;


extern char VarDB_NcML_text_result[];


/* -------------------------------------------------------------------- */
const char *VarDB_GetUnits(const char vn[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(defaults.Units);

  if (VarDB_NcML > 0)
  {
    if (nc_get_att_text(VarDB_NcML, indx, "units", VarDB_NcML_text_result) == NC_NOERR)
      return VarDB_NcML_text_result;
    else
      return 0;
  }

  return(((struct var_v2 *)VarDB)[indx].Units);

}	/* END VARDB_GETUNITS */

/* -------------------------------------------------------------------- */
const char *VarDB_GetAlternateUnits(const char vn[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(defaults.AlternateUnits);

  if (VarDB_NcML > 0)
    return 0;

  return(((struct var_v2 *)VarDB)[indx].AlternateUnits);

}	/* END VARDB_GETALTERNATEUNITS */

/* -------------------------------------------------------------------- */
const char *VarDB_GetTitle(const char vn[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(defaults.Title);

  if (VarDB_NcML > 0)
  {
    if (nc_get_att_text(VarDB_NcML, indx, "long_name", VarDB_NcML_text_result) == NC_NOERR)
      return VarDB_NcML_text_result;
    else
      return 0;
  }

  return(((struct var_v2 *)VarDB)[indx].Title);

}	/* END VARDB_GETTITLE */

/* -------------------------------------------------------------------- */
const char *VarDB_GetDependencies(const char vn[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return 0;

  if (VarDB_NcML > 0)
  {
    if (nc_get_att_text(VarDB_NcML, indx, "Dependencies", VarDB_NcML_text_result) == NC_NOERR)
      return VarDB_NcML_text_result;
  }

  return 0;

}	/* END VARDB_GETDEPENDENCIES */

/* -------------------------------------------------------------------- */
int VarDB_isRangeFixed(const char vn[])
{
  int	indx;
  long	type;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(FALSE);

  if (VarDB_NcML > 0)	// Not really supported under ADS3.
    return FALSE;

  type = ntohl(((struct var_v2 *)VarDB)[indx].type);

  return(type == FIXED ? TRUE : FALSE);

}	/* END VARDB_ISRANGEFIXED */

/* -------------------------------------------------------------------- */
int VarDB_isRangeFloating(const char vn[])
{
  int	indx;
  long	type;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(TRUE);

  if (VarDB_NcML > 0)	// Not really supported under ADS3.
    return FALSE;

  type = ntohl(((struct var_v2 *)VarDB)[indx].type);

  return(type == FLOATING ? TRUE : FALSE);

}	/* END VARDB_ISRANGEFLOATING */

/* -------------------------------------------------------------------- */
float VarDB_GetFixedRangeLower(const char vn[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR || VarDB_NcML > 0)
    return(defaults.FixedRange[0]);

  return(ntohf(((struct var_v2 *)VarDB)[indx].FixedRange[0]));

}	/* END VARDB_GETFIXEDRANGELOWER */

/* -------------------------------------------------------------------- */
float VarDB_GetFixedRangeUpper(const char vn[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR || VarDB_NcML > 0)
    return(defaults.FixedRange[1]);

  return(ntohf(((struct var_v2 *)VarDB)[indx].FixedRange[1]));

}	/* END VARDB_GETFIXEDRANGEUPPER */

/* -------------------------------------------------------------------- */
float VarDB_GetFloatRange(const char vn[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR || VarDB_NcML > 0)
    return(defaults.FloatRange);

  return(ntohf(((struct var_v2 *)VarDB)[indx].FloatRange));

}	/* END VARDB_GETFLOATRANGE */

/* -------------------------------------------------------------------- */
float VarDB_GetFillValue(const char vn[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(default_FillValue);

  if (VarDB_NcML > 0)
  {
    float fv;
    if (nc_get_att_float(VarDB_NcML, indx, "_FillValue", &fv) == NC_NOERR)
      return fv;
    else
      return 0;
  }

  return(default_FillValue);

}	/* END VARDB_GETMINLIMIT */

/* -------------------------------------------------------------------- */
float VarDB_GetMinLimit(const char vn[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(defaults.MinLimit);

  if (VarDB_NcML > 0)
  {
    float range[2];
    if (nc_get_att_float(VarDB_NcML, indx, "valid_range", range) == NC_NOERR)
      return range[0];
    else
      return 0;
  }

  return(ntohf(((struct var_v2 *)VarDB)[indx].MinLimit));

}	/* END VARDB_GETMINLIMIT */

/* -------------------------------------------------------------------- */
float VarDB_GetMaxLimit(const char vn[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(defaults.MaxLimit);

  if (VarDB_NcML > 0)
  {
    float range[2];
    if (nc_get_att_float(VarDB_NcML, indx, "valid_range", range) == NC_NOERR)
      return range[1];
    else
      return 0;
  }

  return(ntohf(((struct var_v2 *)VarDB)[indx].MaxLimit));

}	/* END VARDB_GETMAXLIMIT */

/* -------------------------------------------------------------------- */
float VarDB_GetCalRangeLower(const char vn[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR || VarDB_NcML > 0)
    return(defaults.CalRange[0]);

  return(ntohf(((struct var_v2 *)VarDB)[indx].CalRange[0]));

}	/* END VARDB_GETCALRANGELOWER */

/* -------------------------------------------------------------------- */
float VarDB_GetCalRangeUpper(const char vn[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR || VarDB_NcML > 0)
    return(defaults.CalRange[1]);

  return(ntohf(((struct var_v2 *)VarDB)[indx].CalRange[1]));

}	/* END VARDB_GETCALRANGEUPPER */

/* -------------------------------------------------------------------- */
int VarDB_GetCategory(const char vn[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR || VarDB_NcML > 0)
    return(defaults.Category);

  return(ntohl(((struct var_v2 *)VarDB)[indx].Category));

}	/* END VARDB_GETCATEGORY */

/* -------------------------------------------------------------------- */
int VarDB_GetStandardName(const char vn[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR || VarDB_NcML > 0)
    return(defaults.standard_name);

  return(ntohl(((struct var_v2 *)VarDB)[indx].standard_name));

}	/* END VARDB_GETCATEGORY */

/* END GET.C */
