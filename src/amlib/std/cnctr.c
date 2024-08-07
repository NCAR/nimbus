/*
-------------------------------------------------------------------------
OBJECT NAME:	cnctr.c

FULL NAME:	CN Counter

ENTRY POINTS:	sconcn(), sxicnx(), sfcnc()

STATIC FNS:	none

DESCRIPTION:	Computation for the CN Counter, see Cindy Towhy for
		in-depth information.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2024
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static NR_TYPE StdTemperature = 294.15;

/*  Value from /home/local/proj/Defaults on 23 April 1998           RLR */
static NR_TYPE  DIV = 1.0;  /* counter card prescale factor (none)      */

/* -------------------------------------------------------------------- */
void cnctrInit(var_base *varp)
{
  float  *tmp;

  if ((tmp = GetDefaultsValue("DIV", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function cnctrInit.\n", DIV);
    LogMessage(buffer);
  }
  else
    DIV = tmp[0];

  if (DIV != 1.0)
    varp->addToMetadata("DIV", tmp[0]);

  std::vector<float> values;
  values.push_back(StdTemperature);
  // Add this to FCNC
  ((DERTBL *)varp)->depends[1]->addToMetadata("std_temperature", values);

}  /* END CNCTRINIT */

/* -------------------------------------------------------------------- */
void sconcn(DERTBL *varp)
{
  NR_TYPE  concn, fcnc, cnts;

  cnts  = GetSample(varp, 0);
  fcnc  = GetSample(varp, 1);

  if (FeedBack == HIGH_RATE_FEEDBACK)
    cnts *= (float)Config::HighRate;

  concn  = cnts * DIV / (fcnc * 1000.0 / 60.0);

  /* Particle concentration corrected for coincidence
   * 4.167E-6 is combined product of time in view volume (.25 microseconds for old 3760)
   * 6.667E-6 is combined product of time in view volume (.4 microseconds for 3760a)
   * and factor (1000/60) converting FCNC from lpm to cm3/s.
   */
  concn *= (NR_TYPE)exp((double)(6.667e-6 * concn * fcnc));

  /* Convert to ambient.  This is new as of 12/2007, older projects would
   * need DependTable upgrade.
   */
  if (varp->nDependencies > 5)
  {
    NR_TYPE pcn = GetSample(varp, 2);
    NR_TYPE cntmp = GetSample(varp, 3);
    NR_TYPE psx = GetSample(varp, 4);
    NR_TYPE atx = GetSample(varp, 5);

    /* Use for WaterCN.  Used in TORERO and DC3.
     */
    if (varp->nDependencies == 7)	// Use for WaterCN.
    {
      NR_TYPE ltcn = GetSample(varp, 6);
      concn /= ltcn;
    }

    concn *= (psx / pcn) * (cntmp + Kelvin) / (atx + Kelvin);
  }

  PutSample(varp, concn);

}  /* END SCONCN */

/* -------------------------------------------------------------------- */
void scFlow(DERTBL *varp)
{
  NR_TYPE  flowc, flow, pressure, temperature;

  flow    = GetSample(varp, 0);
  pressure  = GetSample(varp, 1);
  temperature  = GetSample(varp, 2);

  if (pressure <= 0.0)
    pressure = 0.0001;

  /* Corrected sample flow rate in vlpm
   */
  flowc = flow * (StdPress / pressure) * ((temperature + Kelvin) / StdTemperature);

  if (flowc <= 0.0)
    flowc = 0.0001;

  PutSample(varp, flowc);

}  /* END SCFLOW */
/* -------------------------------------------------------------------- */
/* This is for the Water CN.  Which also puts out a Flow and Conc.  I think
 * this was a short term test per Dave Rogers, but looks unused at this time.
 */
void scWFlow(DERTBL *varp)
{
  NR_TYPE  flowc, flow, pressure, wcn_ps, tcn, temperature;

  flow = GetSample(varp, 0);
  wcn_ps = GetSample(varp, 1);
  tcn = GetSample(varp, 2);
  pressure = GetSample(varp, 3);
  temperature = GetSample(varp, 4);

  if (pressure <= 0.0)
    pressure = 0.0001;

  /* Corrected sample flow rate in vlpm
   */
  flowc = flow * (wcn_ps / pressure) * ((temperature + Kelvin) / (tcn + Kelvin));

  if (flowc <= 0.0)
    flowc = 0.0001;

  PutSample(varp, flowc);

}  /* END SCFLOW */

/* -------------------------------------------------------------------- */
void scClarkCN(DERTBL *varp)
{
  NR_TYPE  flowc, flow, pressure, temperature;

  flow    = GetSample(varp, 0);
  pressure  = GetSample(varp, 1);
  temperature  = GetSample(varp, 2);

  if (pressure <= 0.0)
    pressure = 0.0001;

  /* Corrected sample flow rate in vlpm
   */
  flowc = flow * (StdPress / pressure) * ((30.0 + Kelvin) / 294.26);

  if (flowc <= 0.0)
    flowc = 0.0001;

  PutSample(varp, flowc);

}  /* END SCFLOW */

/* END CNCTR.C */
