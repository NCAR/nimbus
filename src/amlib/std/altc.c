/*
-------------------------------------------------------------------------
OBJECT NAME:	altc.c

FULL NAME:	Corrected GPS Altitudes (All brands)

ENTRY POINTS:	sgaltc()	sggaltc()

DESCRIPTION:	Create a corrected altitude variable by combining
		the GPS altitude data with IRS altitude when needed

INPUT:		GALT ALT GSTAT GMODE

OUTPUT:		GALTC, GGALTC

REFERENCES:	none

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 2003
-------------------------------------------------------------------------
*/

#include <values.h>
#include "nimbus.h"
#include "decode.h"
#include "amlib.h"

/*   NOTE:  Any of following constants can be put into the project's
 *          defaults file and initialized below, if necessary.
 */
/*  Regression equation to convert from Pressure Altitude to GPS Altitude  */
static const double Coef1 = 0.9786647;		/*  slope      */
static const double Coef2 = -46.2877565;	/*  intercept  */

static const NR_TYPE AltThresh = 50.0;		/* ALT sample-to-sample error threshold  */

static const int SAVE = 10; /*  number of past calculations to save  */
static const int WAIT = 10; /*  how long to wait after good GPS data return  */
static const int DELAY = 2; /*  delay before walking substituted data back to GPS data */

/*  WARNING:  WAIT > DELAY to avoid blowup  */

/* -------------------------------------------------------------------- */
void sgaltc(DERTBL *varp)
/*  Code for the Trimble TANS-III GPS  */
{
  NR_TYPE        altc = MISSING_VALUE, galt, alt;
  NR_TYPE        newalt, dalt;
  long           gstat, gmode;
  int            i;

  static bool    firstTime[nFeedBackTypes] = { TRUE, TRUE };
  static NR_TYPE DELT[nFeedBackTypes];

  static int     jj;
  static int     badgalt = 0, isav = 0;
  static int     countr = 0, goodGPS = 0;
  static NR_TYPE dalt_sav[SAVE];


  galt		= GetSample(varp, 0);
  alt		= GetSample(varp, 1);
  gstat		= (long)GetSample(varp, 2);
  gmode		= (long)GetSample(varp, 3);

/*  If ALT and/or GALT are missing, so is ALTC  */
  if ( (galt == MISSING_VALUE) || (alt == MISSING_VALUE) )
    {
    altc = MISSING_VALUE;
    dalt = 0.;
    }
  else
    {
    if (firstTime[FeedBack])
      {
/*  Get constant values from Defaults file, if available  */

/*  (Use values within the function instead of those in the Defaults file.)

      if ((tmp = GetDefaultsValue("GPS_GALT_TAU", varp->name)) == NULL)
        {
        sprintf(buffer, "Value set to %f in AMLIB function slatc.\n", GALT_TAU);
        LogMessage(buffer);
        }
      else
        Tau = tmp[0];
 */

      if (FeedBack == LOW_RATE_FEEDBACK)
        DELT[FeedBack]  = 1.0;
      else
        DELT[FeedBack]  = 1.0 / ProcessingRate;

/*  Initialize  */
      newalt = alt*Coef1 + Coef2;
      dalt = galt - newalt;
      for (i=0; i<SAVE; i++) dalt_sav[i] = dalt;

      firstTime[FeedBack] = FALSE;
      }

/* Check GPS status; only do this on the Low-rate pass.  */
    if (FeedBack == LOW_RATE_FEEDBACK)
      {
      ++goodGPS;

      if (gstat == 0x0b00)  /* 3 satellites is also considered good  */
        gstat = 0;

      newalt = alt*Coef1 + Coef2;

    /* Bad GPS Status ?  */
      if (gstat > 0 || gmode < 4 )
        {
        goodGPS = 0;
        }

/*  Check for bad GALT  */
/*    Devise a tood test for only one occurrence (lock out for awhile?)
 *     if GALT goes bad one second before GMODE tells me.  I will need
 *     to monitor second-to-second changes for something drastic until
 *     GMODE lets me know.  Should I use GMODE to reset?
 *     What's the best test to find a bad GALT?  (It does tend to approach
 *     zero.)
 */
      if ( badgalt == 0 && (fabs(galt-(newalt+dalt_sav[isav])) > AltThresh ) )
        {
        goodGPS = 0;
        ++ badgalt;
        }
      }

    if (goodGPS < WAIT)     /* < WAIT seconds of good data before continuing */
      {
/*  Ignore this test for now
      if (countr > 1800.0)      * 30 minutes of operation * 
        {
 */
        if (FeedBack == LOW_RATE_FEEDBACK)
          {

      /* Want to avoid sharp transition to fit because that would
       * produce spike affecting power spectra.  Instead, slowly
       * adjust from last value toward fit value, time constant of
       * about 5 minutes chosen to be small compared to Schuller
       * period but large compared to wavelengths important in
       * spectral analysis.
       */

      /* Above is a noble goal, but for now, let's just do a simple
       * substitution when the data are bad and return when it's not.
       */
          newalt = alt*Coef1 + Coef2;
          dalt = dalt_sav[0];
          for (i=1; i<SAVE; i++) dalt += dalt_sav[i];
/*   Omit the most recent one, since sometimes GSTAT and GMODE flag late  */
          dalt -= dalt_sav[isav];
          dalt /= (SAVE-1);
/*   When GPS data are no good, calculate a replacement */
          if (goodGPS < DELAY || gmode < 4 )
            {
            altc = newalt + dalt;
            jj = 0;
            }
          else
            {
/*   When GPS data return, linearly walk ALTC back to GALT with a delay */
            jj++;
            altc = newalt + dalt +
              ((float)jj/(1.0+(float)WAIT-(float)DELAY))*(galt-(newalt+dalt));
            }
          }
/*
        }
 */
      }
  else
      {
    /* Good GPS comes here.
     */
      badgalt = 0;
      altc = galt;
      newalt = alt*Coef1 + Coef2;
      dalt = galt - newalt;
      ++isav;
      if (isav >= SAVE) isav = 0;
      dalt_sav[isav] = dalt;
      ++countr;
      }
    }

  PutSample(varp, altc);

}	/* END GALTC */

