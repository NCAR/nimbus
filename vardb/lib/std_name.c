/*
-------------------------------------------------------------------------
OBJECT NAME:	std_name.c

FULL NAME:	VarDB_ Standard Name functions

ENTRY POINTS:	SetStandardNameFileName()
		ReadStandardNames()
		VarDB_GetStandardNameName()
		VarDB_AddStandardName()
		VarDB_GetVariablesInStandardName()
		VarDB_GetStandardNameList()

STATIC FNS:	none

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 2005
-------------------------------------------------------------------------
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vardb.h"

#define MAX_STD_NAMES	128
#define STD_NAME_LEN	64


typedef struct
  {
  int	Number;
  char	Name[STD_NAME_LEN];
  } STD_NAME;

static int	firstTime = TRUE;
static char	*fileName;
static int	nStdNames = 0;
static STD_NAME	*StdName[MAX_STD_NAMES];

char	*GetMemory();

/* -------------------------------------------------------------------- */
void SetStandardNameFileName(const char fn[])
{
  char *p;

  fileName = GetMemory(strlen(fn) + 10);
  strcpy(fileName, fn);
  p = strrchr(fileName, '/');

  if (p)
    ++p;
  else
  p = fileName;

  strcpy(p, "StandardNames");

}	/* END SETCATEGORYFILENAME */

/* -------------------------------------------------------------------- */
ReadStandardNames()
{
  char line[128], *p;
  FILE *fp;

  if ((fp = fopen(fileName, "r")) == NULL)
    {
    fprintf(stderr, "std_name.c: Can't open %s.\n", fileName);
    return(ERR);
    }

  for (nStdNames = 0; fgets(line, 128, fp); )
    {
    if (line[0] == '#' || strlen(line) < (size_t)5)
      continue;

    if (nStdNames >= MAX_STD_NAMES-1)
      {
      fprintf(stderr, "vardb/lib/catagory.c: MAX_STD_NAME exceeded.\n");
      fprintf(stderr, " Ignoring extra catagories and continuing.\n");
      fprintf(stderr, " Please notify a programmer of this problem.\n");

      fclose(fp);
      return(OK);
      }

    StdName[nStdNames] = (STD_NAME *)GetMemory(sizeof(STD_NAME));

    StdName[nStdNames]->Number = atoi(line);

    for (p = strchr(line, ',')+1; *p == ' ' || *p == '\t'; ++p)
      ;

    strncpy(StdName[nStdNames]->Name, p, STD_NAME_LEN);
    StdName[nStdNames]->Name[strlen(StdName[nStdNames]->Name)-1]='\0';
    ++nStdNames;
    }

  fclose(fp);
  return(OK);

}	/* END READCATEGORIES */

/* -------------------------------------------------------------------- */
char *VarDB_GetStandardNameName(const char vn[])
{
  int i, rc, indx;
  int catNum;

  if (firstTime)
    {
    if (ReadStandardNames() == ERR)
      return((char *)ERR);

    firstTime = FALSE;
    }


  if ((indx = VarDB_lookup(vn)) == ERR)
    return(StdName[0]->Name);

  catNum = ntohl(((struct var_v2 *)VarDB)[indx].standard_name);

  for (rc = i = 0; i < nStdNames; ++i)
    if (StdName[i]->Number == catNum)
      rc = i;

  return(StdName[rc]->Name);

}	/* END VARDB_GETCATEGORYNAME */

/* -------------------------------------------------------------------- */
char **VarDB_GetVariablesInStandardName(int catNum)
{
  int i, cnt;
  char **p;

  if (firstTime)
    {
    if (ReadStandardNames() == ERR)
      return((char **)ERR);
 
    firstTime = FALSE;
    }
 

  cnt = 0;
  p = (char **)GetMemory(sizeof(char *));

  for (i = 0; i < VarDB_Hdr.nRecords; ++i)
    if (ntohl(((struct var_v2 *)VarDB)[i].standard_name) == catNum)
      {
      p = realloc(p, sizeof(char *) * (cnt+2));
      p[cnt] = ((struct var_v2 *)VarDB)[i].Name;

      ++cnt;
      }

  p[cnt] = NULL;

  return(p);

}	/* END VARDB_GETVARIABLESINCATEGORY */

/* -------------------------------------------------------------------- */
int VarDB_GetStandardNameList(char *list[])
{
  int i;

  if (firstTime)
    {
    if (ReadStandardNames() == ERR)
      return(ERR);
 
    firstTime = FALSE;
    }
 

  for (i = 0; i < nStdNames; ++i)
    list[i] = StdName[i]->Name;

  list[i] = NULL;

  return(i);

}	/* END VARDB_GETCATEGORYLIST */

/* -------------------------------------------------------------------- */
int VarDB_GetStandardNameNumber(const char catagoryName[])
{
  int i;

  if (firstTime)
    {
    if (ReadStandardNames() == ERR)
      return(ERR);
 
    firstTime = FALSE;
    }
 

  for (i = 0; i < nStdNames; ++i)
    if (strcmp(catagoryName, StdName[i]->Name) == 0)
      return(StdName[i]->Number);

  return(0);	/* Category 0 is "None"	*/

}	/* END VARDB_GETCATEGORYNUMBER */

/* END STD_NAME.C */
