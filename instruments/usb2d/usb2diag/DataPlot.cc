#include "DataPlot.h"


/* -------------------------------------------------------------------- */
DataPlot::DataPlot(QWidget * parent, FILE * fp): QWidget(parent), _freeze(false)
{
  //if find 64
  dm= new DataUsb2d64(fp);
  
  _freeze = 1;
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
  if (_freeze ||dm->GetFSize()==0)
    return;    
  if (dm->IsSameFSize()){
    std::cout<<"\n timer plot...\n";
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
 
  std::cout<<"\n repaint...\n";
  Plot(false);
}



void DataPlot::Plot(bool pnew) 
{
  
  QTime t =QTime::currentTime();
  std::cout<<"\n  Plot... "<<t.hour()<<":"<<t.minute()<<":"<<t.second()<<std::endl;
  
  _freeze=1;
  erase(rect());
  _painter.begin(this);
    
  if (pnew) {
    dm->GetPoints();
  }
  _drawIt();
  _painter.end();
  _freeze=0;
 
  t =QTime::currentTime();
  std::cout<<"  Plot end at... "<<t.hour()<<":"<<t.minute()<<":"<<t.second()<<std::endl;
}

void DataPlot::Prt() 
{
  _printer.setOrientation(QPrinter::Landscape);
  _printer.setColorMode(QPrinter::Color);
  _printer.printerSelectionOption ();
  if (! _printer.setup( this ) ) {
    std::cerr<< "\nprinter setup error!\n";
    return;
  }
 
  if( !_painter.begin( &_printer ) ){
    std::cerr<<"\n  printer error. \n";
    return;
  }

  _drawIt();
  _painter.end();
}

void DataPlot::_drawIt()
{
  QPointArray *pts;
  pts=dm->GetPArray();

  if (pts->count() <= 0) {
    std::cerr<<"\n  _drawIt-- get points error. \n";
    return;
  }
  _painter.setPen(Qt::blue);
  _painter.drawPoints(*pts, 0, pts->count());

  QPointArray *ptsln=dm->GetPln();
  _painter.setPen(Qt::green);
  for (unsigned int i=0; i<ptsln->count(); ++i) {
    QPoint p = (QPoint)ptsln->at(i);
    _painter.drawLine(p.x(), p.y(), p.x()+1024, p.y());
  }

  _painter.setPen(Qt::black);
  QPointArray *ptstx=dm->GetPtx();
  QString      tx   =dm->GetTx();

  for (unsigned int i=0; i<ptstx->count(); ++i) {
    _painter.drawText(ptstx->at(i), tx.section( '~', i, i ) );
  }
  // unsetCursor();
  //QApplication::restoreOverrideCursor();
}

