/*
-------------------------------------------------------------------------
OBJECT NAME:	usb2d.c

COPYRIGHT:	University Corporation for Atmospheric Research, 2007
-------------------------------------------------------------------------
*/

#include "CanvasWindow.h"

#include <qapplication.h>

CanvasWindow	*canvasWindow;

/* --------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  canvasWindow = new CanvasWindow(&app);
  canvasWindow->show();

  app.setMainWidget(canvasWindow);
  canvasWindow->setCaption("usb2d");
  canvasWindow->resize(900, 525);

  app.exec();

  return(0);

}
