/*
-------------------------------------------------------------------------
OBJECT NAME:	reference.c

DESCRIPTION:	This function is used for straight assignment of one variable
		to another.  Allows a second backup parameter to be passed in
		also.  If the primary parameter is nan, then the backup is used.

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-2020
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void sRefer(DERTBL *varp)
{
  NR_TYPE x = GetSample(varp, 0);

  // If we have a secondary, use it if primary is nan.
  if (std::isnan(x) && varp->nDependencies > 1)
    x = GetSample(varp, 1);

  PutSample(varp, x);

}	/* END SREFER */

/* -------------------------------------------------------------------- */
/* PCORS use attack.  If attack goes nan, carry previous value for 2 seconds,
 * then set to 3.0 if it continues as nan past that.
 * If the requirement for pressures/PCORs goes away, then this can move back
 * to just using sRefer()
 */
void sReferAttack(DERTBL *varp)
{
  static NR_TYPE attack[nFeedBackTypes] = { 0.0, 0.0 };
  static int attack_miss_cnt[nFeedBackTypes] = { 0, 0 };
  static const int ATTACK_MISS_MAX[nFeedBackTypes] = { 2, 2 * Config::HighRate };
  // Fallback value for Attack if it goes missing.
  static const NR_TYPE ATTACK_DEFAULT_VALUE = 3.0;


  NR_TYPE x = GetSample(varp, 0);

  if (std::isnan(x))
  {
    // Repeat prev attack up to two samples if missing, otherwise set to 3.0.
    if (++attack_miss_cnt[FeedBack] > ATTACK_MISS_MAX[FeedBack])
      attack[FeedBack] = ATTACK_DEFAULT_VALUE;
  }
  else
  {
    attack_miss_cnt[FeedBack] = 0;
    attack[FeedBack] = x;
  }


  PutSample(varp, attack[FeedBack]);
}

/* END REFERENCE.C */
