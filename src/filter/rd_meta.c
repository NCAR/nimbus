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

COPYRIGHT:	University Corporation for Atmospheric Research, 2006-2024
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

void  var_base::addToMetadata(const char attr_name[], const char attr[])
{
  std::string value(attr);
  addToMetadata(attr_name, value);
}

void  var_base::addToMetadata(const char attr_name[], std::string attr)
{
  std::string key(attr_name);
  metadata.push_back(Metadata(key, attr));
}

void var_base::addToMetadata(const char attr_name[], std::vector<float> values)
{
  std::string key(attr_name);
  metadata.push_back(Metadata(key, values));
}


// Search functions
std::string var_base::Units() const
{
  std::string rc;

  for (std::vector<Metadata>::const_iterator it = metadata.begin(); it != metadata.end(); ++it)
    if (it->_attr_name.compare("units") == 0)
      rc = it->_attr_str;

  return rc;
}

std::string var_base::LongName() const
{
  std::string rc;

  for (std::vector<Metadata>::const_iterator it = metadata.begin(); it != metadata.end(); ++it)
    if (it->_attr_name.compare("long_name") == 0)
      rc = it->_attr_str;

  return rc;
}

std::string var_base::AltUnits() const
{
  std::string rc;
/*
  for (std::vector<Metadata>::const_iterator it = metadata.begin(); it != metadata.end(); ++it)
    if (it->_attr_name.compare("units") == 0)
      rc = it->_attr_str;
*/
  return rc;
}

/* END RD_META.C */
