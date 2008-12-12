/*
-------------------------------------------------------------------------
OBJECT NAME:	initAC.c

FULL NAME:	Initialize Aircraft Dependencies

ENTRY POINTS:	InitAircraftDependencies()

STATIC FNS:	none

DESCRIPTION:	p_corr functions for each aircraft.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2007
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static const NR_TYPE XMPHMS = 0.44704; // conversion factor mph to meters/sec

extern int	FlightDate[];

NR_TYPE	recfb, recff, recfkp, recfrh, recfrn, recfw, recfra;

NR_TYPE	(*pcorPSF)(NR_TYPE, NR_TYPE), (*pcorQCF)(NR_TYPE, NR_TYPE),
	(*pcorPSFD)(NR_TYPE, NR_TYPE), (*pcorQCR)(NR_TYPE, NR_TYPE),
	(*pcorQCW)(NR_TYPE, NR_TYPE), (*pcorPSW)(NR_TYPE, NR_TYPE),
	(*pcorPSB)(NR_TYPE, NR_TYPE), (*pcorQCB)(NR_TYPE, NR_TYPE),
	(*pcorQCFR)(NR_TYPE, NR_TYPE);

NR_TYPE	pcorw8(NR_TYPE, NR_TYPE),pcorf8(NR_TYPE, NR_TYPE),pcorw2(NR_TYPE, NR_TYPE),
	pcorr2(NR_TYPE, NR_TYPE),pcorb7(NR_TYPE, NR_TYPE),pcorf7(NR_TYPE, NR_TYPE),
	pcorr1(NR_TYPE, NR_TYPE),pcorf1(NR_TYPE, NR_TYPE), pcorf3(NR_TYPE, NR_TYPE),
	pcorr3(NR_TYPE, NR_TYPE),pcorf1_2(NR_TYPE, NR_TYPE), pcorf1_3(NR_TYPE, NR_TYPE),
	pcorr5_2(NR_TYPE, NR_TYPE),pcorf5_2(NR_TYPE, NR_TYPE),pcorq5_2(NR_TYPE, NR_TYPE),
	pcorr5(NR_TYPE, NR_TYPE),pcorf5(NR_TYPE, NR_TYPE),pcorq5(NR_TYPE, NR_TYPE);

/* reference airspeed on J-W liquid water content converted from MPH
 * to m/s
 */
NR_TYPE	jwref;

NR_TYPE	tfher1,tfher2;

