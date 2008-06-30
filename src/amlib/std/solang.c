/*
-------------------------------------------------------------------------
OBJECT NAME:	solang.c

FULL NAME:	Solar angle calculations

ENTRY POINTS:	ssolze(), ssolaz(), ssolde(), ssolel()

STATIC FNS:		

DESCRIPTION:	Calculate various solar angles for use by other functions
		(especially for "radc.c," the radiation attitude-angle
		corrections)

INPUT:		YEAR MONTH DAY HOUR MINUTE SECOND LAT LON

OUTPUT:		SOLZE SOLAZ SOLDE SOLEL

COPYRIGHT:	University Corporation for Atmospheric Research, 1997

ORIG. AUTHOR:	Ron Ruth
-------------------------------------------------------------------------
 */

#include "nimbus.h"
#include "amlib.h"

#undef TWO_PI

static double	solaz;	/* solar azimuth angle (radians) */
static double	solde;	/* solar declination angle (radians) */
static double	solel;	/* solar elevation angle (radians) */
const double	TWO_PI	= 2.0*M_PI;

/* -------------------------------------------------------------------- */

void ssolze(DERTBL *varp)
/*
 * Function ssolze ("zenith" in FORTRAN)
 *    calculates local azimuth and zenith distance for specified
 *    location and time using approximations to nautical almanac
 * --------------------------------------------------------------------
 * Input arguments:
 *    year = year number (e.g., 1977)
 *    mon  = month of year
 *    day  = day of month
 *    hr   = hour of day (UTC)
 *    min  = minute of hour
 *    sec  = second of minute
 *    lat  = latitude in decimal degrees (ex. -34.75)
 *    lon  = longitude in decimal degrees (ex. 138.75)
 *
 * Global output variables:
 *    solaz   = solar azimuth angle, postive east of south  (radians)
 *    solze   = solar zenith angle (radians)
 *    solel   = solar elevation angle (radians)
 *    solde   = solar declination angle, north +ve (radians)
 *
 * Local variables:
 *    phi     = latitude (radians)
 *    gha     = solar Greenwich hour angle (west) (radians)
 *    zone = zone time from greenwich in fractions of hours (ex. -9.5)
 *            (assumed to be zero and not used in this implementation)
 * --------------------------------------------------------------------
 *    This program was taken from r.walraven(1978) and archer(1980).
 *    Additional code and different format was done by forgan(1980).
 * --------------------------------------------------------------------
 *    Changes of different latitude and longitude format as well as
 *    original exact conversion of the time (hhmm.ss) to time decimal
 *    was done by Bannehr.
 *    updated : 18/1/91   lutz bannehr
 * --------------------------------------------------------------------
 *    Current time conversion method done by Ronald L. Ruth    Sep 1997
 *    Converted to C language by Ronald L. Ruth                Oct 1997
 *    It appears that "zone" is not needed in this
 *     implementation, since ADS time is now using
 *     only UTC.  This would be equivalent to "zone=0."
 */
{

	double	solze;	/* solar zenith angle (radians) */
	int	year, mon, day;
	double	hr, min, sec, lat, lon;

	int	i, k, iday, delyr, leap;
	double	lond, lonr, phi, t, b, zone, time, theta, g, el;
	double eps, sel, a1, a2, gha, sze, cze, sph, st;


	year = (int)GetSample(varp, 0);
	mon  = (int)GetSample(varp, 1);
	day  = (int)GetSample(varp, 2);
	hr   = (double)GetSample(varp, 3);
	min  = (double)GetSample(varp, 4);
	sec  = (double)GetSample(varp, 5);
	lat  = (double)GetSample(varp, 6);
	lon  = (double)GetSample(varp, 7);

	lond = lon;
	if (lond > 0. && lond < 180.) lond = 360. - lond;
	lond = (fabs)(lond);
	lonr = lond*DEG_RAD;
	phi = lat*DEG_RAD;
/*  Convert UTC (hr min sec) into decimal hours */
	t = hr + min/60. + sec/3600.;

/*  Handle ephemeris (calendar) data */
	i = 1;
	b = 0.5;
	k = 0;
	if (mon > 2)
	{
		i = 3;
		b = 59.5;
		k = year/4 - (year-1)/4;
	}
	iday = day + k + (int)((mon - i)*30.6 + b);
	delyr = year - 1980;
	leap = delyr/4;
/*	For UTC, zone = 0
 *	zone = (int)(lon/15.);
 *	t += zone;
 */
	time = (double)delyr * 365.0 + (double)leap + (double)iday - 1. + t / 24.0;
	if (delyr == leap * 4) time -= 1.0;
	if ((delyr < 0) && (delyr != leap * 4)) time -= 1.0;
	theta = (360.*time/365.25)*DEG_RAD;
	g = -0.031271 -4.53963e-7 * time + theta;
	el = 4.900968 +3.67474e-7 * time;
	el += ((0.033434 -2.3e-9 * time) * sin(g));
	el += (0.000349*sin(2.*g) + theta);
	eps = 0.409140 -6.2149e-9 * time;
	sel = sin(el);
	a1 = sel * cos(eps);
	a2 = cos(el);
	gha = atan2(a1,a2);

	if (gha < 0.) gha += TWO_PI;

	solde = asin(sel * sin(eps));
	st = 1.759335 + TWO_PI * (time / 365.25 - delyr);
	st += 3.694e-7 * time;
	if (st >= TWO_PI) st -= TWO_PI;
	st += (-lonr + 15.0 * DEG_RAD * t);
	if (st >= TWO_PI) st -= TWO_PI;
	gha -= st;
	sze = sin(phi) * sin(solde) + cos(phi) * cos(solde) * cos(gha);
	solze = asin(sze);
	cze = cos(solze);
	if (cze == 0.0) cze = 0.00001;
	solaz = cos(solde) * sin(gha) / cze;

	if (solaz >  1.0) solaz =  1.0;
	if (solaz < -1.0) solaz = -1.0;
	solaz = asin(solaz);
	sph = sin(phi);
	if (sph == 0.0) sph = 0.00001;
	if (sze < sin(solde)/sph)
	{
		if (solaz < 0.) solaz += TWO_PI;
		solaz = M_PI - solaz;
	}
	solze = M_PI_2 - solze;	// M_PI_2 is PI/2.
	gha = fmod((lonr-gha),TWO_PI);
	if (gha < 0.0) gha += TWO_PI;
	solel = M_PI_2 - solze;	// M_PI_2 is PI/2.
/*
	printf ("Within zenith function\n");
	printf (" solar azimuth (deg) = %f\n",solaz*RAD_DEG);
	printf (" solar zenith (deg) = %f\n",solze*RAD_DEG);
	printf (" solar elevation (deg) = %f\n",solel*RAD_DEG);
	printf (" solar declination (deg) = %f\n",solde*RAD_DEG);
	printf (" Greenwich hour angle (deg) = %f\n",gha*RAD_DEG);
 */
	PutSample(varp, (NR_TYPE)solze);
/*	printf (" PutSample solar zenith (deg) = %f\n",solze*RAD_DEG); */

}	/* END SSOLZE */

/* -------------------------------------------------------------------- */
void ssolaz(DERTBL *varp)
/*  Solar azimuth angle (radians) */
{
	PutSample(varp, solaz);
/*	printf (" PutSample solar azimuth (deg) = %f\n",solaz*RAD_DEG); */

}	/* END SSOLAZ */

/* -------------------------------------------------------------------- */
void ssolde(DERTBL *varp)
/*  Solar declination angle (radians) */
{
	PutSample(varp, solde);
/*	printf (" PutSample solar declination (deg) = %f\n",solde*RAD_DEG); */

}	/* END SSOLDE */

/* -------------------------------------------------------------------- */
void ssolel(DERTBL *varp)
/*  Solar elevation angle (radians) */
{
	PutSample(varp, solel);
/*	printf (" PutSample solar elevation (deg) = %f\n",solel*RAD_DEG) */;

}	/* END SSOLEL */
