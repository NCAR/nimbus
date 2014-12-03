/*
-------------------------------------------------------------------------
OBJECT NAME:	set.c

FULL NAME:	Variable DataBase Set functions

ENTRY POINTS:	VarDB_Set*()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		Variable Name, value to set

OUTPUT:		OK/ERR

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2005
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "vardb.h"
#include <raf/portable.h>


/* -------------------------------------------------------------------- */
int VarDB_SetUnits(const char vn[], char units[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  strcpy(((struct var_v2 *)VarDB)[indx].Units, units);
  return(OK);

}	/* END VARDB_SETUNITS */

/* -------------------------------------------------------------------- */
int VarDB_SetAlternateUnits(const char vn[], char units[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  strcpy(((struct var_v2 *)VarDB)[indx].AlternateUnits, units);
  return(OK);

}	/* END VARDB_SETALTERNATEUNITS */

/* -------------------------------------------------------------------- */
int VarDB_SetTitle(const char vn[], char title[])
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  strcpy(((struct var_v2 *)VarDB)[indx].Title, title);
  return(OK);

}	/* END VARDB_SETTITLE */

/* -------------------------------------------------------------------- */
int VarDB_SetIsAnalog(const char vn[], bool analog)
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  ((struct var_v2 *)VarDB)[indx].is_analog = htonl(analog);
  return(OK);

}	/* END VARDB_ISANALOG */

/* -------------------------------------------------------------------- */
int VarDB_SetVoltageRangeLower(const char vn[], int32_t value)
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  ((struct var_v2 *)VarDB)[indx].voltageRange[0] = htonl(value);
  return(OK);

}	/* END VARDB_SETFIXEDRANGELOWER */

/* -------------------------------------------------------------------- */
int VarDB_SetVoltageRangeUpper(const char vn[], int32_t value)
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  ((struct var_v2 *)VarDB)[indx].voltageRange[1] = htonl(value);
  return(OK);

}	/* END VARDB_SETFIXEDRANGEUPPER */

/* -------------------------------------------------------------------- */
int VarDB_SetDefaultSampleRate(const char vn[], int32_t value)
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  ((struct var_v2 *)VarDB)[indx].defaultSampleRate = htonl(value);
  return(OK);

}	/* END VARDB_SETFLOATRANGE */

/* -------------------------------------------------------------------- */
int VarDB_SetMinLimit(const char vn[], float value)
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  ((struct var_v2 *)VarDB)[indx].MinLimit = htonf(value);
  return(OK);

}	/* END VARDB_SETMINLIMIT */

/* -------------------------------------------------------------------- */
int VarDB_SetMaxLimit(const char vn[], float value)
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  ((struct var_v2 *)VarDB)[indx].MaxLimit = htonf(value);
  return(OK);

}	/* END VARDB_SETMINLIMIT */

/* -------------------------------------------------------------------- */
int VarDB_SetCalRangeLower(const char vn[], float value)
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  ((struct var_v2 *)VarDB)[indx].CalRange[0] = htonf(value);
  return(OK);

}	/* END VARDB_SETCALRANGELOWER */

/* -------------------------------------------------------------------- */
int VarDB_SetCalRangeUpper(const char vn[], float value)
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  ((struct var_v2 *)VarDB)[indx].CalRange[1] = htonf(value);
  return(OK);

}	/* END VARDB_SETCALRANGEUPPER */

/* -------------------------------------------------------------------- */
int VarDB_SetCategory(const char vn[], uint32_t value)
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  ((struct var_v2 *)VarDB)[indx].Category = htonl(value);
  return(OK);

}	/* END VARDB_SETCATEGORY */

/* -------------------------------------------------------------------- */
int VarDB_SetStandardName(const char vn[], uint32_t value)
{
  int	indx;

  if ((indx = VarDB_lookup(vn)) == ERR)
    return(ERR);

  ((struct var_v2 *)VarDB)[indx].standard_name = htonl(value);
  return(OK);

}	/* END VARDB_SETSTANDARDNAME */

/* END SET.C */
