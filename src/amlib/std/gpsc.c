/*
-------------------------------------------------------------------------
OBJECT NAME:	gpsc.c

FULL NAME:	Corrected IRS/GPS

ENTRY POINTS:	slatc(), slonc(), svnsc(), svewc()

DESCRIPTION:	Create a corrected set of Reference variables by combining
		the IRS and GPS data.  Low Pass filter the GPS and High
		Pass filter the INS, to remove the Schuler oscillation.

INPUT:		LAT LON GLAT GLON VNS VEW GVNS GVEW ROLL GSTAT GMODE

OUTPUT:		LATC LONC VNSC VEWC

REFERENCE:	Electra Measurements of Wind and Position in HaRP.
		William Cooper.
		Latest co-efficients and constants are derived Cooper Wind
		Uncertainty document May 2015.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2015
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include <cassert>
#include <values.h>

static NR_TYPE
	UPFCTR	= 0.999444,
	FCTRF	= 0.98,
	TAUP	= 300.0,
	TAU	= 300.0,
	CDM	= 111120.0,
	ROLL_MAX = 40.0;

static const int NCF = 3;

static NR_TYPE	latc[nFeedBackTypes], lonc[nFeedBackTypes],
		vnsc[nFeedBackTypes], vewc[nFeedBackTypes];

static NR_TYPE	deltaT[nFeedBackTypes], factorp[nFeedBackTypes],
		dlat[nFeedBackTypes], dlon[nFeedBackTypes],
		dvy[nFeedBackTypes], dvx[nFeedBackTypes],
		time_duration[nFeedBackTypes], fctrf[nFeedBackTypes];

static double	h[NCF][NCF], zf[nFeedBackTypes][4][6];
static double	am[2][NCF], bm[2][NCF], c[2][NCF], cp[2][NCF];

static NR_TYPE	filter(double, double *);
static double	invert(double a[][NCF]);

static bool returnMissingValue = false;


/* -------------------------------------------------------------------- */
void initLATC(var_base *varp)
{
  float  *tmp;

  /* Get constant values from Defaults file, if available
   */
  if ((tmp = GetDefaultsValue("GPS_ROLL_MAX", varp->name)) == NULL)
  {
    sprintf(buffer, "GPS_ROLL_MAX set to %f in AMLIB function slatc.\n", ROLL_MAX);
    LogMessage(buffer);
  }
  else
    ROLL_MAX = tmp[0];

  std::vector<float> rollmax;
  rollmax.push_back(ROLL_MAX);
  varp->addToMetadata("ROLL_MAX", rollmax);

  // At this time do not allow over-rides in the Defaults file.  cjw 5/15
/*
  if ((tmp = GetDefaultsValue("GPS_TAUP", varp->name)) == NULL)
  {
    sprintf(buffer, "GPS_TAUP set to %f in AMLIB function slatc.\n", TAUP);
    LogMessage(buffer);
  }
  else
    TAUP = tmp[0];

  if ((tmp = GetDefaultsValue("GPS_TAU", varp->name)) == NULL)
  {
    sprintf(buffer, "GPS_TAU set to %f in AMLIB function slatc.\n", TAU);
    LogMessage(buffer);
  }
  else
    TAU = tmp[0];

  if ((tmp = GetDefaultsValue("GPS_UPFCTR", varp->name)) == NULL)
  {
    sprintf(buffer, "GPS_UPFCTR set to %f in AMLIB function slatc.\n", UPFCTR);
    LogMessage(buffer);
  }
  else
    UPFCTR = tmp[0];

  if ((tmp = GetDefaultsValue("GPS_FCTRF", varp->name)) == NULL)
  {
    sprintf(buffer, "GPS_FCTRF set to %f in AMLIB function slatc.\n", FCTRF);
    LogMessage(buffer);
  }
  else
    FCTRF = tmp[0];
*/

  memset((char *)zf, 0, sizeof(zf));
  memset((char *)am, 0, sizeof(am));
  memset((char *)bm, 0, sizeof(bm));
  memset((char *)h, 0, sizeof(h));
  memset((char *)c, 0, sizeof(c));
  memset((char *)cp, 0, sizeof(cp));

  for (int i = 0; i < nFeedBackTypes; ++i)
    {
    if (i == LOW_RATE_FEEDBACK)
      {
      deltaT[i] = 1.0;
      fctrf[i] = FCTRF;
      }
    else
      {
      float r = (float)(cfg.ProcessingRate() - 1) / cfg.ProcessingRate();
      fctrf[i] = FCTRF + ((1.0 - FCTRF) * r);
      deltaT[i] = 1.0 / (float)cfg.ProcessingRate();
      }

    factorp[i] = 0.002 * (1000.0 / deltaT[i]) * M_PI / TAUP;

    time_duration[i] = dvy[i] = dvx[i] = dlat[i] = dlon[i] = 0.0;
    }

}	/* END INITLATC */

