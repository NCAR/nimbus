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
#include <qpaintdevicemetrics.h>
#include <qwhatsthis.h>
#include <qsimplerichtext.h>
#include <qstylesheet.h>

#include "DataPlot.h"

/* -------------------------------------------------------------------- */
CanvasWindow::CanvasWindow(QApplication *qApp, const char * file_name) : QMainWindow(0, "canvas"), _app(qApp), _fn((char*)file_name)
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


/* QPopupMenu * help = new QPopupMenu( this );
        menuBar()->insertItem( "&Help", help );

        help->insertItem( "&About", this, SLOT(about()), Key_F1 );

*/
  _startplot();
  statusBar()->message( "Ready", 2000 );

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void CanvasWindow::_openf()
{
  //std::cout<<env(DATA_DIR)<<std::endl;
  //QString appdir= _app->applicationDirPath();
  QFileDialog *dlg = new QFileDialog(getenv("DATA_DIR"), 
  QString::null, 0, 0, TRUE );
  dlg->setCaption( QFileDialog::tr( "Open Data File" ) );
  dlg->setMode( QFileDialog::ExistingFile );
  
  QString file_name;
  if ( dlg->exec() == QDialog::Accepted ) {
    file_name = dlg->selectedFile();
    //file_name = file_name.section('/',-1);
    //file_name = appdir+"/"+file_name; 
    std::cout << "selected name:" << file_name <<"\n"<<std::endl;
  }
  delete dlg;

  _fn = (char*)file_name.ascii();
  _startplot();
  
}	/* END file open */

/* -------------------------------------------------------------------- */
void CanvasWindow::_startplot()
{
  if ((_fp = fopen(_fn, "r")) == 0)
  {
    std::cerr << "No file " << _fn <<"\n"<< std::endl;
    return;
  } 
  _plot = new DataPlot(this, _fp);
  setCentralWidget(_plot);
  _plot->show();
}
        /* END _startplot */


/* -------------------------------------------------------------------- */
/*void CanvasWindow::_print()
{

   if ( printer->setup( this ) ) {
        QPainter paint;
        if( !paint.begin( printer ) )
            return;
      //  drawIt( &pa/int );
   }

}*/	/* END PRINT */

void CanvasWindow::_print()
{
   /*
    printer->setFullPage( TRUE );
    if ( printer->setup(this) ) {               // printer dialog
        statusBar()->message( "Printing..." );
        QPainter p;
        if( !p.begin( printer ) ) {               // paint on printer
            statusBar()->message( "Printing aborted", 2000 );
            return;
        }

        QPaintDeviceMetrics metrics( p.device() );
        int dpiy = metrics.logicalDpiY();
        int margin = (int) ( (2/2.54)*dpiy ); // 2 cm margins
        QRect view( margin, margin, metrics.width() - 2*margin, metrics.height() - 2*margin );
        QSimpleRichText richText( QStyleSheet::convertFromPlainText(e->text()),
                                  QFont(),
                                  e->context(),
                                  e->styleSheet(),
                                  e->mimeSourceFactory(),
                                  view.height() );
        richText.setWidth( &p, view.width() );
        int page = 1;
        do {
            richText.draw( &p, margin, margin, view, colorGroup() );
            view.moveBy( 0, view.height() );
            p.translate( 0 , -view.height() );
            p.drawText( view.right() - p.fontMetrics().width( QString::number( page ) ),
                        view.bottom() + p.fontMetrics().ascent() + 5, QString::number( page ) );
            if ( view.top() - margin >= richText.height() )
                break;
            printer->newPage();
            page++;
        } while (TRUE);

        statusBar()->message( "Printing completed", 2000 );
    } else {
        statusBar()->message( "Printing aborted", 2000 );
    }*/
}



/* END CANVASWINDOW.CC */
