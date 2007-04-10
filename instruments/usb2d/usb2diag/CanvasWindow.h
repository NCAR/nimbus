/*
-------------------------------------------------------------------------
OBJECT NAME:	CanvasWindow.h

COPYRIGHT:	University Corporation for Atmospheric Research, 2007
-------------------------------------------------------------------------
*/

#ifndef CANWINDOW_H
#define CANWINDOW_H

#include <qmainwindow.h>

#include "define.h"

class QApplication;
class DataPlot;

/* -------------------------------------------------------------------- */
class CanvasWindow : public QMainWindow
{
  Q_OBJECT

public:
	CanvasWindow(QApplication *);

protected slots:
  void	Print();

private:
  FILE * _fp;
  DataPlot * _plot;

};

#endif
