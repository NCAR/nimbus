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

COPYRIGHT:	University Corporation for Atmospheric Research, 2003-06
-------------------------------------------------------------------------
*/

#include "define.h"

#include <algorithm>
#include <cfloat>
#include <cstring>

#include <qpen.h>

#include "DataPlot.h"

#include <qwt_plot_curve.h>
#include <qwt_math.h>

static char	*color[] = { "red", "blue", "green", "purple", "maroon" };

/* -------------------------------------------------------------------- */
void AddDataToBuffer(NR_TYPE *newData)
{
  for (size_t i = 0; i < nVariables; ++i)
  {
    memcpy(	plotData[i],
		&plotData[i][Variable[i].SampleRate],
		sizeof(double) * Variable[i].nPoints);

    size_t idx = Variable[i].nPoints - Variable[i].SampleRate;

    for (size_t j = 0; j < Variable[i].SampleRate; ++j)
      plotData[i][idx+j] = (double)newData[Variable[i].SRstart+j];
  }

}	/* END ADDDATATOBUFFER */

/* -------------------------------------------------------------------- */
void AddVariable(size_t indx, DataPlot *plot)
{
  strcpy(Variable[nVariables].name, raw[indx]->name);
  Variable[nVariables].SampleRate = raw[indx]->SampleRate;
  Variable[nVariables].SRstart = raw[indx]->SRstart;
  Variable[nVariables].nPoints = raw[indx]->SampleRate * NumberSeconds;

  xData[nVariables] = new double[Variable[nVariables].nPoints];
  plotData[nVariables] = new double[Variable[nVariables].nPoints];
  memset(plotData[nVariables], 0, Variable[nVariables].nPoints * sizeof(double));

  for (size_t i = 0; i < NumberSeconds; ++i)
    for (size_t j = 0; j < Variable[nVariables].SampleRate; ++j)
    {
      xData[nVariables][(i*Variable[nVariables].SampleRate)+j] =
	-NumberSeconds + (double)i + (j*Variable[nVariables].SampleRate/1000);
    }

//  Variable[nVariables].plotID = plot->insertCurve(Variable[nVariables].name);
  QwtPlotCurve * curve = new QwtPlotCurve("");
  curve->attach(plot);
  Variable[nVariables].curve = curve;

  curve->setPen(QPen(color[nVariables]));
  curve->setRawData(xData[nVariables], plotData[nVariables], Variable[nVariables].nPoints);

  ++NumberDataSets;
  ++nVariables;

}	/* END ADDVARIABLE */

/* -------------------------------------------------------------------- */
void DeleteVariable(size_t indx, DataPlot *plot)
{
  Variable[indx].curve->detach();

  delete [] xData[indx];
  delete [] plotData[indx];

  for (size_t i = indx+1; i < NumberDataSets; ++i)
  {
    memcpy(&Variable[i-1], &Variable[i], sizeof(VARTBL));
    plotData[i-1] = plotData[i];
  }

  --NumberDataSets;
  --nVariables;

}	/* END DELETEVARIABLE */

/* END DATAIO.C */
