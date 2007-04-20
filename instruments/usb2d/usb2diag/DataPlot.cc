#include "DataPlot.h"

#include <qevent.h>
#include <qpainter.h>

#include <cstdio>
#include <unistd.h>
#include <qcursor.h>
#include <qapplication.h>


/* -------------------------------------------------------------------- */
DataPlot::DataPlot(QWidget * parent, FILE * fp): QWidget(parent), _freeze(false)
{
  //if find 64
  dm= new DataUsb2d64(fp);
  //dm->Posfp();
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
  if (dm->IsSameFSize()){
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
  Plot(false);
}



void DataPlot::Plot(bool pnew) 
{
  //QApplication::setOverrideCursor( QCursor(Qt::WaitCursor));
  //setCursor(QCursor(Qt::WaitCursor));
  erase(rect());
  QPainter _painter(this);
  _painter.setPen(Qt::blue);
  
  QPointArray *pts;
  if (pnew) {
    pts=dm->GetPoints();
  } else {
    pts=dm->GetPArray();
  }
  if (pts->count() >0) {
    _painter.drawPoints(*pts, 0, pts->count());
  }

  QPointArray *ptsln=dm->GetPln();
  _painter.setPen(Qt::green);
  for (int i=0; i<ptsln->count(); ++i) {
    QPoint p = (QPoint)ptsln->at(i);
    _painter.drawLine(p.x(), p.y(), p.x()+1024, p.y());
  }

  _painter.setPen(Qt::black);
  QPointArray *ptstx=dm->GetPtx();
  QString      tx   =dm->GetTx();

  for (int i=0; i<ptstx->count(); ++i) {
    _painter.drawText(ptstx->at(i), tx.section( '~', i, i ) );
  }
 // unsetCursor();
  //QApplication::restoreOverrideCursor();
  
}

