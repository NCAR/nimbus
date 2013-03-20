/*
-------------------------------------------------------------------------
OBJECT NAME:	Probe.cc

FULL NAME:	Probe Class

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1997-9
-------------------------------------------------------------------------
*/

#include "Probe.h"

#include <Xm/TextF.h>

static float	pcasDefSize[] =
        { 0.1, 0.12, 0.14, 0.17, 0.2, 0.25, 0.3, 0.4,
                0.5, 0.7, 0.9, 1.2, 1.5, 2.0, 2.5, 3.0 };

static float	f300DefSize[] =
        { 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7,
          0.8, 0.9, 1.0,                          // Step 0.1
          1.2, 1.4,                               // Step 0.2
          1.7, 2.0,                               // Step 0.3
          2.5, 3.0, 3.5, 4.0, 4.5, 5.0,           // Step 0.5
          6.0, 7.0, 8.0, 9.0, 10.0,               // Step 1.0
          12.0, 14.0, 16.0, 18.0, 20.0 };         // Step 2.0

static float	fsspDefSize[] =
        { 2.0, 5.0, 8.0, 11.0, 14.0, 17.0, 20.0, 23.0, 26.0,
           29.0, 32.0, 35.0, 38.0, 41.0, 44.0, 47.0,      // range 0
          2.0, 4.0, 6.0, 8.0, 10.0, 12.0, 14.0, 16.0, 18.0,
           20.0, 22.0, 24.0, 26.0, 28.0, 30.0, 32.0,      // range 1
          1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0,
           10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0,      // range 2
          0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5,
           5.0, 5.5, 6.0, 6.5, 7.0, 7.5, 8.0 };           // range 3


