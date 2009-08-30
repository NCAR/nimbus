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
int     Mode    = Config::RealTime;

extern int timeIndex[];

/* --------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  Initialize();
  process_args(argv);

  DecodeHeader();
  AllocateDataArrays();
  timeIndex[0] = raw[SearchTable(raw, "HOUR")]->SRstart;
  timeIndex[1] = raw[SearchTable(raw, "MINUTE")]->SRstart;
  timeIndex[2] = raw[SearchTable(raw, "SECOND")]->SRstart;

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
