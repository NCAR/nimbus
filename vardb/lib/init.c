/*
-------------------------------------------------------------------------
OBJECT NAME:	init.c

FULL NAME:	Initialize

ENTRY POINTS:	InitializeVarDB()
		ReleaseVarDB()
		VarDB_lookup()

STATIC FNS:	readFile()

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2006
-------------------------------------------------------------------------
*/

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/param.h>
#include <netinet/in.h>	// htonl macros.

#include "vardb.h"
#include "netcdf.h"
#include "portable.h"

const long	VarDB_MagicCookie	= 0x42756c6c;
const long	VarDB_CurrentVersion	= 3;

struct vardb_hdr	master_VarDB_Hdr;
void			*master_VarDB = NULL;

struct vardb_hdr	VarDB_Hdr;
void			*VarDB = NULL;

int	VarDB_NcML = -1;
char	VarDB_NcML_text_result[512];

long	VarDB_RecLength, VarDB_nRecords;

static void *readFile(const char fileName[], struct vardb_hdr *vdbHdr);


/* -------------------------------------------------------------------- */
int InitializeVarDB(const char fileName[])
{
  char	*p, masterFileName[MAXPATHLEN];


  /* Try to open new NcML version first.  If it opens we are done.
   */
  strcpy(masterFileName, fileName);
  strcat(masterFileName, ".nc");
  if (nc_open(masterFileName, 0, &VarDB_NcML) != NC_NOERR)
  {
    fprintf(stderr, "VarDB: NcML version not found, reverting to standard VarDB.\n");
    VarDB_NcML = -1;
  }

  /* Try to open the master VarDB first, then we'll overlay the user
   * requested file.
   */
  p = getenv("PROJ_DIR");
  if (p)
  {
    sprintf(masterFileName, "%s/Configuration/raf/VarDB", p);
// We are not ready for a master file.  lookup returns a master, but then
// the index always goes into the regular not master VarDB list.  Needs work.
//    master_VarDB = readFile(masterFileName, &master_VarDB_Hdr);
  }


  SetCategoryFileName(masterFileName);
  SetStandardNameFileName(masterFileName);

  if (VarDB_NcML < 0)
  {
    VarDB = readFile(fileName, &VarDB_Hdr);

    if (VarDB == 0 && master_VarDB == 0)
      return(ERR);
  }

  return(OK);

}	/* INITIALIZEVARDB */

/* -------------------------------------------------------------------- */
static void *readFile(const char fileName[], struct vardb_hdr *vdbHdr)
{
  FILE	*fp;
  int	rc;
  void	*varDB;

  VarDB_nRecords = 0;
  VarDB_RecLength = 0;

  if ((fp = fopen(fileName, "rb")) == NULL)
  {
    fprintf(stderr, "VarDB: can't open %s.\n", fileName);
    return(0);
  }

  fread((char *)vdbHdr, sizeof(struct vardb_hdr), 1, fp);

  if (ntohl(vdbHdr->MagicCookie) != VarDB_MagicCookie)
  {
    fprintf(stderr, "VarDB: bad VarDB file %s.\n", fileName);
    fclose(fp);
    return(0);
  }

  if (ntohl(vdbHdr->Version) > VarDB_CurrentVersion)
  {
    fprintf(stderr,"VarDB: File has newer version than code can handle.\n");
    fclose(fp);
    return(0);
  }

  VarDB_nRecords = ntohl(vdbHdr->nRecords);
  VarDB_RecLength = ntohl(vdbHdr->RecordLen);

  if ((varDB = (void *)malloc((unsigned)VarDB_nRecords * VarDB_RecLength)) == NULL)
  {
    fprintf(stderr, "VarDB: out of memory.\n");
    fclose(fp);
    return(0);
  }

  if ((rc = fread((char *)varDB, VarDB_RecLength, VarDB_nRecords, fp)) != VarDB_nRecords)
  {
    fprintf(stderr, "VarDB: read %d out of %d entries\n", rc, VarDB_nRecords);
    fclose(fp);
    return(0);
  }

  fclose(fp);

  if (ntohl(vdbHdr->Version) < VarDB_CurrentVersion)
  {
    int		i;

    fprintf(stderr, "VarDB: File has older version, converting to new version.\n");

    for (i = 0; i < VarDB_nRecords; ++i)
    {
      ((struct var_v2 *)varDB)[i].standard_name = 0;
      ((struct var_v2 *)varDB)[i].reference = FALSE;;
    }

    vdbHdr->Version = htonl(VarDB_CurrentVersion);
    vdbHdr->RecordLen = htonl(sizeof(struct var_v2));

    VarDB = varDB;
    SaveVarDB(fileName);
  }

  return(varDB);

}	/* END READFILE */

/* -------------------------------------------------------------------- */
bool VarDB_isNcML()
{
  if (VarDB_NcML == -1)
    return false;	// Old style vardb, binary file.
  else
    return true;	// New, netCDF/NcML version.
}

/* -------------------------------------------------------------------- */
void ReleaseVarDB()
{
  if (VarDB)
  {
    free(VarDB);
    free(master_VarDB);
    VarDB = NULL;
    master_VarDB = NULL;
  }
}	/* END RELEASEVARDB */

/* -------------------------------------------------------------------- */
int VarDB_lookup(const char vn[])
{
  int	i, masterNrecords;
  char	tname[64];

  // Strip off suffix for lookup.
  for (i = 0; vn[i] && vn[i] != '_'; ++i)
    tname[i] = vn[i];

  tname[i] = '\0';

  if (VarDB_NcML > 0)
  {
    int varID;
    if (nc_inq_varid(VarDB_NcML, tname, &varID) == NC_NOERR)
      return(varID);
    else
      return(ERR);
  }

  for (i = 0; i < VarDB_nRecords; ++i)
    if (strcmp(((struct var_v2 *)VarDB)[i].Name, tname) == 0)
      return(i);
/*
  masterNrecords = ntohl(master_VarDB_Hdr.nRecords);

  for (i = 0; i < masterNrecords; ++i)
    if (strcmp(((struct var_v2 *)master_VarDB)[i].Name, tname) == 0)
      return(i);
*/
  return(ERR);

}	/* END VARDB_LOOKUP */

/* END INIT.C */
