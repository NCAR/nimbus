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

INPUT:		none

OUTPUT:      

REFERENCES:	OpenProjectFile()

REFERENCED BY:	hdr_decode.c, cb_main(), various AMLIB fn's.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-8
-------------------------------------------------------------------------
*/

#include <stdio.h>

#include "nimbus.h"
#include "decode.h"
#include "netcdf.h"

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

  /* Read proj/defaults/Defaults first.
   */
/* Removed since hardcoding of unfound values was added.
  fp = OpenProjectFile("%s/defaults/Defaults", "r", EXIT);

  while (fgets(buffer, BUFF_SIZE, fp) != NULL)
    {
    if (buffer[0] == COMMENT || strlen(buffer) < (size_t)3)
      continue;

    process_line(buffer, fp);
    }

  fclose(fp);
*/

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
NR_TYPE *GetDefaultsValue(char target[], char var[])
{
  for (size_t i = 0; i < nDefaults; ++i)
    {
    if (strcmp(Defaults[i]->Name, target) == 0)
      {
      Defaults[i]->Used = true;
      strcpy(Defaults[i]->var, var);
      return(Defaults[i]->Value);
      }
    }
 
  sprintf(buffer, "Request for non-existent default, %s.\n", target);
  LogMessage(buffer);
 
  return(NULL);
 
}  /* END GETDEFAULTSVALUE */

/* -------------------------------------------------------------------- */
void CheckAndAddAttrs(int fd, int varid, char name[])
{
  for (size_t i = 0; i < nDefaults; ++i)
    {
    if (strcmp(name, Defaults[i]->var) == 0)
      {
      ncattput(fd, varid, Defaults[i]->Name, NC_FLOAT,
		Defaults[i]->nValues, Defaults[i]->Value);
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



  if (is_array)
    {
    Defaults[whichOne]->nValues = atoi(is_array+1);

    strcpy(Defaults[whichOne]->Name, name);
    *is_array = '[';
    }
  else
    {
    Defaults[whichOne]->nValues = 1;
    strcpy(Defaults[whichOne]->Name, name);
    }

  Defaults[whichOne]->var[0] = '\0';
  Defaults[whichOne]->Dirty = false;
  Defaults[whichOne]->Used  = false;
  Defaults[whichOne]->Value = new NR_TYPE[Defaults[whichOne]->nValues];

  if (is_array)
    {
    for (size_t i = 0; i < Defaults[whichOne]->nValues; ++i)
      {
      nxt = strtok((char *)NULL, ARRAY_TOKENS);

      if (nxt == NULL || nxt[0] == COMMENT)
        {
        fgets(line_p, BUFF_SIZE, fp);
        nxt = strtok(line_p, ARRAY_TOKENS);
        }

      Defaults[whichOne]->Value[i] = (float)atof(nxt);
      }
    }
  else
    Defaults[whichOne]->Value[0] =
        (float)atof(strtok((char *)NULL, TOKENS));

}  /* END PROCESS_LINE */

/* END RD_DEFS.C */
