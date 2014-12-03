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
#include <netcdf.h>
#include <raf/portable.h>

const long	VarDB_MagicCookie	= 0x42756c6c;
const long	VarDB_CurrentVersion	= 3;

VarDB_Hdr	master_VarDB_Hdr;
void		*master_VarDB = NULL;

VarDB_Hdr	varDB_Hdr;
void		*VarDB = NULL;

int	VarDB_NcML = -1;
char	VarDB_NcML_text_result[512];

long	VarDB_RecLength, VarDB_nRecords;

static void *readFile(const char fileName[], VarDB_Hdr *vdbHdr);
static bool checkIfOlderVersionAndConvert(void *varDB, VarDB_Hdr *vdbHdr);
static bool performCorrections(void *varDB);


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
    VarDB = readFile(fileName, &varDB_Hdr);

    if (VarDB == 0 && master_VarDB == 0)
      return(ERR);
  }

  return(OK);

}	/* INITIALIZEVARDB */

/* -------------------------------------------------------------------- */
static void *readFile(const char fileName[], VarDB_Hdr *vdbHdr)
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

  fread((char *)vdbHdr, sizeof(VarDB_Hdr), 1, fp);

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
    fprintf(stderr, "VarDB: read %d out of %ld entries\n", rc, VarDB_nRecords);
    fclose(fp);
    return(0);
  }

  fclose(fp);

  if (checkIfOlderVersionAndConvert(varDB, vdbHdr) || performCorrections(varDB))
  {
    fprintf(stderr, "VarDB modified, saving.\n");
    VarDB = varDB;
    SaveVarDB(fileName);
  }

  return(varDB);
}

static bool checkIfOlderVersionAndConvert(void *varDB, VarDB_Hdr *vdbHdr)
{
  bool modified = false;

  if (ntohl(vdbHdr->Version) == 1)
  {
    fprintf(stderr, "VarDB: File is version 1, cannot convert at this time.\n");
    exit(1);
  }

  if (ntohl(vdbHdr->Version) < VarDB_CurrentVersion)
  {
    fprintf(stderr, "VarDB: File has older version, converting to new version.\n");

    for (int i = 0; i < VarDB_nRecords; ++i)
    {
      ((struct var_v2 *)varDB)[i].standard_name = 0;
      ((struct var_v2 *)varDB)[i].reference = false;
    }

    vdbHdr->Version = htonl(VarDB_CurrentVersion);
    vdbHdr->RecordLen = htonl(sizeof(struct var_v2));
    modified = true;
  }

  return(modified);
}

#ifdef notdef
static void addVariable(void *varDB, var_v2 *p)
{
  void	*tmp;
  int	pos;

  // Don't add if already exists.
  for (pos = 0; pos < VarDB_nRecords; ++pos)
    if (strcmp(p->Name, ((struct var_v2 *)varDB)[pos].Name) == 0)
      return;

  if ((tmp = (void *)realloc(varDB, (unsigned)(VarDB_nRecords+1) *
                                (unsigned)VarDB_RecLength)) == NULL)
    return;

  varDB = tmp;

  // Add in sorted position
  for (pos = VarDB_nRecords-1; pos >= 0; --pos)
{
    if (strcmp(p->Name, ((struct var_v2 *)varDB)[pos].Name) < 0)
      memcpy( &((struct var_v2 *)varDB)[pos+1],
                &((struct var_v2 *)varDB)[pos],
                sizeof(struct var_v2));
    else
      break;
}

  ++pos;
  ++VarDB_nRecords;
  memcpy(&((struct var_v2 *)varDB)[pos], p, sizeof(var_v2));
}
#endif

static bool performCorrections(void *varDB)
{
  bool modified = false;

  // Perform some sanity.
  for (int i = 0; i < VarDB_nRecords; ++i)
  {
    struct var_v2 *p = &((struct var_v2 *)varDB)[i];

    if (strcmp(p->Units, "degC") == 0)
    {
      modified = true;
      strcpy(p->Units, "deg_C");
      printf("%s - degC changed to deg_C\n", p->Name);
    }

    if (strcmp(p->Units, "mbar") == 0)
    {
      modified = true;
      strcpy(p->Units, "hPa");
      printf("%s - mbar changed to hPa\n", p->Name);
    }

    if (p->is_analog == false && p->defaultSampleRate != 0)
    {
      modified = true;
      p->defaultSampleRate = 0;
      p->voltageRange[0] = 0;
      p->voltageRange[1] = 0;
      printf("%s - is !analog cleanup, clear rate/range fields\n", p->Name);
    }
  }

/*
  struct var_v2 newVar;
  memset(&newVar, 0, sizeof(newVar));
  strcpy(newVar.Name, "RTX");
  strcpy(newVar.Units, "deg_C");
  strcpy(newVar.Title, "Recovery Temperature, Reference");
  newVar.is_analog = false;
  newVar.Category = 6;
  newVar.standard_name = 0;
  newVar.reference = true;
  addVariable(varDB, &newVar);
modified = true;
*/
  return(modified);
}

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
  int	i;
  /*int masterNrecords;*/
  char	tname[64];

  // Strip off suffix for lookup.
  for (i = 0; vn[i] && vn[i] != '_'; ++i)
    tname[i] = vn[i];

  tname[i] = '\0';

  if (VarDB_NcML > 0)
  {
    int varID;
    if (nc_inq_varid(VarDB_NcML, vn, &varID) == NC_NOERR ||
        nc_inq_varid(VarDB_NcML, tname, &varID) == NC_NOERR)
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
