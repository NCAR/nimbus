/*
-------------------------------------------------------------------------
OBJECT NAME:	plwcc.c

FULL NAME:	King Liquid Water Content

ENTRY POINTS:	sfunc()

STATIC FNS:	

DESCRIPTION:	King Liquid Water content calculation.  Two algorithms, one
		for C130 and one for the GV.  Difference is in number of
		coefficients for Nusselt number.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2012
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static const int nLWC = 2;

static const double cond0 = 0.0238;
static const double cond1 = 0.000071;
static const double visc0 = 1.718e-5;
static const double visc1 = 0.0049e-5;

// Coefficients to determine the boiling point.
static const float bp[4] = { 0.03366503,1.34236135,-0.33479451,0.03519342 };

// consts.c?
static const double Lv0 = 2.501e6;

//  Values that can be over-ridden from Defaults file.
static NR_TYPE	tWire[nLWC] = {130.0, 130.0};	// Wire temperature (C)
static NR_TYPE	dWire[nLWC] = {0.1805e-2, 0.1805e-2}; // Wire Diameter
static NR_TYPE	lWire[nLWC] = {2.1e-2, 2.1e-2};	// Wire length (mm)
static int	tau_Nu[nLWC] = {120, 120};		// Nusselt-number time constant.
static NR_TYPE	cloud_conc_threshold = 1.0;		// Cloud Concentration Baseline Threshold

static NR_TYPE Pdry[nLWC];

static NR_TYPE CooperLWC_GV(const NR_TYPE plwc, const NR_TYPE tasx, const NR_TYPE atx,
	const NR_TYPE psxc, NR_TYPE concf, const int indx);

static NR_TYPE CooperLWC_C130(const NR_TYPE plwc, const NR_TYPE tasx, const NR_TYPE atx,
	const NR_TYPE psxc, NR_TYPE concf, const int indx);

static NR_TYPE (*lwcFn)(const NR_TYPE, const NR_TYPE, const NR_TYPE, const NR_TYPE, NR_TYPE, const int);


/* -------------------------------------------------------------------- */
void plwccInit(var_base *varp)
{
  float *tmp;
  int probe_index = 0;
  char defaultName[256];

  if ( strstr(varp->name, "1") )
    probe_index = 1;

  varp->ProbeCount = probe_index;

  if (cfg.Aircraft() == Config::HIAPER)
    lwcFn = CooperLWC_GV;
  else
    lwcFn = CooperLWC_C130;

  // Check for obsolete / deprecated Defaults name.
  if ( probe_index == 0 && (tmp = GetDefaultsValue("TWIRE_PMS", varp->name)) )
    tWire[probe_index] = tmp[0];

  if ( probe_index == 1 && (tmp = GetDefaultsValue("TWIRE_PMS1", varp->name)) )
    tWire[probe_index] = tmp[0];


  sprintf(defaultName, "KING%d_WIRE_TEMP", probe_index);
  if ((tmp = GetDefaultsValue(defaultName, varp->name)) == NULL)
  {
    std::vector<float> values;
    values.push_back(tWire[probe_index]);
    AddToDefaults(varp->name, defaultName, values);
  }
  else
    tWire[probe_index] = tmp[0];

  sprintf(defaultName, "KING%d_WIRE_DIAM", probe_index);
  if ((tmp = GetDefaultsValue(defaultName, varp->name)) == NULL)
  {
    std::vector<float> values;
    values.push_back(dWire[probe_index]);
    AddToDefaults(varp->name, defaultName, values);
  }
  else
    dWire[probe_index] = tmp[0];

  sprintf(defaultName, "KING%d_WIRE_LEN", probe_index);
  if ((tmp = GetDefaultsValue(defaultName, varp->name)) == NULL)
  {
    std::vector<float> values;
    values.push_back(lWire[probe_index]);
    AddToDefaults(varp->name, defaultName, values);
  }
  else
    lWire[probe_index] = tmp[0];

  sprintf(defaultName, "KING%d_TAU_NUSSELT", probe_index);
  if ((tmp = GetDefaultsValue(defaultName, varp->name)) == NULL)
  {
    std::vector<float> values;
    values.push_back(tau_Nu[probe_index]);
    AddToDefaults(varp->name, defaultName, values);
  }
  else
    tau_Nu[probe_index] = (int)tmp[0];

  sprintf(defaultName, "KING_CLOUD_CONC_THRESHOLD");
  if ((tmp = GetDefaultsValue(defaultName, varp->name)) == NULL)
  {
    std::vector<float> values;
    values.push_back(cloud_conc_threshold);
    AddToDefaults(varp->name, defaultName, values);
  }
  else
    cloud_conc_threshold = tmp[0];

}	/* END PLWCCINIT */

/* -------------------------------------------------------------------- */
void spdry(DERTBL *varp)
{
  PutSample(varp, Pdry[varp->ProbeCount]);
  Pdry[varp->ProbeCount] = floatNAN;
}
/* -------------------------------------------------------------------- */
void splwcc(DERTBL *varp)
{
  NR_TYPE plwc, tasx, atx, psxc, concf = floatNAN;
  NR_TYPE plwcc;

  plwc  = GetSample(varp, 0);	// Raw Liquid Water measurement
  tasx  = GetSample(varp, 1);	// True Airspeed
  atx   = GetSample(varp, 2);	// Ambient Temperature
  psxc  = GetSample(varp, 3);	// Static Pressure
 
  if (plwc < 10.0 || isnan(plwc))
  {
    PutSample(varp, floatNAN);
    return;
  }

  if (tasx < 30.0)
  {
    PutSample(varp, 0.0);
    return;
  }

  if (varp->ndep >= 5)
    concf = GetSample(varp, 4);	// FSSP (preferably) concntration

  plwcc = (*lwcFn)(plwc, tasx, atx, psxc, concf, varp->ProbeCount);

  if (plwcc < 0.0)
    plwcc = 0.0;

  PutSample(varp, plwcc);
}


