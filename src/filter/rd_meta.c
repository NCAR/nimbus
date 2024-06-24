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
      nc_put_att_text(	ncid, raw[index]->varid, attr_name,
		strlen(attr_value), attr_value);

    if ((index = SearchTable(derived, var)) != ERR)
      nc_put_att_text(	ncid, derived[index]->varid, attr_name,
		strlen(attr_value), attr_value);
  }

  FreeTextFile(meta);

}	/* END READMETADATA */



/* -------------------------------------------------------------------- */
// These functions are not related to the above function....other than they deal with meatdata.

void  AddToMetadata(var_base *varp, const char attr_name[], const char attr[])
{
  std::string key(attr_name), value(attr);
  varp->metadata.push_back(Metadata(key, value));
}


void  AddToMetadata(var_base *varp, const char attr_name[], std::vector<float> values)
{
  std::string key(attr_name);
  varp->metadata.push_back(Metadata(key, values));
}

/* END RD_META.C */
