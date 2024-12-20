/*
-------------------------------------------------------------------------
OBJECT NAME:	gust.c

FULL NAME:	Gust calculations

ENTRY POINTS:	swi(), sui(), svi(), sux(), svy()

DESCRIPTION:	3D wind field calculations.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2024
-------------------------------------------------------------------------
*/

#include "sync_reader.hh"
#include <nidas/core/Project.h>

#include "nimbus.h"
#include "amlib.h"

static const int MAX_PROBES = 8;

static const NR_TYPE	THDG_TEST	= 180.0 * M_PI / 180.0;
static const NR_TYPE	PITCH_TEST	= 22.5 * M_PI / 180.0;

static NR_TYPE	ui[MAX_PROBES], vi[MAX_PROBES], ux[MAX_PROBES], vy[MAX_PROBES],
		wind_flag[MAX_PROBES];


static float		boomln[MAX_PROBES];
static NR_TYPE		pitch0[MAX_PROBES][nFeedBackTypes],
			thdg0[MAX_PROBES][nFeedBackTypes],
			DELT[nFeedBackTypes];
static bool		firstTime[MAX_PROBES][nFeedBackTypes];


NR_TYPE defaultATTACK(), defaultSSLIP();


/* -------------------------------------------------------------------- */
bool
getBoomLengthParameter(var_base* varp, float* boomlength)
{
  nidas::core::Project* project = nidas::core::Project::getInstance();

  varlist_t variables;
  variables = selectVariablesFromProject(project);

  nidas::core::Variable* var = lookup_variable(variables, varp->name);
  if (! var)
  {
    WLOG(("get boomlength: variable name not found: %s", varp->name));
    return false;
  }

  const std::list<const nidas::core::Parameter *> parms = var->getParameters();
  std::list<const nidas::core::Parameter *>::const_iterator it;
  for (it = parms.begin(); it != parms.end(); ++it)
  {
    if ((*it)->getName() == "BOOMLENGTH")
    {
      *boomlength = (*it)->getNumericValue(0);
      DLOG(("variable %s has boomlength=%g", varp->name, *boomlength));
      return true;
    }
  }
  ILOG(("variable %s does not have BOOMLENGTH parameter", varp->name));
  return false;
}


/* -------------------------------------------------------------------- */
void initGust(var_base *varp)
{
  NR_TYPE	GetBoomLength(const char *key);
  bool		gustPod = false;
  char		*boom_str = 0;

  if (strstr(varp->name, "_GP"))
    gustPod = true;

  DELT[LOW_RATE_FEEDBACK] = 1.0;
  DELT[HIGH_RATE_FEEDBACK] = 1.0/(float)cfg.ProcessingRate();

  for (int i = 0; i < MAX_PROBES; i++)
  {
    firstTime[i][LOW_RATE_FEEDBACK] = firstTime[i][HIGH_RATE_FEEDBACK] = true;
  }

  memset(pitch0, 0, sizeof(pitch0));
  memset(thdg0, 0, sizeof(thdg0));

  /* Boom length is the distrance from the tip of the radome to the IRS.
   * If we ever start getting heading & pitch from GPS, then use GPS_BOOM_LEN.
   */
  boom_str = "IRS_BOOM_LEN";
  boomln[varp->ProbeCount] = GetBoomLength(boom_str);

  /* Since each aircraft traditionally only had one gust system, the boomlength is
   * stored as a aircraft parameter.  The gustpod is a standalone system and can't
   * be identified as a specific aircraft.  So hack the boomlength here for the
   * time being.
   * @todo  This could be fetched from the nidas XML file after the sync_server/nimbus merge.
   */
  bool blfound = getBoomLengthParameter(varp, &boomln[varp->ProbeCount]);

  if (gustPod && !blfound)
  {
    boomln[varp->ProbeCount] = 0.67;
    ELOG(("Gust pod variable %s missing BOOMLENGTH parameter.\n"
	  "Resorting to the hardcoded default: %g.\n"
	  "Add BOOMLENGTH to the GustPodPPT sensor ID in the NIDAS XML file.",
	  varp->name, boomln[varp->ProbeCount]));
  }

  std::vector<float> bl;
  bl.push_back(boomln[varp->ProbeCount]);
  varp->addToMetadata("BoomLength", bl);

  // Need to figute out how to add this same thing to all the vars calculated in here
  // UI/VI & WS/WD
  varp->addToMetadata("ancillary_variables", "WINDSFLG");
}

