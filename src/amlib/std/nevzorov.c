/*
-------------------------------------------------------------------------
OBJECT NAME:	nevzorov.c

FULL NAME:	

ENTRY POINTS:	sfunc()

STATIC FNS:	

DESCRIPTION:	Template for AMLIB std & user directories.

INPUT:		

OUTPUT:		

REFERENCES:	
  equations can be found in instrument manual or -
  Korolev, A.V., J.W. STrapp, G.A. Isaac and A.N. Nevzorov, 1998:
  The Nevzorov airborne hot-wire LWC–TWC probe: Principle of operation and
  performance characteristics, J. Atmos. Oceanic Techn., 15, 1495-1510

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 2002
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

/* #delete this static NR_TYPE L = 2598.0; not used, average latent heat of evap, J/g */
static NR_TYPE Cl = 4.184;      /* average heat capacity of liquid water, J/g */
static NR_TYPE Ci = 1.875;      /* average heat capacity of ice, J/g */
static NR_TYPE TMP[] = { 70.0, 90.0 };   /* operating temp set points, C */
static NR_TYPE RSL[] = { 0.905, 0.986 }; /* product of resistance R and surface
        area S of Liquid sensor, ohm cm^2*/
static NR_TYPE RST[] = { 1.134, 1.235 }; /* ditto, total sensor, ohm cm^2 */

static int	indx = 0;

NR_TYPE LatentHeatMelting(NR_TYPE temp);
NR_TYPE LatentHeatEvaporation(NR_TYPE temp);

/* -------------------------------------------------------------------- */
void sNevLWC(DERTBL *varp)
{
  NR_TYPE	sig, tas, atx, out, Ll;

  sig = GetSample(varp, 0);
  tas = GetSample(varp, 1) * 1.05; /* TAS is 5% higher at starboard fuselage
        station, 40 cm from skin, per Dave Rogers-IDEAS3 */
  atx = GetSample(varp, 2);

  Ll = Cl * (TMP[indx] - atx) + LatentHeatEvaporation(TMP[indx]); /* J/g */

/* #change
  out = (sig * sig) / tas * Ll * RSL[indx];
  PutSample(varp, out / 1000.0);
*/
  out = (sig * sig) / (tas * Ll * RSL[indx]);  /* g/(m cm^2)*/
  PutSample(varp, out * 1.0e4);  /* convert to g/m^3 */

}

/* -------------------------------------------------------------------- */
void sNevTWC(DERTBL *varp)
{
  NR_TYPE       sig, tas, atx, out, Li;

  sig = GetSample(varp, 0);
  tas = GetSample(varp, 1) * 1.05; /* TAS is 5% higher at starboard fuselage
        station, 40 cm from skin, per Dave Rogers-IDEAS3 */
  atx = GetSample(varp, 2);

/* #change
  Li = Cl * TMP[indx] + LatentHeatEvaporation(TMP[indx]);
  if (atx <= 0.0)
        Li += Ci * -atx + LatentHeatMelting(TMP[indx]);
  out = (sig * sig) / tas * Li * RST[indx];
  PutSample(varp, out / 1000.0);
*/

  Li = Cl * (TMP[indx] - atx) + LatentHeatEvaporation(TMP[indx]); /* J/g */
  if (atx <= 0.0)
        Li += Ci * -atx + LatentHeatMelting(TMP[indx]);
  out = (sig * sig) / (tas * Li * RST[indx]);   /* g/(m cm^2)*/
  PutSample(varp, out * 1.0e4);   /* convert to g/m^3 */

}

/* -------------------------------------------------------------------- */
NR_TYPE LatentHeatMelting(NR_TYPE temp)       /* Joules / g */
{
  return(((79.7 + 0.485 * temp - 2.5e-3 * temp * temp) * 1055.0) / 252.0);

}
/* -------------------------------------------------------------------- */
NR_TYPE LatentHeatEvaporation(NR_TYPE temp)      /* Joules / g */
{
  return(((597 - ((597-540) * (temp / 100.0))) * 1055.0) / 252.0);

}

/* END NEVZOROV.C */
