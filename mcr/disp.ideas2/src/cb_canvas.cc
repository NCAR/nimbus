/*
-------------------------------------------------------------------------
OBJECT NAME:	cb_canvas.cc

FULL NAME:	Callback Wrappers for the main canvas.

ENTRY POINTS:	CanvasExpose()
		CanvasResize()
		SetChannel()
		PrintSave()
		SetStartVoltage()
		SetEndVoltage()
		Quit()

STATIC FNS:	

DESCRIPTION:	

REFERENCES:	

REFERENCED BY:	XtAppMainLoop()

COPYRIGHT:	University Corporation for Atmospheric Research, 1997
-------------------------------------------------------------------------
*/

#include "mcrtd.h"
#include <time.h>

#include "Canvas.h"
#include "CanvasWindow.h"
#include "DataBuffer.h"
#include "XmFile.h"

extern Canvas		*mainCanvas;
extern CanvasWindow	*canvasWindow;
extern DataBuffer	*proc;
extern XmFile		*file;

extern bool	Frozen;


/* -------------------------------------------------------------------- */
void CanvasExpose(Widget w, XtPointer client, XtPointer call)
{
  mainCanvas->ExposeArea((XmDrawingAreaCallbackStruct *)call);

}	/* END CANVASEXPOSE */

/* -------------------------------------------------------------------- */
void CanvasResize(Widget w, XtPointer client, XtPointer call)
{
  mainCanvas->Resize();

}	/* END CANVASRESIZE */

/* -------------------------------------------------------------------- */
void Freeze(Widget w, XtPointer client, XtPointer call)
{
  Frozen = 1 - Frozen;
printf("Freeze toggled\n");
}	/* END FREEZE */

/* -------------------------------------------------------------------- */
void Record(Widget w, XtPointer client, XtPointer call)
{
  char		dataFile[256];
  time_t	t = time(NULL);
  struct tm	*now;

  now = localtime(&t);
  sprintf(dataFile, "/jnet/data/%02d%02d%02d%02d", now->tm_mday,
	now->tm_hour, now->tm_min, now->tm_sec);

  proc->StartRecording(dataFile);

}	/* END RECORD */

/* -------------------------------------------------------------------- */
void SetChannel(Widget w, XtPointer client, XtPointer call)
{
  proc->SetChannel((int)client);
  canvasWindow->SetStartVolt(proc->GetStartVoltage());
  canvasWindow->SetEndVolt(proc->GetEndVoltage());
printf("New channel = %d\n", (int)client);
}	/* END SETCHANNEL */

/* -------------------------------------------------------------------- */
void SetStartVoltage(Widget w, XtPointer client, XtPointer call)
{
  proc->SetStartVoltage(canvasWindow->GetStartVolt());
  canvasWindow->SetStartVolt(proc->GetStartVoltage());

}	/* END SETSTARTVOLTAGE */

/* -------------------------------------------------------------------- */
void SetEndVoltage(Widget w, XtPointer client, XtPointer call)
{
  proc->SetEndVoltage(canvasWindow->GetEndVolt());
  canvasWindow->SetEndVolt(proc->GetEndVoltage());

}	/* END SETENDVOLTAGE */

/* -------------------------------------------------------------------- */
void PrintSave(Widget w, XtPointer client, XtPointer call)
{

}	/* END PRINTSAVE */

/* -------------------------------------------------------------------- */
void Quit(Widget w, XtPointer client, XtPointer call)
{
  exit(0);

}	/* END QUIT */

/* END CB_CANVAS.CC */