/* -------------------------------------------------------------------- */
void InitAircraftDependencies()
{
  /* Set up default values first, then AC dependant.
   */

  /* TFHER = function to compute deiced Rosemount total temperature.
   * ERROR (C) as a function of Z, where Z is mach number adjusted by
   * the density ratio (STP is reference with dry air assumed).
   * Dual element model 102DB1CB: Electra uses SN34263
   * Dual element model 102DB1CB:Kingair uses SN34260
   * Single element model 102U: Sabreliner uses SN27581
   * TFHER(Z) = 10.**(-1.7841*ALOG10(Z)-1.4025)    (308)
   * TFHER(Z) = 10.**(-1.7841*ALOG10(Z)-1.4025)    (312)
   * TFHER(Z) = 10.**(-1.7244*ALOG10(Z)-1.5989)    (307)
   * The function is included in the subroutine.  Only the constants
   * are set here 
   */
  tfher1 = -1.7841;
  tfher2 = -1.4025;

  // RECRN = recovery factor for reverse flow temp
  recfrn = 0.625;

  // RECFB = Rosemount 102E2AL (boom) recovery factor
  recfb = 0.95;

  // RECFF = Rosemount 102E2AL (fuselage) recovery factor
  recff = 0.985;

  // RECFKP = Fast response temp recovery factor
  recfkp = 0.80;

  // RECFRH= Rosemount 102E2AL (Heated) recovery factor
  recfrh = 0.985;

  // RECFW = Rosemount 102E2AL (wing) recovery factor
  recfw = 0.95;

  /* RECFRA = recovery factor for radome true air speed - depends on
   * TT used use 0.95 for now until search code is installed
   */
  recfra = 0.95;


  // Set up aircraft dependant values.
  switch (cfg.Aircraft())
  {
    case Config::KINGAIR:
      LogMessage("NCAR Kingair pcor's installed.");
      jwref	= 230 * XMPHMS;

      pcorQCW	= pcorw2;
      pcorPSW	= pcorw2;
      pcorPSF	= pcorr2;
      pcorQCF	= pcorr2;
      pcorQCR	= pcorr2;
      pcorPSFD	= pcorr2;
      break;

    case Config::ELECTRA:
      LogMessage("NCAR Electra pcor's installed.");
      jwref	= 300 * XMPHMS;
      recfrn	= 0.65;

      pcorQCW	= pcorw8;
      pcorPSW	= pcorw8;
      pcorQCR	= pcorf8;
      pcorQCF	= pcorf8;
      pcorPSF	= pcorf8;
      pcorPSFD	= pcorf8;
      break;

    case Config::NRL_P3:
      LogMessage("NRL P3 pcor's installed.");
      jwref	= 300 * XMPHMS;
      recfrn	= 0.65;

      pcorQCF	= pcorf3;
      pcorPSF	= pcorf3;
      pcorQCR	= pcorr3;
      break;


    case Config::NOAA_G4:
      LogMessage("NOAA G4 pcor's installed.");
      jwref	= 1 * XMPHMS;
      recfrn	= 0.65;

      pcorPSFD	= pcorf1;
      pcorQCR	= pcorr1;
      pcorQCF	= pcorf1_2;
      pcorPSF	= pcorf1_3;
      pcorQCW	= NULL;
      pcorPSW	= NULL;
      break;

    case Config::B57:
      LogMessage("NCAR WB57 pcor's installed.");
      pcorQCB	= pcorb7;
      pcorPSB	= pcorb7;

    case Config::C130:
    case Config::TECHS:
    case Config::TADS:	/* Ground systems */
      LogMessage("NCAR C-130 pcor's installed.");
      jwref	= 1 * XMPHMS;
      recfrn	= 0.65;

      sprintf(buffer, "%04d%02d", FlightDate[2], FlightDate[0]);
//printf("[%s] == [%s]\n", buffer, "200309");
      if (strcmp(buffer, "200309") > 0)
      { // AIRS-II & Later
        LogMessage("PCORS:  AIRS-II and later pcors().");
        pcorPSF	= pcorf1_3;
        pcorQCFR = pcorf1_3;	
      }
      else
      { // Pre-AIRS-II
        LogMessage("PCORS:  Pre-AIRS-II pcors().");
        pcorPSF	= pcorf1;
        pcorQCFR = pcorf1_2;
      }

      pcorPSFD	= pcorf1;
      pcorQCF	= pcorf1_2;
      pcorQCR	= pcorr1;	
      pcorQCW	= NULL;
      pcorPSW	= NULL;
      break;

    case Config::HIAPER:
      LogMessage("NCAR G5 pcor's installed.");

      sprintf(buffer, "%04d%02d", FlightDate[2], FlightDate[0]);
//printf("[%s] == [%s]\n", buffer, "200607");
      if (strcmp(buffer, "200607") > 0)
      { // Post-TREX
        LogMessage("PCORS:  Post-TREX pcors().");
        pcorQCF	= pcorf5_2;      
        pcorQCR	= pcorq5_2;
        pcorPSF	= pcorr5_2;
      }
      else
      { // TREX and earlier.
        LogMessage("PCORS:  TREX and ealier pcors().");
        pcorQCF	= pcorf5;
        pcorQCR	= pcorq5;
        pcorPSF	= pcorr5;
      }

      recfb	= 0.985;
      recfrh	= 0.985;
      tfher1	= -1.7244;
      tfher2	= -1.5989;
      break;

    case Config::SABRELINER:
      LogMessage("NCAR Saberliner pcor's installed.");
      recff	= 0.95;
      jwref	= 200 * XMPHMS;
      tfher1	= -1.7244;
      tfher2	= -1.5989;

      pcorQCB	= pcorb7;
      pcorPSB	= pcorb7;
      pcorQCF	= pcorf7;
      pcorPSF	= pcorf7;
      pcorQCR	= pcorf7;
      pcorPSFD	= pcorf7;
      break;

    case Config::SAILPLANE:
      LogMessage("NCAR Sailplane pcor's installed.");
      pcorQCW	= pcorw2;
      pcorPSW	= pcorw2;
      pcorPSF	= pcorr2;
      pcorQCF	= pcorr2;
      pcorQCR	= pcorr2;
      pcorPSFD	= pcorr2;
      break;

    default:
      fprintf(stderr, "Unknown aircraft [%d] encountered.\n", (int)cfg.Aircraft());
      exit(1);
  }

  NR_TYPE *tmp;
  if ( (tmp = GetDefaultsValue("RECFRH", "INIT_AC")) )
  {
    LogMessage("initAC.c: RECFRH found in Defaults, using.");
    recfrh = tmp[0];
  }
  if ( (tmp = GetDefaultsValue("RECFRN", "INIT_AC")) )
  {
    LogMessage("initAC.c: RECFRN found in Defaults, using.");
    recfrn = tmp[0];
  }
  if ( (tmp = GetDefaultsValue("RECFB", "INIT_AC")) )
  {
    LogMessage("initAC.c: RECFB found in Defaults, using.");
    recfb = tmp[0];
  }
}	/* END INITAIRCRAFTDEPENDANCIES */

