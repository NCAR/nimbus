/*
-------------------------------------------------------------------------
OBJECT NAME:	rhola.c

FULL NAME:	Absolute humidity (Lyman-alpha) (g/m3)

ENTRY POINTS:	srhola()

STATIC FNS:	none

DESCRIPTION:  

INPUT:    

OUTPUT:    

REFERENCES:	lyalf.c

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2005
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

#define GL1  0.05
#define STPT  273.0	/* STP temperature reference (K)    */
#define CKO2  0.34	/* Absorption coefficient for oxygen    */
#define CAMP  0.4630	/* LOG amplification factor      */
#define STPP  1013.0	/* STP pressure reference (mbar)  */
#define CO2P  0.2315	/* Percentage of oxygen in dry air    */

static NR_TYPE  rholap[2], corc1[2] = {0.0, 0.0};
static int  n1[2] = {0, 0};

static NR_TYPE compute_rhola(int indx, DERTBL *varp, NR_TYPE vla, NR_TYPE psxc, NR_TYPE atx, NR_TYPE qcxc, NR_TYPE rhodt, NR_TYPE XC, NR_TYPE THRSH, NR_TYPE CX[]);

/*  Values from /home/local/proj/Defaults on 23 April 1998           RLR */
static NR_TYPE cx_1[4] = {4.2, 0.8712, -0.0045, -0.0056};
static NR_TYPE cx_2[4] = {4.2, 0.8712, -0.0045, -0.0056};
static NR_TYPE  XC_1 = 0.50, XC_2 = 0.50, THRSH_1 = 900.0, THRSH_2 = 900.0;
static NR_TYPE  *CX_1, *CX_2;

