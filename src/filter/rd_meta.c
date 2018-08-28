/*
-------------------------------------------------------------------------
OBJECT NAME:	rd_meta.c

FULL NAME:	Read meta-data file.

ENTRY POINTS:	ReadMetaData()

DESCRIPTION:	Read the Meta Data file and apply the meta data to the
		netCDF file.

INPUT:		${PROJ_DIR}/###/MiscMetaData

OUTPUT:		RAWTBL static lags updated

REFERENCES:	SearchTable(), AccessProjectFile()

REFERENCED BY:	netcdf.c

COPYRIGHT:	University Corporation for Atmospheric Research, 2006
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include <netcdf.h>


/* -------------------------------------------------------------------- */
void ReadMetaData(int ncid)
{
  int	index;
  char	*meta[512], var[64], attr_name[128], attr_value[128];

  ReadTextFile(META_DATA, meta);

  for (int i = 0; meta[i]; ++i)
  {
    sscanf(meta[i], "%s %s %s", var, attr_name, attr_value);

    if ((index = SearchTable(raw, var)) != ERR)
      ncattput(	ncid, raw[index]->varid, attr_name,
		NC_CHAR, strlen(attr_value)+1, attr_value);

    if ((index = SearchTable(derived, var)) != ERR)
      ncattput(	ncid, derived[index]->varid, attr_name,
		NC_CHAR, strlen(attr_value)+1, attr_value);
  }

  FreeTextFile(meta);

}	/* END READMETADATA */

/* END RD_META.C */
