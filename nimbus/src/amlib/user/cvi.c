/*
-------------------------------------------------------------------------
OBJECT NAME:	cvi.c

FULL NAME:	Calculate flow out CVI tip (slpm)

ENTRY POINTS:	scvl()
		scvfcc()
		scvf2c()
		scvfhc()
		scvfxc()
		scvcnc()
		scvcno()
		scfact()
		scvs4()
		scvs7()
		srhcv()
		sconcud()	// 2006 and later CVI.

STATIC FNS:	TempCorrection()

DESCRIPTION:  	Most of these functions are for the original CVI.  concud
		variable is derived for the new CVI, flown VOCALS and forward.

COPYRIGHT:	University Corporation for Atmospheric Research, 1994-2011
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

#define CVR4  4e-4
#define CVR7  7e-4
#define RHOD  1.0

static NR_TYPE  cvs7;

static NR_TYPE  TempCorrection(NR_TYPE, NR_TYPE, NR_TYPE);

double  esubt(double, double);

/*  Values from /home/local/proj/defaults/Defaults on 29 April 1998  RLR  */
static NR_TYPE  CVTBL = 7.62,
                CVTBR = 0.3,
                CVOFF = 0.0,
                CVDIV = 1.0;

/* This constant was 1.2 for the VOCALS project.  Subsequent projects (PREDICT
 * and ICE-T) that have the pressure gauge that Darrin Toohey installed should
 * pass in two more variables and perform the correction that does not use this
 * value (see below).  cjw, Aug 2011
 */
static const NR_TYPE concud_fudge_factor = 1.2;

