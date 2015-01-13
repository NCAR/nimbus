/*
-------------------------------------------------------------------------
OBJECT NAME:	initAC.c

FULL NAME:	Initialize Aircraft Dependencies

DESCRIPTION:	Pressure Correction (pcors) functions for each aircraft.
		At startup, assign pcorrs based on aircraft to a set of
		function pointers.  These fn ptrs are then called by all
		PS and QC amlib fns (qcbc.c, psbc.c, etc).

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2014
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static const NR_TYPE XMPHMS = 0.44704; // conversion factor mph to meters/sec

// P3 PCORS made available to Defaults file starting with TPARC.
static NR_TYPE P3_RADOME_PCORS[2] = { 0.0, 0.02 };
static NR_TYPE P3_FUSELAGE_PCORS[3] = { -0.046, -0.0265, 0.000087 };

extern int	FlightDate[];

NR_TYPE	recfb, recff, recfrh, recfw, recfra, recfrhGV[2];

// Function pointers called by QC & PS functions (qcrc.c psfc.c, etc).
NR_TYPE	(*pcorPSF)(NR_TYPE), (*pcorQCF)(NR_TYPE),
	(*pcorPSFD)(NR_TYPE), (*pcorQCR)(NR_TYPE),
	(*pcorQCW)(NR_TYPE), (*pcorPSW)(NR_TYPE),
	(*pcorPSB)(NR_TYPE), (*pcorQCB)(NR_TYPE);
//	(*pcorQCFR)(NR_TYPE);

// Version 2 of above functions, that use attack.  GV & C130 only.
NR_TYPE	(*pcorPSFv2)(NR_TYPE, NR_TYPE, NR_TYPE), (*pcorQCFv2)(NR_TYPE, NR_TYPE, NR_TYPE),
	(*pcorPSFDv2)(NR_TYPE, NR_TYPE, NR_TYPE), (*pcorQCRv2)(NR_TYPE, NR_TYPE, NR_TYPE),
	(*pcorQCFRv2)(NR_TYPE, NR_TYPE, NR_TYPE);


// Aircraft specific pcor (pressure correction).
//  Letter designation is for location; w=wing, b=boom, r=radome, f=fuselage.
//  Number designation is for aircraft; 8=Electtra, 2=Kingair, 1=C130, 5=GV, 7=Saberliner.
NR_TYPE	pcorw8(NR_TYPE), pcorf8(NR_TYPE),
	pcorw2(NR_TYPE), pcorr2(NR_TYPE),
	pcorb7(NR_TYPE), pcorf7(NR_TYPE),
// NRL-P3, T-PARC only?
	pcorf3(NR_TYPE), pcorr3(NR_TYPE),
// C130
	pcorr1(NR_TYPE), pcorf1(NR_TYPE), pcorf1_2(NR_TYPE), pcorf1_3(NR_TYPE),
	pcorf1v2(NR_TYPE, NR_TYPE, NR_TYPE), pcorr1v2(NR_TYPE, NR_TYPE, NR_TYPE),
// GV
	pcorr5(NR_TYPE),pcorf5(NR_TYPE), pcorq5(NR_TYPE), pcorr5_2(NR_TYPE, NR_TYPE, NR_TYPE),
	pcorf5_2(NR_TYPE, NR_TYPE, NR_TYPE), pcorq5_2(NR_TYPE, NR_TYPE, NR_TYPE),
	pcorf5v2(NR_TYPE, NR_TYPE, NR_TYPE);

/* reference airspeed on J-W liquid water content converted from MPH
 * to m/s
 */
NR_TYPE	jwref;

NR_TYPE	tfher1,tfher2;

