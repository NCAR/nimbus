/*
-------------------------------------------------------------------------
OBJECT NAME:	solangac.c

FULL NAME:	Solar angle calculations, relative to the aircraft

ENTRY POINTS:	ssolelac(), ssolazac()

STATIC FNS:		SolarToAircraftFrame()

DESCRIPTION:	Rotate the ground-referenced solar position (SOLZE/SOLAZ,
		see solang.c) into the aircraft body frame, using the
		aircraft's true heading, pitch, and roll, to give the sun's
		elevation and azimuth as seen relative to the airframe
		itself rather than relative to true north/local vertical.

		Unlike radc.c's radiation correction factor, which is
		specific to the Eppley radiometer's cosine response and
		only valid within +-6 deg of pitch/roll, these are general
		purpose geometric angles, valid at any attitude, intended
		for broadcast (e.g. IWG1) to any instrument that needs the
		sun's position relative to the aircraft rather than the
		ground (sun trackers, imagers, glint-sensitive sensors).

INPUT:		PITCH ROLL THDG SOLZE SOLAZ

OUTPUT:		SOLELAC SOLAZAC

COPYRIGHT:	University Corporation for Atmospheric Research, 2026

ORIG. AUTHOR:	Chris Webster
-------------------------------------------------------------------------
 */

#include "nimbus.h"
#include "amlib.h"

static NR_TYPE	elevAC, azAC;

/* -------------------------------------------------------------------- */
static void SolarToAircraftFrame(NR_TYPE solzeDeg, NR_TYPE solazDeg,
	NR_TYPE pitchDeg, NR_TYPE rollDeg, NR_TYPE thdgDeg,
	NR_TYPE *elevACDeg, NR_TYPE *azACDeg)
/*
 * Rotate the ground-referenced solar position into the aircraft body
 * frame via the standard yaw/pitch/roll (3-2-1) Euler sequence.
 * --------------------------------------------------------------------
 * Input:
 *    solzeDeg = solar zenith angle, ground-referenced (deg)
 *    solazDeg = solar azimuth, ground-referenced, +east of south (deg)
 *    pitchDeg = aircraft pitch, +nose up (deg)
 *    rollDeg  = aircraft roll, +right wing down (deg)
 *    thdgDeg  = aircraft true heading (deg)
 *
 * Output:
 *    *elevACDeg = sun elevation relative to the aircraft (deg)
 *    *azACDeg   = sun azimuth relative to the aircraft, +clockwise from
 *                 the nose; 0=ahead, 90=right wing, 180=astern,
 *                 270=left wing (deg)
 * --------------------------------------------------------------------
 */
{
	NR_TYPE	elev, azN, psi, theta, phi;
	NR_TYPE	sunN, sunE, sunD;
	NR_TYPE	n1, e1, d1, n2, e2, d2, n3, e3, d3;

	elev = (90.0 - solzeDeg) * DEG_RAD;
	azN  = (180.0 - solazDeg) * DEG_RAD;	/* east-of-south -> from-north */

	sunN =  cosf(elev) * cosf(azN);
	sunE =  cosf(elev) * sinf(azN);
	sunD = -sinf(elev);

	psi   = thdgDeg * DEG_RAD;
	theta = pitchDeg * DEG_RAD;
	phi   = rollDeg * DEG_RAD;

	/* Yaw (heading) */
	n1 =  sunN * cosf(psi) + sunE * sinf(psi);
	e1 = -sunN * sinf(psi) + sunE * cosf(psi);
	d1 =  sunD;

	/* Pitch */
	n2 = n1 * cosf(theta) - d1 * sinf(theta);
	e2 = e1;
	d2 = n1 * sinf(theta) + d1 * cosf(theta);

	/* Roll */
	n3 =  n2;
	e3 =  e2 * cosf(phi) + d2 * sinf(phi);
	d3 = -e2 * sinf(phi) + d2 * cosf(phi);

	*elevACDeg = asinf(-d3) * RAD_DEG;
	*azACDeg   = atan2f(e3, n3) * RAD_DEG;
	if (*azACDeg < 0.0)
		*azACDeg += 360.0;

}	/* END SOLARTOAIRCRAFTFRAME */

/* -------------------------------------------------------------------- */
void ssolelac(DERTBL *varp)
/*
 * Function ssolelac
 *    Solar elevation relative to the aircraft body frame (deg).
 * --------------------------------------------------------------------
 * Input arguments (order set by the DependTable entry):
 *    pitch = aircraft pitch (deg)
 *    roll  = aircraft roll (deg)
 *    thdg  = aircraft true heading (deg)
 *    solze = solar zenith angle, ground-referenced (deg)
 *    solaz = solar azimuth, ground-referenced (deg)
 * --------------------------------------------------------------------
 */
{
	NR_TYPE	pitch, roll, thdg, solze, solaz;

	pitch = (NR_TYPE)GetSample(varp, 0);
	roll  = (NR_TYPE)GetSample(varp, 1);
	thdg  = (NR_TYPE)GetSample(varp, 2);
	solze = (NR_TYPE)GetSample(varp, 3);
	solaz = (NR_TYPE)GetSample(varp, 4);

	SolarToAircraftFrame(solze, solaz, pitch, roll, thdg, &elevAC, &azAC);

	PutSample(varp, elevAC);

}	/* END SSOLELAC */

/* -------------------------------------------------------------------- */
void ssolazac(DERTBL *varp)
/*
 * Function ssolazac
 *    Solar azimuth relative to the aircraft body frame (deg), positive
 *    clockwise from the nose (0=ahead, 90=right wing, 180=astern,
 *    270=left wing).  Depends on SOLELAC having already run this cycle
 *    (see DependTable), which computes the shared elevAC/azAC pair.
 * --------------------------------------------------------------------
 */
{
	PutSample(varp, azAC);

}	/* END SSOLAZAC */
