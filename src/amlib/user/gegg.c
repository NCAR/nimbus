/************************************************************/
/*  gegg.c - routine to calculate temperatures and gust     */
/*           velocities from golden egg probe.              */
/*  05/03/93                                                */
/*           Adapted from Fortran version.                  */
/************************************************************/

/*   NOTE: Following code adapted for use in DLR project
         in Summer 1993.  Code is preliminary for Pre-SCUM
         and should be verified with Paul Lawson.
*/

#include <stdlib.h>
#include <math.h>

#include "nimbus.h"
#include "amlib.h"
#include "gegg.h"

#if defined(CMNT)
/********************************************************************/

c....... Golden Egg temperature probe - Modifications made 9/6/89 for Dropchem
c
c....... Further modifications made on 3/28/90 for Corpus Christi project.
c        Second prototype probe with three sensors and 5-hole gust system
c        added.  New calibrations for Prototype II from amplifier and
c        bath calibrations at NCAR. Probe to be mounted under left wing
c        at bomb rack hard point. Prototype I mounted on top of fuselage 
c        as in DropChem. 
c
c........variables for Prototype II: NOTE: Thermocouple (TC) and AD590
c                                    reference temperatures are numbered
c                                    from 3 to 5 (1 and 2 do not exist).
c
c           VOUT6  - voltage from TC3
c           VOUT7  - voltage from TC4              
c           VOUT8  - voltage from TC5
c           VOUT9  - voltage from AD590 ref3
c           VOUT10 - voltage from AD590 ref4
c           VOUT11 - voltage from AD590 ref5
c           VOUT12 - voltage from Egg II angle of attack
c           VOUT13 - voltage from Egg angle of sideslip
c           VOUT14 - voltage from Egg total pressure
c
c 
c....... also needed:
c           FACTOR: factor for dynamic-heating correction
c                   FACTOR = (1.+DELP/PMB)**RBYCP-1.   
c           where DELP = dynamic pressure in mb from DLR aircraft through SEA
c                        data sysem - set = 60 mb for laboratory
c                        tests at SPEC
c                 PMB  = static pressure in mb from DLR aircraft through SEA
c                        data system - set = 1000 mb for laboratory tests
c                        at SPEC
c                 RBYCP= (gas constant/specific heat)=0.2859
c
c           ATB = ROSEMOUNT TEMPERATURE (static temperature from DLR aircraft
c                 obtained through SEA data system - set equal to THMCP3 for
c                 laboratory tests at SPEC)
c           

/********************************************************************/
#endif

static NR_TYPE	xthmp3, xthmp4, xthmp5, xattck, xsslip, xtas;

