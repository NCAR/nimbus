/*
-------------------------------------------------------------------------
OBJECT NAME:	DataSet.h

FULL NAME:	Data set information

COPYRIGHT:	University Corporation for Atmospheric Research, 1997-2007
-------------------------------------------------------------------------
*/

#ifndef DATASET_H
#define DATASET_H

#include "define.h"
#include <vector>

#include <raf/Time.h>

class DataFile;
class Probe;

/* -------------------------------------------------------------------- */
class DataSet
{
public:
	DataSet(DataFile *df, Probe *prb, FlightClock& start, int nRecs, DataType dt, NormType n);
	~DataSet();

  friend class SetManager;	// Manipulate endTime.

  FlightClock   StartTime()     const   { return(startTime); }
  FlightClock   EndTime()       const   { return(endTime); }

  void	SetDataTypes(DataType dt);
  void	SetNormalize(NormType nt);
  void	SetCompute(bool x);
  void	ResizeData(int nRecs);

  void	ReadData(int nRecs, int avRate);

  float	Accumulation(int s, int cell) const;
  float	Concentration(int s, int cell) const;
  float	Surface(int s, int cell) const;
  float	Volume(int s, int cell) const;

  float NormalizeFactor(int cell)	{ return(normalization[cell]); }

  float	OtherVar(int var, int idx)
		{ return(otherVars[var][idx]); }

  Probe *probe() const { return _probe; }
  DataFile *file() const { return _file; }

private:
  int	nWords, nRecords;
  float	*accum, minAccum, maxAccum;
  float	*conc, minConc, maxConc;
  float	*surface, minSurf, maxSurf;
  float	*volume, minVol, maxVol;
  std::vector<float *> otherVars;
  std::vector<float> normalization;

  FlightClock	startTime, endTime;

  NormType	normType;

  void	findMinMax();

  bool computeConc;

  DataFile *_file;
  Probe *_probe;

};	// END DATASET.H

#endif
