/*
-------------------------------------------------------------------------
OBJECT NAME:	cvi2.c

FULL NAME:	Calculate flow out CVI tip (slpm)

ENTRY POINTS:	scvrho()
		scvcwc()
		sconcud()	// 2006 and later CVI.

STATIC FNS:	TempCorrection()

DESCRIPTION:	Second generation CVI, 2006 and later.

COPYRIGHT:	University Corporation for Atmospheric Research, 1994-2018
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/*  Values from /home/local/proj/defaults/Defaults */

static NR_TYPE	TAS_MIN = 85.0;	// Good value for C130.  GV should be about 115.
static NR_TYPE  TDL_Offset = 0.0;
static NR_TYPE  CVCFACT_COEF[2] = { 0.968, 0.0015934 };

static NR_TYPE	C0_P[3] = { 0.0, 1.0, 0.0 };
static NR_TYPE	C1_P[3] = { 0.0, 1.0, 0.0 };
static NR_TYPE	C2_P[3] = { 0.0, 1.0, 0.0 };

/* This constant was 1.2 for the VOCALS project.  Subsequent projects (PREDICT
 * and ICE-T) that have the pressure gauge that Darrin Toohey installed should
 * pass in two more variables and perform the correction that does not use this
 * value (see below).  cjw, Aug 2011
 * SOCRATES, WECAN, and forward will not use the fudge_factor.  Set to 1.0 here.
 * See cvi2init() for VOCALS set to 1.2.
 */
static NR_TYPE concud_fudge_factor = 1.0;

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

  if ((tmp = GetDefaultsValue("CVI_CVCFACTC", varp->name)) == NULL)
  {
    sprintf(buffer,
		"cvi2Init:CVCFACT_COEF values defaulting to %f, %f.\n",
		CVCFACT_COEF[0], CVCFACT_COEF[1]);
    LogMessage(buffer);
  }
  else
    for (int i = 0; i < 2; ++i)
      CVCFACT_COEF[i] = tmp[i];

  if (cfg.ProjectName().compare("VOCALS") == 0)
    concud_fudge_factor = 1.2;

}

/* -------------------------------------------------------------------- */
void sconcud(DERTBL *varp)
{
  static int	counter = 0;
  static int	prevInlet = 0;

  NR_TYPE cnts = GetSample(varp, 0);
  NR_TYPE cvcfact = GetSample(varp, 1);
  NR_TYPE flow = GetSample(varp, 2);
  int	cvinlet = (int)GetSample(varp, 3);
  NR_TYPE concud = 0.0;

  if (flow > 0.0)	// basic check.
    concud = cnts / (flow * cvcfact);

  // If we have come out of cloud.  Output nan for 20 seconds.
  if (prevInlet != 0 && cvinlet == 0)
    counter = 20;

  // Avoid take-off & landing, and when CVINLET is 1 (no counterflow).
  if (cvinlet > 0 || counter > 0)
  {
    --counter;
    concud = floatNAN;
  }


  if (varp->nDependencies == 5)
  {
    NR_TYPE cvpcn = GetSample(varp, 3);
    NR_TYPE upress = GetSample(varp, 4);

    concud *= (cvpcn / upress);
  }
  else
    concud *= concud_fudge_factor;	// VOCALS. cjw, Aug2011

  PutSample(varp, concud);

  prevInlet = cvinlet;
}

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
void scvcfacttdl(DERTBL *varp)
{
  NR_TYPE cvcfact= GetSample(varp, 0);
  NR_TYPE cvpcn	= GetSample(varp, 1);
  NR_TYPE cvtcn = GetSample(varp, 2);
  NR_TYPE ptdlr = GetSample(varp, 3);
  NR_TYPE ttdlr = GetSample(varp, 4);
  static NR_TYPE previousTCN = 288.0;	// 15C.

  // Convert to Kelvin, if not already.
  if (varp->depends[2]->Units.find('C') != std::string::npos)
    cvtcn += Kelvin;

  // Replicate previous samle if out of bounds (15C and 35C).
  if (std::isnan(cvtcn) || cvtcn < 288.0 || cvtcn > 308.0)
    cvtcn = previousTCN;
  else
    previousTCN = cvtcn;

  cvcfact = cvcfact * (ptdlr / cvpcn) * (cvtcn) / (ttdlr+Kelvin);

  PutSample(varp, cvcfact);
}

/* -------------------------------------------------------------------- */
void scvcwcc(DERTBL *varp)
{
  static int	counter = 0;
  static int	prevInlet = 0;
  NR_TYPE	cvcwcc;
  NR_TYPE	cvrho, cvcfact, tasx;
  int		cvinlet;

  cvinlet	= (int)GetSample(varp, 2);
  tasx		= GetSample(varp, 3);

  // If we have come out of cloud.  Output nan for 20 seconds.
  if (prevInlet != 0 && cvinlet == 0)
    counter = 20;

  // Avoid take-off & landing, and when CVINLET is 1 (no counterflow).
  if (tasx < TAS_MIN || cvinlet > 0 || counter > 0)
  {
    --counter;
    cvcwcc	= floatNAN;
  }
  else
  {
    cvrho	= GetSample(varp, 0);
    cvcfact	= GetSample(varp, 1);
    cvcwcc	= cvrho / cvcfact;
  }

  PutSample(varp, cvcwcc);
  prevInlet = cvinlet;
}

/* -------------------------------------------------------------------- */
// UHSAS data flag.
void scviuflag(DERTBL *varp)
{
  NR_TYPE tact = GetSample(varp, 0);

  PutSample(varp, (NR_TYPE)(tact > 6000 ? 0 : 1));
}

/* The below was added for SOCRATES to mark icing conditions.
 * Add CVIFLAG CVCFACTC to UserNames.
 * Add to DependTable
 * CVCFACTC        CVCFACT CVCWC ATX
 * CVIFLAG         CVCFACTC
 * and replace CVCFACT with CVCFACTC in CONCUD and CVCFACTTDL.
 */

static int cviflag = 1;
/* -------------------------------------------------------------------- */
void scviflag(DERTBL *varp)
{
  PutSample(varp, (NR_TYPE)cviflag);
}

/* -------------------------------------------------------------------- */
void scvcfactc(DERTBL *varp)
{
  NR_TYPE cvcfactc = GetSample(varp, 0);
  NR_TYPE cvinlet = GetSample(varp, 1);
  NR_TYPE cvcwc = GetSample(varp, 2);
  NR_TYPE atx	= GetSample(varp, 3);
  static size_t elapsed_time = 0;

  // if icing conditions...
  if (cvinlet == 0 && cvcwc > 0.006 && atx < -7.0 && atx > -20.0)
  {
    // We just arrived, start elapsed time counter and flag data.
    if (cviflag == 1)
    {
      cviflag = 0;
    }

    cvcfactc *= (CVCFACT_COEF[0] - CVCFACT_COEF[1] * elapsed_time++);
  }
  else
    cviflag = 1;

  if (cvcwc < 0.003)
    elapsed_time = 1;

  if (cvcfactc < 3.0)
    cvcfactc = 3.0;

  PutSample(varp, cvcfactc);
}

/* END CVI2.C */
