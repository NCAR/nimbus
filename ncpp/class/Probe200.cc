/*
-------------------------------------------------------------------------
OBJECT NAME:	Probe200.cc

FULL NAME:	260X, 200X, 200Y Class

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1999
-------------------------------------------------------------------------
*/

#include "Probe200.h"

const float Probe200::DiodeDiameter = 0.2;

/* -------------------------------------------------------------------- */
Probe200::Probe200(NcFile *file, NcVar *av) : Probe(file, av), _nDiodes(0), _resolution(0), _dof_const(2.37)
{
  NcAtt *attr;

  if ((attr = _avar->get_att("ResponseTime")))
    _responseTime = attr->as_float(0);
  else
    _responseTime = 0.35;

  if ((attr = _avar->get_att("nDiodes")))
    _nDiodes = attr->as_int(0);
  else
    _nDiodes = 32;

  if ((attr = _cvar->get_att("PLWfactor")) || (attr = _avar->get_att("PLWfactor")))
    _PLWfac = attr->as_float(0);
  else
    _PLWfac = 1.0e-9;

  if ((attr = _cvar->get_att("Density")) || (attr = _avar->get_att("Density")))
    _DENS = attr->as_float(0);
  else
    _DENS = 1.0;

  if ((attr = _cvar->get_att("DBZfactor")) || (attr = _avar->get_att("DBZfactor")))
    _DBZfac = attr->as_float(0);
  else
    _DBZfac = 1.0e3;


  _radius.resize(VectorLength());
  _esw.resize(VectorLength());
  _dof.resize(VectorLength());

  ComputeWidths();

  _deadTimeIdx = -1;
  for (int i = 0; i < _otherVars.size(); ++i)
    {
    if (strcmp(_otherVars[i]->name(), "TASX") == 0)
      _tasIdx = i;


    if (strncmp(_otherVars[i]->name(), "CONC", 4) == 0)
      _concIdx = i;

    if (strncmp(_otherVars[i]->name(), "PLWC", 4) == 0)
      _lwIdx = i;

    if (strncmp(_otherVars[i]->name(), "DBAR", 4) == 0)
      _dbarIdx = i;

    if (strncmp(_otherVars[i]->name(), "DISP", 4) == 0)
      _dispIdx = i;

    if (strncmp(_otherVars[i]->name(), "DBZ", 3) == 0)
      _dbzIdx = i;

    if (strncmp(_otherVars[i]->name(), "DT", 2) == 0)
      _deadTimeIdx = i;
    }

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void Probe200::ComputeConcentration(float *accum, float *conc, long countV[],
	const std::vector<float *> & otherVarData)
{
  int	time, bin;
  std::vector<float> dia;
  float	*counts, *concentration;
  float	*tas, tasx;

  tas = otherVarData[_tasIdx];

  for (time = 0; time < countV[0] * countV[1]; ++time)
    {
    counts = &accum[time * VectorLength()];
    concentration = &conc[time * VectorLength()];

    tasx = tas[time] / DataRate();

    ComputeDOF200(tasx);

    for (bin = FirstBin(); bin <= LastBin(); ++bin)
      _sampleVolume[bin] = tasx * (_dof[bin] * _esw[bin]);

    dia = _midPointDiam;

#define PLWC

#include "pms1d_cv"

    otherVarData[_concIdx][time] = _totalConcen;
    otherVarData[_lwIdx][time] = _plwc;
    otherVarData[_dbarIdx][time] = _dbar;
    otherVarData[_dispIdx][time] = _disp;
    }

}	/* END COMPUTECONCENTRATION */

/* -------------------------------------------------------------------- */
void Probe200::UpdateCellDiams(const int first, const int last, const float *newDiams)
{
  Probe::UpdateCellDiams(first, last, newDiams);
  ComputeWidths();

}       /* END UPDATECELLDIAMS */

/* -------------------------------------------------------------------- */
void Probe200::ComputeWidths()
{
  Probe::ComputeWidths();

  // All binWidths should be equal for 200 series probes.
  _mag = DiodeDiameter / (_binWidth[1] / 1000);

  for (int i = 1; i <= LastBin(); ++i)
    {
    _radius[i]   = _midPointDiam[i] / 2000;
    _esw[i]      = DiodeDiameter * (_nDiodes - i - 1) / _mag;
    _dof[i]      = _dof_const * _midPointDiam[i] * _midPointDiam[i] / 1000.0;
    if (_dof[i] > _armDistance)
      _dof[i] = (float)_armDistance;
    }

}	/* END COMPUTEWIDTHS */

/* -------------------------------------------------------------------- */
void Probe200::ComputeDOF200(float tasx)
{
  int		i;
  double	szz, timex, frac, f, z;

  for (i = FirstBin(); i <= LastBin(); ++i)
    {
    /* Determine size in terms of # of array elements
     */
    szz = 2.0 * _radius[i];

    /* Diminish this radius by an amount that accounts for
     * the fact that the particle is not on the diode array
     * 100% of the time 50% shadowed. This amount is approximately
     * one half diode array width wide.
     */
    szz += (DiodeDiameter / _mag) / 2.0;

    /* Calculate amount of time particle will be on the array
     */
    timex = szz / (tasx * 1000.0) * 1.0e6;

    /* This is the fraction of total amplitude caused by response
     * time
     */
    frac = (double)1.0 - exp((double)-timex / _responseTime);

    /* Have to meet 70% occultation for the 1D.
     */
    if (frac < 0.7)
      _dof[i] = 0.0;
    else
      /* Cautionary note to users of this routine, the relationship
       * between dof and Z was derived from laboratory studies which
       * are somewhat prelimnary in nature and need additional
       * verification, A new algorithm should be available by 9/1/92
       */
      {
      f = (0.7 / frac) * 100.0;
      z = 30.46 - 0.628 * f + 0.003246 * f * f;
      _dof[i] = 1580.0 * z * _radius[i] * _radius[i];
      }

    if (_dof[i] > _armDistance)
      _dof[i] = _armDistance;
    }

}	/* END COMPUTEDOF */

/* END PROBE200.CC */
