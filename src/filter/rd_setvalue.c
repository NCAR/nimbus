/*
-------------------------------------------------------------------------
OBJECT NAME:	rd_setvalue.c

FULL NAME:	Read SetValue file.

ENTRY POINTS:	ReadSetValue()

DESCRIPTION:	Read proj/###/Production/SetValue file, which contains
		start and end times of of variables that are masked
		should be set to a constant.

INPUT:		${PROJ_DIR}/###/Production/SetValue

COPYRIGHT:	University Corporation for Atmospheric Research, 2024
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"

static void clear_sacred_variables();

/* -------------------------------------------------------------------- */
void ReadSetValues(std::string fileName, bool chk_config)
{
  float value;
  char	*bo[1024], target[NAMELEN];

  struct tm st, et;
  memset(&st, 0, sizeof(struct tm));
  memset(&et, 0, sizeof(struct tm));

  if (chk_config && cfg.BlankoutVariables() == false)
    return;

  snprintf(buffer, 8192, "%s.%s", fileName.c_str(), cfg.FlightNumber().c_str());
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


    // Test for hh:mm:ss vs hhmmss
    if ( strchr(bo[0], ':') )
      sscanf(bo[i], "%s %d:%d:%d %d:%d:%d %f", target,
	&st.tm_hour, &st.tm_min, &st.tm_sec, &et.tm_hour, &et.tm_min, &et.tm_sec, &value);
    else
      sscanf(bo[i], "%s %02d%02d%02d %02d%02d%02d %f", target,
	&st.tm_hour, &st.tm_min, &st.tm_sec, &et.tm_hour, &et.tm_min, &et.tm_sec, &value);

    SETVAL sv;
    sv.sTime[0] = st.tm_hour;
    sv.sTime[1] = st.tm_min;
    sv.sTime[2] = st.tm_sec;
    sv.eTime[0] = et.tm_hour;
    sv.eTime[1] = et.tm_min;
    sv.eTime[2] = et.tm_sec;
    sv.start = mktime(&st);
    sv.end   = mktime(&et);
    if(chk_config == true)
      sv.value = nan(""); 
    else
      sv.value = value; 

    if (strcmp(target, "ALL") == 0)
    {
      printf("Setting %s\n to constant value %f", bo[i], value);
      for (size_t j = 0; j < raw.size(); ++j)
        raw[j]->set_value.push_back(sv);
    }
    else
    {
      int index;
      if ((index = SearchTable(raw, target)) != ERR)
      {
	// raw vars are handled in rec_decode.c, using epoch time tests
        raw[index]->set_value.push_back(sv);
      }

      if ((index = SearchTable(derived, target)) != ERR)
      {
	// derived vars are handled in NetCDF.cc and is expecting seconds since midnight
	sv.start = SecondsSinceMidnight(sv.sTime);
	sv.end = SecondsSinceMidnight(sv.eTime);
	if(std::isnan(sv.value))
          sv.value=(float)MISSING_VALUE;
        derived[index]->set_value.push_back(sv);
      }
    }
  }

  FreeTextFile(bo);

  clear_sacred_variables();

}	/* END READSETVALUE*/

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
    snprintf(msg, 128, "SetValue can not apply to variable %s, clearing.\n", var->name);
    LogMessage(msg);
    var->set_value.clear();
  }
}

static void clear_sacred_variables()
{
  for (size_t j = 0; j < raw.size(); ++j)
    check_var(raw[j]);

  for (size_t j = 0; j < derived.size(); ++j)
    check_var(derived[j]);
}

/* END RD_SETVALUE.C */
