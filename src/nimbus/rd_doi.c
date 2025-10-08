/*
-------------------------------------------------------------------------
OBJECT NAME:	rd_doi.c

FULL NAME:	Read DOI file.

ENTRY POINTS:	ReadMetaData()

DESCRIPTION:	Read the DOI file and apply as attribute in the netCDF file
		netCDF file.

COPYRIGHT:	University Corporation for Atmospheric Research, 2024
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include <netcdf.h>

static const char *naming_authority = "org.doi";
static const char *doi_hostname = "https://doi.org";


/* -------------------------------------------------------------------- */
void ReadDOI(int ncid)
{
  char	*dois[32], dataSet[64], doi[64], doi_value[64], attr_value[128], *p;

  if ( !cfg.ProductionRun() )
    return;


  ReadTextFile(DOI_FILE, dois);
  strcpy(attr_value, naming_authority);

  for (int i = 0; dois[i]; ++i)
  {
    if ((p = strchr(dois[i], '=')) == 0)
      continue;

    for (int j = 0; j < 3; ++j)
      dois[i][j] = tolower(dois[i][j]);

    for (++p; *p == ' ' || *p == '\t'; ++p)
      ;

    if (cfg.ProcessingRate() == Config::LowRate && strncmp(dois[i], "lrt", 3) == 0)
      strcpy(doi_value, p);

    if (cfg.ProcessingRate() == Config::SampleRate && strncmp(dois[i], "srt", 3) == 0)
      strcpy(doi_value, p);

    if (cfg.ProcessingRate() == Config::HighRate && strncmp(dois[i], "hrt", 3) == 0)
      strcpy(doi_value, p);
  }

  snprintf(attr_value, 125, "%s/%s", doi_hostname, doi_value);
  nc_put_att_text(ncid, NC_GLOBAL, "DOI", strlen(attr_value), attr_value);

  // The DataDiscovery way.... https://commons.esipfed.org/acdd_1-3_references
  nc_put_att_text(ncid, NC_GLOBAL, "naming_authority", strlen(naming_authority), naming_authority);
  nc_put_att_text(ncid, NC_GLOBAL, "id", strlen(doi_value), doi_value);

  FreeTextFile(dois);

}	/* END READDOI */

/* END RD_DOI.C */