/* -------------------------------------------------------------------- */
void sxthmp3(varp)
DERTBL	*varp;
{
	NR_TYPE	xtc3, xtc4, xtc5, xrf3, xrf4, xadif, xbdif, xqceg, atx,
		psxc, qcxc, atrf;
	double	factor, delpeg, facteg, taseg2;
	double	tiso3c, tiso4c, tiso5c, tiso3, tiso4, tiso5;
	double	t3emf, t4emf, t5emf, tiemf3, tiemf4, tiemf5;
	double	tem3, tem4, tem5, thm3t, thm5t, delt3, delt5;
	double	tasfac, pratio;

	xtc3	= GetSample(varp, 0);
	xtc4	= GetSample(varp, 1);
	xtc5	= GetSample(varp, 2);
	xrf3	= GetSample(varp, 3);
	xrf4	= GetSample(varp, 4);
	xadif	= GetSample(varp, 5);
	xbdif	= GetSample(varp, 6);
	xqceg	= GetSample(varp, 7);
	atx	= GetSample(varp, 8);
	psxc	= GetSample(varp, 9);
	qcxc	= GetSample(varp, 10);
	atrf	= GetSample(varp, 11);

	pratio = 1.0 + qcxc / psxc;
	factor = pow(pratio, RBYCP) - 1.0;

	/* Compute FACTEG from EGG using Rosemount 1201 0-15 psia =0-10 VDC
	 * pressure transducer.  NOTE: not used for laboratory tests
	 */
	delpeg = xqceg * 68.9476 * 1.5 - psxc;

	if (delpeg > 0.0)
		facteg = pow((1.0 + delpeg / psxc), RBYCP) - 1.0;
	else
		facteg = 0.0;


	/* Compute airspeed in m/s from gust port on EGG. NOTE: not used for
	 * laboratory tests.
	 */
	taseg2 = 2.0 * 1004.88 * (atrf + 273.15) * facteg;

	if (taseg2 > 0.0)
		xtas = sqrt(taseg2);
	else
		xtas = 0.0;


/* Thermocouple temperature computation procedure                   */
/*         (a).  get temperature of internal reference block (TISO) */
/*               from VOUT3                                         */
/*         (b).  convert from measured voltages to thermocouple emf */
/*               according to UW calibration                        */
/*         (c).  calculate emf contribution from Cu-Ch and Cu-Con   */
/*               junctions at reference block (E2 from TISO)        */
/*         (d).  correct from measured emf (due to exposed Con-Ch   */
/*               junction + reference-block junctions) to get       */
/*               contribution from exposed junction                 */
/*         (e).  invert from junction potential to get temperature  */
/*         (f).  apply standard dyn-heating correction with r=0.70  */

/* Start Prototype II calculations */

/*         (a): *****FIRST apply gain and offset form ICL7652 ampl  */
/*              in probe (note: on 900421 used first guess as gain of 10,000 */
/*              & offset of 250 uA - 20k res  from nominal values   */
/*              before RAF calibration was available).              */

	tiso3c = (xrf3 * 10.0 + 250.0) - 273.15;    
/*	tiso4c = (xrf4 * 10.0 + 250.0) - 273.15; 
	tiso5c = (xrf5 * 10.0 + 250.0) - 273.15; 
*/

	/* *****NEXT: correct AD590 sensors per calibration in NCAR         */
	/* bath on 900220 NOTE: numbers stamped on AD590's have following   */
	/* relationship to parameter names:	Sensor #5 = TISO3	*/
	/*					Sensor #6 = TISO4	*/
	/*					Sensor #7 = TISO5	*/
                                                 
	tiso3 = 0.996508 * tiso3c + 0.729857;
/*	tiso4 = 0.993968 * tiso4c - 3.23689;
	tiso5 = 1.00567 * tiso5c + 0.192579;
*/
	tiso4 = tiso3;		/* Hardwired 4 and 5 to 3 5-10-93 */
	tiso5 = tiso3;

	/*...... (b): */
	/* *****RANGE: -1 TO +1 mV (EMF) = +10 to -10 VDC (negative slope)  */
	/*             NOTE: preliminary calibration used on 900421         */

	t3emf =  0.1 * xtc3;
	t4emf =  0.1 * xtc4;
	t5emf =  0.1 * xtc5;
 
	/*...... (c): */
	/*          (calibration from thermocouple tables) */

	tiemf3=(5.86959E1 + (4.31109E-2 + (5.72204E-5
           + (-5.40207E-7 + (1.54259E-9 + (- 2.485E-12
           +(2.33897E-15 + (- 1.194629E-18 +2.556E-22*tiso3)*tiso3)
           *tiso3)*tiso3)*tiso3)*tiso3)*tiso3)*tiso3)*tiso3;

	tiemf4=(5.86959E1 + (4.31109E-2 + (5.72204E-5
           + (-5.40207E-7 + (1.54259E-9 + (- 2.485E-12
           +(2.33897E-15 + (- 1.194629E-18 +2.556E-22*tiso4)*tiso4)
           *tiso4)*tiso4)*tiso4)*tiso4)*tiso4)*tiso4)*tiso4;

	tiemf5=(5.86959E1 + (4.31109E-2 + (5.72204E-5
           + (-5.40207E-7 + (1.54259E-9 + (- 2.485E-12
           +(2.33897E-15 + (- 1.194629E-18 +2.556E-22*tiso5)*tiso5)
           *tiso5)*tiso5)*tiso5)*tiso5)*tiso5)*tiso5)*tiso5;

	/*..... convert from uV to mV */
	tiemf3 = 1e-3*tiemf3;
	tiemf4 = 1e-3*tiemf5;
	tiemf5 = 1e-3*tiemf5;

	/*...... (d): */
	tem3 = t3emf + tiemf3;
	tem4 = t4emf + tiemf4;
	tem5 = t5emf + tiemf5;

	/*....... (e): */
	xthmp3 = (1.70225E1 + (-2.2097E-1 + (5.48093E-3
                     -5.76699E-5*tem3)*tem3)*tem3)*tem3;
	xthmp4 = (1.70225E1 + (-2.2097E-1 + (5.48093E-3
                     -5.76699E-5*tem4)*tem4)*tem4)*tem4;
	xthmp5 = (1.70225E1 + (-2.2097E-1 + (5.48093E-3
                     -5.76699E-5*tem5)*tem5)*tem5)*tem5;

/* set up recovery factor calc - NOTE: not used in laboratory tests */

	thm3t = xthmp3;
	thm5t = xthmp5;
	delt3 = thm3t - atx;
	delt5 = thm5t - atx;
	tasfac = (0.5 * (NR_TYPE)pow((double)xtas, (double)2.0))/1004.9;

  
	/*...... (f): Correct for dynamic heating; r=0.7 */
	xthmp3 = (xthmp3 + 273.15) / (1.0 + 0.7 * factor) - 273.15; 
	xthmp4 = (xthmp4 + 273.15) / (1.0 + 0.7 * factor) - 273.15;
	xthmp5 = (xthmp5 + 273.15) / (1.0 + 0.7 * factor) - 273.15; 
 

	/*....End of Prototype II calculations, start computation of gust */
	/*      velocities from Prototype II 5-hole system.               */



/* **NOTE: Code needs to be added here for 5-hole gust system.*****/
/*
	xattck = ((xadif / xqceg) + 0.01414) / 0.08485;

        xsslip = ((xbdif / xqceg) - 0.002825) / 0.07448;
 

	xsslip = .....
*/

	PutSample(varp, xthmp3);

}	/* END SGEGG1 */

/* -------------------------------------------------------------------- */
void sxthmp4(varp)
DERTBL	*varp;
{
	PutSample(varp, xthmp4);
}

/* -------------------------------------------------------------------- */
void sxthmp5(varp)
DERTBL	*varp;
{
	PutSample(varp, xthmp5);
}

/* -------------------------------------------------------------------- */
void sxattck(varp)
DERTBL	*varp;
{
	PutSample(varp, xattck);
}

/* -------------------------------------------------------------------- */
void sxsslip(varp)
DERTBL	*varp;
{
	PutSample(varp, xsslip);
}

/* -------------------------------------------------------------------- */
void sxtas(varp)
DERTBL	*varp;
{
	PutSample(varp, xtas);
}

/* GEGG.C */
