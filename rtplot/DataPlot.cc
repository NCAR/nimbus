#include "DataPlot.h"

#include <qwt/qwt_legend.h>

/* -------------------------------------------------------------------- */
DataPlot::DataPlot(QWidget *parent, ADS_rtFile *f) : QwtPlot(parent)
{
  file = f;
  freeze = false;
  units = VOLTS;

  setAxisTitle(QwtPlot::xBottom, "Time");
  setAxisTitle(QwtPlot::yLeft, "Volts");
  setMargin(10);
  setAutoLegend(true);
  setCanvasBackground(QColor("white"));

  file->LastSyncRecord(ADSrecord);

  (void)startTimer(50);

}

/* -------------------------------------------------------------------- */
void DataPlot::ToggleFreeze()
{
  freeze = 1 - freeze;

}	/* END TOGGLEFREEZE */

/* -------------------------------------------------------------------- */
void DataPlot::setUnits(int Units)
{
  units = Units;

  switch (units)
  {
    case COUNTS:
      setAxisTitle(QwtPlot::yLeft, "Counts");
      break;

    case VOLTS:
      setAxisTitle(QwtPlot::yLeft, "Volts");
      break;

    case ENGINEERING:
      setAxisTitle(QwtPlot::yLeft, "Engineering");
      break;
  }

}       /* END TOGGLEUNITS */

/* -------------------------------------------------------------------- */
void DataPlot::timerEvent(QTimerEvent *)
{   
  if (freeze || file->NextSyncRecord(ADSrecord) == false)
    return;    

  Hdr_blk *hdr = (Hdr_blk *)ADSrecord;

  DecodeRecord();
  ApplyCalCoes(SampledData);

  sprintf(buffer, "UTC (%02d:%02d:%02d)",
	ntohs(hdr->hour), ntohs(hdr->minute), ntohs(hdr->second));
  setAxisTitle(QwtPlot::xBottom, buffer);

  switch (units)
  {
    case COUNTS:
      AddDataToBuffer(bits);
      break;

    case VOLTS:
      AddDataToBuffer(volts);
      break;

    case ENGINEERING:
      AddDataToBuffer(SampledData);
      break;  

  }         

  replot();

}

/* END DATAPLOT.CC */
