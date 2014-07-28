/*
-------------------------------------------------------------------------
OBJECT NAME:	cnctr.c

FULL NAME:	CN Counter

ENTRY POINTS:	sconcn(), sxicnx(), sfcnc()

STATIC FNS:	none

DESCRIPTION:	Computation for the CN Counter, see Cindy Towhy for
		in-depth information.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-95
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

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
   * 4.167E-6 is time in view volume (.25 microseconds)
   * conversion factor for FCNC to cm3/s.
   */
  concn *= (NR_TYPE)exp((double)(4.167e-6 * concn * fcnc));

  /* Convert to ambient.  This is new as of 12/2007, older projects would
   * need DependTable upgrade.
   */
  if (varp->ndep > 2)
  {
    NR_TYPE pcn = GetSample(varp, 2);
    NR_TYPE cntmp = GetSample(varp, 3);
    NR_TYPE psx = GetSample(varp, 4);
    NR_TYPE atx = GetSample(varp, 5);
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
  flowc = flow * (StdPress / pressure) * ((temperature + Kelvin) / 294.15);

  if (flowc <= 0.0)
    flowc = 0.0001;

  PutSample(varp, flowc);

}  /* END SCFLOW */
/* -------------------------------------------------------------------- */
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
