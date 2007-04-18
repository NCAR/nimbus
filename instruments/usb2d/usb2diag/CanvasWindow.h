/*
-------------------------------------------------------------------------
OBJECT NAME:	CanvasWindow.h

COPYRIGHT:	University Corporation for Atmospheric Research, 2007
-------------------------------------------------------------------------
*/

#ifndef _CanvasWindow_h_
#define _CanvasWindow_h_

#include <qmainwindow.h>
#include <qfiledialog.h>
#include <qprinter.h>
#include <qpainter.h>

class QApplication;
class DataPlot;

/* -------------------------------------------------------------------- */
class CanvasWindow : public QMainWindow
{
  Q_OBJECT

public:
  CanvasWindow(QApplication *, const char * file_name);

protected slots:
  void	_print();
  void	_openf();
  void  _rstTimer();

private:
  FILE * _fp;
  QApplication*  _app;
  QPrinter     *printer;
  char*  _fn;

  DataPlot * _plot;
  void _startplot();

};

#endif
