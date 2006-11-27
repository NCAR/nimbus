/*
-------------------------------------------------------------------------
OBJECT NAME:	init.c

FULL NAME:	Initialize

ENTRY POINTS:	InitializeVarDB()
		ReleaseVarDB()
		VarDB_lookup()

STATIC FNS:	none

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2006
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <netinet/in.h>	// htonl macros.

#include "vardb.h"
#include "portable.h"

const long	VarDB_MagicCookie	= 0x42756c6c;
const long	VarDB_CurrentVersion	= 3;

struct vardb_hdr	master_VarDB_Hdr;
void			*master_VarDB = NULL;

struct vardb_hdr	VarDB_Hdr;
void			*VarDB = NULL;

long	VarDB_RecLength, VarDB_nRecords;

void	*readFile(const char fileName[], struct vardb_hdr *vdbHdr);


/* -------------------------------------------------------------------- */
int InitializeVarDB(const char fileName[])
{
  char	*p, masterFileName[256];

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

  SetCategoryFileName(fileName);
  SetStandardNameFileName(fileName);

  VarDB = readFile(fileName, &VarDB_Hdr);

  if (VarDB == 0 && master_VarDB == 0)
    return(ERR);

  return(OK);

}	/* INITIALIZEVARDB */

/* -------------------------------------------------------------------- */
void *readFile(const char fileName[], struct vardb_hdr *vdbHdr)
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

}	/* END _INIT */

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
  int		i, masterNrecords;
  char		tname[64];

  for (i = 0; vn[i] && vn[i] != '_'; ++i)
    tname[i] = vn[i];

  tname[i] = '\0';

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
