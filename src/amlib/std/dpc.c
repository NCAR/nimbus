/**************************** DPC *********************************** DPC 
    Dewpoint temperature (Thermoelec) (C)
   DPCOR equation computes difference between dew point and frost point
   derived from Goff/Gratch vapor pressure equations for plane water and
   ice surfaces. Final corrected dew point (DPC) is with respect to a plane
   water surface.  The accuracy of conversion (one sigma) is 0.02C over
   range of 0C to -80 C.
        Input:
                dp - raw dew point
        Output:
                dpc - corrected dewpoint temp
*/
   
#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void sdpc(DERTBL *varp)
{
	NR_TYPE	dp;

	dp = GetSample(varp, 0);
	
	if (dp < 0.0)
		dp = 0.009109+dp*(1.134055+dp*0.001038);

	PutSample(varp, dp);

}	/* END SDPC */

/* END DPC.C */
