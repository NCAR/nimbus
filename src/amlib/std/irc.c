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

#define XKT  5.5
#define XKB  5.5
#define E0   0.986
#define SBC  5.6686E-8
   
/* -------------------------------------------------------------------- */
void sirc(DERTBL *varp)
{
  NR_TYPE	ir,st,dt;
  NR_TYPE	dscor,tcor;

  ir = GetSample(varp, 0);
  st = GetSample(varp, 1);
  dt = GetSample(varp, 2);
	
  /* Pyrgeometer dome and sink temperature corrections        dscor
  */
  dscor = XKB*SBC*(pow((double)(dt+273.16),(double)4.0) -
		 pow((double)(st+273.16),(double)4.0));

  /* Pyrgeometer sink temperature compensation corrections    tcor
   */
  tcor = E0*SBC*pow((double)(st+273.16),(double)4.0);

  /* Corrected upward i. r. irradiance (watts/m2)             irc
   */
  PutSample(varp, ir - dscor + tcor);

}	/* END SIRC */

/* END IRC.C */
