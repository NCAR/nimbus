/*
-------------------------------------------------------------------------
OBJECT NAME:	DataSet.cc

FULL NAME:	Data Set Class

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1997-2000
-------------------------------------------------------------------------
*/

#include "DataSet.h"
#include "DataFile.h"
#include "Probe.h"

#include <cfloat>
#include <algorithm>


/* -------------------------------------------------------------------- */
DataSet::DataSet(DataFile *df, Probe *prb, FlightClock& start, int nRecs, DataType dt, NormType nt) : _file(df), _probe(prb), startTime(start)
{
  accum = NULL;
  conc = surface = volume = NULL;

  computeConc = False;

  nRecords = nRecs;
  nWords = _probe->DataRate() * _probe->VectorLength() * nRecs;
  SetDataTypes(dt);

  normalization.resize(_probe->VectorLength());
  for (size_t i = 0; i < _probe->VectorLength(); ++i)
    normalization[i] = 1.0;

  otherVars.resize(_probe->nOtherVars());
  for (size_t i = 0; i < _probe->nOtherVars(); ++i)
    otherVars[i] = new float[nRecs];

  SetNormalize(nt);

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void DataSet::SetDataTypes(DataType dt)
{
  if (dt & COUNTS || computeConc) {
    if (!accum)
      accum = new float[nWords];
    }
  else
    if (accum) {
      delete [] accum;
      accum = NULL;
      }

  if (_probe->HaveConcentrations() && dt & (CONCENTRATION | SURFACE | VOLUME)) {
    if (!conc)
      conc = new float[nWords];
    }
  else
    if (conc) {
      delete [] conc;
      conc = NULL;
      }

  if (_probe->HaveConcentrations() && dt & SURFACE) {
    if (!surface)
      surface = new float[nWords];
    }
  else
    if (surface) {
      delete [] surface;
      surface = NULL;
      }

  if (_probe->HaveConcentrations() && dt & VOLUME) {
    if (!volume)
      volume = new float[nWords];
    }
  else
    if (volume) {
      delete [] volume;
      volume = NULL;
      }

}	/* END SETDATATYPE */

/* -------------------------------------------------------------------- */
void DataSet::SetNormalize(NormType nt)
{
  normType = nt;

  for (int i = 1; i < _probe->VectorLength(); ++i)
    {
    switch (normType)
      {
      case LINEAR:
        normalization[i] = _probe->BinWidth(i);
        break;

      case LOG:
        normalization[i] = log10(_probe->CellSize(i)) - log10(_probe->CellSize(i-1));
        break;

      default:
        normalization[i] = 1.0;
      }
    }

  findMinMax();

}	/* END SETNORMALIZE */

/* -------------------------------------------------------------------- */
void DataSet::SetCompute(bool x)
{
  computeConc = x;

  if (computeConc && !accum)
    accum = new float[nWords];

}	/* END SETCOMPUTE */

/* -------------------------------------------------------------------- */
void DataSet::ResizeData(int nRecs)
{
  int	i;

  nRecords = nRecs;
  nWords = _probe->DataRate() * _probe->VectorLength() * nRecs;

  if (accum)
    {
    delete [] accum;
    accum = new float[nWords];
    }

  if (conc)
    {
    delete [] conc;
    conc = new float[nWords];
    }

  if (surface)
    {
    delete [] surface;
    surface = new float[nWords];
    }

  if (volume)
    {
    delete [] volume;
    volume = new float[nWords];
    }

  for (i = 0; i < _probe->nOtherVars(); ++i)
    {
    delete [] otherVars[i];
    otherVars[i] = new float[nRecs];
    }

}	/* END RESIZEDATA */

/* -------------------------------------------------------------------- */
void DataSet::ReadData(int nRecs, int avRate)
{
  int	i, j, k;
  long	startV[3], countV[3], nPoints;
  float	*accumBuff = NULL;
  float	*concBuff = NULL;
  std::vector<float *> timeSeriesData;

  nRecords = nRecs;

  if (_probe->DataRate() > 1)
    avRate = 1;


// Setup for reading counts and/or concentrations.
  startV[0] = startTime - _file->StartTime();
  startV[1] = startV[2] = 0;

  countV[0] = avRate;
  countV[1] = 1;
  countV[2] = _probe->VectorLength();

  if (countV[1] == 0 || countV[2] == 0)
    {
    std::cerr << "Count[] = " << countV[0] << countV[1] << countV[2] << "\n";
    exit(1);
    }

  nPoints = countV[0] * countV[1] * countV[2];

/*
cout << "nRecs = " << nRecs << ", avRate = " << avRate << "\n";
cout << "Start[] = " << startV[0] << ", "<< startV[1]<<", "<< startV[2] << "\n";
cout << "Count[] = " << countV[0] << ", "<< countV[1]<<", "<< countV[2] << "\n";
*/
  if (accum)
    {
    accumBuff = new float[nPoints];
    memset(accum, 0, nRecs * _probe->VectorLength() * sizeof(float));
    }

  if (conc)
    {
    concBuff = new float[nPoints];

    for (i = 0; i < nRecs * _probe->VectorLength(); ++i)
      conc[i] = _probe->FillValue();

//    memset(conc, 0, nRecs * _probe->VectorLength() * sizeof(float));
    }

  if (surface)
    memset(surface, 0, nRecs * _probe->VectorLength() * sizeof(float));

  if (volume)
    memset(volume, 0, nRecs * _probe->VectorLength() * sizeof(float));

  timeSeriesData.resize(_probe->nOtherVars());
  for (i = 0; i < _probe->nOtherVars(); ++i)
    timeSeriesData[i] = new float[avRate];


  for (i = 0; i < nRecs; ++i)
    {
    if (accum)
      _probe->ReadCounts(startV, (const long *)countV, accumBuff);

    for (j = 0; j < _probe->nOtherVars(); ++j)
      _probe->ReadOtherVar(j, startV, countV, timeSeriesData[j]);

    if (conc)
      if (computeConc)
        _probe->ComputeConcentration(accumBuff,concBuff, countV, timeSeriesData);
      else
        _probe->ReadConcen(startV, (const long *)countV, concBuff);


    // Average other vars;
    for (j = 0; j < _probe->nOtherVars(); ++j)
      {
      otherVars[j][i] = 0.0;

      for (k = 0; k < avRate; ++k)
        otherVars[j][i] += timeSeriesData[j][k];

      otherVars[j][i] /= avRate;

      if (strncmp(_probe->OtherVarName(j), "FRNG", 4) == 0)
        _probe->SetRange((int)otherVars[j][0]);
      }


    // Average data.
    for (j = 1; j < _probe->VectorLength(); ++j)
      {
      int dest = (i * _probe->VectorLength()) + j, avCntr = 0;

      for (k = 0; k < avRate; ++k)
        {
        if (accum && accumBuff[(k * _probe->VectorLength()) + j] != _probe->FillValue())
          accum[dest] += accumBuff[(k * _probe->VectorLength()) + j];

        if (conc && concBuff[(k * _probe->VectorLength()) + j] != _probe->FillValue())
          {
          if (conc[dest] == _probe->FillValue())
            conc[dest] = concBuff[(k * _probe->VectorLength()) + j];
          else
            conc[dest] += concBuff[(k * _probe->VectorLength()) + j];

          ++avCntr;
          }
        }

      if (conc && avCntr > 0)
        conc[dest] /= avCntr;

      // Convert #/L data to #/cm3 so we have consistant data.
      if ((conc && conc[dest] != _probe->FillValue()) &&
		_probe->Units().find("/L") != _probe->Units().npos ||
		_probe->Units().find("liter") != _probe->Units().npos)
        conc[dest] /= 1000;

      if (surface)
        surface[dest] = conc[dest] * 4.0 * M_PI *
		pow((_probe->CellSize(j-1) + _probe->CellSize(j)) / 4.0, 2.0);

      if (volume)
        volume[dest] = conc[dest] * 4.0 / 3.0 * M_PI *
		pow((_probe->CellSize(j-1) + _probe->CellSize(j)) / 4.0, 3.0);
      }

    if (_probe->DataRate() > 1)
      {
      if (++startV[1] >= _probe->DataRate())
        {
        startV[1] = 0;
        ++startV[0];
        }
      }
    else
      startV[0] += avRate;
    }


  if (accumBuff)
    delete [] accumBuff;

  if (concBuff)
    delete [] concBuff;

  for (i = 0; i < _probe->nOtherVars(); ++i)
    delete [] timeSeriesData[i];

  findMinMax();

}	/* END READDATA */

/* -------------------------------------------------------------------- */
float DataSet::Accumulation(int s, int cell) const
{
  float value = accum[(s * _probe->VectorLength()) + cell];

  if (value == _probe->FillValue())
    return _probe->FillValue();
  else
    return value;
}

/* -------------------------------------------------------------------- */
float DataSet::Concentration(int s, int cell) const
{
  float value = conc[(s * _probe->VectorLength()) + cell];

  if (value == _probe->FillValue())
    return _probe->FillValue();
  else
    return value / normalization[cell];
}

/* -------------------------------------------------------------------- */
float DataSet::Surface(int s, int cell) const
{
  float value = surface[(s * _probe->VectorLength()) + cell];

  if (value == _probe->FillValue())
    return _probe->FillValue();
  else
    return value / normalization[cell];
}

/* -------------------------------------------------------------------- */
float DataSet::Volume(int s, int cell) const
{
  float value = volume[(s * _probe->VectorLength()) + cell];

  if (value == _probe->FillValue())
    return _probe->FillValue();
  else
    return value / normalization[cell];
}

/* -------------------------------------------------------------------- */
void DataSet::findMinMax()
{
  int	i, j;
  float	c;

  if (accum)
    {
    minAccum = FLT_MAX;
    maxAccum = -FLT_MAX;

    for (i = 0; i < nRecords; ++i)
      for (j = 1; j < _probe->VectorLength(); ++j)
        {
        c = Accumulation(i, j);

        minAccum = std::min(minAccum, c);
        maxAccum = std::max(maxAccum, c);
        }
    }

  if (conc)
    {
    minConc = FLT_MAX;
    maxConc = -FLT_MAX;

    for (i = 0; i < nRecords; ++i)
      for (j = _probe->FirstBin(); j <= _probe->LastBin(); ++j)
        {
        c = Concentration(i, j);

        if (c > 0.0)	// Smallest conc != 0.
          minConc = std::min(minConc, c);

        maxConc = std::max(maxConc, c);
        }

    if (maxConc == 0.0)
      minConc = 0.0;
    }

  if (surface)
    {
    minSurf = FLT_MAX;
    maxSurf = -FLT_MAX;

    for (i = 0; i < nRecords; ++i)
      for (j = _probe->FirstBin(); j <= _probe->LastBin(); ++j)
        {
        c = Surface(i, j);

        if (c > 0.0)	// Smallest surface != 0.
          minSurf = std::min(minSurf, c);
        maxSurf = std::max(maxSurf, c);
        }

    if (maxSurf == 0.0)
      minSurf = 0.0;
    }

  if (volume)
    {
    minVol = FLT_MAX;
    maxVol = -FLT_MAX;

    for (i = 0; i < nRecords; ++i)
      for (j = _probe->FirstBin(); j <= _probe->LastBin(); ++j)
        {
        c = Volume(i, j);

        if (c > 0.0)	// Smallest vol != 0.
          minVol = std::min(minVol, c);
        maxVol = std::max(maxVol, c);
        }

    if (maxVol == 0.0)
      minVol = 0.0;
    }

}	/* END FINDMINMAX */

/* -------------------------------------------------------------------- */
DataSet::~DataSet()
{
  if (accum)	delete [] accum;
  if (conc)	delete [] conc;
  if (surface)	delete [] surface;
  if (volume)	delete [] volume;
}

/* END DATASET.CC */
