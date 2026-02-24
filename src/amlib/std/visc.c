/*
-------------------------------------------------------------------------
OBJECT NAME:	visc.c

FULL NAME:	Visable Radiometer Corrections

ENTRY POINTS:	svis()

DESCRIPTION:	Perform PITCH/ROLL limit blanking for stabilized platform.
		As of Sept 2024 only applies to C130.
		To engage this, the raw variable names require a
		trailing 'V' (e.g. VISTV, not VIST).

COPYRIGHT:	University Corporation for Atmospheric Research, 2024
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include <map>

static std::map<std::string, std::vector<float> > pitch_threshold;
static std::map<std::string, float> roll_threshold;

/* -------------------------------------------------------------------- */
void initVisPyrgeometer(var_base *varp)
{
  DERTBL *dp = (DERTBL *)varp;

  roll_threshold[varp->name] = 90.0;
  pitch_threshold[varp->name].push_back(-90.0);
  pitch_threshold[varp->name].push_back(90.0);

  float	*tmp;
  char	name[100];
  sprintf(name, "SP_%s_PITCH_THRESHOLD",
		strchr(varp->name, 'B') ? "BOTTOM" : "TOP");
  if ((tmp = GetDefaultsValue(name, varp->name)))
  {
    pitch_threshold[varp->name].clear();
    pitch_threshold[varp->name].push_back(tmp[0]);
    pitch_threshold[varp->name].push_back(tmp[1]);
    dp->addToMetadata("PitchThreshold", pitch_threshold[varp->name]);
  }

  strcpy(name, "SP_ROLL_THRESHOLD");
  if ((tmp = GetDefaultsValue(name, varp->name)))
  {
    roll_threshold[varp->name] = tmp[0];
    dp->addToMetadata("RollThreshold", tmp[0]);
  }
}

/* -------------------------------------------------------------------- */
void svisc(DERTBL *varp)
{
  NR_TYPE vis = GetSample(varp, 0);

  bool fail_test = false;

  // Pitch & roll thershold tests, if defined in Defaults file.
  NR_TYPE pitch = GetSample(varp, 1);
  NR_TYPE roll = GetSample(varp, 2);

  if (!std::isnan(roll) && fabs(roll) > roll_threshold[varp->name])
      fail_test = true;
  if (!std::isnan(pitch)&&(pitch < (pitch_threshold[varp->name])[0] ||
      pitch > (pitch_threshold[varp->name])[1]))
      fail_test = true;

  if (fail_test)
    PutSample(varp, floatNAN);
  else
  {
    if (vis < 0.0)
      vis = 0.0;

    PutSample(varp, vis);
  }
}
