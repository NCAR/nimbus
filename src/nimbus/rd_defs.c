/*
-------------------------------------------------------------------------
OBJECT NAME:	rd_defs.c

FULL NAME:	Read Defaults Object

ENTRY POINTS:	ReadDefaultsFile()
		GetDefaultsValue()
		FreeDefaults()

STATIC FNS:	process_line()

DESCRIPTION:	The Defaults file contains constants for various probe
		calculations.

REFERENCES:	OpenProjectFile()

REFERENCED BY:	cb_main.c, cb_defs.c various AMLIB fn's.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2017
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
void FreeDefaults()
{
  for (size_t i = 0; i < nDefaults; ++i)
  {
    delete [] Defaults[i]->Name;
    delete Defaults[i];
  }
}

/* -------------------------------------------------------------------- */
void readDefaultsFile(const std::string file)
{
  FILE *fp = OpenProjectFile(file, "r", RETURN);

  if (fp == 0)
    return;

  while (fgets(buffer, BUFF_SIZE, fp) != NULL)
  {
    if (buffer[0] == COMMENT || strlen(buffer) < (size_t)3)
      continue;

    process_line(buffer, fp);
  }

  fclose(fp);

}  /* END READDEFAULTSFILE */

void ReadDefaultsFile()
{
  std::string file;

  nDefaults = 0;

  // Read project specific Defaults file.
  file = DEFAULTS;
  readDefaultsFile(file);
  // Read flight specific Defaults file.
  file += ".";
  file += cfg.FlightNumber();
  readDefaultsFile(file);

}  /* END READDEFAULTSFILE */

/* -------------------------------------------------------------------- */
float *GetDefaultsValue(const char target[], const char var[])
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

  return(NULL);

}	/* END GETDEFAULTSVALUE */

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

  Defaults[whichOne]->Name = new char[strlen(name)+1];
  strcpy(Defaults[whichOne]->Name, name);
  Defaults[whichOne]->var[0] = '\0';
  Defaults[whichOne]->Dirty = false;
  Defaults[whichOne]->Used  = false;
  Defaults[whichOne]->Values.clear();

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
