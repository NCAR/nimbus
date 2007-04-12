/*
-------------------------------------------------------------------------
OBJECT NAME:	CanvasWindow.h

COPYRIGHT:	University Corporation for Atmospheric Research, 2007
-------------------------------------------------------------------------
*/

#ifndef _CanvasWindow_h_
#define _CanvasWindow_h_

#include <qmainwindow.h>

class QApplication;
class DataPlot;

/* -------------------------------------------------------------------- */
class CanvasWindow : public QMainWindow
{
  Q_OBJECT

public:
	CanvasWindow(QApplication *, const char * file_name);

protected slots:
  void	Print();

private:
  FILE * _fp;
  DataPlot * _plot;

};

#endif
