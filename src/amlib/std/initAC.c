/*
-------------------------------------------------------------------------
OBJECT NAME:	initAC.c

FULL NAME:	Initialize Aircraft Dependencies

ENTRY POINTS:	InitAircraftDependencies()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		none

OUTPUT:		Globals for AMLIB corrections

REFERENCES:	none

REFERENCED BY:	cb_main.c, ps*.c, qc*.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2004
-------------------------------------------------------------------------
*/

#include "raf.h"
#include "nimbus.h"
#include "amlib.h"

#define XMPHMS	0.44704    /* conversion factor mph to meters/sec */

extern int	Aircraft, FlightDate[];

NR_TYPE	recfb, recff, recfkp, recfrh, recfrn, recfw, recfra;

NR_TYPE	(*pcorPSF)(NR_TYPE, NR_TYPE), (*pcorQCF)(NR_TYPE, NR_TYPE),
	(*pcorPSFD)(NR_TYPE, NR_TYPE), (*pcorQCR)(NR_TYPE, NR_TYPE),
	(*pcorQCW)(NR_TYPE, NR_TYPE), (*pcorPSW)(NR_TYPE, NR_TYPE),
	(*pcorPSB)(NR_TYPE, NR_TYPE), (*pcorQCB)(NR_TYPE, NR_TYPE),
	(*pcorQCFR)(NR_TYPE, NR_TYPE);

NR_TYPE	pcorw8(NR_TYPE, NR_TYPE),pcorf8(NR_TYPE, NR_TYPE),pcorw2(NR_TYPE, NR_TYPE),
	pcorr2(NR_TYPE, NR_TYPE), pcorb7(NR_TYPE, NR_TYPE),pcorf7(NR_TYPE, NR_TYPE),
	pcorr1(NR_TYPE, NR_TYPE),pcorf1(NR_TYPE, NR_TYPE), pcorf3(NR_TYPE, NR_TYPE),
	pcorr3(NR_TYPE, NR_TYPE),pcorf1_2(NR_TYPE, NR_TYPE), pcorf1_3(NR_TYPE, NR_TYPE),
	pcorf1_4(NR_TYPE, NR_TYPE);

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

  /* RECRN = recovery factor for reverse flow temp */
  recfrn = 0.625;

  /* RECFB = Rosemount 102E2AL (boom) recovery factor */
  recfb = 0.95;

  /* RECFF = Rosemount 102E2AL (fuselage) recovery factor */
  recff = 0.985;

  /* RECFKP = Fast response temp recovery factor  */
  recfkp = 0.80;

  /* RECFRH= Rosemount 102E2AL (Heated) recovery factor */
  recfrh = 0.98;

  /* RECFW = Rosemount 102E2AL (wing) recovery factor */
  recfw = 0.95;

  /* RECFRA = recovery factor for radome true air speed - depends on
   * TT used use 0.95 for now until search code is installed
   */
  recfra = 0.95;


  /* Set up aircraft dependant values.
   */
  switch (Aircraft)
  {
    case KINGAIR:
      jwref	= 230 * XMPHMS;

      pcorQCW	= pcorw2;
      pcorPSW	= pcorw2;
      pcorPSF	= pcorr2;
      pcorQCF	= pcorr2;
      pcorQCR	= pcorr2;
      pcorPSFD	= pcorr2;
      break;

    case ELECTRA:
      jwref	= 300 * XMPHMS;
      recfrn	= 0.65;

      pcorQCW	= pcorw8;
      pcorPSW	= pcorw8;
      pcorQCR	= pcorf8;
      pcorQCF	= pcorf8;
      pcorPSF	= pcorf8;
      pcorPSFD	= pcorf8;
      break;

    case NRL_P3:
      jwref	= 300 * XMPHMS;
      recfrn	= 0.65;

      pcorQCF	= pcorf3;
      pcorPSF	= pcorf3;
      pcorQCR	= pcorr3;
      break;


    case NOAA_G4:
      jwref	= 1 * XMPHMS;
      recfrn	= 0.65;

      pcorPSFD	= pcorf1;
      pcorQCR	= pcorr1;
      pcorQCF	= pcorf1_2;
      pcorPSF	= pcorf1_3;
      pcorQCW	= NULL;
      pcorPSW	= NULL;
      break;

    case B57:
      pcorQCB	= pcorb7;
      pcorPSB	= pcorb7;

    case C130:
    case 300:
    case 600:	/* Ground systems....to become HIAPER??? */
      jwref	= 1 * XMPHMS;
      recfrn	= 0.65;

      sprintf(buffer, "%04d%02d", FlightDate[2], FlightDate[0]);
//printf("[%s] == [%s]\n", buffer, "200309");
      if (strcmp(buffer, "200309") > 0)
      { // AIRS-II & Later
        LogMessage("PCORS:  AIRS-II and later pcors().\n");
        pcorPSF	= pcorf1_3;
        pcorQCFR = pcorf1_4;	
      }
      else
      { // Pre-AIRS-II
        LogMessage("PCORS:  Pre-AIRS-II pcors().\n");
        pcorPSF	= pcorf1;
        pcorQCFR = pcorf1_2;
      }

      pcorPSFD	= pcorf1;
      pcorQCF	= pcorf1_2;
      pcorQCR	= pcorr1;	
      pcorQCW	= NULL;
      pcorPSW	= NULL;
      break;

    case SABRELINER:
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

    case SAILPLANE:
      pcorQCW	= pcorw2;
      pcorPSW	= pcorw2;
      pcorPSF	= pcorr2;
      pcorQCF	= pcorr2;
      pcorQCR	= pcorr2;
      pcorPSFD	= pcorr2;
      break;

    default:
      fprintf(stderr, "Unknown aircraft [%d] encountered.\n", Aircraft);
      exit(1);
  }
}	/* END INITAIRCRAFTDEPENDANCIES */

