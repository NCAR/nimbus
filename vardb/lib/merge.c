/*
-------------------------------------------------------------------------
OBJECT NAME:	merge.c

FULL NAME:	

ENTRY POINTS:	CreateVarDB()

STATIC FNS:	none

DESCRIPTION:	Creates a VarDB from WINDS' Bulliten 9 and plot_attr files.

NOTES:		This file is not fully converted for byte swapping.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-7
-------------------------------------------------------------------------
*/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "vardb.h"

#include <netinet/in.h> // htonl macros.

extern const long	VarDB_MagicCookie, VarDB_CurrentVersion;
extern long		VarDB_RecLength;


/* -------------------------------------------------------------------- */
int CreateVarDB(const char Bull9_name[], const char Pattr_name[])
{
  int		i, j, cnt;
  float		f1, f2;
  char		buffer[256], name[VARDB_NL];
  FILE		*Bull9, *Pattr;
  struct var_v2	*vdb[1024];

  if ((Bull9 = fopen(Bull9_name, "r")) == NULL)
    {
    fprintf(stderr, "CreateVarDB: can't open %s\n", Bull9_name);
    return(ERR);
    }

  if ((Pattr = fopen(Pattr_name, "r")) == NULL)
    {
    fprintf(stderr, "CreateVarDB: can't open %s\n", Pattr_name);
    fclose(Bull9);
    return(ERR);
    }


  /* Scan Plot_Attr first, then Bulletin 9.
   */
  for (cnt = 0; fgets(buffer, 256, Pattr); ++cnt)
    {
    vdb[cnt] = (struct var_v2 *)malloc(sizeof(struct var_v2));
    sscanf(buffer, "%s %s %f %f", vdb[cnt]->Name, vdb[cnt]->Units, &f1,&f2);

    strcpy(vdb[cnt]->Title, "No title");
    strcpy(vdb[cnt]->AlternateUnits, "");

    vdb[cnt]->is_analog = false;
    vdb[cnt]->voltageRange[0] = -10;
    vdb[cnt]->voltageRange[1] = 10;
    vdb[cnt]->defaultSampleRate = 100;
    }


  while (fgets(buffer, 256, Bull9))
    {
    if (strlen(buffer) <= 45)
      continue;

    sscanf(buffer, "%*44c %*s %s", name);

    for (i = 0; i < cnt; ++i)
      if (strcmp(vdb[i]->Name, name) == 0)
        {
        strncpy(vdb[i]->Title, buffer, 44);
        break;
        }

    if (i == cnt)	/* Not Found, then add	*/
      {
      vdb[cnt] = (struct var_v2 *)malloc(sizeof(struct var_v2));
      sscanf(buffer, "%44c %s %s %f %f", vdb[cnt]->Title, vdb[cnt]->Units,
              vdb[cnt]->Name, &f1, &f2);

      strcpy(vdb[cnt]->AlternateUnits, "");

      vdb[cnt]->is_analog = false;
      vdb[cnt]->voltageRange[0] = -10;
      vdb[cnt]->voltageRange[1] = 10;
      vdb[cnt]->defaultSampleRate = 100;

      ++cnt;
      }

    vdb[i]->Title[44] = '\0';

    for (j = 43; vdb[i]->Title[j] == ' '; --j)
      vdb[i]->Title[j] = '\0';
    }

  fclose(Bull9);
  fclose(Pattr);

  /* Now transfer the data into the VarDB variables provided by the API
   */
  varDB_Hdr.MagicCookie = htonl(VarDB_MagicCookie);
  varDB_Hdr.Version = htonl(VarDB_CurrentVersion);
  varDB_Hdr.nRecords = htonl(cnt);
  varDB_Hdr.RecordLen = htonl(sizeof(struct var_v2));

  if ((VarDB = (char *)calloc((unsigned)cnt, VarDB_RecLength)) == NULL)
    {
    fprintf(stderr, "VarDB: calloc failure\n");
    return(ERR);
    }

  for (i = 0; i < cnt; ++i)
    {
    memcpy((char *)&((struct var_v2 *)VarDB)[i], (char *)vdb[i],
						sizeof(struct var_v2));
    free(vdb[i]);
    }

  return(OK);

}	/* END CREATEVARDB */

/* END MERGE.C */
