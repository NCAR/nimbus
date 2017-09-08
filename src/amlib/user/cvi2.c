/*
-------------------------------------------------------------------------
OBJECT NAME:	cvi2.c

FULL NAME:	Calculate flow out CVI tip (slpm)

ENTRY POINTS:	scvrho()
		scvcwc()
		sconcud()	// 2006 and later CVI.

STATIC FNS:	TempCorrection()

DESCRIPTION:  	Second generation CVI, 2006 and later.

COPYRIGHT:	University Corporation for Atmospheric Research, 1994-2017
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/*  Values from /home/local/proj/defaults/Defaults */

static NR_TYPE	TAS_MIN = 85.0;	// Good value for C130.  GV should be about 115.
static NR_TYPE  TDL_Offset = 0.0;

static NR_TYPE	C0_P[3] = { 0.0, 1.0, 0.0 };
static NR_TYPE	C1_P[3] = { 0.0, 1.0, 0.0 };
static NR_TYPE	C2_P[3] = { 0.0, 1.0, 0.0 };

/* This constant was 1.2 for the VOCALS project.  Subsequent projects (PREDICT
 * and ICE-T) that have the pressure gauge that Darrin Toohey installed should
 * pass in two more variables and perform the correction that does not use this
 * value (see below).  cjw, Aug 2011
 */
static const NR_TYPE concud_fudge_factor = 1.2;

/* -------------------------------------------------------------------- */
void cvi2Init(var_base *varp)
{
  float *tmp;
  if ((tmp = GetDefaultsValue("CVI_TAS_MIN", varp->name)) == NULL)
  {
    sprintf(buffer, "cvi2Init:CVI_TAS_MIN value defaulting to %f.\n", TAS_MIN);
    LogMessage(buffer);
  }
  else
    TAS_MIN = tmp[0];

  if ((tmp = GetDefaultsValue("CVI_TDL_Offset", varp->name)) == NULL)
  {
    sprintf(buffer, "cvi2Init:CVI_TDL_Offset value defaulting to %f.\n", TDL_Offset);
    LogMessage(buffer);
  }
  else
    TDL_Offset = tmp[0];

  if ((tmp = GetDefaultsValue("CVI_C0_P", varp->name)) == NULL)
  {
    sprintf(buffer, "cvi2Init:C0_P values defaulting to 0.0, 1.0, 0.0.\n");
    LogMessage(buffer);
  }
  else
    for (int i = 0; i < 3; ++i)
      C0_P[i] = tmp[i];

  if ((tmp = GetDefaultsValue("CVI_C1_P", varp->name)) == NULL)
  {
    sprintf(buffer, "cvi2Init:C1_P values defaulting to 0.0, 1.0, 0.0.\n");
    LogMessage(buffer);
  }
  else
    for (int i = 0; i < 3; ++i)
      C1_P[i] = tmp[i];

  if ((tmp = GetDefaultsValue("CVI_C2_P", varp->name)) == NULL)
  {
    sprintf(buffer, "cvi2Init:C2_P values defaulting to 0.0, 1.0, 0.0.\n");
    LogMessage(buffer);
  }
  else
    for (int i = 0; i < 3; ++i)
      C2_P[i] = tmp[i];


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
void scvrho(DERTBL *varp)
{
  NR_TYPE ptdlr, h2oR, cvrho;
  NR_TYPE c[3];

  ptdlr	= GetSample(varp, 0);
  h2oR	= GetSample(varp, 1);

  c[0] = C0_P[0] + C0_P[1] * ptdlr + C0_P[2] * ptdlr * ptdlr;
  c[1] = C1_P[0] + C1_P[1] * ptdlr + C1_P[2] * ptdlr * ptdlr;
  c[2] = C2_P[0] + C2_P[1] * ptdlr + C2_P[2] * ptdlr * ptdlr;

  cvrho = c[0] + c[1] * h2oR + c[2] * h2oR * h2oR;
  cvrho -= TDL_Offset;

  PutSample(varp, cvrho);
}

/* -------------------------------------------------------------------- */
void scvcwc(DERTBL *varp)
{
  static int		counter = 0;
  static NR_TYPE	prevInlet = 0.0;
  NR_TYPE	cvcwc;
  NR_TYPE	cvrho, cvcfact, cvinlet, tasx;

  cvinlet	= GetSample(varp, 2);
  tasx		= GetSample(varp, 3);

  // If we have come out of cloud.  Output nan for 20 seconds.
  if (prevInlet == 1 && (int)cvinlet == 0)
    counter = 20;

  // Avoid take-off & landing, and when CVINLET is 1 (no counterflow).
  if (tasx < TAS_MIN || cvinlet > 0.0 || counter > 0)
  {
    --counter;
    cvcwc = floatNAN;
  }
  else
  {
    cvrho	= GetSample(varp, 0);
    cvcfact	= GetSample(varp, 1);
    cvcwc = cvrho / cvcfact;
  }
  
  PutSample(varp, cvcwc);

  prevInlet = cvinlet;
}

/* END CVI2.C */