/* Electra ------------------------------------------------------------ */
NR_TYPE pcorw8(NR_TYPE q, NR_TYPE q1)
{
/*  return(0.517 - 0.0202 * q); */
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
/*  return(-0.134 - 0.0182 * q);			*/
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

/* C130 --------------------------------------------------------------- */
NR_TYPE pcorf1(NR_TYPE q, NR_TYPE q1)		/* For PSFD */
{
  NR_TYPE	pcor;

/*	return(2.14);	From C130/Electra intercomp 6/95	*/
/*	return(3.66 - q * 0.01882);		*/
/*  test run from trailing cone data  9/19/2003	*/
 
  pcor = (4.66 + 11.4405 * q);

  if (q < -0.31)
    pcor = 1.113;

  return(pcor);

}

NR_TYPE pcorr1(NR_TYPE q, NR_TYPE q1)		/* For QCR */
{
  NR_TYPE	pcor;

/*	return(1.80);	From C130/Electra intercomp 6/95	*/	
/*	return(3.47 - q * 0.02865);	*/
/* test run from trailing cone data	*/
/*	return(2.17 - q * 0.01906);	*/

  pcor = (4.66 + 11.4405 * q);

  if (q < -0.31)
    pcor = 1.113;

  return(pcor);

}

NR_TYPE pcorf1_4(NR_TYPE q, NR_TYPE q1)	/* For new QCFR */
{
  NR_TYPE	pcor, pfix;
  

   pcor = (4.26 + q * (0.000368 * q - 0.01464));

  if (q < 60.6)
    pcor = (7.75 - 0.05 * q);	

   pfix = (-1.54 + 0.00154 * q1);

  if (q1 < 675.0)	
    pfix = -0.5;	

   pcor = (pcor + pfix);


  return(pcor);

}

NR_TYPE pcorf1_3(NR_TYPE q, NR_TYPE q1)	/* For new PSF */
{
  NR_TYPE	pcor, pfix;

/*	return(1.26);	From C130/Electra intercomp 6/95	*/
/*	return(3.66 - q * 0.01882);	*/
/* test run from trailing cone data	9/19/2003	*/

   pcor = (4.26 + q * (0.000368 * q - 0.01464));

  if (q < 60.6)
    pcor = (7.75 - 0.050 * q);

  pfix = (-1.54 + 0.00154 * q1);

  if (q1 < 675.0)	
    pfix = -0.5;	

   pcor = (pcor + pfix);

  return(pcor);

}

NR_TYPE pcorf1_2(NR_TYPE q, NR_TYPE q1)	/* For QCF */
{
  NR_TYPE	pcor;

/*	return(0.40 + 0.0229 * q);	From C130/Electra intercomp 6/95 */
/*	return(2.59 - q * 0.01276);	*/
/* test rnu from trailing cone data	*/
/*	return(1.69 - q * 0.01276);	*/


  pcor = (4.66 + 11.4405 * q);

  if (q < -0.31)
    pcor = 1.113;

  return(pcor);

}

/* END INITAC.C */
