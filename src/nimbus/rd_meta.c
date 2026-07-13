/*
-------------------------------------------------------------------------
OBJECT NAME:	rd_meta.c

FULL NAME:	Read meta-data file.

ENTRY POINTS:	ReadMetadata()

DESCRIPTION:	Read the Metadata file and apply the meta data to the
		netCDF file.

INPUT:		${PROJ_DIR}/###/MiscMetaData

OUTPUT:		RAWTBL static lags updated

REFERENCES:	SearchTable(), AccessProjectFile()

REFERENCED BY:	NetCDF.c

COPYRIGHT:	University Corporation for Atmospheric Research, 2006-2026
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include <netcdf.h>

void processMetadata(int ncid, int varid, char *name, char *value);

/* -------------------------------------------------------------------- */
void ReadMetadata(int ncid)
{
  char	*meta[512], var[64], attr_name[64], attr_value[256];
  char	alt_name[64];

  ReadTextFile(META_DATA, meta);

  for (int i = 0; meta[i]; ++i)
  {
    sscanf(meta[i], "%64s %64s %128[^\n]", var, attr_name, attr_value);

    strcpy(alt_name, var); strcat(alt_name, "_");
    for (int j = 0; j < raw.size(); ++j)
    {
      if (strcmp(raw[j]->name, var) == 0 || strncmp(raw[j]->name, alt_name, strlen(alt_name)) == 0)
        processMetadata(ncid, raw[j]->varid, attr_name, attr_value);
    }

    for (int j = 0; j < derived.size(); ++j)
    {
      if (strcmp(derived[j]->name, var) == 0 || strncmp(derived[j]->name, alt_name, strlen(alt_name)) == 0)
        processMetadata(ncid, derived[j]->varid, attr_name, attr_value);
    }
  }

  FreeTextFile(meta);

}	/* END READMETADATA */

/* -------------------------------------------------------------------- */
void processMetadata(int ncid, int varid, char *attr_name, char *attr_value)
{
  std::vector<int> iv;

  if (strcmp(attr_name, "valid_range") == 0 ||
      strcmp(attr_name, "flag_values") == 0 ||
      strcmp(attr_name, "flag_masks") == 0)
  {
    char temp[128];
    strcpy(temp, attr_value);

    char *p = strtok(temp, ", \t\n");
    while (p)
    {
      if (isdigit(*p))
        iv.push_back(atoi(p));
      else
        iv.push_back((int)*p);
      p = strtok(NULL, ", \t\n");
    }

    // Make sure both don't exist.  This is for IRIG_Status which is different for PC-104SG vs AENT.
    if (strcmp(attr_name, "flag_values") == 0)
      nc_del_att(ncid, varid, "flag_masks");
    if (strcmp(attr_name, "flag_masks") == 0)
      nc_del_att(ncid, varid, "flag_values");

    nc_put_att_int(ncid, varid, attr_name, NC_INT, iv.size(), &iv[0]);
  }
  else
    nc_put_att_text(ncid, varid, attr_name, strlen(attr_value), attr_value);

}


/* -------------------------------------------------------------------- */
// These functions are not related to the above function....other than they deal with metadata.

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


void var_base::addToMetadata(const char attr_name[], float value)
{
  std::string key(attr_name);
  std::vector<float> val = {value};
  metadata.push_back(Metadata(key, val));
}

void var_base::addToMetadata(const char attr_name[], std::vector<float> values)
{
  std::string key(attr_name);
  metadata.push_back(Metadata(key, values));
}


void var_base::addToMetadata(const char attr_name[], int value)
{
  std::string key(attr_name);
  std::vector<int> val = {value};
  metadata.push_back(Metadata(key, val));
}

void var_base::addToMetadata(const char attr_name[], std::vector<int> values)
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
