/*
-------------------------------------------------------------------------
OBJECT NAME:	Probe.h

FULL NAME:	Probe Information

TYPE:		Base class

COPYRIGHT:	University Corporation for Atmospheric Research, 1997-2007
-------------------------------------------------------------------------
*/

#ifndef PROBE_H
#define PROBE_H

#include "define.h"
#include <netcdf.hh>

#include <vector>

#define MAX_O_VARS	40

/* -------------------------------------------------------------------- */
class Probe
{
public:
	Probe(NcFile *file, NcVar *av);
       ~Probe();

  enum	ProbeType	{ NoProbe, FSSP, F300, PCASP, X260, X200, Y200, MASP, X300, TWODC, TWODP, HVPS, S100, S200, S300, RDMA, CLIMET, CMCA };

  std::string	Name() const	{ return(name); }
  std::string	Units() const	{ return(units); }
  std::string	Title() const	{ return(units); }
  ProbeType	Type()		{ return(type); }

  int	VectorLength() const	{ return(vectorLength); }
  int	DataRate() const	{ return(dataRate); }

  short	FirstBin() const	{ return(firstBin); }
  short	LastBin() const		{ return(lastBin); }

  bool	HaveConcentrations()	{ return(cvar); }

  virtual float CellSize(int idx)	{ return(diameter[idx]); }
  virtual float BinWidth(int idx)	{ return(binWidth[idx]); }

  bool	ReadCounts(long start[], const long count[], float *data);
  bool	ReadConcen(long start[], const long count[], float *data);

  virtual void	UpdateCellDiams(const int first, const int last,
		const float *newDiams);

  virtual void ComputeConcentration(float *accum, float *conc, long countV[],
		float *otherVarData[]);

  virtual void SetRange(int)	{}

  int	nOtherVars()			{ return(otherVars.size()); }
  const char	*OtherVarName(int idx)	{ return(otherVars[idx]->name()); }

  bool	ReadOtherVar(int idx, long start[], const long count[], float *data);

  virtual int	SetEditWindow(Widget txt[]);
  virtual int	ApplyEditWindow(Widget txt[]);

  bool	operator==(Probe& rhs) { return(name == rhs.name); }

  bool	operator==(const Probe& rhs) { return(name == rhs.name); }

protected:
  virtual void	ComputeWidths();

  NcVar		*avar, 	*cvar;		// netCDF variable ID.
  std::vector<NcVar *> otherVars;	// Hskping & Derived Vars for action box
  
  ProbeType	type;
  std::string	name;
  std::string	serialNum;
  std::string	units;

  unsigned int	vectorLength;
  unsigned int	dataRate;
  int		firstBin, lastBin;

  std::vector<float> diameter, midPointDiam, binWidth, sampleVolume;

};	// END PROBE.H

#endif