/* -------------------------------------------------------------------- */
NR_TYPE CooperLWC_GV(const NR_TYPE plwc, const NR_TYPE tasx, const NR_TYPE atx,
	const NR_TYPE psxc, const NR_TYPE concf, const int indx)
{
  static NR_TYPE Nu_a0 = 1.8679, Nu_b0 = 0.13506, Nu_c0 = 0.13289;
  static NR_TYPE prevTAS = 0.0, prevRe = 0.0;

  static const double Nu_a1 = 0.3433589;
  static const double Nu_b1 = 0.638318;
  static const double Nu_c1 = 0.3823;

  NR_TYPE Nu;
  NR_TYPE tm = (atx + tWire[indx]) / 2.0;		// Mean T
  NR_TYPE cond = cond0 + cond1 * tm;			// thermal conductivity, air
  NR_TYPE visc = visc0 + visc1 * tm;			// Viscosity of air
  NR_TYPE dens = 100.0 * psxc / (Rd * (tm + Kelvin));	// air density
  NR_TYPE Re = dens * tasx * dWire[indx] / visc;	// Reynolds number

  if (isnan(Re))
    return floatNAN;

  // Update Nusselt-number coefficients
  if (concf < cloud_conc_threshold && !isnan(concf))
  {
    Nu = plwc / (M_PI * lWire[indx] * cond * (tWire[indx] - atx));
    if (tasx < 150.0)
    {
      if (prevTAS >= 150.0)
        Nu_c0 = Nu_b0 * pow(Re, Nu_b1 - Nu_c1);

      Nu_c0 += (Nu / pow(Re, Nu_c1) - Nu_c0) / tau_Nu[indx];
    }
    else
    if (Re < 7244.0)
    {
      if (prevTAS < 150.0)
        Nu_a0 = Nu_c0 * pow(Re, Nu_c1 - Nu_a1);
      else
      if (prevRe >= 7244.0)
        Nu_a0 = Nu_b0 * pow(Re, Nu_b1 - Nu_a1);

      Nu_a0 += (Nu / pow(Re, Nu_a1) - Nu_a0) / tau_Nu[indx];
    }
    else
    {
      if (prevTAS < 150.0)
        Nu_b0 = Nu_c0 * pow(Re, Nu_c1 - Nu_b1);
      else
      if (prevRe < 7244.0)
        Nu_b0 = Nu_a0 * pow(Re, Nu_a1 - Nu_b1);

      Nu_b0 += (Nu / pow(Re, Nu_b1) - Nu_b0) / tau_Nu[indx];
    }
  }

  if (tasx < 150.0)
    { Nu = Nu_c0 * pow(Re, Nu_c1); }
  else
  if (Re < 7244.0)
    { Nu = Nu_a0 * pow(Re, Nu_a1); }
  else
    { Nu = Nu_b0 * pow(Re, Nu_b1); }

  prevTAS = tasx;
  prevRe = Re;

  // Power required in dry air.
  Pdry[indx] = M_PI * lWire[indx] * cond * (tWire[indx] - atx) * Nu;
  NR_TYPE xp = log10(psxc);

  NR_TYPE Tbp = pow(10.0, (bp[0]+xp*(bp[1]+xp*(bp[2]+xp*bp[3]))));

  // Liquid water content, convreted from MKS to g/m^3
  return 1000.0 * (plwc - Pdry[indx]) / (lWire[indx] * dWire[indx] * tasx * ((Lv0+Lv*Tbp) + Cw * (Tbp - atx)));
}

/* -------------------------------------------------------------------- */
NR_TYPE CooperLWC_C130(	const NR_TYPE plwc, const NR_TYPE tasx, const NR_TYPE atx,
			const NR_TYPE psxc, const NR_TYPE concf, const int indx)
{
  static NR_TYPE Nu_a0 = 0.1179;
  static const NR_TYPE Nu_a1 = 0.67452;

  NR_TYPE Nu;
  NR_TYPE tm = (atx + tWire[indx]) / 2.0;		// Mean T
  NR_TYPE cond = cond0 + cond1 * tm;			// thermal conductivity, air
  NR_TYPE visc = visc0 + visc1 * tm;			// Viscosity of air
  NR_TYPE dens = 100.0 * psxc / (Rd * (tm + Kelvin));	// air density
  NR_TYPE Re = dens * tasx * dWire[indx] / visc;	// Reynolds number

  if (isnan(Re))
    return floatNAN;

  // Update Nusselt-number coefficients
  if (concf < cloud_conc_threshold && !isnan(concf))
  {
    Nu = plwc / (M_PI * lWire[indx] * cond * (tWire[indx] - atx));
    Nu_a0 += (Nu / pow(Re, Nu_a1) - Nu_a0) / tau_Nu[indx];
  }

  Nu = Nu_a0 * pow(Re, Nu_a1);

  // Power required in dry air.
  Pdry[indx] = M_PI * lWire[indx] * cond * (tWire[indx] - atx) * Nu;
  NR_TYPE xp = log10(psxc);

  NR_TYPE Tbp = pow(10.0, (bp[0]+xp*(bp[1]+xp*(bp[2]+xp*bp[3]))));

  // Liquid water content, convreted from MKS to g/m^3
  return 1000.0 * (plwc - Pdry[indx]) / (lWire[indx] * dWire[indx] * tasx * ((Lv0+Lv*Tbp) + Cw * (Tbp - atx)));
}
