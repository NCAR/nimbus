/*
-------------------------------------------------------------------------
OBJECT NAME:	dataIO.c

FULL NAME:	Data IO

ENTRY POINTS:	ReadData()
		GetData()
		AddVariable()
		DeleteVariable()

STATIC FNS:	free_data()

DESCRIPTION:	Read data from shared memory and process it.

INPUT:		none

OUTPUT:		none

COPYRIGHT:	University Corporation for Atmospheric Research, 2003
-------------------------------------------------------------------------
*/

#include "define.h"

#include <algorithm>
#include <cfloat>
#include <cstring>

#include <qpen.h>

#include "DataPlot.h"

#include <qwt/qwt_math.h>

static char	*color[] = { "red", "blue", "green", "purple", "maroon" };

/* -------------------------------------------------------------------- */
void AddDataToBuffer(NR_TYPE *newData)
{
  int	i, j, idx;

  for (i = 0; i < nVariables; ++i)
  {
    qwtShiftArray(plotData[i], Variable[i].nPoints, -Variable[i].SampleRate);

    idx = Variable[i].nPoints - Variable[i].SampleRate;

    for (j = 0; j < Variable[i].SampleRate; ++j)
      plotData[i][idx+j] = (double)newData[Variable[i].SRstart+j];
  }

}	/* END ADDDATATOBUFFER */

/* -------------------------------------------------------------------- */
void AddVariable(int indx, DataPlot *plot)
{
  if (indx >= sdi.size())
  {
    indx -= sdi.size();

    strcpy(Variable[nVariables].name, raw[indx]->name);
    Variable[nVariables].SampleRate = raw[indx]->SampleRate;
    Variable[nVariables].SRstart = raw[indx]->SRstart;
    Variable[nVariables].nPoints = raw[indx]->SampleRate * NumberSeconds;
  }
  else
  {
    strcpy(Variable[nVariables].name, sdi[indx]->name);
    Variable[nVariables].SampleRate = sdi[indx]->SampleRate;
    Variable[nVariables].SRstart = sdi[indx]->SRstart;
    Variable[nVariables].nPoints = sdi[indx]->SampleRate * NumberSeconds;
  }

  xData[nVariables] = new double[Variable[nVariables].nPoints];
  plotData[nVariables] = new double[Variable[nVariables].nPoints];
  memset(plotData[nVariables], 0, Variable[nVariables].nPoints * sizeof(double));

  for (int i = 0; i < NumberSeconds; ++i)
    for (int j = 0; j < Variable[nVariables].SampleRate; ++j)
    {
      xData[nVariables][(i*Variable[nVariables].SampleRate)+j] =
	-NumberSeconds + (double)i + (j*Variable[nVariables].SampleRate/1000);
    }

  Variable[nVariables].plotID = plot->insertCurve(Variable[nVariables].name);

  plot->setCurvePen(Variable[nVariables].plotID, QPen(color[nVariables]));
  plot->setCurveRawData(Variable[nVariables].plotID,
      xData[nVariables], plotData[nVariables], Variable[nVariables].nPoints);

  ++NumberDataSets;
  ++nVariables;

}	/* END ADDVARIABLE */

/* -------------------------------------------------------------------- */
void DeleteVariable(int indx, DataPlot *plot)
{
  int	i;

  delete [] xData[indx];
  delete [] plotData[indx];

  plot->removeCurve(Variable[indx].plotID);

  for (i = indx+1; i < NumberDataSets; ++i)
  {
    memcpy(&Variable[i-1], &Variable[i], sizeof(VARTBL));
    plotData[i-1] = plotData[i];
  }

  --NumberDataSets;
  --nVariables;

}	/* END DELETEVARIABLE */

/* END DATAIO.C */
