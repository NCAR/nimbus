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
 * then set to 3.0 if it continues as nan past that.  The 3.0 comes from a
 * Al Cooper memo October 3, 2014.
 *
 * Setting to 3.0 has been moved in the pressure functions (or point of use.
 * Revert to nan after two second carryover.  Also added similar for sslip.
 * cjw 10/2024
 *
 * If the requirement for pressures/PCORs goes away, then this can move back
 * to just using sRefer()
 */
void sReferAttack(DERTBL *varp)
{
  static NR_TYPE attack[nFeedBackTypes] = { 0.0, 0.0 };
  static int attack_miss_cnt[nFeedBackTypes] = { 0, 0 };
  static const int ATTACK_MISS_MAX[nFeedBackTypes] = { 2, 2 * Config::HighRate };

  NR_TYPE aoa = GetSample(varp, 0);

  if (std::isnan(aoa))
  {
    // Repeat prev attack up to two samples if missing.
    if (++attack_miss_cnt[FeedBack] > ATTACK_MISS_MAX[FeedBack])
      attack[FeedBack] = aoa;
  }
  else
  {
    attack_miss_cnt[FeedBack] = 0;
    attack[FeedBack] = aoa;
  }

  PutSample(varp, attack[FeedBack]);
}


void sReferSSlip(DERTBL *varp)
{
  static NR_TYPE sslip[nFeedBackTypes] = { 0.0, 0.0 };
  static int sslip_miss_cnt[nFeedBackTypes] = { 0, 0 };
  static const int SSLIP_MISS_MAX[nFeedBackTypes] = { 2, 2 * Config::HighRate };

  NR_TYPE slipin = GetSample(varp, 0);

  if (std::isnan(slipin))
  {
    // Repeat prev sslip up to two samples if missing.
    if (++sslip_miss_cnt[FeedBack] > SSLIP_MISS_MAX[FeedBack])
      sslip[FeedBack] = slipin;
  }
  else
  {
    sslip_miss_cnt[FeedBack] = 0;
    sslip[FeedBack] = slipin;
  }

  PutSample(varp, sslip[FeedBack]);
}

/* END REFERENCE.C */
