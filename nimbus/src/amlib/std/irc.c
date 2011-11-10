/*                  CORRECTED PYRGEOMETER DATA MODULE                     *
 *                                                                        *
 ** CORRECTED PYRGEOMTER IRRADIANCE (WATTS/M2)
        Input:
                ir - raw pyrgeometer data
                st - raw pyrg. sink data
                dt - raw pyrg. dome data
        Output:
                irc - derived pyrgeometer irradiance (w/m2)
        Include:
                irc.data - Silicon dome constants
        Endspec:

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

std::map<std::string, std::vector<double> > cals;

/* -------------------------------------------------------------------- */
void initPyrgeometer(var_base *varp)
{
  float	*tmp;
  char	name[100];
  std::vector<double> cal;

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

  cals[varp->name] = cal;
}

/* -------------------------------------------------------------------- */
void sirc(DERTBL *varp)
{
  NR_TYPE ir = GetSample(varp, 0);

  if (varp->ndep == 2)	// New CGR4 Pyrgeometer (2011).
  {
    NR_TYPE dt = GetSample(varp, 1);

    /* Since the cal for Dome Temp has a log in it and I didn't want to
     * create an interim variable, we will apply the cal here and back-write
     * DTT and DTB.
     */
    dt = 26.505 + -61.487 * log10(dt);
    PutSample(varp->depends[1], dt);
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