/* -------------------------------------------------------------------- */
void cviInit(var_base *varp)
{
  float *tmp;
  if ((tmp = GetDefaultsValue("CVTBL", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function cviInit.\n", CVTBL);
    LogMessage(buffer);
  }
  else
    CVTBL = tmp[0];

  if ((tmp = GetDefaultsValue("CVTBR", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function cviInit.\n", CVTBR);
    LogMessage(buffer);
  }
  else
    CVTBR = tmp[0];

  if ((tmp = GetDefaultsValue("CVOFF", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function cviInit.\n", CVOFF);
    LogMessage(buffer);
  }
  else
    CVOFF = tmp[0];

  if ((tmp = GetDefaultsValue("CVDIV", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function cviInit.\n", CVDIV);
    LogMessage(buffer);
  }
  else
    CVDIV = tmp[0];

}  /* END CVIINIT */

/* -------------------------------------------------------------------- */
void sconcud(DERTBL *varp)
{
  // Routine for VOCALS and subsequent projects.

  NR_TYPE cnts = GetSample(varp, 0);
  NR_TYPE cvcfact = GetSample(varp, 1);
  NR_TYPE flow = GetSample(varp, 2);

  NR_TYPE concud = cnts / (flow * cvcfact);

  if (varp->ndep == 5)
  {
    NR_TYPE cvpcn = GetSample(varp, 3);
    NR_TYPE upress = GetSample(varp, 4);

    concud *= (cvpcn / upress);
  }
  else
    concud *= concud_fudge_factor;	// VOCALS. cjw, Aug2011
  
  PutSample(varp, concud);

}  /* END SCONCUD */

/* -------------------------------------------------------------------- */
void scvl(DERTBL *varp)
{
  NR_TYPE  cvf1, cvf2, cvfcn, cvfh, cvfx;
  NR_TYPE  cvf3;

  cvf1  = GetSample(varp, 0);
  cvf2  = GetSample(varp, 1);
  cvfcn = GetSample(varp, 2);
  cvfh  = GetSample(varp, 3);
  cvfx  = GetSample(varp, 4);

  cvf3  = cvf1 - cvf2 - cvfcn - cvfh - cvfx + CVOFF;


  /* Calculate distance to stagnation plane
   */
  if (cvf1 < 0.0)
    cvf1 = 0.0001;

  PutSample(varp, CVTBL * cvf3 / cvf1);

}  /* END SXCVL */

/* -------------------------------------------------------------------- */
void scvfcc(DERTBL *varp)
{
  NR_TYPE  cvfcc, cvfcn, cvpcn, cvtemp;

  cvfcn  = GetSample(varp, 0);
  cvpcn  = GetSample(varp, 1);
  cvtemp  = GetSample(varp, 2);

  cvfcc = TempCorrection(cvfcn, cvpcn, cvtemp);

  if (cvfcc < 0.0)
    cvfcc = 0.0001;

  PutSample(varp, cvfcc);

}  /* END SXCVFCC */

/* -------------------------------------------------------------------- */
void scvf2c(DERTBL *varp)
{
  NR_TYPE  cvf2, cvtemp, cvpcn;

  cvf2  = GetSample(varp, 0);
  cvpcn  = GetSample(varp, 1);
  cvtemp  = GetSample(varp, 2);

  PutSample(varp, TempCorrection(cvf2, cvpcn, cvtemp));

}  /* END SCVF2C */

/* -------------------------------------------------------------------- */
void scvfhc(DERTBL *varp)
{
  NR_TYPE  cvfh, cvtemp, cvpcn;

  cvfh  = GetSample(varp, 0);
  cvpcn  = GetSample(varp, 1);
  cvtemp  = GetSample(varp, 2);

  PutSample(varp, TempCorrection(cvfh, cvpcn, cvtemp));

}  /* END SCVFHC */

/* -------------------------------------------------------------------- */
void scvfxc(DERTBL *varp)
{
  NR_TYPE  cvfx, cvtemp, cvpcn;

  cvfx  = GetSample(varp, 0);
  cvpcn  = GetSample(varp, 1);
  cvtemp  = GetSample(varp, 2);

  PutSample(varp, TempCorrection(cvfx, cvpcn, cvtemp));

}  /* END SCVFXC */

/* -------------------------------------------------------------------- */
void scvcnc(DERTBL *varp)
{
  NR_TYPE  cvcnc, cvcnt, cvfcc;

  cvcnt  = GetSample(varp, 0);
  cvfcc  = GetSample(varp, 1);

  cvcnc = cvcnt * CVDIV / (cvfcc * 1000.0 / 60.0);
  cvcnc *= exp(cvcnc * cvfcc * 4.167E-6);

  PutSample(varp, cvcnc);

}  /* END SCVCNC */

/* -------------------------------------------------------------------- */
void scvcno(DERTBL *varp)
{
  NR_TYPE  cvcnc, cvfact;

  cvcnc  = GetSample(varp, 0);
  cvfact  = GetSample(varp, 1);

  PutSample(varp, cvcnc / cvfact);

}  /* END SCVCNO */

/* -------------------------------------------------------------------- */
void scfact(DERTBL *varp)
{
  NR_TYPE  cfact, tasx, cvftv, cvfcc, cvf2c, cvfhc, cvfxc, tascc;

  tasx  = GetSample(varp, 0);
  cvfcc  = GetSample(varp, 1);
  cvf2c  = GetSample(varp, 2);
  cvfhc  = GetSample(varp, 3);
  cvfxc  = GetSample(varp, 4);


  /* Calculate CN concentrations (inside CVI and equivalent outside)
   */
  if ((tascc = tasx * 100.0) < 0.0)
    tascc = 0.0001;

  cvftv = cvfcc + cvf2c + cvfhc + cvfxc;

  if (cvftv < 0.0)
    cvftv = 0.001;

  cfact = (tascc * M_PI * pow(CVTBR, 2.0)) / (cvftv * 1000.0 / 60.0);

  PutSample(varp, cfact);

}  /* END SCFACT */

/* -------------------------------------------------------------------- */
void scvs4(DERTBL *varp)
{
  NR_TYPE  cvs4, tasx, psxc, qcxc, cvtt;
  NR_TYPE  tascc, rhoa, gnu, re4, re7;

  static bool  firstTime = TRUE;
  static NR_TYPE  EXP_P5, EXP_MP5, EXP_M1P5;

  tasx  = GetSample(varp, 0);
  psxc  = GetSample(varp, 1);
  qcxc  = GetSample(varp, 2);
  cvtt  = GetSample(varp, 3);

  if (firstTime)
    {
    EXP_P5  = exp(0.5);
    EXP_MP5  = exp(-0.5);
    EXP_M1P5= exp(-1.5);

    firstTime = FALSE;
    }


  /* Calculate ambient conditions at CVI tip and stopping
   * distance for a 4 um radius droplet (cm)
   */
  tascc  = tasx * 100.0;
  rhoa  = (psxc + qcxc) / (2870.12 * (cvtt + Kelvin));
  gnu  = (0.0049 * cvtt + 1.718) * 0.0001;
  re4  = 2.0 * CVR4 * tascc * rhoa / gnu;

  if (re4 < 0.0)
    cvs4 = -100.0;
  else
    cvs4 = CVR4 * EXP_M1P5 * RHOD *
      (pow(re4, 0.333333) * EXP_P5 +
      atan(pow(re4, -0.333333) * EXP_MP5) - 0.5 * M_PI) /
      (3.0 * rhoa);


  /* Calculate stopping distance for a 7 um radius droplet (cm)
   */
  re7 = 2.0 * CVR7 * tascc * rhoa / gnu;

  if (re7 < 0.0)
    cvs7 = -100.0;
  else
    cvs7 = CVR7 * EXP_M1P5 * RHOD * (pow(re7, 0.333333) *
      EXP_P5 + atan(pow(re7, -0.333333) *
      EXP_MP5) - 0.5 * M_PI) / (3.0 * rhoa);

  PutSample(varp, cvs4);

}  /* END SCVS4 */

/* -------------------------------------------------------------------- */
void scvs7(DERTBL *varp)
{
  PutSample(varp, cvs7);
}

/* -------------------------------------------------------------------- */
void srhcv(DERTBL *varp)
{
  NR_TYPE  cvdp, cvtemp;

  cvdp  = GetSample(varp, 0);
  cvtemp  = GetSample(varp, 1);

  if (cvdp < 0.0)
    cvdp = 0.0009 + cvdp * (1.134055 + cvdp * 0.001038);

  PutSample(varp, 216.68 * esubt(cvdp, 0.0) / (cvtemp + Kelvin));

}  /* END SRHCV */

/* -------------------------------------------------------------------- */
static NR_TYPE TempCorrection(NR_TYPE cv, NR_TYPE pcn, NR_TYPE cvtemp)
{
  if (pcn <= 0.0)
    pcn = 0.0001;

  return(cv * (StdPress / pcn) * ((cvtemp + Kelvin) / 294.26));

}  /* END TEMPCORRECTION */

/* END CVI.C */
