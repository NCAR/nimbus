#include "DataPlot.h"

#include <unistd.h>
#include <qwt_legend.h>

P2d_rec	hvpsRecord;


/* -------------------------------------------------------------------- */
DataPlot::DataPlot(QWidget *parent, ADS_rtFile *f) : QwtPlot(parent)
{
  file = f;
  freeze = false;

  setAxisTitle(QwtPlot::xBottom, "Diode #");
  setAxisTitle(QwtPlot::yLeft, "Volts");
  setMargin(10);
  setAutoLegend(true);
  setCanvasBackground(QColor("white"));

  crv1 = insertCurve("Masked");
  crv2 = insertCurve("Valid");
  setCurvePen(crv1, QPen(red));
  setCurvePen(crv2, QPen(green));
  setCurveStyle(crv1, QwtCurve::Sticks);
  setCurveStyle(crv2, QwtCurve::Sticks);

  file->FirstPMS2dRecord(&hvpsRecord);

  (void)startTimer(10);

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void DataPlot::ToggleFreeze()
{
  freeze = 1 - freeze;

}	/* END TOGGLEFREEZE */

/* -------------------------------------------------------------------- */
void DataPlot::timerEvent(QTimerEvent *)
{   
  if (freeze || file->NextPMS2dRecord(&hvpsRecord) == false)
    return;    

  Hdr_blk	*hdr = (Hdr_blk *)&hvpsRecord;
  ushort	*data = (ushort *)hvpsRecord.data;

  if (ntohs(data[0]) != 0xcaaa)
    return;

  ushort	*mask = (ushort *)&hvpsRecord.data[4];
  double	xVal1[300], xVal2[300], yVal1[300], yVal2[300];
  int		rCnt, gCnt;

  sprintf(buffer, "UTC (%02d:%02d:%02d)",
	ntohs(hdr->hour), ntohs(hdr->minute), ntohs(hdr->second));
  setAxisTitle(QwtPlot::xBottom, buffer);

  rCnt = gCnt = 0;
  for (int i = 0; i < 256; ++i)
  {
//printf("mask[%d]=%x i%16=%d = %d\n", i/16, mask[i/16], i%16, (0x8000 >> (i%16)));
    if ( mask[i/16] & (0x8000 >> (i%16)) )
    {
      xVal2[gCnt] = i;
      yVal2[gCnt++] = data[28 + i];
    }
    else
    {
      xVal1[rCnt] = i;
      yVal1[rCnt++] = data[28 + i];
    }
  }

  setCurveData(crv1, xVal1, yVal1, rCnt);
  setCurveData(crv2, xVal2, yVal2, gCnt);

  replot();
  sleep(1);

}	/* END TIMEREVENT */

/* END DATAPLOT.CC */
