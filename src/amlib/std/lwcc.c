/*******       CORRECTED C-T LIQUID WATER CONTENT (G/M3)               LWCC
 
        Input:
                lwc - raw liquid water content
                tasx - derived true airspeed
                concf - derived fssp concentration
        Output:
                lwcc - derived liquid water content
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE jwref;        /* reference a/c speed in m/s */
   
/* -------------------------------------------------------------------- */
void slwcc(DERTBL *varp)
{
  NR_TYPE        lwcc, lwc, tasx, concf;
  static int    incld[nFeedBackTypes];
  static NR_TYPE    jwoff[nFeedBackTypes];

  lwc    = GetSample(varp, 0);
  tasx    = GetSample(varp, 1);
  concf    = GetSample(varp, 2);
    
  lwcc = 0.0;

  /* Blow-up protection:  output zero while on ground (TASX < 30.0 m/s)
     installed by Ron Ruth  18 October 2001 */

  if (tasx >= 30.0)
    {
    if (concf > 10.0)
      incld[FeedBack] = 0;

    if (incld[FeedBack] < 300)
      ++incld[FeedBack];

    if (incld[FeedBack] > 10)
      jwoff[FeedBack] = jwoff[FeedBack] * 0.95 + lwc * 0.05;

/*      if (tasx >= 30.0)   */
    lwcc = (lwc - jwoff[FeedBack]) * jwref / tasx;
    }

  PutSample(varp, lwcc);

}    /* END SLWCC */

/* END LWCC.C */
