/*
-------------------------------------------------------------------------
OBJECT NAME:	rtplot.c

FULL NAME:	Real Time Plot (Counts & Volts).

TYPE:		X11/R6 Qt

DESCRIPTION:	See man page

INPUT:		Command line options

COPYRIGHT:	University Corporation for Atmospheric Research, 1994-9
-------------------------------------------------------------------------
*/

//#include "define.h"
//#include "decode.h"
#include "CanvasWindow.h"

#include <qapplication.h>

CanvasWindow	*canvasWindow;

void	Initialize(), process_args(char **argv), AllocateDataArrays(),
	RealTimeLoop();

bool	RawData = true;
int     Mode    = REALTIME;


/* --------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  Initialize();
  process_args(argv);

  DecodeHeader();
  AllocateDataArrays();
  printf("Header decoded\n"); fflush(stdout);

  QApplication app(argc, argv);

  canvasWindow = new CanvasWindow(&app);
  canvasWindow->show();

//  varList = canvasWindow->List();

//  PlotData((Widget)NULL, (XtPointer)NULL, (XmDrawingAreaCallbackStruct*)NULL);

  app.setMainWidget(canvasWindow);
  canvasWindow->setCaption("rtplot 2.0");
  canvasWindow->resize(900, 525);

  app.exec();

  return(0);

}	/* END MAIN */

/* END RTPLOT.CC */
