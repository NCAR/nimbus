/*                  CORRECTED PYRGEOMETER DATA MODULE                     *
 *                                                                        *
 ** CORRECTED EPPLEY PYRGEOMTER IRRADIANCE (WATTS/M2)
        Input:
                ir - raw pyrgeometer data
                st - raw pyrg. sink data
                dt - raw pyrg. dome data

 ** CORRECTED CGR4 PYRGEOMTER IRRADIANCE (WATTS/M2)
        Input:
                ir - raw pyrgeometer data
                dt - raw pyrg. dome data
        Output:
                irc - derived pyrgeometer irradiance (w/m2)

*/

/*  NOTE!!! XKB=XKT when this routine created.  If one is changed a separate
	 routine must be created for each pyrgeometer with corresponding
	 changes in dependency and setup tables
    Data pertaining to pyrgeometer corrections  (IRT,IRB)           IRT,IRB
    Silicon dome constants for pyrgeometers     (IRT,IRB)
*/

#include "nimbus.h"
#include "amlib.h"
#include <map>

#define XKT  5.5
#define XKB  5.5
#define E0   0.986

static const double SBC = 5.6686E-8;

static std::vector<float> dome_temp_cal;

static std::map<std::string, std::vector<float> > cals;
static std::map<std::string, std::vector<float> > pitch_threshold;
static std::map<std::string, float> roll_threshold;

/* -------------------------------------------------------------------- */
void initPyrgeometer(var_base *varp)
{
  DERTBL *dp = (DERTBL *)varp;
  
  roll_threshold[varp->name] = 90;
  pitch_threshold[varp->name].push_back(-90);
  pitch_threshold[varp->name].push_back(90);

  /* If old Eppley processing, then leave.  Eppley's had two dependencies,
   * sink tempearture and dome temperature.  The new CGR4 has just dome
   * temperature.
   */
  if (strchr(dp->depends[0]->name,'V')==0) // If old Eppley processing, then leave.
  {
    sprintf(buffer, "irc.c: Pyrgeometer, no 'V' detected in first variable, setting to Eppley.");
    LogMessage(buffer);
    return;
  }
  
  float	*tmp;
  char	name[100];
  std::vector<float> cal;

  sprintf(name, "%s_CAL", varp->name);
  if ((tmp = GetDefaultsValue(name, varp->name)) == NULL)
  {
    sprintf(buffer, "irc.c: Pyrgeometer, no %s in defaults file.", name);
    LogMessage(buffer);
    cal.push_back(floatNAN);
    cal.push_back(floatNAN);
  }
  else
  {
    cal.push_back(tmp[0]);
    cal.push_back(tmp[1]);
  }
  varp->addToMetadata("Coefficients", cal);

  cals[varp->name] = cal;

  // Get cals for IRTHT/IRBHT.
  cal.clear();
  sprintf(name, "%s_CAL", dp->depend[1]);
  if ((tmp = GetDefaultsValue(name, dp->depend[1])) == NULL)
  {
    sprintf(buffer, "irc.c: Pyrgeometer, no %s in defaults file.", name);
    LogMessage(buffer);
    cal.push_back(26.505);
    cal.push_back(-61.487);
  }
  else
  {
    cal.push_back(tmp[0]);
    cal.push_back(tmp[1]);
  }
  dp->depends[1]->addToMetadata("Coefficients", cal);

  dome_temp_cal = cal;

  sprintf(name, "%s_PITCH_THRESHOLD", varp->name);
  if ((tmp = GetDefaultsValue(name,varp->name)))
  {
    pitch_threshold[varp->name].clear();
    pitch_threshold[varp->name].push_back(tmp[0]);
    pitch_threshold[varp->name].push_back(tmp[1]);
  }

  sprintf(name, "%s_ROLL_THRESHOLD", varp->name);
  if ((tmp = GetDefaultsValue(name, varp->name)))
  {
    roll_threshold[varp->name] = tmp[0];
  }
}

/* -------------------------------------------------------------------- */
void sirc(DERTBL *varp)
{
  NR_TYPE ir = GetSample(varp, 0);

  if (strchr(varp->depends[0]->name,'V'))	// New CGR4 Pyrgeometer (2011).
  {
    bool fail_test = false;
    NR_TYPE dt = GetSample(varp, 1);

    /* Since the cal for Dome Temp has a log in it and I didn't want to
     * create an interim variable, we will apply the cal here and back-write
     * IRTHT and IRBHT (formerly known DTT/DTB).
     */
    dt = dome_temp_cal[0] + dome_temp_cal[1] * log10(dt);
    PutSample(varp->depends[1], dt);  // this does not write into SampleRateData, just AveragedData.

    // Pitch & roll thershold tests, if defined in Defaults file.
    if (varp->nDependencies == 4)
    {
      NR_TYPE pitch = GetSample(varp, 2);
      NR_TYPE roll = GetSample(varp, 3);
      if (!std::isnan(roll) && fabs(roll) > roll_threshold[varp->name])
        fail_test = true;
      if (!std::isnan(pitch)&&(pitch < (pitch_threshold[varp->name])[0] ||
          pitch > (pitch_threshold[varp->name])[1]))
        fail_test = true;
    }

    if (fail_test)
      PutSample(varp, floatNAN);
    else
      PutSample(varp, ((cals[varp->name])[0] * ir - (cals[varp->name])[1]) + SBC * pow(dt+Kelvin, 4.0));
  }
  else	// Eppley Pyrgeometer (up to about 2005).
  {
    NR_TYPE dscor,tcor;
    NR_TYPE st = pow(GetSample(varp, 1) + Kelvin, 4.0);
    NR_TYPE dt = pow(GetSample(varp, 2) + Kelvin, 4.0);

    /* Pyrgeometer dome and sink temperature corrections        dscor
    */
    dscor = XKB * SBC * (dt - st);

    /* Pyrgeometer sink temperature compensation corrections    tcor
     */
    tcor = E0 * SBC * st;

    /* Corrected upward i. r. irradiance (watts/m2)             irc
     */
    PutSample(varp, ir - dscor + tcor);
  }
}
