/*
-------------------------------------------------------------------------
OBJECT NAME:	get.c

FULL NAME:	VarDB_Get* functions

ENTRY POINTS:	VarDB_Get*()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		Variable Name

OUTPUT:		Value

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2005
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <sys/types.h>

#include "vardb.h"
#include "portable.h"

static struct var_v2 defaults =
	{
	"", "Unk", "Unk", "No title",
	FLOATING, {0.0, 100.0}, 200.0, 0.0, 0.0, {0.0, 0.0}, 0, 0.0
	};


/* -------------------------------------------------------------------- */
char *VarDB_GetUnits(const char vn[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(defaults.Units);

  return(((struct var_v2 *)VarDB)[indx].Units);

}	/* END VARDB_GETUNITS */

/* -------------------------------------------------------------------- */
char *VarDB_GetAlternateUnits(const char vn[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(defaults.AlternateUnits);

  return(((struct var_v2 *)VarDB)[indx].AlternateUnits);

}	/* END VARDB_GETALTERNATEUNITS */

/* -------------------------------------------------------------------- */
char *VarDB_GetTitle(const char vn[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(defaults.Title);

  return(((struct var_v2 *)VarDB)[indx].Title);

}	/* END VARDB_GETTITLE */

/* -------------------------------------------------------------------- */
int VarDB_isRangeFixed(const char vn[])
{
  int	indx;
  long	type;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(FALSE);

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

  type = ntohl(((struct var_v2 *)VarDB)[indx].type);

  return(type == FLOATING ? TRUE : FALSE);

}	/* END VARDB_ISRANGEFLOATING */

/* -------------------------------------------------------------------- */
float VarDB_GetFixedRangeLower(const char vn[])
{
  int	indx;
  ulong	p;
  float	*rc = (float *)&p;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(defaults.FixedRange[0]);

  return(ntohf(((struct var_v2 *)VarDB)[indx].FixedRange[0]));

}	/* END VARDB_GETFIXEDRANGELOWER */

/* -------------------------------------------------------------------- */
float VarDB_GetFixedRangeUpper(const char vn[])
{
  int	indx;
  ulong	p;
  float	*rc = (float *)&p;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(defaults.FixedRange[1]);

  return(ntohf(((struct var_v2 *)VarDB)[indx].FixedRange[1]));

}	/* END VARDB_GETFIXEDRANGEUPPER */

/* -------------------------------------------------------------------- */
float VarDB_GetFloatRange(const char vn[])
{
  int	indx;
  ulong	p;
  float	*rc = (float *)&p;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(defaults.FloatRange);

  return(ntohf(((struct var_v2 *)VarDB)[indx].FloatRange));

}	/* END VARDB_GETFLOATRANGE */

/* -------------------------------------------------------------------- */
float VarDB_GetMinLimit(const char vn[])
{
  int	indx;
  ulong	p;
  float	*rc = (float *)&p;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(defaults.MinLimit);

  return(ntohf(((struct var_v2 *)VarDB)[indx].MinLimit));

}	/* END VARDB_GETMINLIMIT */

/* -------------------------------------------------------------------- */
float VarDB_GetMaxLimit(const char vn[])
{
  int	indx;
  ulong	p;
  float	*rc = (float *)&p;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(defaults.MaxLimit);

  return(ntohf(((struct var_v2 *)VarDB)[indx].MaxLimit));

}	/* END VARDB_GETMINLIMIT */

/* -------------------------------------------------------------------- */
float VarDB_GetCalRangeLower(const char vn[])
{
  int	indx;
  ulong	p;
  float	*rc = (float *)&p;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(defaults.CalRange[0]);

  return(ntohf(((struct var_v2 *)VarDB)[indx].CalRange[0]));

}	/* END VARDB_GETCALRANGELOWER */

/* -------------------------------------------------------------------- */
float VarDB_GetCalRangeUpper(const char vn[])
{
  int	indx;
  ulong	p;
  float	*rc = (float *)&p;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(defaults.CalRange[1]);

  return(ntohf(((struct var_v2 *)VarDB)[indx].CalRange[1]));

}	/* END VARDB_GETCALRANGEUPPER */

/* -------------------------------------------------------------------- */
int VarDB_GetCategory(const char vn[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(defaults.Category);

  return(ntohl(((struct var_v2 *)VarDB)[indx].Category));

}	/* END VARDB_GETCATEGORY */

/* END GET.C */
