/*
-------------------------------------------------------------------------
OBJECT NAME:	catagory.c

FULL NAME:	VarDB_ Category functions

ENTRY POINTS:	SetCategoryFileName()
		ReadCategories()
		VarDB_GetCategoryName()
		VarDB_AddCategory()
		VarDB_GetVariablesInCategory()
		VarDB_GetCategoryList()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		Variable Name

OUTPUT:		Category Name

COPYRIGHT:	University Corporation for Atmospheric Research, 1994-7
-------------------------------------------------------------------------
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vardb.h"

#define MAX_CATEGORIES	128
#define CAT_NAME_LEN	64


typedef struct
	{
	int	Number;
	char	Name[CAT_NAME_LEN];
	} CATEGORY;

static int	firstTime = TRUE;
static char	*fileName;
static int	nCategories = 0;
static CATEGORY	*Category[MAX_CATEGORIES];

char	*GetMemory();

/* -------------------------------------------------------------------- */
void SetCategoryFileName(const char fn[])
{
  char	*p;

  fileName = GetMemory(strlen(fn) + 10);
  strcpy(fileName, fn);
  p = strrchr(fileName, '/');

  if (p)
    ++p;
  else
  p = fileName;

  strcpy(p, "Categories");

}	/* END SETCATEGORYFILENAME */

/* -------------------------------------------------------------------- */
ReadCategories()
{
  char	line[128], *p;
  FILE	*fp;

  if ((fp = fopen(fileName, "r")) == NULL)
    {
    fprintf(stderr, "catagory.c: Can't open %s.\n", fileName);
    return(ERR);
    }

  for (nCategories = 0; fgets(line, 128, fp); )
    {
    if (line[0] == '#' || strlen(line) < (size_t)5)
      continue;

    if (nCategories >= MAX_CATEGORIES-1)
      {
      fprintf(stderr, "vardb/lib/catagory.c: MAX_CATEGORIES exceeded.\n");
      fprintf(stderr, " Ignoring extra catagories and continuing.\n");
      fprintf(stderr, " Please notify a programmer of this problem.\n");

      fclose(fp);
      return(OK);
      }

    Category[nCategories] = (CATEGORY *)GetMemory(sizeof(CATEGORY));

    Category[nCategories]->Number = atoi(line);

    for (p = strchr(line, ',')+1; *p == ' ' || *p == '\t'; ++p)
      ;

    strncpy(Category[nCategories]->Name, p, CAT_NAME_LEN);
    Category[nCategories]->Name[strlen(Category[nCategories]->Name)-1]='\0';
    ++nCategories;
    }

  fclose(fp);
  return(OK);

}	/* END READCATEGORIES */

/* -------------------------------------------------------------------- */
char *VarDB_GetCategoryName(const char vn[])
{
  int	i, rc, indx;
  int	catNum;

  if (firstTime)
    {
    if (ReadCategories() == ERR)
      return((char *)ERR);

    firstTime = FALSE;
    }


  if ((indx = VarDB_lookup(vn)) == ERR)
    return(Category[0]->Name);

  catNum = ntohl(((struct var_v2 *)VarDB)[indx].Category);

  for (rc = i = 0; i < nCategories; ++i)
    if (Category[i]->Number == catNum)
      rc = i;

  return(Category[rc]->Name);

}	/* END VARDB_GETCATEGORYNAME */

/* -------------------------------------------------------------------- */
char **VarDB_GetVariablesInCategory(int catNum)
{
  int	i, cnt;
  char	**p;

  if (firstTime)
    {
    if (ReadCategories() == ERR)
      return((char **)ERR);
 
    firstTime = FALSE;
    }
 

  cnt = 0;
  p = (char **)GetMemory(sizeof(char *));

  for (i = 0; i < VarDB_Hdr.nRecords; ++i)
    if (ntohl(((struct var_v2 *)VarDB)[i].Category) == catNum)
      {
      p = realloc(p, sizeof(char *) * (cnt+2));
      p[cnt] = ((struct var_v2 *)VarDB)[i].Name;

      ++cnt;
      }

  p[cnt] = NULL;

  return(p);

}	/* END VARDB_GETVARIABLESINCATEGORY */

/* -------------------------------------------------------------------- */
int VarDB_GetCategoryList(char *list[])
{
  int	i;

  if (firstTime)
    {
    if (ReadCategories() == ERR)
      return(ERR);
 
    firstTime = FALSE;
    }
 

  for (i = 0; i < nCategories; ++i)
    list[i] = Category[i]->Name;

  list[i] = NULL;

  return(i);

}	/* END VARDB_GETCATEGORYLIST */

/* -------------------------------------------------------------------- */
int VarDB_GetCategoryNumber(const char catagoryName[])
{
  int	i;

  if (firstTime)
    {
    if (ReadCategories() == ERR)
      return(ERR);
 
    firstTime = FALSE;
    }
 

  for (i = 0; i < nCategories; ++i)
    if (strcmp(catagoryName, Category[i]->Name) == 0)
      return(Category[i]->Number);

  return(0);	/* Category 0 is "None"	*/

}	/* END VARDB_GETCATEGORYNUMBER */

/* END CATEGORY.C */