/* Electra ------------------------------------------------------------ */
NR_TYPE pcorw8(NR_TYPE q, NR_TYPE q1)
{
  return(1.23 - 0.0846 * q);	
}

NR_TYPE pcorf8(NR_TYPE q, NR_TYPE q1)
{
  return(0.366 - 0.0182 * q);	
}

/* NRL P-3 ------------------------------------------------------------ */
NR_TYPE pcorr3(NR_TYPE q, NR_TYPE q1)
{
  return(0.0 - 0.02 * q);
}

NR_TYPE pcorf3(NR_TYPE q, NR_TYPE q1)
{
  return(-0.046 + q * (-0.0265 + 0.000087 * q));
}

/* KingAir ------------------------------------------------------------ */
NR_TYPE pcorw2(NR_TYPE q, NR_TYPE q1)
{
  return(-0.3112 - 0.0339 * q);
}

NR_TYPE pcorr2(NR_TYPE q, NR_TYPE q1)
{
  return(-1.781 + (0.0655 - 0.00025 * q) * q);
}

/* SaberLiner --------------------------------------------------------- */
NR_TYPE pcorb7(NR_TYPE q, NR_TYPE q1)
{
  return(0.9404 + q * (0.000239 * q - 0.078));
}

NR_TYPE pcorf7(NR_TYPE q, NR_TYPE q1)
{
  return(-0.8901 + q * (0.0460 + 0.000075 * q));
}

/* GV ---------------------------------------------------------------- */
NR_TYPE pcorr5(NR_TYPE q, NR_TYPE q1)
{
  return (-1.02 + 0.1565*q) + q1*(0.008 + q1*(7.1979e-09*q1 - 1.4072e-05));
}

NR_TYPE pcorq5(NR_TYPE q, NR_TYPE q1)
{
  return (1.65 +q*(0.04589 - q*0.000266) + q1*(-0.00685 + q1*3.0795e-06));
}

NR_TYPE pcorf5(NR_TYPE q, NR_TYPE q1)
{
  return ((3.08 - 0.0894*q1) + q*(-0.007474 + q*4.0161e-06));
}

// --- GV Post TREX.
NR_TYPE pcorr5_2(NR_TYPE q, NR_TYPE q1)
{
  NR_TYPE	pfax;

  pfax = (-1.02 + 0.1565*q) + q1*(0.008 + q1*(7.1979e-09*q1 - 1.4072e-05));  

/* Friehe pcor parameterization with q = adifr & q1 = mach_a   */
/*   pfax = (-2.089 + q *(0.196 + 0.00138*q) + q1 *(9.609 - 8.307*q1));  */
  return(pfax);
}

NR_TYPE pcorq5_2(NR_TYPE q, NR_TYPE q1)
{
  return (2.00 -q*(0.023809 + q*0.0001361));
}

NR_TYPE pcorf5_2(NR_TYPE q, NR_TYPE q1)
{
  NR_TYPE	pfix;

  pfix = (1.02+q1*(0.215 - 0.04*q/1000.) + q*(-0.003266 + q*1.613e-06)); 

  // Trailing Cone parameterization  
/*  pfix = (-1.02 + 0.1565*q1) + q*(0.008 + q*(7.1979e-09*q - 1.4072e-05));  */

  return(pfix);
}

/* C130 --------------------------------------------------------------- */
NR_TYPE pcorf1(NR_TYPE q, NR_TYPE q1)		/* For PSFD */
{
  NR_TYPE	pcor;
 
  pcor = (4.66 + 11.4405 * q);

  if (q < -0.31)
    pcor = 1.113;

  return(pcor);
}

NR_TYPE pcorr1(NR_TYPE q, NR_TYPE q1)		/* For QCR */
{
  NR_TYPE	pcor;

  pcor = (4.66 + 11.4405 * q);

  if (q < -0.31)
    pcor = 1.113;

  return(pcor);
}

NR_TYPE pcorf1_3(NR_TYPE q, NR_TYPE q1)	/* For new PSF */
{
  NR_TYPE	pcor;

  pcor = (3.29 + q * 0.0273);

  if  (q < 55.0)
    pcor = 4.7915;

  return(pcor);
}

NR_TYPE pcorf1_2(NR_TYPE q, NR_TYPE q1)	/* For QCF */
{
  NR_TYPE	pcor;

  pcor = (4.66 + 11.4405 * q);

  if (q < -0.31)
    pcor = 1.113;

  return(pcor);
}

/* END INITAC.C */
