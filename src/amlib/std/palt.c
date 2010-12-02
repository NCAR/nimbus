/*
-------------------------------------------------------------------------
OBJECT NAME:	palt.c

FULL NAME:	NACA pressure altitude (M)

ENTRY POINTS:	spalt()

STATIC FNS:	none

DESCRIPTION:    Source documents:
                U.S. Standard Atmosphere 1976 (NASA-TM-X-74335)
                N77-16482, 241 pages.
                ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19770009539_1977009539.pdf

                RAF science wiki:
                wiki.eol.ucar.edu/rafscience/ProcessingAlgorithms?action=AttachFile&do=view&target=PressureAltitude.pdf
                File: PressureAltitude.pdf
                8 Nov 2010

                The US Standard Atmosphere is defined as
                having a pressure of 1013.25 hPa and temperature 15 degC
                at an altitude of 0 m.
                The lapse rate is lambda=-0.0065degC/m (z<11000m)
                and lapse rate lambda=0 (z>11000m).

                There is a further lapse rate change at
                z=20000 m, but that is not included here due to the lower
                flight altitudes of NCAR aircraft.

                The US Standard Atmosphere is identical to the ICAO standard
		1964 up to 32 km and the ISO standard 1973 up to 50km.

                The US Standard Atmosphere is an avionics
                standard, and it does therefore not follow common science
                standards for the values of several constants.
                In particular:
                    gravity g=9.80665
                    gas constant  R=287.0531 J/(kg K)
                Note that the gas constant given here is a
                derived value based on the US Standard Atmosphere's
                values for R* (US standard, table 2) and Md.

                Current coding and comments were completed on 22 Nov 2010.
                Prior versions allowed for different surface
                temperatures and used somewhat different constants;
                prior versions should no longer be used.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2010
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

/*  Reference pressure for US standard atmosphere (hPa)			*/
static NR_TYPE ASTG = 1013.25;

/*  Transition pressure at the assumed ISA tropopause (hPa)		*/
/*  This value can be calculated by upwards integration			*/
static NR_TYPE ISAP1 = 226.3206;


/* -------------------------------------------------------------------- */
void spalt(DERTBL *varp)
{
  NR_TYPE  psxc, palt;

  psxc = GetSample(varp, 0);

  if (palt > ISAP1)
    /*  Branch for upper altitude lapse rate.				*/
    palt = 44330.77 * (1.0 - pow(psxc / ASTG, 0.1902632));
  else
    /*  Branch for lower altitude lapse rate.				*/
    palt = 11000.0 + 14602.12 * log10(ISAP1 / psxc);

  PutSample(varp, palt);
}

/* -------------------------------------------------------------------- */
void meters2feet(DERTBL *varp)
{
  PutSample(varp, GetSample(varp, 0) * 3.2808399);
}

/* -------------------------------------------------------------------- */
void feet2meters(DERTBL *varp)
{
  /* Exact definition, as also per US Standards Atmosphere document	*/
  PutSample(varp, GetSample(varp, 0) * 0.3048);
}

/* END PALT.C */
