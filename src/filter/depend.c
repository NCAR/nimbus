/*
-------------------------------------------------------------------------
OBJECT NAME:	depend.c

FULL NAME:	Setup Dependencies

ENTRY POINTS:	SetUpDependencies()
		CleanOutUnwantedVariables()
		DependIndexLookup()

STATIC FNS:	doubleCheck()

DESCRIPTION:	

INPUT:		

OUTPUT:		none

REFERENCES:	SearchList(), SearchTable()

REFERENCED BY:	hdr_decode.c, cb_main.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "amlibProto.h"

char	*DependMsg =
	"%s is depending on non-existent variable %s, turning off.\n";

static void	doubleCheck(DERTBL *dp);

/* -------------------------------------------------------------------- */
void SetUpDependencies()
{
  DERTBL	*dp;
  int		i, j;
  char		tokens[] = " \t";
  char		*dependlist[MAX_DERIVE*4];
  char		*s, name[NAMELEN], location[NAMELEN];


  ReadTextFile(DEPENDTBL, dependlist);

  for (i = 0; (dp = derived[i]); ++i)
    {
    strcpy(name, dp->name);
    location[0] = '\0';

    if ((s = SearchList(dependlist, name)) == NULL)
      {
      if ((s = strchr(name, '_')) != NULL)
        {
        strcpy(location, s);
        *s = '\0';
        }
      }

    if ((s = SearchList(dependlist, name)) == NULL)
      {
      sprintf(buffer, "%s has no dependencies, turning off.\n", name);
      LogMessage(buffer);
      dp->Output = false;
      dp->compute = (void(*)(void *))smissval;
      continue;
      }

    strcpy(buffer, s);
    s = strtok(buffer, tokens);

    for (j = 0; (s = strtok((char *)NULL, tokens)); ++j)
      {
      strcpy(dp->depend[j], s);


      /* We need to check both cases of whether dependency needs the
      * location tacked on.  (e.g. CFSSP depends on TASX, we do not
      * want location tacked onto TASX.
      */
      if (DependIndexLookup(dp, j) == ERR)
        {
        strcat(dp->depend[j], location);

        if (DependIndexLookup(dp, j) == ERR)
          {
          /* Make one last check on the PMs1D stuff.  We want all the same
           * derived names for the new DMT probes, but raw names have been
           * changed.
           */
          if (dp->ProbeType & PROBE_PMS1D)
            {
            if (strncmp(dp->depend[j], "CFSSP", 5) == 0)
              {
              strcpy(dp->depend[j], "CS100");
              strcat(dp->depend[j], location);
              }

            if (strncmp(dp->depend[j], "CPCAS", 5) == 0)
              {
              strcpy(dp->depend[j], "CS200");
              strcat(dp->depend[j], location);
              }

            if (strncmp(dp->depend[j], "CF300", 5) == 0)
              {
              strcpy(dp->depend[j], "CS300");
              strcat(dp->depend[j], location);
              }
            }

          if (DependIndexLookup(dp, j) == ERR)
            {
            sprintf(buffer, DependMsg, name, dp->depend[j]);
            LogMessage(buffer);
            dp->Output = false;
            dp->compute = (void(*)(void *))smissval;
            break;
            }
          }
        }
      }

    dp->ndep = j;
    }

  FreeTextFile(dependlist);

}	/* END SETUPDEPENDANCIES */

/* -------------------------------------------------------------------- */
void CleanOutUnwantedVariables()
{
  int	i, cnt;

  for (i = 0; derived[i]; ++i)
    if (derived[i]->Output)
      doubleCheck(derived[i]);


  for (cnt = 0, i = 0; i < nsdi; ++i)
    if (sdi[i]->Output || sdi[i]->DependedUpon)
      sdi[cnt++] = sdi[i];

  sdi[(nsdi = cnt)] = NULL;

  for (cnt = 0, i = 0; i < nraw; ++i)
    if (raw[i]->Output || raw[i]->DependedUpon)
      raw[cnt++] = raw[i];

  raw[(nraw = cnt)] = NULL;

  for (cnt = 0, i = 0; i < nderive; ++i)
    if (derived[i]->Output || derived[i]->DependedUpon)
      derived[cnt++] = derived[i];

  derived[(nderive = cnt)] = NULL;

}	/* CLEANOUTUNWANTEDVARIABLES */

/* -------------------------------------------------------------------- */
int DependIndexLookup(DERTBL *dp, int which_dep)
{
  int		di;

  if ((di = SearchTable((char **)sdi, nsdi, dp->depend[which_dep])) != ERR)
    {
    dp->depend_LRindex[which_dep] = sdi[di]->LRstart;
    dp->depend_HRindex[which_dep] = sdi[di]->HRstart;
    }
  else
  if ((di = SearchTable((char **)raw, nraw, dp->depend[which_dep])) != ERR)
    {
    dp->depend_LRindex[which_dep] = raw[di]->LRstart;
    dp->depend_HRindex[which_dep] = raw[di]->HRstart;
    }
  else
  if ((di = SearchTable((char **)derived, nderive, dp->depend[which_dep])) !=ERR)
    {
    dp->depend_LRindex[which_dep] = derived[di]->LRstart;
    dp->depend_HRindex[which_dep] = derived[di]->HRstart;
    }
  else
    return(ERR);

  return(OK);

}	/* END DEPENDINDEXLOOKUP */

/* -------------------------------------------------------------------- */
static void doubleCheck(DERTBL *dp)	/* This function is recursive	*/
{
  int		i, indx;

  for (i = 0; i < dp->ndep; ++i)
    {
    if ((indx = SearchTable((char **)sdi, nsdi, dp->depend[i])) != ERR)
      sdi[indx]->DependedUpon = true;
    else
    if ((indx = SearchTable((char **)raw, nraw, dp->depend[i])) != ERR)
      raw[indx]->DependedUpon = true;
    else
    if ((indx = SearchTable((char **)derived,nderive, dp->depend[i])) != ERR)
      {
      derived[indx]->DependedUpon = true;
      doubleCheck(derived[indx]);
      }
    }

}	/* END DOUBLECHECK */

/* END DEPEND.C */