/* -------------------------------------------------------------------- */
void InitAircraftDependencies()
{
  float *tmp;

  ILOG(("InitAircraftDependencies"));

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

  // RECFB = Rosemount 102E2AL (boom) recovery factor
  recfb = 0.95;

  // RECFF = Rosemount 102E2AL (fuselage) recovery factor
  recff = 0.985;

  // RECFRH= Rosemount 102E2AL (Heated) recovery factor
  recfrh = 0.985;
  recfrhGV[0] = 0.985;
  recfrhGV[1] = 0.985;

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

      if ((tmp = GetDefaultsValue("P3_RADOME_PCORS", "INIT_AC")) != NULL)
      {
        P3_RADOME_PCORS[0] = tmp[0];
        P3_RADOME_PCORS[1] = tmp[1];
        sprintf(buffer,
        "initAC: P3_RADOME_PCORS set to %f, %f from Defaults file.\n",
          P3_RADOME_PCORS[0], P3_RADOME_PCORS[1]);
        LogMessage(buffer);
      }

      if ((tmp = GetDefaultsValue("P3_FUSELAGE_PCORS", "INIT_AC")) != NULL)
      {
        P3_FUSELAGE_PCORS[0] = tmp[0];
        P3_FUSELAGE_PCORS[1] = tmp[1];
        P3_FUSELAGE_PCORS[2] = tmp[2];
        sprintf(buffer,
        "initAC: P3_FUSELAGE_PCORS set to %f, %f, %f from Defaults file.\n",
          P3_RADOME_PCORS[0], P3_RADOME_PCORS[1], P3_FUSELAGE_PCORS[2]);
        LogMessage(buffer);
      }

      pcorQCF	= pcorf3;
      pcorPSF	= pcorf3;
      pcorQCR	= pcorr3;
      break;

/*  Should use something closer to GV, than C130.
    case Config::NOAA_G4:
      LogMessage("NOAA G4 pcor's installed.");
      jwref	= 1 * XMPHMS;

      pcorPSFD	= pcorf1;
      pcorQCR	= pcorr1;
      pcorQCF	= pcorf1_2;
      pcorPSF	= pcorf1_3;
      pcorQCW	= NULL;
      pcorPSW	= NULL;
      break;
*/
    case Config::B57:
      LogMessage("NCAR WB57 pcor's installed.");
      pcorQCB	= pcorb7;
      pcorPSB	= pcorb7;

    case Config::C130:
    case Config::TECHS:
    case Config::TADS:	/* Ground systems */
      LogMessage("NCAR C-130 pcor's installed.");
      jwref	= 1 * XMPHMS;

      sprintf(buffer, "%04d%02d", FlightDate[2], FlightDate[0]);
//printf("[%s] == [%s]\n", buffer, "200309");
      if (strcmp(buffer, "200309") > 0)
      { // AIRS-II & Later
        LogMessage("PCORS:  AIRS-II and later pcors().");
        pcorPSFv2	= pcorr1v2;
        pcorQCFRv2	= pcorr1v2;
      }
      else
      { // Pre-AIRS-II
        LogMessage("PCORS:  Pre-AIRS-II pcors().");
        pcorPSFv2	= pcorr1v2;
        pcorQCFRv2	= pcorr1v2;
      }

      pcorPSFDv2= pcorf1v2;
      pcorQCFv2	= pcorf1v2;
      pcorQCRv2	= pcorf1v2;
      pcorQCW	= NULL;
      pcorPSW	= NULL;
      break;

    case Config::HIAPER:
      LogMessage("NCAR G5 pcor's installed.");

      sprintf(buffer, "%04d%02d", FlightDate[2], FlightDate[0]);
//printf("[%s] == [%s]\n", buffer, "200607");
//      if (strcmp(buffer, "200607") > 0)
//      { // Post-TREX
        LogMessage("PCORS:  Post-TREX pcors().");
        pcorQCFv2 = pcorf5v2;
        pcorQCRv2 = pcorf5v2;
        pcorPSFv2 = pcorf5v2;
/*
      }
      else
      { // TREX and earlier.
        LogMessage("PCORS:  TREX and ealier pcors().");
        pcorQCF	= pcorf5;
        pcorQCR	= pcorq5;
        pcorPSF	= pcorr5;
      }
*/
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
      sprintf(buffer, "Unknown aircraft [%d] encountered.\n", (int)cfg.Aircraft());
      HandleFatalError(buffer);
  }

  if ( (tmp = GetDefaultsValue("RECFRH", "INIT_AC")) )
  {
    LogMessage("initAC.c: RECFRH found in Defaults, using.");
    recfrhGV[0] = tmp[0];
    recfrhGV[1] = tmp[1];
  }
  if ( (tmp = GetDefaultsValue("RECFB", "INIT_AC")) )
  {
    LogMessage("initAC.c: RECFB found in Defaults, using.");
    recfb = tmp[0];
  }
}	/* END INITAIRCRAFTDEPENDANCIES */

/* Electra ------------------------------------------------------------ */
NR_TYPE pcorw8(NR_TYPE q)
{
  return(1.23 - 0.0846 * q);	
}

NR_TYPE pcorf8(NR_TYPE q)
{
  return(0.366 - 0.0182 * q);	
}

/* NRL P-3 ------------------------------------------------------------ */
NR_TYPE pcorr3(NR_TYPE q)
{
  return(P3_RADOME_PCORS[0] - P3_RADOME_PCORS[1] * q);
}

NR_TYPE pcorf3(NR_TYPE q)
{
  return(P3_FUSELAGE_PCORS[0] + q * (P3_FUSELAGE_PCORS[1] + P3_FUSELAGE_PCORS[2] * q));
}

/* KingAir ------------------------------------------------------------ */
NR_TYPE pcorw2(NR_TYPE q)
{
  return(-0.3112 - 0.0339 * q);
}

NR_TYPE pcorr2(NR_TYPE q)
{
  return(-1.781 + (0.0655 - 0.00025 * q) * q);
}