/* -------------------------------------------------------------------- */
void slatc(DERTBL *varp)
{
  int		i, j, k;
  NR_TYPE	alat, alon, vew, vns, roll, glat, glon, gvew, gvns;
  NR_TYPE	omegat, sinwt, coswt;
  /*NR_TYPE       gvnsf, gvewf, vnsf, vewf;*/
  long		nSatellites, gps_quality;

  double	det;

  static bool	firstTime[nFeedBackTypes] = { true, true },
		matrix_updated[nFeedBackTypes] = { false, false };
  static int	countr[nFeedBackTypes] = { 0, 0 },
		goodGPS = 0, gps_is_flat = 0;

  static NR_TYPE old_glat[nFeedBackTypes], old_glon[nFeedBackTypes];


  alat	= GetSample(varp, 0);	// IRS Lat
  alon	= GetSample(varp, 1);	// IRS Lon
  glat	= GetSample(varp, 2);	// GPS Lat
  glon	= GetSample(varp, 3);	// GPS Lon
  vns	= GetSample(varp, 4);	// IRS NS ground speed
  vew	= GetSample(varp, 5);	// IRS EW ground speed
  gvns	= GetSample(varp, 6);	// GPS NS ground speed
  gvew	= GetSample(varp, 7);	// GPS EW ground speed
  roll	= GetSample(varp, 8);	// IRS Roll
  nSatellites	= (long)GetSample(varp, 9);	/* GGNSATS	*/
  gps_quality	= (long)GetSample(varp, 10);	/* NMEA GGQUAL	*/

  returnMissingValue = false;

  // If any gps values are isnan(), then just force quality to invalid.
  if (std::isnan(gps_quality) ||
      std::isnan(glat) || std::isnan(glon)|| std::isnan(gvns) || std::isnan(gvew))
    {
    gps_quality = 0;
    goodGPS = 0;
    }

  if (firstTime[FeedBack])
    {
    if (gps_quality > 0 && nSatellites > 2)
      {
      old_glat[FeedBack] = glat;
      old_glon[FeedBack] = glon;

      firstTime[FeedBack] = false;
      }
    else
      returnMissingValue = true;
    }


  /* If no IRS, then bail out.
   */
  if ((alat == 0.0 && alon == 0.0) ||
      std::isnan(alat) || std::isnan(alon) || std::isnan(vns) || std::isnan(vew))
    {
    if (gps_quality > 1) // differential or better, then we will return GPS values directly
      {
      latc[FeedBack] = glat;
      lonc[FeedBack] = glon;
      vewc[FeedBack] = gvew;
      vnsc[FeedBack] = gvns;
      PutSample(varp, latc[FeedBack]);
      return;
      }
    else
      {
      returnMissingValue = true;
      latc[FeedBack] = lonc[FeedBack] = vewc[FeedBack] = vnsc[FeedBack] = 0.0;
      }
    }

  if (returnMissingValue)
    {
    PutSample(varp, floatNAN);
    return;
    }

  time_duration[FeedBack] += deltaT[FeedBack];
  omegat = 2.0 * M_PI * time_duration[FeedBack] / 5067.0;

  /* Check GPS status, only do this on the Low-rate pass.
   */
  if (FeedBack == LOW_RATE_FEEDBACK)
    {
    double dx, dy, dgps;
    dx	= glat - old_glat[FeedBack];
    dy	= glon - old_glon[FeedBack];
    dgps = dx*dx + dy*dy;

    ++goodGPS;

    /* GPS quality indication.  We typically get, 0, 1, 2, & 5.
     *      0 = fix not available
     *      1 = non-differential GPS fix available
     *      2 = differential GPS (DGPS) fix available
     *      3 = Military PPS
     *      4 = RTK
     *      5 = FRTK - Omnistar, Terrastar, etc
     *      6 = estimated
     */

    /* Bad Positions for using GPS?
     */
    if (nSatellites < 3 || gps_quality < 1)
      {
      sprintf(buffer, "latc: GPS disabled, status reject nSats=%d, gqual=%d.",
		(int)nSatellites, (int)gps_quality);
      LogStdMsg(buffer);
      goodGPS = 0;
      }

    if (fabs(glat) > 90.0 || fabs(glon) > 180.0)
      {
      LogStdMsg("latc: GPS disabled, bad position.");
      goodGPS = 0;
      }

    if (fabs(roll) > ROLL_MAX)
      {
      sprintf(buffer, "latc: GPS disabled, ROLL_MAX of %.1f exceeded.", ROLL_MAX);
      LogStdMsg(buffer);
      goodGPS = 0;
      }

    if (dgps == 0.0)
      {
      if (++gps_is_flat > 2)	/* > 2 seconds	*/
        {
        LogStdMsg("latc: GPS is flat-lined.");
        goodGPS = 0;
        }
      }
    else
      gps_is_flat = 0;
    }


  if (goodGPS < 10)	/* < 10 seconds		*/
    {
    if (countr[FeedBack] > 1800 / deltaT[FeedBack])
      { /* 30 minutes of operation.	*/
      if (FeedBack == LOW_RATE_FEEDBACK && matrix_updated[FeedBack])
        {
        matrix_updated[FeedBack] = false;

        h[0][1] = h[1][0];
        h[0][2] = h[2][0];
        h[1][2] = h[2][1];

        double hi[NCF][NCF];

        memcpy(hi, h, sizeof(h));

        det = invert(hi);

        /* Beware of matrix that cannot be inverted.
         */
        if (det == 0.0)
          {
          LogStdMsg("latc: GPS determinate is zero, reseting countr.");
          countr[FeedBack] = 0;
          goto label546;
          }

        /* Now multiply times am to get matrix of coefficients.
         */
        for (k = 0; k < 2; ++k)
          for (i = 0; i < NCF; ++i)
            {
            c[k][i] = cp[k][i] = 0.0;

            for (j = 0; j < NCF; ++j)
              {
              c[k][i] += hi[i][j] * am[k][j];
              cp[k][i] += hi[i][j] * bm[k][j];
              }
            }
        }

      sinwt = sin(omegat);
      coswt = cos(omegat);

      /* Want to avoid sharp transition to fit because that would
       * produce spike affecting power spectra.  Instead, slowly
       * adjust from last value toward fit value, time constant of
       * about 5 minutes chosen to be small compared to Schuller
       * period but large compared to wavelengths inportant in
       * spectral analysis.
       */
/*      gpsflg = 1.0;
*/
      dvy[FeedBack] = dvy[FeedBack]*fctrf[FeedBack]+(1.0-fctrf[FeedBack])*(c[0][0]+c[0][1]*sinwt+c[0][2]*coswt);
      dvx[FeedBack] = dvx[FeedBack]*fctrf[FeedBack]+(1.0-fctrf[FeedBack])*(c[1][0]+c[1][1]*sinwt+c[1][2]*coswt);
      dlat[FeedBack]= dlat[FeedBack]*fctrf[FeedBack]+(1.0-fctrf[FeedBack])*(cp[0][0]+cp[0][1]*sinwt+cp[0][2]*coswt);
      dlon[FeedBack]= dlon[FeedBack]*fctrf[FeedBack]+(1.0-fctrf[FeedBack])*(cp[1][0]+cp[1][1]*sinwt+cp[1][2]*coswt);
      }
    else
      {
      /* No good fit accumulated yet, so just let last correction
       * factors slowly decay.
       */
/*      gpsflg = 0.001;
*/
      /* Adjust the correction factors coming from the complementary
       * filter toward the IRU solution. There does not seem to be any
       * benefit from doing this, instead of just continuing to apply the
       * offset determined from the complementary filter. Comment out.*/
/*    dvy[FeedBack]	*= fctrf[FeedBack];
      dvx[FeedBack]	*= fctrf[FeedBack];
      dlat[FeedBack]	*= fctrf[FeedBack];
      dlon[FeedBack]	*= fctrf[FeedBack];
*/
      }
    }
  else
    {
    /* Good GPS comes here.
     */
    assert(!std::isnan(glat));
    assert(!std::isnan(glon));
    assert(!std::isnan(gvew));
    assert(!std::isnan(gvns));

    dvy[FeedBack]	+= (1.-fctrf[FeedBack])\
	   *(filter((double)(gvns-vns),zf[FeedBack][0])-dvy[FeedBack]);
    dvx[FeedBack]	+= (1.-fctrf[FeedBack])\
	   *(filter((double)(gvew-vew),zf[FeedBack][1])-dvx[FeedBack]);
    dlat[FeedBack]	= glat - alat;
    dlon[FeedBack]	= glon - alon;

    if (FeedBack == LOW_RATE_FEEDBACK) /* Only do this in the Low-rate pass */
      {
      sinwt = sin(omegat);
      coswt = cos(omegat);

      am[0][0] = UPFCTR * am[0][0] + dvy[FeedBack];
      am[0][1] = UPFCTR * am[0][1] + dvy[FeedBack] * sinwt;
      am[0][2] = UPFCTR * am[0][2] + dvy[FeedBack] * coswt;
      am[1][0] = UPFCTR * am[1][0] + dvx[FeedBack];
      am[1][1] = UPFCTR * am[1][1] + dvx[FeedBack] * sinwt;
      am[1][2] = UPFCTR * am[1][2] + dvx[FeedBack] * coswt;
      bm[0][0] = UPFCTR * bm[0][0] + dlat[FeedBack];
      bm[0][1] = UPFCTR * bm[0][1] + dlat[FeedBack] * sinwt;
      bm[0][2] = UPFCTR * bm[0][2] + dlat[FeedBack] * coswt;
      bm[1][0] = UPFCTR * bm[1][0] + dlon[FeedBack];
      bm[1][1] = UPFCTR * bm[1][1] + dlon[FeedBack] * sinwt;
      bm[1][2] = UPFCTR * bm[1][2] + dlon[FeedBack] * coswt;

      /* Note: only one information matrix needed: independant of dx,dy
       */
      h[0][0] = UPFCTR * h[0][0] + 1.0;
      h[1][0] = UPFCTR * h[1][0] + sinwt;
      h[2][0] = UPFCTR * h[2][0] + coswt;
      h[1][1] = UPFCTR * h[1][1] + sinwt * sinwt;
      h[2][1] = UPFCTR * h[2][1] + sinwt * coswt;
      h[2][2] = UPFCTR * h[2][2] + coswt * coswt;

      matrix_updated[FeedBack] = true;
      ++countr[FeedBack];
      }
    }

label546:
  vnsc[FeedBack] = vns + dvy[FeedBack];
  vewc[FeedBack] = vew + dvx[FeedBack];

  if (fabs(lonc[FeedBack] - alon) > 5 || fabs(latc[FeedBack] - alat) > 5)
    {
    lonc[FeedBack] = alon;
    latc[FeedBack] = alat;
    }

  lonc[FeedBack] += vewc[FeedBack] / (CDM * cos(alat * DEG_RAD));
  latc[FeedBack] += vnsc[FeedBack] / CDM;
  lonc[FeedBack] += factorp[FeedBack] * (alon+dlon[FeedBack]-lonc[FeedBack]);
  latc[FeedBack] += factorp[FeedBack] * (alat+dlat[FeedBack]-latc[FeedBack]);

  PutSample(varp, latc[FeedBack]);

}	/* END SLATC */

