/*
-------------------------------------------------------------------------
OBJECT NAME:	rd_blanks.c

FULL NAME:	Read BlankOuts file.

ENTRY POINTS:	ReadBlankOuts()

DESCRIPTION:	Read proj/###/Production/BlanksOuts file, which contains
		start and end times of of variables that are considered
		bad and should be written in with nan.

INPUT:		${PROJ_DIR}/###/Production/BlankOuts

COPYRIGHT:	University Corporation for Atmospheric Research, 2006-2011
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"

static void clear_sacred_variables();

/* -------------------------------------------------------------------- */
void ReadBlankOuts()
{
  char	*bo[1024], target[NAMELEN];

  struct tm st, et;

  if (cfg.BlankoutVariables() == false)
    return;

  sprintf(buffer, "%s.%s", BLANKVARS.c_str(), cfg.FlightNumber().c_str());
  if (AccessProjectFile(buffer, "r") == FALSE)
    return;

  ReadTextFile(buffer, bo);

  for (int i = 0; bo[i]; ++i)
  {
    if (cfg.FlightDate().size() > 0)
    {
      strcpy(buffer, cfg.FlightDate().c_str());
      sscanf(buffer, "%d/%d/%d", &st.tm_mon, &st.tm_mday, &st.tm_year);
      sscanf(buffer, "%d/%d/%d", &et.tm_mon, &et.tm_mday, &et.tm_year);
      st.tm_year -= 1900;
      et.tm_year -= 1900;
      st.tm_mon -= 1;
      et.tm_mon -= 1;
    }

    sscanf(bo[i], "%s %d:%d:%d %d:%d:%d", target,
	&st.tm_hour, &st.tm_min, &st.tm_sec, &et.tm_hour, &et.tm_min, &et.tm_sec);

    std::pair<int, int> tm(mktime(&st), mktime(&et));

    if (strcmp(target, "ALL") == 0)
    {
      printf("Blanking out all raw variables for %s\n", bo[i]);
      for (size_t j = 0; j < raw.size(); ++j)
        raw[j]->blank_out.push_back(tm);
    }
    else
    {
      int index;
      if ((index = SearchTable(raw, target)) != ERR)
        raw[index]->blank_out.push_back(tm);

      if ((index = SearchTable(derived, target)) != ERR)
        derived[index]->blank_out.push_back(tm);
    }
  }

  FreeTextFile(bo);

  clear_sacred_variables();

}	/* END READBLANKOUTS */

/* -------------------------------------------------------------------- */
static void check_var(var_base *var)
{
  if (strcmp(var->name, "YEAR") == 0 ||
      strcmp(var->name, "MONTH") == 0 ||
      strcmp(var->name, "DAY") == 0 ||
      strcmp(var->name, "HOUR") == 0 ||
      strcmp(var->name, "MINUTE") == 0 ||
      strcmp(var->name, "ONE") == 0 ||
      strcmp(var->name, "ZERO") == 0 ||
      strcmp(var->name, "SECOND") == 0)
  {
    char msg[128];
    sprintf(msg, "Blankout can not apply to variable %s, clearing.\n", var->name);
    LogMessage(msg);
    var->blank_out.clear();
  }
}

static void clear_sacred_variables()
{
  for (size_t j = 0; j < raw.size(); ++j)
    check_var(raw[j]);

  for (size_t j = 0; j < derived.size(); ++j)
    check_var(derived[j]);
}

/* END RD_BLANKS.C */