/* -------------------------------------------------------------------- */
void lymanInit(DERTBL *varp)
{
  NR_TYPE	*tmp;

  if ((tmp = GetDefaultsValue("XC_1", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function lymanInit.\n", XC_1);
    LogMessage(buffer);
  }
  else
    XC_1 = tmp[0];

  if ((tmp = GetDefaultsValue("THRSH_1", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function lymanInit.\n", THRSH_1);
    LogMessage(buffer);
  }
  else
    THRSH_1 = tmp[0];

  if ((tmp = GetDefaultsValue("CX_1", varp->name)) == NULL)
  {
    CX_1 = cx_1;
    sprintf(buffer, "Values returned = %f, %f, %f, %f in AMLIB function lymanInit.\n", CX_1[0], CX_1[1], CX_1[2], CX_1[3]);
    LogMessage(buffer);
  }
  else
    CX_1 = tmp;

}  /* END LYMANINIT */

/* -------------------------------------------------------------------- */
void lyman1Init(DERTBL *varp)
{
  NR_TYPE	*tmp;

  if ((tmp = GetDefaultsValue("XC_2", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function lymanInit.\n", XC_2);
    LogMessage(buffer);
  }
  else
    XC_2 = tmp[0];

  if ((tmp = GetDefaultsValue("THRSH_2", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function lymanInit.\n", THRSH_2);
    LogMessage(buffer);
  }
  else
    THRSH_2 = tmp[0];

  if ((tmp = GetDefaultsValue("CX_2", varp->name)) == NULL)
  {
    CX_2 = cx_2;
    sprintf(buffer, "Values returned = %f, %f, %f, %f in AMLIB function lymanInit.\n", CX_2[0], CX_2[1], CX_2[2], CX_2[3]);
    LogMessage(buffer);
  }
  else
    CX_2 = tmp;

}  /* END LYMAN1INIT */

/* -------------------------------------------------------------------- */
void srhola(DERTBL *varp)
{
  NR_TYPE  vla, psxc, atx, qcxc, rhodt;

  vla  = GetSample(varp, 0);
  psxc  = GetSample(varp, 1);
  atx  = GetSample(varp, 2);
  qcxc  = GetSample(varp, 3);
  rhodt  = GetSample(varp, 4);

  PutSample(varp, compute_rhola(0, varp, vla, psxc, atx, qcxc, rhodt,
        XC_1, THRSH_1, CX_1));

}  /* END SRHOLA */

/* -------------------------------------------------------------------- */
void srhola1(DERTBL *varp)
{
  NR_TYPE  vla, psxc, atx, qcxc, rhodt;

  vla  = GetSample(varp, 0);
  psxc  = GetSample(varp, 1);
  atx  = GetSample(varp, 2);
  qcxc  = GetSample(varp, 3);
  rhodt  = GetSample(varp, 4);

  PutSample(varp, compute_rhola(1, varp, vla, psxc, atx, qcxc, rhodt,
        XC_2, THRSH_2, CX_2));

}  /* END SRHOLA1 */

/* -------------------------------------------------------------------- */
static NR_TYPE compute_rhola(int indx, DERTBL *varp, NR_TYPE vla, NR_TYPE psxc, NR_TYPE atx, NR_TYPE qcxc, NR_TYPE rhodt, NR_TYPE XC, NR_TYPE THRSH, NR_TYPE CX[])
{
  NR_TYPE  rhola, rflag, rhota, rxpr1, vo2, vh2o, dfdla, del, errlr, ertst;

  rhola = rholap[indx];

  /* Calculate oxygen correction to VLA
   */
  vo2 = ((CKO2*CO2P*XC*STPT) / (STPP*CAMP)) * psxc / (atx+273.16) ;
  vh2o = vla - vo2;

  if (n1[indx] < 300)
    ++n1[indx];


  /* Estimate Lyman-Alpha humidity using empirical 3rd order fit
   */
  rhota = (CX[0] + vh2o * (CX[1] + vh2o * (CX[2] + vh2o * CX[3]))) / XC;

  /* Take derivative of RHOTA with XC a constant  */
  rxpr1 = (CX[1] + vh2o * (2.0 * CX[2] + vh2o * (3.0 * CX[3]))) / XC;

  if (rxpr1 < 0.1)
    rxpr1 = 0.1;

  /* Set recouple conditions  */
  if (n1[indx] == 1)
    {
    rhola = rhota;
    corc1[indx] = 0.0;
    }

  /* Couple system and determine the correction for each time step */
  if ((dfdla = rhola - rhodt) == 0.0 || psxc <= 300.0)
    del = 0.0;
  else
    {
    NR_TYPE  coup = exp(-GL1 * n1[indx]);
    NR_TYPE  tdel = 1.0 / varp->OutputRate;

    del = fabs((double)1.0 / rxpr1 * fabs((double)dfdla) *
              (coup + 0.001 * tdel));

    if (dfdla < 0.0)
      del = -del;
    }

  corc1[indx] += del;
  vh2o -= corc1[indx];

  /* Calculate corrected Lyman-alpha humidity  */
  rhola = (CX[0] + vh2o * (CX[1] + vh2o * (CX[2] + vh2o * CX[3]))) / XC;
//printf("del = %f, vh2o = %f, rhola = %f\n", del, vh2o, rhola);

  /* Following lines added to keep routine from blowing up.   RLR  920331 */
  if (rhola > 99999.)
    rhola = 99999.;
  /*  Old test
  if (rhola < 0.)
    rhola = 0.;     */
  /*  New test  */
  if (rhola < -99999.)
    rhola = -99999.;

  /* Calculate mean error and test for recouple  */
  rflag = 0.0;
  if (rhodt < 0.1)
    rhodt = 0.1;
  errlr = 100.0 * (rhola-rhodt) / rhodt;
  ertst = (NR_TYPE)fabs(errlr);

/*  (code lines 154-168 changed by RLR on 7 May 1997 to agree with GENPRO)
// Allen Schanot's safety check, 7/5/88
  if (n1[indx] < 10)
    ertst = 0.0;
*/

/*  Allen Schanot's safety checks, 4/10/92 */
  if (n1[indx] < 10)
    ertst = 5.0;

  if (ertst > ((rhola < 0.3) ? THRSH * 10.0 : THRSH))
    {
    n1[indx] = 0;
    rflag = 1.0;
    }

  if (qcxc < 25.0)
    {
    n1[indx] = 0;
    rflag = 1.1;
    }

  rholap[indx] = rhola;

  return(rhola);

}  /* END SRHOLA */

/* END RHOLA.C */