/* -------------------------------------------------------------------- */
void slonc(DERTBL *varp)
{
  if (returnMissingValue)
    PutSample(varp, floatNAN);
  else
    PutSample(varp, lonc[FeedBack]);
}

/* -------------------------------------------------------------------- */
void svewc(DERTBL *varp)
{
  if (returnMissingValue)
    PutSample(varp, floatNAN);
  else
    PutSample(varp, vewc[FeedBack]);
}

/* -------------------------------------------------------------------- */
void svnsc(DERTBL *varp)
{
  if (returnMissingValue)
    PutSample(varp, floatNAN);
  else
    PutSample(varp, vnsc[FeedBack]);
}

/* -------------------------------------------------------------------- */
/* Version 2 (A Cooper, 5/28/92, coded by CJW 7/30/93)
 * This function returns the low-pass-filtered value of the time series
 * 'x', if the function is called sequentially for each element in x.
 * The filter is a three-pole Butterworth lowpass filter with
 * cutoff frequency (i.e., attentuation by 0.707) at T/tau where T is
 * the sampling frequency and tau is the data constant below.  (For
 * a sequence sampled once per second and tau=600, the cutoff frequency
 * is at 1/600 = (10 min)**{-1}.)  The algorithm used is described
 * in Bosic, p. 49.
 * The array zf is used to save values for the next call, to make it
 * easy to filter more than one variable.
 *
 * Version 3 (A Cooper, 8/18/2014, coded by JAA 9/24/2014)
 * Replace with a 3rd order Butterworth filter. The ARMA filter coefficients
 * were derived by running the R function bf <- butter (3,2./tau). The filter is
 * very sensitive to small errors in the coefficients so full-precision is 
 * retained.
 * NOTE: THIS HAS NOT BEEN CODED FOR HRT YET. CONSTANTS ARE ONLY APPLICABLE TO LRT DATA
 */
