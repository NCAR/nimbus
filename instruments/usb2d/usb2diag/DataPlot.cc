#include "DataPlot.h"

#include <qevent.h>
#include <qpainter.h>

#include <cstdio>
#include <unistd.h>

/* -------------------------------------------------------------------- */
DataPlot::DataPlot(QWidget * parent, FILE * fp): QWidget(parent), _freeze(false)
{
  //if find 64
  dm= (DataMng*) new DataMng(fp);
  dm->posfp();
  plot();
  startTimer(2000);
}

/* -------------------------------------------------------------------- */
void DataPlot::ToggleFreeze()
{
  _freeze = 1 - _freeze;

}

/* -------------------------------------------------------------------- */
void DataPlot::timerEvent(QTimerEvent *)
{   
  if (_freeze)
    return;    
  if (dm->posfp()){
    plot();
  }
}

/* -------------------------------------------------------------------- */
void DataPlot::RstTimer(int s)
{
  killTimers();
  startTimer(s*1000);

}
/* -------------------------------------------------------------------- */
void DataPlot::paintEvent(QPaintEvent * e)
{
  QWidget::paintEvent(e);
  dm->posfp();
  plot();
}



void DataPlot::plot() 
{
  erase(rect());
  QPainter _painter(this);
  _painter.setPen(Qt::blue);
  QPointArray *pts=dm->getPoints();
  _painter.drawPoints(*pts, 0, dm->getP());
}

