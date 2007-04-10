/*
-------------------------------------------------------------------------
OBJECT NAME:	CanvasWindow.cc

COPYRIGHT:	University Corporation for Atmospheric Research, 2007
-------------------------------------------------------------------------
*/

#include "CanvasWindow.h"

#include <iostream>

#include <qapplication.h>
#include <qstatusbar.h>
#include <qtoolbutton.h>

#include "DataPlot.h"

/* -------------------------------------------------------------------- */
CanvasWindow::CanvasWindow(QApplication *qApp) : QMainWindow(0, "canvas")
{
  QToolBar *toolBar = new QToolBar(this, "options");
  toolBar->setLabel( "File Operations" );


  QToolButton *q = new QToolButton(toolBar);
  q->setTextLabel("Quit");
  q->setUsesTextLabel(true);
  q->setUsesBigPixmap(false);
  connect(q, SIGNAL(clicked()), qApp, SLOT(quit()));

  QToolButton *p = new QToolButton(toolBar);
  p->setTextLabel("Print");
  p->setUsesTextLabel(true);
  p->setUsesBigPixmap(false);
  connect(p, SIGNAL(clicked()), SLOT(Print()));


  if ((_fp = fopen("/scr/jdata/usb_data.2d", "r")) == 0)
  {
    std::cerr << "Can't open file " << std::endl;
    exit(1);
  }

  _plot = new DataPlot(this, _fp);
  setCentralWidget(_plot);
  _plot->resize(1054, 800);
  _plot->show();

  statusBar()->message( "Ready", 2000 );

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void CanvasWindow::Print()
{

}	/* END PRINT */

/* END CANVASWINDOW.CC */