static NR_TYPE filter(double x, double zf[])
{
  /* static double	a[nFeedBackTypes], a2[nFeedBackTypes],
		a3[nFeedBackTypes], a4[nFeedBackTypes]; */
  static double bfb[4];
  static double bfa[4];
  static double xf;
  static bool	firstTime[nFeedBackTypes] = { true, true };

  if (firstTime[FeedBack])
    {
    /* double	b1, b2, c, e, f; */
    sprintf(buffer,"Processing rate = %d\n",cfg.ProcessingRate());
    LogMessage(buffer);

    if (FeedBack == HIGH_RATE_FEEDBACK)
    {
      /* TAU *= (float)cfg.ProcessingRate(); */
      /* These constants were calculated with tau = 25sps*600 */
      if (cfg.ProcessingRate() == 25) {
        bfb[0] = 1.25734516578082039e-07;
        bfb[1] = 3.77203549734246089e-07;
        bfb[2] = 3.77203549734246089e-07;
        bfb[3] = 1.25734516578082039e-07;
        bfa[0] = 1.0;
        bfa[1] = -2.97989389167679875;
        bfa[2] =  2.95998940318073878;
        bfa[3] = -0.980094505627807311;
      } else {
	  sprintf(buffer,"Butterworth filter not implemented for processing rate %d. Edit gpsc.c and follow instructions for generating coefficients for this rate.\n",cfg.ProcessingRate());
          LogMessage(buffer);
      }
    } else {
      bfb[0] = 1.56701035058826933e-03;
      bfb[1] = 4.70103105176480820e-03; 
      bfb[2] = 4.70103105176480820e-03; 
      bfb[3] = 1.56701035058826933e-03;
      bfa[0] = 1.0;
      bfa[1] = -2.49860834469117776; 
      bfa[2] =  2.11525412700315840;
      bfa[3] = -6.04109699507274889e-1;
    }

    firstTime[FeedBack] = false;
    }

  xf =  (bfb[0] * x + bfb[1] * zf[0] + bfb[2]*zf[1] + bfb[3]*zf[2] 
        - (bfa[1]*zf[3] + bfa[2]*zf[4] + bfa[3]*zf[5]));

  /* Store terms for the next call. */
  zf[2] = zf[1];
  zf[1] = zf[0];
  zf[0] = x;
  zf[5] = zf[4];
  zf[4] = zf[3];
  zf[3] = xf;

  return(xf);

}	/* END FILTER */