/* -------------------------------------------------------------------- */
void sggaltc(DERTBL *varp)
/*  Code for the Garmin GPS  */
{
  NR_TYPE        altc = MISSING_VALUE, ggalt, alt;
  NR_TYPE        newalt, dalt;
  long           ggstat, ggnsat;
  int            i;

  static bool    firstTime[nFeedBackTypes] = { TRUE, TRUE };
  static NR_TYPE DELT[nFeedBackTypes];

  static int     jj;
  static int     badgalt = 0, isav = 0;
  static int     countr = 0, goodGPS = 0;
  static NR_TYPE dalt_sav[SAVE];


  ggalt		= GetSample(varp, 0);
  alt		= GetSample(varp, 1);
  ggnsat	= (long)GetSample(varp, 2);
  ggstat	= (long)GetSample(varp, 3);

/*  If ALT and/or GGALT are missing, so is ALTC  */
  if ( (ggalt == MISSING_VALUE) || (alt == MISSING_VALUE) )
    {
    altc = MISSING_VALUE;
    dalt = 0.;
    }
  else
    {
    if (firstTime[FeedBack])
      {
/*  Get constant values from Defaults file, if available  */

/*  (Use values within the function instead of those in the Defaults file.)

      if ((tmp = GetDefaultsValue("GPS_GALT_TAU", varp->name)) == NULL)
        {
        sprintf(buffer, "Value set to %f in AMLIB function slatc.\n", GALT_TAU);
        LogMessage(buffer);
        }
      else
        Tau = tmp[0];
 */

      if (FeedBack == LOW_RATE_FEEDBACK)
        DELT[FeedBack]  = 1.0;
      else
        DELT[FeedBack]  = 1.0 / ProcessingRate;

/*  Initialize  */
      newalt = alt*Coef1 + Coef2;
      dalt = ggalt - newalt;
      for (i=0; i<SAVE; i++) dalt_sav[i] = dalt;

      firstTime[FeedBack] = FALSE;
      }

/* Check GPS status; only do this on the Low-rate pass.  */
    if (FeedBack == LOW_RATE_FEEDBACK)
      {
      ++goodGPS;

      newalt = alt*Coef1 + Coef2;

    /* Bad GPS Status ?  */
    /*  Garmin's status comes from GGA's GPS quality indication:
     *   0 = fix not available                          <-- don't use
     *   1 = non-differential GPS fix available
     *   2 = differential GPS (DGPS) fix available
     *   6 = estimated                                  <-- don't use       */
      if (ggstat == 0 || ggstat == 6 || ggnsat < 4 ) {
        goodGPS = 0;
        }

/*  Check for bad GGALT  */
/*    Devise a tood test for only one occurrence (lock out for awhile?)
 *     if GGALT goes bad one second before GGNSAT tells me.  I will need
 *     to monitor second-to-second changes for something drastic until
 *     GGNSAT lets me know.  Should I use GGNSAT to reset?
 *     What's the best test to find a bad GGALT?  (It does tend to approach
 *     zero.)
 */
      if ( badgalt == 0 && (fabs(ggalt-(newalt+dalt_sav[isav])) > AltThresh ) )
        {
        goodGPS = 0;
        ++ badgalt;
        }
      }

    if (goodGPS < WAIT)     /* < WAIT seconds of good data before continuing */
      {
/*  Ignore this test for now
      if (countr > 1800.0)      * 30 minutes of operation * 
        {
 */
        if (FeedBack == LOW_RATE_FEEDBACK)
          {

      /* Want to avoid sharp transition to fit because that would
       * produce spike affecting power spectra.  Instead, slowly
       * adjust from last value toward fit value, time constant of
       * about 5 minutes chosen to be small compared to Schuller
       * period but large compared to wavelengths important in
       * spectral analysis.
       */

      /* Above is a noble goal, but for now, let's just do a simple
       * substitution when the data are bad and return when it's not.
       */
          newalt = alt*Coef1 + Coef2;
          dalt = dalt_sav[0];
          for (i=1; i<SAVE; i++) dalt += dalt_sav[i];
/*   Omit the most recent one, since sometimes GGSTAT and GGNSAT flag late  */
          dalt -= dalt_sav[isav];
          dalt /= (SAVE-1);
/*   When GPS data are no good, calculate a replacement */
          if (goodGPS < DELAY || ggnsat < 4 )
            {
            altc = newalt + dalt;
            jj = 0;
            }
          else
            {
/*   When GPS data return, linearly walk ALTC back to GGALT with a delay */
            jj++;
            altc = newalt + dalt +
              ((float)jj/(1.0+(float)WAIT-(float)DELAY))*(ggalt-(newalt+dalt));
            }
          }
/*
        }
 */
      }
  else
      {
    /* Good GPS comes here.
     */
      badgalt = 0;
      altc = ggalt;
      newalt = alt*Coef1 + Coef2;
      dalt = ggalt - newalt;
      ++isav;
      if (isav >= SAVE) isav = 0;
      dalt_sav[isav] = dalt;
      ++countr;
      }
    }

/*  ###############   Begin Debug Print   ############### *
  printf (" altc.c debug: ggalt = %f, alt = %f, altc = %f, dalt_sav[%i] = %f\n",
     ggalt, alt, altc, isav, dalt_sav[isav]);
 *  ###############    End  Debug Print   ############### */

  PutSample(varp, altc);

}	/* END GGALTC */

/* END ALTC.C */
