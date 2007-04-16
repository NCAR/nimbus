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
CanvasWindow::CanvasWindow(QApplication *qApp, const char * file_name) : QMainWindow(0, "canvas"), _app(qApp)
{
  
  QToolBar *toolBar = new QToolBar(this, "options");
  toolBar->setLabel( "" );

  
  QToolButton *f = new QToolButton(toolBar);
  f->setTextLabel("OpenFile");
  f->setUsesTextLabel(true);
  f->setUsesBigPixmap(false);
  connect(f, SIGNAL(clicked()), SLOT(_openf()));


  QToolButton *p = new QToolButton(toolBar);
  p->setTextLabel("Print");
  p->setUsesTextLabel(true);
  p->setUsesBigPixmap(false);
  connect(p, SIGNAL(clicked()), SLOT(_print()));


  QToolButton *q = new QToolButton(toolBar);
  q->setTextLabel("Quit");
  q->setUsesTextLabel(true);
  q->setUsesBigPixmap(false);
  connect(q, SIGNAL(clicked()), qApp, SLOT(quit()));


  if ((_fp = fopen(file_name, "r")) == 0)
  {
    std::cerr << "No file " << file_name <<"\n"<< std::endl;
  } else{
    _startplot();   
  }
  statusBar()->message( "Ready", 2000 );

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void CanvasWindow::_openf()
{
  std::cout<<_app->applicationDirPath().ascii()<<std::endl;
  QString appdir= _app->applicationDirPath();
  QFileDialog *dlg = new QFileDialog(_app->applicationDirPath().ascii(), 
  QString::null, 0, 0, TRUE );
  dlg->setCaption( QFileDialog::tr( "Open Data File" ) );
  dlg->setMode( QFileDialog::ExistingFile );
  
  QString file_name;
  if ( dlg->exec() == QDialog::Accepted ) {
    file_name = dlg->selectedFile();
    file_name = file_name.section('/',-1);
    file_name = (appdir+"/"+file_name).ascii(); 
    std::cout << "selected name:" << file_name <<"\n"<<std::endl;
  }
  delete dlg;

  //std::cout << "selected name2:" << file_name <<"\n"<<std::endl;
  //delete _fp;
  if ((_fp = fopen(file_name, "r")) == 0)
  {
    std::cerr << "No file. " << file_name << std::endl;
  } else{
    std::cout << "Start Plotting. " << std::endl;
    _startplot();   
  }
}	/* END file open */

/* -------------------------------------------------------------------- */
void CanvasWindow::_startplot()
{
  //if (_plot!=NULL) {delete _plot;}
std::cout << "_StartPlotting. 1 _fp: " <<_fp<< std::endl;
  _plot = new DataPlot(this, _fp);
  setCentralWidget(_plot);
  _plot->show();
}
        /* END _startplot */


/* -------------------------------------------------------------------- */
void CanvasWindow::_print()
{

}	/* END PRINT */

/* END CANVASWINDOW.CC */