/* -------------------------------------------------------------------- */
static int ludcmp(double a[NCF][NCF], int N, int indx[], double *det)
{
  int		i, imax = 0, j, k;
  double	big, dum, sum, temp;
  double	vv[NCF];

  *det = 1.0;

  for (i = 0; i < N; ++i)
    {
    big = 0.0;

    for (j = 0; j < N; ++j)
      if ((temp = fabs(a[i][j])) > big)
        big = temp;

    if (big == 0.0)
      {
      LogStdMsg("gpsc.c: Singular matrix in LUdcmp.");
      return(ERR);
    }

    vv[i] = 1.0 / big;
    }

  for (j = 0; j < N; ++j)
    {
    for (i = 0; i < j; ++i)
      {
      sum = a[i][j];

      for (k = 0; k < i; ++k)
        sum -= a[i][k] * a[k][j];

      a[i][j] = sum;
      }

    big = 0.0;

    for (i = j; i < N; ++i)
      {
      sum = a[i][j];

      for (k = 0; k < j; ++k)
        sum -= a[i][k] * a[k][j];

      a[i][j] = sum;

      if ((dum = vv[i] * fabs(sum)) >= big)
        {
        big = dum;
        imax = i;
        }
      }

    if (j != imax)
      {
      for (k = 0; k < N; ++k)
        {
        dum = a[imax][k];
        a[imax][k] = a[j][k];
        a[j][k] = dum;
        }

      *det = -(*det);
      vv[imax] = vv[j];
      }

    indx[j] = imax;

    if (a[j][j] == 0.0)
      a[j][j] = MINFLOAT;

    if (j != N-1)
      {
      dum = 1.0 / a[j][j];

      for (i = j+1; i < N; ++i)
        a[i][j] *= dum;
      }
    }

  return(OK);

}	/* END LUDCMP */

