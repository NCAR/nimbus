/*
-------------------------------------------------------------------------
OBJECT NAME:	init.c

FULL NAME:	Initialize

ENTRY POINTS:	InitializeVarDB()
		ReleaseVarDB()
		VarDB_lookup()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		FileName

OUTPUT:		OK/ERR

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-7
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
const long	VarDB_CurrentVersion	= 2;

struct vardb_hdr	VarDB_Hdr;
void			*VarDB = NULL;

void	readXML(FILE *fp);

long	VarDB_RecLength, VarDB_nRecords;

/* -------------------------------------------------------------------- */
int InitializeVarDB(const char fileName[])
{
  FILE	*fp;
  int	rc;

  if ((fp = fopen(fileName, "rb")) == NULL)
  {
    fprintf(stderr, "VarDB: can't open %s\n", fileName);
    return(ERR);
  }

  SetCategoryFileName(fileName);

  fread((char *)&VarDB_Hdr, sizeof(struct vardb_hdr), 1, fp);

  if (memcmp((char *)&VarDB_Hdr, "<?xml", 5) == 0)
  {
    readXML(fp);
    fclose(fp);
    return(OK);
  }

  if (ntohl(VarDB_Hdr.MagicCookie) != VarDB_MagicCookie)
  {
    fprintf(stderr, "VarDB: bad VarDB file %s.\n", fileName);
    fclose(fp);
    return(ERR);
  }

  if (ntohl(VarDB_Hdr.Version) > VarDB_CurrentVersion)
  {
    fprintf(stderr,"VarDB: File has newer version than code can handle.\n");
    fclose(fp);
    return(ERR);
  }

  VarDB_nRecords = ntohl(VarDB_Hdr.nRecords);
  VarDB_RecLength = ntohl(VarDB_Hdr.RecordLen);

  if ((VarDB = (void *)malloc((unsigned)VarDB_nRecords * VarDB_RecLength)) == NULL)
  {
    fprintf(stderr, "VarDB: out of memory.\n");
    fclose(fp);
    return(ERR);
  }

  if ((rc = fread((char *)VarDB, VarDB_RecLength, VarDB_nRecords, fp)) != VarDB_nRecords)
  {
    fprintf(stderr, "VarDB: read %d out of %d entries\n", rc, VarDB_nRecords);
    fclose(fp);
    return(ERR);
  }


  if (ntohl(VarDB_Hdr.Version) < VarDB_CurrentVersion)
  {
    int		i;
    void	*oldDB = VarDB;

    fclose(fp);
    fprintf(stderr, "VarDB: File has older version, converting to new version.\n");


    if ((VarDB = (void *)malloc((unsigned)VarDB_nRecords *
				(unsigned)sizeof(struct var_v2))) == NULL)
    {
      fprintf(stderr, "VarDB: out of memory.\n");
      return(ERR);
    }

    memset((char *)VarDB, '\0', VarDB_nRecords * sizeof(struct var_v2));

    for (i = 0; i < VarDB_nRecords; ++i)
      memcpy((char *)&((struct var_v2 *)VarDB)[i],
             (char *)&((struct var_v1 *)oldDB)[i],
             (unsigned)VarDB_RecLength);

    VarDB_Hdr.Version = htonl(VarDB_CurrentVersion);
    VarDB_Hdr.RecordLen = htonl(sizeof(struct var_v2));

    SaveVarDB(fileName);
  }


  fclose(fp);
  return(OK);

}	/* END INITIALIZEVARDB */

/* -------------------------------------------------------------------- */
static const int	BlkingCount = 50;

void readXML(FILE *fp)
{
  int	cntr = BlkingCount;
  char *name, *p;
  char	line[512];
  float	f;
  struct var_v2	*vdbp;

  while (fgets(line, 512, fp) )
  {
    if (strstr(line, "<variable"))
    {
      p = strchr(line, '=')+1;
      name = strtok(p, ">'\" \t");

      if (cntr >= BlkingCount)
      {
	VarDB = realloc(VarDB, sizeof(struct var_v2) *
	    (BlkingCount+VarDB_nRecords));
	memset(&((char *)VarDB)[sizeof(struct var_v2) * VarDB_nRecords], 0,
	    sizeof(struct var_v2) * BlkingCount);
	cntr = 0;
      }

      vdbp = &((struct var_v2 *)VarDB)[VarDB_nRecords];

      strcpy(vdbp->Name, name);
      do
      {
        fgets(line, 512, fp);
        p = strchr(line, '<')+1;

	if (*p == '/')
	  break;

        if (strncmp(p, "long_name>", 10) == 0)
        {
          if (*(p = strchr(p, '>')+1) != '<')
            strcpy(vdbp->Title, strtok(p, "><"));
        }

        if (strncmp(p, "units>", 6) == 0)
        {
          if (*(p = strchr(p, '>')+1) != '<')
            strcpy(vdbp->Units, strtok(p, "><"));
        }

        if (strncmp(p, "alt_units>", 10) == 0)
        {
          if (*(p = strchr(p, '>')+1) != '<')
            strcpy(vdbp->AlternateUnits, strtok(p, "><"));
        }

        if (strncmp(p, "valid_range>", 12) == 0)
        {
          if (*(p = strchr(p, '>')+1) != '<')
	  {
            vdbp->MinLimit = htonf((float)atof(strtok(p, " <>")));
            vdbp->MinLimit = htonf((float)atof(strtok(NULL, "><")));
	  }
        }

        if (strncmp(p, "display_range ", 14) == 0)
        {
	  if (strstr(p, "Fixed"))
	    vdbp->type = htonl(FIXED);
	  else
	    vdbp->type = htonl(FLOATING);

          if (*(p = strchr(p, '>')+1) != '<' && vdbp->type == FIXED)
	  {
            f = atof(strtok(p, " <>"));
            vdbp->FixedRange[0] = htonf(f);
            f = atof(strtok(NULL, "><"));
            vdbp->FixedRange[1] = htonf(f);
	  }
	  else
            vdbp->FloatRange = htonf((float)atof(strtok(p, " <>")));

        }





      }
      while (1);

      ++VarDB_nRecords;
      ++cntr;
    }
  }

}

/* -------------------------------------------------------------------- */
void ReleaseVarDB()
{
  if (VarDB)
  {
    free(VarDB);
    VarDB = NULL;
  }

}	/* END RELEASEVARDB */

/* -------------------------------------------------------------------- */
int VarDB_lookup(const char vn[])
{
  int		i;
  char		tname[64];
  static int	indx = 0;

  for (i = 0; vn[i] && vn[i] != '_'; ++i)
    tname[i] = vn[i];

  tname[i] = '\0';

  if (strcmp(((struct var_v2 *)VarDB)[indx].Name, tname) == 0)
    return(indx);

  for (i = 0; i < VarDB_nRecords; ++i)
    if (strcmp(((struct var_v2 *)VarDB)[i].Name, tname) == 0)
      return((indx = i));

  return(ERR);

}	/* END VARDB_LOOKUP */

/* END INIT.C */
