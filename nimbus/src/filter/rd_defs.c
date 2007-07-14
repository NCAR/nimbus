/*
-------------------------------------------------------------------------
OBJECT NAME:	rd_defs.c

FULL NAME:	Read Defaults Object

ENTRY POINTS:	ReadDefaultsFile()
		GetDefaultsValue()
		CheckAndAddAttrs()

STATIC FNS:	process_line()

DESCRIPTION:	The Defaults file contains constants for various probe
		calculations.

REFERENCES:	OpenProjectFile()

REFERENCED BY:	cb_main.c, cb_defs.c various AMLIB fn's.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2007
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include <netcdf.h>

#define BUFF_SIZE  256
#define TOKENS    "\t "
#define ARRAY_TOKENS  ", \t\n"


extern size_t	nDefaults;
extern DEFAULT	*Defaults[];

static void  process_line(char *line_p, FILE *fp);


/* -------------------------------------------------------------------- */
void ReadDefaultsFile()
{
  FILE  *fp;

  nDefaults = 0;

  /* Now process project specific one.
   */
  fp = OpenProjectFile(DEFAULTS, "r", EXIT);

  while (fgets(buffer, BUFF_SIZE, fp) != NULL)
  {
    if (buffer[0] == COMMENT || strlen(buffer) < (size_t)3)
      continue;

    process_line(buffer, fp);
  }

  fclose(fp);

}  /* END READDEFAULTSFILE */

/* -------------------------------------------------------------------- */
NR_TYPE *GetDefaultsValue(const char target[], const char var[])
{
  for (size_t i = 0; i < nDefaults; ++i)
  {
    if (strcmp(Defaults[i]->Name, target) == 0)
    {
      Defaults[i]->Used = true;
      strcpy(Defaults[i]->var, var);
      return(&Defaults[i]->Values[0]);
    }
  }
 
  sprintf(buffer, "Request for non-existent default, %s.\n", target);
  LogMessage(buffer);
 
  return(NULL);
 
}	/* END GETDEFAULTSVALUE */

/* -------------------------------------------------------------------- */
void AddToDefaults(const char varName[], const char attrName[],
        const std::vector<NR_TYPE>& values)
{
  Defaults[nDefaults] = new DEFAULT;
  strcpy(Defaults[nDefaults]->var, varName);
  strcpy(Defaults[nDefaults]->Name, attrName);
  Defaults[nDefaults]->Dirty = false;
  Defaults[nDefaults]->Used = true;
  Defaults[nDefaults]->Values = values;

  ++nDefaults;
}

/* -------------------------------------------------------------------- */
void CheckAndAddAttrs(int fd, int varid, char name[])
{
  for (size_t i = 0; i < nDefaults; ++i)
  {
    if (strcmp(name, Defaults[i]->var) == 0)
    {
      ncattput(fd, varid, Defaults[i]->Name, NC_FLOAT,
		Defaults[i]->Values.size(), &Defaults[i]->Values[0]);
    }
  }
}	/* END CHECKANDADDATTRS */

/* -------------------------------------------------------------------- */
static void process_line(char *line_p, FILE *fp)
{
  size_t whichOne;
  char  *name, *nxt, *is_array;

  name = strupr(strtok(line_p, TOKENS));
  if ( (is_array = strchr(name, '[')) )
    *is_array = '\0';

  /* Check if it already exists, if so, then overwrite old one.
   */
  whichOne = nDefaults;

  for (size_t i = 0; i < nDefaults; ++i)
    if (strcmp(name, Defaults[i]->Name) == 0)
    {
      whichOne = i;
      break;
    }

  if (whichOne == nDefaults)
  {
    Defaults[whichOne] = new DEFAULT;
    ++nDefaults;
  }


  size_t nValues = 1;

  strcpy(Defaults[whichOne]->Name, name);
  Defaults[whichOne]->var[0] = '\0';
  Defaults[whichOne]->Dirty = false;
  Defaults[whichOne]->Used  = false;

  if (is_array)
  {
    nValues = atoi(is_array+1);
    *is_array = '[';

    for (size_t i = 0; i < nValues; ++i)
    {
      nxt = strtok((char *)NULL, ARRAY_TOKENS);

      if (nxt == NULL || nxt[0] == COMMENT)
      {
        fgets(line_p, BUFF_SIZE, fp);
        nxt = strtok(line_p, ARRAY_TOKENS);
      }

      Defaults[whichOne]->Values.push_back((float)atof(nxt));
    }
  }
  else
    Defaults[whichOne]->Values.push_back(
        (float)atof(strtok((char *)NULL, TOKENS)));

}  /* END PROCESS_LINE */

/* END RD_DEFS.C */