/* -------------------------------------------------------------------- */
static void lubksub(double a[NCF][NCF], int N, int indx[], double b[])
{
  int		i, ii = -1, ip, j;
  double	sum;

  for (i = 0; i < N; ++i)
    {
    ip = indx[i];
    sum = b[ip];
    b[ip] = b[i];

    if (ii != -1)
      for (j = ii; j < i; ++j)
        sum -= a[i][j] * b[j];
    else
      if (sum != 0.0)
        ii = i;

    b[i] = sum;
    }

  for (i = N - 1; i >= 0; --i)
    {
    sum = b[i];

    for (j = i+1; j < N; ++j)
      sum -= a[i][j] * b[j];

    b[i] = sum / a[i][i];
    }
}	/* END LUBKSUB */

/* -------------------------------------------------------------------- */
static double invert(double array[NCF][NCF])
{
  int		i, j, indx[NCF];
  double	y[NCF][NCF], col[NCF], det;

  if (ludcmp(array, NCF, indx, &det) == ERR)
    return(0.0);

  for (i = 0; i < NCF; ++i)
    det *= array[i][i];

  for (j = 0; j < NCF; ++j)
    {
    for (i = 0; i < NCF; ++i)
      col[i] = 0.0;

    col[j] = 1.0;
    lubksub(array, NCF, indx, col);

    for (i = 0; i < NCF; ++i)
      y[i][j] = col[i];
    }

  memcpy(array, y, sizeof(y));
  return(det);
}

/* END GPSC.C */
