/*
-------------------------------------------------------------------------
OBJECT NAME:	rtplot.c

FULL NAME:	Real Time Plot (Counts & Volts).

TYPE:		X11/R5 Motif 1.2

DESCRIPTION:	See man page

INPUT:		Command line options

COPYRIGHT:	University Corporation for Atmospheric Research, 1994-9
-------------------------------------------------------------------------
*/

#include "define.h"
#include "decode.h"
#include "fbr.h"
#include <raf/Application.h>
#include <raf/Canvas.h>
#include <raf/CanvasWindow.h>
#include <raf/XmError.h>

Application     *application;
Canvas          *canvas;


Widget	CreateMainWindow(Widget);
void	Initialize(), process_args(char **argv), AllocateDataArrays(),
	RealTimeLoop();

bool	RawData = true;


/* --------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int	n;
  Arg	args[6];

  Initialize();
  process_args(argv);

  DecodeHeader(NULL);
  AllocateDataArrays();
  printf("Header decoded\n"); fflush(stdout);

  application = new Application("XmNCpp", &argc, argv, fallback_resources);

  CanvasWindow  canvasWindow(application->Shell());
  canvasWindow.PopUp();

  canvas = new Canvas(canvasWindow.DrawingArea());
  varList = canvasWindow.List();

  PlotData((Widget)NULL, (XtPointer)NULL, (XmDrawingAreaCallbackStruct*)NULL);
  RealTimeLoop();

  return(0);

}	/* END MAIN */

/* --------------------------------------------------------------------- */
void ErrorMsg(char msg[])
{
  new XmError(application->Shell(), msg);

}       /* END ERRORMSG */

/* END RTPLOT.CC */
