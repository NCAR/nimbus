/*
-------------------------------------------------------------------------
OBJECT NAME:	depend.c

FULL NAME:	Setup Dependencies

ENTRY POINTS:	SetupDependencies()
		CleanOutUnwantedVariables()
		DependIndexLookup()

STATIC FNS:	doubleCheck()

DESCRIPTION:	

REFERENCED BY:	hdr_decode.c, cb_main.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include <raf/vardb.hh>
#include "amlibProto.h"

char	*DependMsg =
	"%s is depending on non-existent variable %s, turning off.\n";

static void	doubleCheck(DERTBL *dp), setupDependencies(bool set_depends);

static std::vector<std::string> dcList;

extern VDBFile *vardb;


/* -------------------------------------------------------------------- */
void ReadDependencies()
{
  size_t	j;
  char		tokens[] = " \t";
  char		*dependlist[2000];
  char		*s, name[NAMELEN];

  ReadTextFile(DEPENDTBL, dependlist);

  for (size_t i = 0; i < derived.size(); ++i)
  {
    DERTBL *dp = derived[i];
    buffer[0] = '\0';

    strcpy(name, dp->name);

    // Retrieve dependency list.
    if (cfg.isADS3())
    {
      VDBVar *vdb_var = vardb->get_var(dp->name);
      if (vdb_var)
        sprintf(buffer, "%s\t%s", dp->name, 
		vdb_var->get_attribute(VDBVar::DEPENDENCIES).c_str());
    }
    else
    {
      if ((s = strrchr(name, '_')) != NULL)
        *s = '\0';

      s = SearchList(dependlist, name);
      if (s)
        strcpy(buffer, s);
    }

    if (strlen(buffer) == 0)
    {
      sprintf(buffer, "%s has no dependencies, turning off.\n", name);
      LogMessage(buffer);
      dp->Output = false;
      dp->compute = (void(*)(void *))smissval;
      continue;
    }

    s = strtok(buffer, tokens);


    // Process dependencies.
    for (j = 0; (s = strtok((char *)NULL, tokens)); ++j)
      strcpy(dp->depend[j], s);

    dp->ndep = j;
  }

  FreeTextFile(dependlist);

  setupDependencies(false);
}

/* -------------------------------------------------------------------- */
void SetupDependencies()
{
  ILOG(("SetupDependencies"));
  setupDependencies(true);
}

/* -------------------------------------------------------------------- */
static void setupDependencies(bool set_depends)
{
  size_t	j;
  char		*s, name[NAMELEN], location[NAMELEN];

  for (size_t i = 0; i < derived.size(); ++i)
  {
    DERTBL *dp = derived[i];
    dp->depends.clear();

    strcpy(name, dp->name);

    if ((s = strrchr(name, '_')) != NULL)
    {
      strcpy(location, s);
      *s = '\0';
    }
    else
      location[0] = '\0';

    for (j = 0; j < dp->ndep; ++j)
    {
      /* We need to check both cases of whether dependency needs the
       * location tacked on.  (e.g. CFSSP depends on TASX, we do not
       * want location tacked onto TASX.  Start with location.
       */
      strcat(dp->depend[j], location);
      if (DependIndexLookup(dp, j, set_depends) == ERR)
      {
        /* Did not find variable with location, try without.
         */
        char *p = strrchr(dp->depend[j], '_');
        if (p)
          *p = '\0';

        if (DependIndexLookup(dp, j, set_depends) == ERR)
        {
          /* Make one last check on the PMS1D stuff.  We want all the same
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

          if (DependIndexLookup(dp, j, set_depends) == ERR)
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
  }
}	/* END SETUPDEPENDANCIES */

/* -------------------------------------------------------------------- */
void CleanOutUnwantedVariables()
{
  size_t i, cnt;

  for (i = 0; i < derived.size(); ++i)
    if (derived[i]->Output)
    {
      dcList.clear();
      doubleCheck(derived[i]);
    }

  for (cnt = 0, i = 0; i < raw.size(); ++i)
    if (raw[i]->Output || raw[i]->DependedUpon)
      raw[cnt++] = raw[i];

  raw.resize(cnt);

  for (cnt = 0, i = 0; i < derived.size(); ++i)
    if (derived[i]->Output || derived[i]->DependedUpon)
      derived[cnt++] = derived[i];

  derived.resize(cnt);

}	/* CLEANOUTUNWANTEDVARIABLES */

/* -------------------------------------------------------------------- */
int DependIndexLookup(DERTBL *dp, int which_dep, bool init_depends)
{
  int di;

  if ((di = SearchTable(raw, dp->depend[which_dep])) != ERR)
  {
    if (init_depends)
      dp->depends.push_back(raw[di]);
  }
  else
  if ((di = SearchTable(derived, dp->depend[which_dep])) !=ERR)
  {
    if (init_depends)
      dp->depends.push_back(derived[di]);
  }
  else
    return(ERR);

  return(OK);

}	/* END DEPENDINDEXLOOKUP */

/* -------------------------------------------------------------------- */
bool isDependedUpon(DERTBL *dp)
{
  for (size_t i = 0; i < derived.size(); ++i)
    for (size_t j = 0; j < derived[i]->ndep; ++j)
      if (strcmp(dp->name, derived[i]->depend[j]) == 0)
        return true;

  return false;
}

/* -------------------------------------------------------------------- */
static void doubleCheck(DERTBL *dp)	/* This function is recursive	*/
{
  int indx;

  dcList.push_back(dp->name);
  if (dcList.size() > 1000)
  {
    fprintf(stderr, "depend.c::doublCheck infinite loop, stack = \n\n");
    for (size_t i = 0; i < dcList.size(); ++i)
      fprintf(stderr, "%s ", dcList[i].c_str());
    fprintf(stderr, "\n\n");
    quit();
  }

  for (size_t i = 0; i < dp->ndep; ++i)
  {
    if ((indx = SearchTable(raw, dp->depend[i])) != ERR)
      raw[indx]->DependedUpon = true;
    else
    if ((indx = SearchTable(derived, dp->depend[i])) != ERR)
    {
      derived[indx]->DependedUpon = true;
      doubleCheck(derived[indx]);
    }
  }
}	/* END DOUBLECHECK */

/* END DEPEND.C */