/* -------------------------------------------------------------------- */
Probe::Probe(NcFile *file, NcVar *av) : _avar(av), _firstBin(0), _lastBin(VectorLength()), _missing_value(-32767)
{
  std::string	cname;
  int		i;
  char		*location;
  NcVar		*var;
  NcAtt		*attr;

  _name = &_avar->name()[1];

  cname = "C"; cname += _name;
  _cvar = file->get_var(cname.c_str());

  if (_cvar && !_cvar->is_valid())
    _cvar = NULL;

  if (_cvar == NULL)
    std::cerr << "  Concentrations (" << cname << ") not found.\n";

  _type = NoProbe;

  if (_name.find("S100") != _name.npos)
    _type = S100;
  else
  if (_name.find("CDP") != _name.npos)
    _type = CDP;
  else
  if (_name.find("S200") != _name.npos)
    _type = S200;
  else
  if (_name.find("UHSAS") != _name.npos)
    _type = UHSAS;
  else
  if (_name.find("S300") != _name.npos)
    _type = S300;
  else
  if (_name.find("260X") != _name.npos)
    _type = X260;
  else
  if (_name.find("FSSP") != _name.npos)
    _type = FSSP;
  else
  if (_name.find("F300") != _name.npos)
    _type = F300;
  else
  if (_name.find("PCAS") != _name.npos || _name.find("ASAS") != _name.npos)
    _type = PCASP;
  else
  if (_name.find("2DC") != _name.npos || _name.find("1DC") != _name.npos)
    _type = TWODC;
  else
  if (_name.find("2DP") != _name.npos || _name.find("1DP") != _name.npos)
    _type = TWODP;
  else
  if (_name.find("2D3") != _name.npos || _name.find("1D3") != _name.npos)
    _type = TWODS;
  else
  if (_name.find("HVPS") != _name.npos)
    _type = HVPS;
  else
  if (_name.find("RDMA") != _name.npos)
    _type = RDMA;
  else
  if (_name.find("CLMT") != _name.npos)
    _type = CLIMET;
  else
  if (_name.find("SID") != _name.npos)
    _type = SIDII;
  else
  if (_name.find("CAPS") != _name.npos)
    _type = CAPS;
  else
  if (_name.find("MASP") != _name.npos)
    _type = MASP;
  else
  if (_name.find("200X") != _name.npos)
    _type = X200;
  else
  if (_name.find("200Y") != _name.npos)
    _type = Y200;
  else
  if (_name.find("300X") != _name.npos)
    _type = X300;
  else
  if (_name.find("MCA") != _name.npos)
    _type = CMCA;
  else
  if (_name.find("PDI") != _name.npos)
    _type = PDI;


  if (Type() == NoProbe)
    std::cerr << "Probe:: No probe type for [" << _name << "]\n";


  _dataRate = _avar->get_dim(1)->size();
  _vectorLength = _avar->get_dim(2)->size();

  int   nCells = Type() == FSSP ? 64 : VectorLength();
  _sampleVolume.resize(nCells);
  _diameter.resize(nCells);
  _midPointDiam.resize(nCells);
  _binWidth.resize(nCells);

  if (_cvar == 0)
    return;

  _units = _cvar->get_att("units")->as_string(0);

  if ((attr = _avar->get_att("SerialNumber")))
    _serialNum = attr->as_string(0);

  if ((attr = _avar->get_att("_FillValue")))
    _missing_value = attr->as_float(0);

  if ((attr = _cvar->get_att("FirstBin")) || (attr = _avar->get_att("FirstBin")))
    _firstBin = attr->as_short(0);
  else
    {
    _firstBin = 1;
    fprintf(stderr, "netCDF attribute FirstBin not found for %s, defaulting to %d\n", cname.c_str(), _firstBin);
    }

  if ((attr = _cvar->get_att("LastBin")) || (attr = _avar->get_att("LastBin")))
    _lastBin = attr->as_short(0);
  else
    {
    _lastBin = _avar->get_dim(2)->size()-1;
    fprintf(stderr, "netCDF attribute LastBin not found for %s, defaulting to %d\n", cname.c_str(), _lastBin);
    }

  if ((attr = _cvar->get_att("CellSizes")) || (attr = _avar->get_att("CellSizes")))
    {
    if (attr->num_vals() != nCells)
      fprintf(stderr, "Warning: number of cell sizes in netCDF file does not match expected, variable: %s, file=%d, expected=%d.\n", cname.c_str(), attr->num_vals(), nCells);

    for (i = 0; i < nCells; ++i)
      _diameter[i] = attr->as_float(i);
    }
  else
    {
    std::cout << "Cell diameters for " << cname << " not present in file, using defaults.\n";
    sprintf(buffer, "Cell diameters for %s not present in file, using defaults.", cname.c_str());

    ErrorMsg(buffer);

    if (Type() == X260)
      for (i = 0; i < 64; ++i)
        _diameter[i] = 5 + i * 10;	// start 5um w/ 10um pixel/diode size.
    else
    if (Type() == X200)
      for (i = 0; i < 16; ++i)
        _diameter[i] = 10 + i * 20;
    else
    if (Type() == Y200)
      for (i = 0; i < 16; ++i)
        _diameter[i] = 150 + i * 300;
    else
    if (Type() == PCASP)
      for (i = 0; i < 16; ++i)
        _diameter[i] = pcasDefSize[i];
    else
    if (Type() == F300)
      for (i = 0; i < 32; ++i)
        _diameter[i] = f300DefSize[i];
    else
    if (Type() == FSSP)
      for (i = 0; i < 64; ++i)
        _diameter[i] = fsspDefSize[i];
    else
    if (Type() == TWODC)
      for (i = 0; i < 128; ++i)
        _diameter[i] = 12.5 + i * 25;
    else
    if (Type() == TWODP)
      for (i = 0; i < 64; ++i)
        _diameter[i] = 100 + i * 200;
    else
    if (Type() == TWODS)
      for (i = 0; i < 256; ++i)
        _diameter[i] = 5 + i * 10;
    else
    if (Type() == HVPS)
      for (i = 0; i < 256; ++i)
        _diameter[i] = 100 + i * 200;
    else
      {
      std::cerr << "Probe.cc: No default CellSizes for probe " << cname
        << " available.  Requires programmer intervention.\n";
      }
    }


  // Find associated houskeeping & derived variables (using location)
  location = strchr(const_cast<char *>(_name.c_str()), '_');
  if (location == 0)
    {
    std::cerr << "Probe.cc: variable " << _name << " has no location!\n";
    }

  for (i = 0; i < file->num_vars(); ++i)
    {
    var = file->get_var(i);

    if (var->num_dims() < 3 &&
       (strstr(var->name(), location) && strncmp(var->name(), "EVENT_", 6)) ||
       strcmp(var->name(), "TASX") == 0)
      {
      _otherVars.push_back(var);
      }
    }

  if (_diameter[0] == 0.0)
    {
    std::cout << "Cell diameters for " << cname <<
		" appear to be mid-point not end-point values.\n";
    std::cout << "  This will cause mildly erroneous/amusing results.\n";

    _diameter[0] = _diameter[1] - (_diameter[2] - _diameter[1]);
    }

  ComputeWidths();

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
bool Probe::ReadCounts(long start[], const long count[], float *data)
{
  if (_avar == NULL)
    return(false);

  _avar->set_cur(start);
  return(_avar->get(data, count));

}	/* END READCOUNTS */

/* -------------------------------------------------------------------- */
bool Probe::ReadConcen(long start[], const long count[], float *data)
{
  if (_cvar == NULL)
    return(false);

  _cvar->set_cur(start);
  return(_cvar->get(data, count));

}	/* END READCONCEN */

/* -------------------------------------------------------------------- */
void Probe::ComputeConcentration(float *accum, float *conc, long countV[],
	const std::vector<float *> & otherVarData)
{
  std::cerr << "No Compute function at this time for " << _cvar->name() << ".\n";

}	/* END COMPUTECONCENTRATION */

/* -------------------------------------------------------------------- */
bool Probe::ReadOtherVar(int idx, long start[], const long count[], float *data)
{
  if (idx >= _otherVars.size())
    return(false);

  _otherVars[idx]->set_cur(start);
  return(_otherVars[idx]->get(data, count));

}	/* END READOTHERVAR */

/* -------------------------------------------------------------------- */
void Probe::UpdateCellDiams(const int first, const int last, const float *newDiams)
{
  int	nCells = Type() == FSSP ? 64 : VectorLength();

  _firstBin = first;
  _lastBin = last;

  for (int i = 0; i < nCells; ++i)
    _diameter[i] = newDiams[i];

  ComputeWidths();

}	/* END UPDATECELLDIAMS */

/* -------------------------------------------------------------------- */
void Probe::ComputeWidths()
{
  int	nCells = Type() == FSSP ? 64 : VectorLength();

  _midPointDiam[0] = _binWidth[0] = 0.0;

  for (int i = 1; i < nCells; ++i)
    {
    _midPointDiam[i] = (_diameter[i] + _diameter[i-1]) / 2;
    _binWidth[i] = _diameter[i] - _diameter[i-1];
    }

}	/* END COMPUTEWIDTHS */

/* -------------------------------------------------------------------- */
int Probe::SetEditWindow(Widget txt[])
{
  int	nTxt, i, j, cnt;

  cnt = 0;

  sprintf(buffer, "%d", FirstBin());
  XmTextFieldSetString(txt[cnt++], buffer);

  sprintf(buffer, "%d", LastBin());
  XmTextFieldSetString(txt[cnt++], buffer);


  nTxt = VectorLength() / 16;
  if (Type() == FSSP)
    nTxt *= 4;

  for (i = 0; i < nTxt; ++i)
    {
    buffer[0] = '\0';

    for (j = 0; j < 16; ++j)
      sprintf(&buffer[strlen(buffer)], "%5g ", CellSize((i*16)+j));

    XmTextFieldSetString(txt[cnt++], buffer);
    }

  return(cnt);

}	/* END SETEDITWINDOW */

/* -------------------------------------------------------------------- */
int Probe::ApplyEditWindow(Widget text[])
{
  int	nTxt, i, j, cnt;
  float	cells[256];
  char	*p, *p1;

  cnt = 0;

  p = XmTextFieldGetString(text[cnt++]);
  i = atoi(p);
  if (i > 0 && i < VectorLength())
    _firstBin = i;
  XtFree(p);

  p = XmTextFieldGetString(text[cnt++]);
  i = atoi(p);
  if (i > FirstBin() && i < VectorLength())
    _lastBin = i;
  XtFree(p);


  nTxt = VectorLength() / 16;
  if (Type() == FSSP)
    nTxt *= 4;

  for (i = 0; i < nTxt; ++i)
    {
    p = XmTextFieldGetString(text[cnt++]);
    p1 = strtok(p, ", ");

    for (j = 0; j < 16; ++j)
      {
      cells[(i*16)+j] = atof(p1);
      p1 = strtok(NULL, ", ");
      }

    XtFree(p);
    }

  UpdateCellDiams(FirstBin(), LastBin(), cells);

  return(cnt);

}	/* END APPLYEDITWINDOW */

/* -------------------------------------------------------------------- */
Probe::~Probe() { }

/* END PROBE.CC */