/* -------------------------------------------------------------------- */
void initWindsFlag(var_base *varp)
{
  std::vector<int> values;
  values.push_back(0);	// good
  values.push_back(1);	// bad attack
  values.push_back(2);	// bad sslip
  values.push_back(3);	// bad both
  values.push_back(4);	// icing attack
  values.push_back(5);	// icing sslip
  values.push_back(6);	// icing both

  varp->addToMetadata("flag_values", values);
  varp->addToMetadata("flag_meanings", "good bad_attack_angle bad_sideslip_angle bad_both_angles iced_attack iced_sideslip iced_both");

}

/* -------------------------------------------------------------------- */
void swi(DERTBL *varp)
{
  int		probeCnt = varp->ProbeCount;
  NR_TYPE	wi, attack, sslip, tas, thdg, vew, vns, pitch, roll, vspd;
  NR_TYPE	tas_dab, delph, thedot, delth,
		e, f, h, ab, p, r, s, t, psidot, bvns, bvew,
		cs, ss, ch, sh, cr, sr, ta, tb;

  bool		attack_compromised = false, sslip_compromised = false;

  tas	= GetSample(varp, 0);
  vew	= GetSample(varp, 1);
  vns	= GetSample(varp, 2);
  pitch	= GetSample(varp, 3) * DEG_RAD;
  roll	= GetSample(varp, 4);
  thdg	= GetSample(varp, 5) * DEG_RAD;
  attack= GetSample(varp, 6);
  sslip	= GetSample(varp, 7);
  vspd	= GetSample(varp, 8);

  if (std::isnan(pitch) || std::isnan(roll) || std::isnan(thdg) || std::isnan(tas) || std::isnan(vspd))
  {
    ui[probeCnt] = floatNAN;
    vi[probeCnt] = floatNAN;
    ux[probeCnt] = floatNAN;
    vy[probeCnt] = floatNAN;
    wi = floatNAN;
    PutSample(varp, wi);
    return;
  }

  /* Blow-up protection:  output no winds while on-ground (TAS < 30.0 m/s)
       installed by Ron Ruth  18 October 2001 */
  if (tas < 30.0)
  {
    ui[probeCnt] = 0.0;
    vi[probeCnt] = 0.0;
    ux[probeCnt] = 0.0;
    vy[probeCnt] = 0.0;
    wi = 0.0;
    PutSample(varp, wi);
    return;
  }

  if (firstTime[probeCnt][FeedBack])
  {
    if (!std::isnan(pitch))
      pitch0[probeCnt][FeedBack] = pitch;
    if (!std::isnan(thdg))
      thdg0[probeCnt][FeedBack] = thdg;

    firstTime[probeCnt][FeedBack] = FALSE;
  }

  wind_flag[probeCnt] = 0.0;

  // If attack or sslip are nan, use a constant average and set flag.
  if (std::isnan(attack)) {
    attack_compromised = true;
    wind_flag[probeCnt] = 1.0;	// adifr compromised
    if (fabs(roll) <= 2.5)
      attack = defaultATTACK();
  }

  if (std::isnan(sslip)) {
    sslip_compromised = true;
    if (attack_compromised)
      wind_flag[probeCnt] = 3.0;	// both are compromised
    else
      wind_flag[probeCnt] = 2.0;	// bdifr compromised
    if (fabs(roll) <= 2.5)
      sslip = defaultSSLIP();
  }

  attack *= DEG_RAD;
  sslip *= DEG_RAD;
  roll *= DEG_RAD;

  /* Coordinate transformation
   */
  cs	= (NR_TYPE)cos((double)thdg);
  ss	= (NR_TYPE)sin((double)thdg);
  ch	= (NR_TYPE)cos((double)pitch);
  sh	= (NR_TYPE)sin((double)pitch);
  cr	= (NR_TYPE)cos((double)roll);
  sr	= (NR_TYPE)sin((double)roll);
  ta	= (NR_TYPE)tan((double)attack);
  tb	= (NR_TYPE)tan((double)sslip);

  tas_dab	= -tas * 1.0 / (NR_TYPE)sqrt((double)1.0 + ta*ta + tb*tb);
  e	= -tb * (ss * cr - cs * sh * sr);
  f	= cs * sh * cr + ss * sr;
  h	= tb * (cs * cr + ss * sh * sr);
  p	= ss * sh * cr - cs * sr;
  ab	= sh - tb * ch * sr - ta * ch * cr;


  /* Nose boom angular velocity correction terms
   */
  delph	= pitch - pitch0[probeCnt][FeedBack];

  if ((NR_TYPE)fabs((double)delph) >= PITCH_TEST)
  {
    if (delph > 0)
      pitch0[probeCnt][FeedBack] += PITCH_TEST * 2.0;

    if (delph < 0)
      pitch0[probeCnt][FeedBack] -= PITCH_TEST * 2.0;
  }

  thedot = (pitch - pitch0[probeCnt][FeedBack]) / DELT[FeedBack];


  delth	= thdg - thdg0[probeCnt][FeedBack];

  if ((NR_TYPE)fabs((double)delth) >= THDG_TEST)
  {
    if (delth > 0)
      thdg0[probeCnt][FeedBack] += THDG_TEST * 2.0;

    if (delth < 0)
      thdg0[probeCnt][FeedBack] -= THDG_TEST * 2.0;
  }

  psidot = (thdg - thdg0[probeCnt][FeedBack]) / DELT[FeedBack];


  bvns	= boomln[probeCnt] * (psidot * ss * ch + thedot * cs * sh);
  bvew	= boomln[probeCnt] * (thedot * sh * ss - psidot * cs * ch);

  pitch0[probeCnt][FeedBack]	= pitch;
  thdg0[probeCnt][FeedBack]	= thdg;


  /* Compute wind vector components WRT geographic coordinates
   */
  r  = ss * ch + h + ta * p;
  s  = cs * ch + e + ta * f;
  t  = vspd + boomln[probeCnt] * thedot * ch;

/*  Removing this since we have adjusted how we are handling nan values.
 * but left it here for a bit in case we want to revert.  10/15/2024 cjw

  if (std::isnan(attack))	// no longer true with const attack above
  {
    ui[probeCnt] = -tas * sin((thdg+sslip))+vew;
    vi[probeCnt] = -tas * cos((thdg+sslip))+vns;
  }
  else
*/
  {
    ui[probeCnt] = tas_dab * r + (vew - bvew);
    vi[probeCnt] = tas_dab * s + (vns - bvns);
  }

  ux[probeCnt] =  ui[probeCnt] * ss + vi[probeCnt] * cs;
  vy[probeCnt] = -ui[probeCnt] * cs + vi[probeCnt] * ss;

  if (attack_compromised)
    wi = nan("");
  else
    wi = tas_dab * ab + t;

//printf("wi=%g, tas_dab=%g, ab=%g, t=%g, thedot=%g\n", wi, tas_dab, ab, t, thedot);
  PutSample(varp, wi);

}	/* END SWI */

/* -------------------------------------------------------------------- */
void sui(DERTBL *varp)
{
  PutSample(varp, ui[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void svi(DERTBL *varp)
{
  PutSample(varp, vi[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void sux(DERTBL *varp)
{
  PutSample(varp, ux[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void svy(DERTBL *varp)
{
  PutSample(varp, vy[varp->ProbeCount]);
}

/* -------------------------------------------------------------------- */
void swindflg(DERTBL *varp)
{
  PutSample(varp, wind_flag[varp->ProbeCount]);
}

/* END GUST.C */
