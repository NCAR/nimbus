/*
-------------------------------------------------------------------------
OBJECT NAME:	set.c

FULL NAME:	Variable DataBase Set functions

ENTRY POINTS:	VarDB_Set*()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		Variable Name, value to set

OUTPUT:		OK/ERR

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h> // htonl macros.

#include "vardb.h"

extern struct vardb_hdr	VarDB_Hdr;
extern void		*VarDB;

extern long	VarDB_nRecords, VarDB_RecLength;

/* -------------------------------------------------------------------- */
VarDB_SetUnits(char vn[], char units[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  strcpy(((struct var_v2 *)VarDB)[indx].Units, units);
  return(OK);

}	/* END VARDB_SETUNITS */

/* -------------------------------------------------------------------- */
VarDB_SetAlternateUnits(char vn[], char units[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  strcpy(((struct var_v2 *)VarDB)[indx].AlternateUnits, units);
  return(OK);

}	/* END VARDB_SETALTERNATEUNITS */

/* -------------------------------------------------------------------- */
VarDB_SetTitle(char vn[], char title[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  strcpy(((struct var_v2 *)VarDB)[indx].Title, title);
  return(OK);

}	/* END VARDB_SETTITLE */

/* -------------------------------------------------------------------- */
VarDB_SetRangeFixed(char vn[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  ((struct var_v2 *)VarDB)[indx].type = htonl(FIXED);
  return(OK);

}	/* END VARDB_ISRANGEFIXED */

/* -------------------------------------------------------------------- */
VarDB_SetRangeFloating(char vn[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  ((struct var_v2 *)VarDB)[indx].type = htonl(FLOATING);
  return(OK);

}	/* END VARDB_ISRANGEFIXED */

/* -------------------------------------------------------------------- */
VarDB_SetFixedRangeLower(char vn[], float value)
{
  int	indx;
  ulong	*p = (ulong *)&value;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  *((int *)&(((struct var_v2 *)VarDB)[indx].FixedRange[0])) = htonl(*p);
  return(OK);

}	/* END VARDB_SETFIXEDRANGELOWER */

/* -------------------------------------------------------------------- */
VarDB_SetFixedRangeUpper(char vn[], float value)
{
  int	indx;
  ulong	*p = (ulong *)&value;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  *((int *)&(((struct var_v2 *)VarDB)[indx].FixedRange[1])) = htonl(*p);
  return(OK);

}	/* END VARDB_SETFIXEDRANGEUPPER */

/* -------------------------------------------------------------------- */
VarDB_SetFloatRange(char vn[], float value)
{
  int	indx;
  ulong	*p = (ulong *)&value;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  *((int *)&(((struct var_v2 *)VarDB)[indx].FloatRange)) = htonl(*p);
  return(OK);

}	/* END VARDB_SETFLOATRANGE */

/* -------------------------------------------------------------------- */
VarDB_SetMinLimit(char vn[], float value)
{
  int	indx;
  ulong	*p = (ulong *)&value;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  *((int *)&(((struct var_v2 *)VarDB)[indx].MinLimit)) = htonl(*p);
  return(OK);

}	/* END VARDB_SETMINLIMIT */

/* -------------------------------------------------------------------- */
VarDB_SetMaxLimit(char vn[], float value)
{
  int	indx;
  ulong	*p = (ulong *)&value;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  *((int *)&(((struct var_v2 *)VarDB)[indx].MinLimit)) = htonl(*p);
  return(OK);

}	/* END VARDB_SETMINLIMIT */

/* -------------------------------------------------------------------- */
VarDB_SetCalRangeLower(char vn[], float value)
{
  int	indx;
  ulong	*p = (ulong *)&value;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  *((int *)&(((struct var_v2 *)VarDB)[indx].CalRange[0])) = htonl(*p);
  return(OK);

}	/* END VARDB_SETCALRANGELOWER */

/* -------------------------------------------------------------------- */
VarDB_SetCalRangeUpper(char vn[], float value)
{
  int	indx;
  ulong	*p = (ulong *)&value;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  *((int *)&(((struct var_v2 *)VarDB)[indx].CalRange[1])) = htonl(*p);
  return(OK);

}	/* END VARDB_SETCALRANGEUPPER */

/* -------------------------------------------------------------------- */
VarDB_SetCategory(char vn[], int value)
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  ((struct var_v2 *)VarDB)[indx].Category = htonl(value);
  return(OK);

}	/* END VARDB_SETCATEGORY */

/* END SET.C */
