/*
-------------------------------------------------------------------------
OBJECT NAME:    atfh.c

FULL NAME:      Ambient Temperature Wrapper

ENTRY POINTS:   satfh()

STATIC FNS:     none

DESCRIPTION:    Ambient Temperature for anti-iced sensors:
			GV	Harco anti-iced sensor
			C130	Rosemount 102 heated

COPYRIGHT:      University Corporation for Atmospheric Research, 1992-2008
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

extern NR_TYPE	recfrh;
extern NR_TYPE	tfher1,tfher2;	/* set in initAC.c */

static const int MAX_TT = 8;
static int ProbeCount = 0;

static NR_TYPE atfh[nFeedBackTypes][MAX_TT];

/* -------------------------------------------------------------------- */
void atfhInit(var_base *varp)
{
  int	indx;

  if (ProbeCount > MAX_TT)
  {
    fprintf(stderr, "\natfh.c: atfhInit: MAX_TT exceeded, get a programmer to fix.  Fatal.\n");
    exit(1);
  }


  std::vector<NR_TYPE> values;
  values.push_back(recfrh);
  AddToDefaults(varp->name, "RecoveryFactor", values);

  /* Frequently ProbeCount gets set in hdr_decode.c, but we are doing it here for
   * this instrument.
   */
  if ((indx = SearchTable(raw, ((DERTBL *)varp)->depend[0])) != -1)
  {
    raw[indx]->ProbeCount = ProbeCount;
  }
  else
  if ((indx = SearchTable(derived, ((DERTBL *)varp)->depend[0])) != -1)
  {
    derived[indx]->ProbeCount = ProbeCount;
  }
  else
  {
    fprintf(stderr, "\natfh.c: atfhInit: Can't find %s, shouldn't happen.  Fatal.\n",
		((DERTBL *)varp)->depend[0]);
    exit(1);
  }

  varp->ProbeCount = ProbeCount;
  ++ProbeCount;

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void tthcInit(var_base *varp)
{
  memset(atfh, 0, sizeof(atfh));
  std::vector<NR_TYPE> values;
  values.push_back(tfher2);
  values.push_back(tfher1);
  AddToDefaults(varp->name, "Calibrations", values);

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void sttwhc(DERTBL *varp)
{
  NR_TYPE zee, tth, xmach2, psxc;
  static bool firstTime[nFeedBackTypes] = { true, true };

  tth = GetSample(varp, 0);
  xmach2 = GetSample(varp, 1);

  if (isnan(tth) || isnan(xmach2))
  {
    PutSample(varp, floatNAN);
    return;
  }


  if (firstTime[FeedBack])
  {
    firstTime[FeedBack] = false;
    atfh[FeedBack][varp->ProbeCount] = tth;
  }

  if (tth < -273.15)
    tth = -273.15;

  if (xmach2 <= 0.0 || isnan(xmach2))
    xmach2 = 0.0001;

  switch (cfg.Aircraft())
  {
    case Config::HIAPER:
      if (xmach2 < 0.25)
        zee = 1.00202; 
      else
        zee = (0.99355 + 0.0097071 * sqrt(xmach2) + 0.014429 * xmach2); 

      tth = ((tth + Kelvin) * zee) - 273.15; 
      break;

    default:
      psxc = GetSample(varp, 2);

      zee = 0.269589 * psxc * sqrt((double)xmach2) / (atfh[FeedBack][varp->ProbeCount] + 273.16);

      if (zee < 0.18 || isnan(zee))
        zee = 0.18;

      tth -= (NR_TYPE)pow((double)10.0,
			(double)tfher1 * log10((double)zee) + tfher2);
    }

  if (tth < -273.15)
    tth = -273.15;

  PutSample(varp, tth);
}

/* -------------------------------------------------------------------- */
void satfh(DERTBL *varp)
{
  NR_TYPE ttfh, xmach2;

  ttfh = GetSample(varp, 0);
  xmach2 = GetSample(varp, 1);

  atfh[FeedBack][varp->ProbeCount] = AMBIENT(ttfh, (NR_TYPE)recfrh, xmach2);

  PutSample(varp, atfh[FeedBack][varp->ProbeCount]);

}	/* END SATFH */

/* END ATFH.C */