/* SaberLiner --------------------------------------------------------- */
NR_TYPE pcorb7(NR_TYPE q)
{
  return(0.9404 + q * (0.000239 * q - 0.078));
}

NR_TYPE pcorf7(NR_TYPE q)
{
  return(-0.8901 + q * (0.0460 + 0.000075 * q));
}

/* GV ---------------------------------------------------------------- */
/* delete pcorr5 and pcorq5  as not used 

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

  // LAMS PCOR per Al Cooper.  4/6/12
  pfax = (0.00696 + 0.6678*q) + q1*(-0.05965 + q1*(-0.2833 - 0.2437*q1)); 

  return(pfax);
}

NR_TYPE pcorq5_2(NR_TYPE q, NR_TYPE q1)
{
  // LAMS PCOR per Al Cooper.  4/6/12
  return (1.37 +q*(-0.01773 - 0.000156*q));
}

NR_TYPE pcorf5_2(NR_TYPE q, NR_TYPE q1)
{
  NR_TYPE	pfix;

  // LAMS PCOR per Al Cooper.  4/6/12
  pfix = (0.00696 + 0.6678*q) + q1*(-0.05965 + q1*(-0.2833 - 0.2437*q1)); 

  return(pfix);
}
*/

// --- Cooper August 2014
NR_TYPE pcorf5v2(NR_TYPE Qm, NR_TYPE Pm, NR_TYPE Attack)
{
  static double a[] = { -0.012255, 0.075372, -0.087508, 0.002148 }; // New 10/1/14

  NR_TYPE M, deltaP;
  if (Qm < 0.01) Qm = 0.01;

  // --- Added by JAA Oct 2014 per Cooper 3 Oct memo
  if (isnan(Attack)) Attack = 3.0;

  M = sqrt( 5.0 * (pow((Qm+Pm)/Pm, Rd_DIV_Cpd) - 1.0) ); // Mach #
  deltaP = Pm * (a[0] + a[1] * Qm/Pm + a[2] * M*M*M + a[3] * Attack);

  // Taper if Qm is too small (take-off / landing).
  if (Qm < 40.0) deltaP *= pow(Qm/40.0, 3.0);

  return deltaP;
}


/* C130 --------------------------------------------------------------- */
/*
NR_TYPE pcorf1(NR_TYPE q, NR_TYPE q1)	// For PSFD
{
  return (0.00163 + 0.0214 * q + 0.0145 * pow(q1,0.5));
}

NR_TYPE pcorr1(NR_TYPE q, NR_TYPE q1)	// For QCR
{
  return (0.00163 + 0.0214 * q + 0.0145 * pow(q1,0.5));
}

NR_TYPE pcorf1_3(NR_TYPE q, NR_TYPE q1)	// For new PSF
{
  return (2.79 + q * 0.0273);
}

NR_TYPE pcorf1_2(NR_TYPE q, NR_TYPE q1)	// For QCF
{
  return (0.00163 + 0.0214 * q + 0.0145 * pow(q1,0.5));
}
*/

NR_TYPE pcorf1v2(NR_TYPE Qm, NR_TYPE Pm, NR_TYPE Attack)	// PSFD
{
  static double a[] = { -0.00637, 0.001366, 0.0149 }; // New 12/9/14

  NR_TYPE M, deltaP;
  if (Qm < 0.01) Qm = 0.01;

  // --- Added by JAA Oct 2014 per Cooper 3 Oct memo
  if (isnan(Attack)) Attack = 3.0;

  M = sqrt( 5.0 * (pow((Qm+Pm)/Pm, Rd_DIV_Cpd) - 1.0) ); // Mach #
  deltaP = Pm * (a[0] + a[1] * Attack + a[2] * M);

  // Taper if Qm is too small (take-off / landing).
  if (Qm < 40.0) deltaP *= pow(Qm/40.0, 3.0);

  return deltaP;
}

NR_TYPE pcorr1v2(NR_TYPE Qm, NR_TYPE Pm, NR_TYPE Attack)	// PSFRD
{
  static double a[] = { -0.00754, 0.000497, 0.0368 }; // New 12/9/14

  NR_TYPE M, deltaP;
  if (Qm < 0.01) Qm = 0.01;

  // --- Added by JAA Oct 2014 per Cooper 3 Oct memo
  if (isnan(Attack)) Attack = 3.0;

  M = sqrt( 5.0 * (pow((Qm+Pm)/Pm, Rd_DIV_Cpd) - 1.0) ); // Mach #
  deltaP = Pm * (a[0] + a[1] * Attack + a[2] * M);

  // Taper if Qm is too small (take-off / landing).
  if (Qm < 40.0) deltaP *= pow(Qm/40.0, 3.0);

  return deltaP;
}

/* END INITAC.C */
