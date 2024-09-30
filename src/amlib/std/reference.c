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
 * If the requirement for pressures/PCORs goes away, then this can move back
 * to just using sRefer()
 */
void sReferAttack(DERTBL *varp)
{
  static NR_TYPE attack[nFeedBackTypes] = { 0.0, 0.0 };
  static int attack_miss_cnt[nFeedBackTypes] = { 0, 0 };
  static const int ATTACK_MISS_MAX[nFeedBackTypes] = { 2, 2 * Config::HighRate };

  // Fallback value for Attack if it goes missing.
  static const NR_TYPE GV_ATTACK_DEFAULT_VALUE = 3.0;
  static const NR_TYPE C130_ATTACK_DEFAULT_VALUE = 2.0;


  NR_TYPE aoa = GetSample(varp, 0);

  if (std::isnan(aoa))
  {
    NR_TYPE substitute_attack = cfg.Aircraft() == Config::HIAPER ?
			GV_ATTACK_DEFAULT_VALUE : C130_ATTACK_DEFAULT_VALUE;

    // Repeat prev attack up to two samples if missing, otherwise set to 3.0.
    if (++attack_miss_cnt[FeedBack] > ATTACK_MISS_MAX[FeedBack])
      attack[FeedBack] = substitute_attack;
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

  // Fallback value for Attack if it goes missing.
  static const NR_TYPE GV_SSLIP_DEFAULT_VALUE = nan("");
  static const NR_TYPE C130_SSLIP_DEFAULT_VALUE = 1.1;


  NR_TYPE slipin = GetSample(varp, 0);

  if (std::isnan(slipin))
  {
    NR_TYPE substitute_sslip = cfg.Aircraft() == Config::HIAPER ?
			GV_SSLIP_DEFAULT_VALUE : C130_SSLIP_DEFAULT_VALUE;

    // Repeat prev sslip up to two samples if missing, otherwise set to 3.0.
    if (++sslip_miss_cnt[FeedBack] > SSLIP_MISS_MAX[FeedBack])
      sslip[FeedBack] = substitute_sslip;
  }
  else
  {
    sslip_miss_cnt[FeedBack] = 0;
    sslip[FeedBack] = slipin;
  }

  PutSample(varp, sslip[FeedBack]);
}

/* END REFERENCE.C */
