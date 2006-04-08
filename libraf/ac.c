/*
-------------------------------------------------------------------------
OBJECT NAME:	ac.c

FULL NAME:	AircraftSpecs reader.

ENTRY POINTS:	InitAircraftSpecs()
		GetAircraftParameter()
		GetAircraftList()
		ReleaseAircraftSpecs()

DESCRIPTION:	See example at the bottom of this file.

COPYRIGHT:	University Corporation for Atmospheric Research, 1995-2005
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "constants.h"
#include "ac.h"

static char	**ACfile = NULL;

char	*GetMemory();

/* -------------------------------------------------------------------- */
void InitAircraftSpecs(char fileName[])
{
  int	i, cnt, len;
  char	*file[512], ACbuffer[512], *p;
  FILE	*fp;

  if (ACfile)
    return;

  if ((p = getenv("PROJ_DIR")) == NULL)
    {
    fprintf(stderr, "Environment variable PROJ_DIR non-existant\n");
    exit(1);
    }

  strcpy(ACbuffer, p);
  strcat(ACbuffer, "/Configuration/raf/");
  strcat(ACbuffer, fileName);

  if ((fp = fopen(ACbuffer, "r")) == NULL)
    {
    fprintf(stderr, "Can't open %s\n", fileName);
    exit(1);
    }


  /* Read file and strip out comments.
   */
  for (cnt = 0; fgets(ACbuffer, 512, fp); )
    {
    if (ACbuffer[0] == COMMENT || strlen(ACbuffer) < 2)
      continue;

    ACbuffer[strlen(ACbuffer)] = '\0';		/* Ditch newline	*/

    if ( (p = strchr(ACbuffer, COMMENT)) )
      *p = '\0';
    else
      p = &ACbuffer[strlen(ACbuffer)];

    while ( isspace(*(--p)) )	/* Strip trailing white-space	*/
      *p = '\0';

    if ( isspace(ACbuffer[0]) )	/* Belongs to previous line		*/
      {
      p = realloc(file[cnt-1], strlen(file[cnt-1]) + strlen(ACbuffer)+1);

      if (p == NULL)
        {
        fprintf(stderr, "InitAircraft: realloc failure.\n");
        exit(1);
        }

      file[cnt-1] = p;
      strcat(file[cnt-1], ACbuffer);
      }
    else	/* Fresh line		*/
      {
      file[cnt] = GetMemory(strlen(ACbuffer)+1);
      strcpy(file[cnt], ACbuffer);
      ++cnt;
      }
    }

  fclose(fp);


  if ((ACfile = calloc((size_t)cnt+1, sizeof(char *))) == NULL)
    {
    fprintf(stderr, "InitAircraft: out of memory.\n");
    exit(1);
    }

  for (i = 0; i < cnt; ++i)
  ACfile[i] = file[i];

  ACfile[i] = NULL;

}	/* END INITAIRCRAFT */

/* -------------------------------------------------------------------- */
char *GetAircraftParameter(char tailNumber[],  char parameter[])
{
  int	i, j, k;
  char	*p;

  for (i = 0; ACfile[i]; ++i)
    {
    if (strncmp(ACfile[i], "START", 5) == 0)
      {
      for (p = &ACfile[i][5]; *p == ' ' || *p == '\t'; ++p)
        ;

      if (strcmp(tailNumber, p) == 0)
        {
        int	len = strlen(parameter);

        for (; ACfile[i]; ++i)
          {
          if (strcmp(ACfile[i], "END") == 0)
            return(NULL);

          if (strncmp(ACfile[i], parameter, len) == 0)
            {
            if (ACfile[i][len] != ' ' && ACfile[i][len] != '\t')
              continue;

            for (p = &ACfile[i][len]; *p == ' ' || *p == '\t'; ++p)
              ;

            return(p);
            }
          }
        }
      }
    }

  return(NULL);

}	/* END GETAIRCRAFTPARAMETER */

/* -------------------------------------------------------------------- */
void ReleaseAircraftSpecs()
{
  if (ACfile)
    {
    int	i;

    for (i = 0; ACfile[i]; ++i)
      free(ACfile[i]);

    free(ACfile);

    ACfile = NULL;
    }

}	/* END RELEASEAIRCRAFT */

/* -------------------------------------------------------------------- */
int GetAircraftList(char *list[])
{
  int		i, cnt = 0;
  char	*p;

  list[0] = NULL;

  for (i = 0; ACfile[i]; ++i)
    if (strncmp(ACfile[i], "START", 5) == 0)
      {
      for (p = &ACfile[i][5]; *p == ' ' || *p == '\t'; ++p)
        ;

      list[cnt] = GetMemory(strlen(p)+1);
      strcpy(list[cnt], p);
      list[++cnt] = NULL;
      }

  return(cnt);

}	/* END GETAIRCRAFTLIST */

/* Sample.c
#include <stdio.h>
#include "ac.h"

main()
{
  int   i;
  char  *p, *acList[30];

  InitAircraftSpecs("AircraftSpecs");

  // Print while list.
  GetAircraftList(acList);
  for (i = 0; acList[i]; ++i)
    printf("%s\n", acList[i]);

  // Find something specific.
  if ((p = GetAircraftParameter("N308D", "BOOM_LEN")) == NULL)
  {
    printf("Not found.\n");
    exit(1);
  }

  printf("%s\n", p);

  ReleaseAircraftSpecs();

}
*/
/* END AC.C */
