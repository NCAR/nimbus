/*
-------------------------------------------------------------------------
OBJECT NAME:	IDEAS.c

FULL NAME:	

ENTRY POINTS:	sfunc()

STATIC FNS:	

DESCRIPTION:	Template for AMLIB std & user directories.

INPUT:		

OUTPUT:		

REFERENCES:	

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 2002
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static const NR_TYPE Tzero = 273.0;
static NR_TYPE	cal[] = { 58.915, 33.57, 1.0 };
static NR_TYPE	supersat, conc;

static NR_TYPE	esw(NR_TYPE T);


/* -------------------------------------------------------------------- */
void suwflow(DERTBL *varp)
{
  NR_TYPE	volts, flow, atx, psx;

  volts = GetSample(varp, 0);
  atx = GetSample(varp, 1);
  psx = GetSample(varp, 2);

  if (volts < 0.0)
    volts = 0.0;

  flow = M_PI / 4.0 * pow(2.54*0.875, 2) * sqrt(2 * (volts - 1) * 980.2 *
      2.54 * 2.87e6 * (atx + 273.15) / ((psx + 50)*1e3)) * 60 * 1e-3;

  PutSample(varp, flow);

}

/* -------------------------------------------------------------------- */
void sccns(DERTBL *varp)
{
  PutSample(varp, supersat);

}

/* -------------------------------------------------------------------- */
void sccnc(DERTBL *varp)
{
  int		ccn_status, sig;
  NR_TYPE	bt, avet, delv;
  NR_TYPE	dt, tt, lp, stat;
  static int	ccnprev=10, sigmin=10, sigmax=-10, prestat=10;
  static int	difstat, sec=0, flg=0;
  static NR_TYPE	ttave = 0, btave = 0;

  dt = GetSample(varp, 0);
  tt = GetSample(varp, 1);
  lp = GetSample(varp, 2);
  sig = (int)GetSample(varp, 3);
  stat = GetSample(varp, 4);

  /*
  set ccn status:
  0: idle
  1: twait
  2: flush
  3: detect
  10: error
  */

  if (stat > -0.1) ccn_status = 0;
  if (stat > 0.9) ccn_status = 1;
  if (stat > 1.9) ccn_status = 2;
  if (stat > 2.8) ccn_status = 3;
  if (stat > 9.9) ccn_status = 10;

  // set plate temp
  bt = tt + dt;

  // min CCN voltage during last 2 sec of "flush" and 1st sec of "detect"
  // max CCN voltage during "detect" (after first sec)
  // save values for supersaturation during flush stage

  if (ccn_status == 2){
    sec++;
    ttave += tt;
    btave += bt;
    if (sec > 3){
      sigmin = (sigmin < sig) ? sigmin : sig ;
      flg = 1;
    }
  }
  if (ccn_status == 3){
    if (flg == 1){
      sec++;
      sigmin = (sigmin < sig) ? sigmin : sig ;
      ttave += tt;
      btave += bt;
    }
    sigmax = (sigmax > sig) ? sigmax : sig ;
  }
  difstat = prestat - ccn_status;

  // calculate supersaturation

  if (difstat == 2){
    tt = ttave/sec;
    bt = btave/sec;
        avet = (tt + bt) / 2;
    if (esw(avet+Tzero) > 0)
      supersat = (((esw(tt+Tzero) + esw(bt+Tzero))/2) - esw(avet+Tzero)) 
        / esw(avet+Tzero) * 100;
    else
      supersat = 0;

    // calculate CCN concentration

    if (ccn_status != 0)
    delv = sigmax - sigmin;
    if (supersat > 0)
      conc = delv * 91.35 * pow(supersat, -0.562);
    sigmin = 10;
    sigmax = -10;
    ttave = 0;
    btave = 0;
    sec = 0;
  }


  prestat = ccn_status;

  if (stat == 10 || stat == 0){
    sigmin = 10;
    sigmax - -10;
  }


//  printf("status=%d,sec=%d,tt=%f,bt=%f,difstat=%d,ss=%f,conc=%f\n",

/*       ccn_status,sec,tt,bt,difstat,supersat,conc); */

  PutSample(varp, conc);
  
}

/*-------------------------------------------------------------------------*/
static NR_TYPE esw(NR_TYPE T)
{
  // determines the saturated vapour pressure (P&K) constants
  NR_TYPE a0=6.107799961;
  NR_TYPE a1=4.436518521*(pow(10,-1));
  NR_TYPE a2=1.428945805*(pow(10,-2));
  NR_TYPE a3=2.650648471*(pow(10,-4));
  NR_TYPE a4=3.031240396*(pow(10,-6));
  NR_TYPE a5=2.034080948*(pow(10,-8));
  NR_TYPE a6=6.136820929*(pow(10,-11));

  // calculation
  T -= Tzero;
  return(a0+T*(a1+T*(a2+T*(a3+T*(a4+T*(a5+a6*T))))));

}

/* END IDEAS.C */
