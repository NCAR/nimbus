#include "DataPlot.h"

#include <qevent.h>
#include <qpainter.h>

#include <cstdio>
#include <unistd.h>


/* -------------------------------------------------------------------- */
DataPlot::DataPlot(QWidget * parent, FILE * fp): QWidget(parent), _freeze(false)
{
  //if find 64
  dm= new DataUsb2d64(fp);
  dm->Posfp();
  Plot();
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
  if (dm->Posfp()){
    Plot();
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
  dm->Posfp();
  Plot();
}



void DataPlot::Plot() 
{
  erase(rect());
  QPainter _painter(this);
  _painter.setPen(Qt::blue);
  QPointArray *pts=dm->GetPoints();
  if (pts->count() >0) {
    _painter.drawPoints(*pts, 0, pts->count());
  }
}

