/*
-------------------------------------------------------------------------
OBJECT NAME:	CanvasWindow.h

FULL NAME:	Create Main Canvas Window

TYPE:		

DESCRIPTION:	

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1998
-------------------------------------------------------------------------
*/

#ifndef CANWINDOW_H
#define CANWINDOW_H

#include "mcrtd.h"

#include <raf/Window.h>

#include <Xm/CascadeB.h>
#include <Xm/DrawingA.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Separator.h>
#include <Xm/TextF.h>


/* -------------------------------------------------------------------- */
class CanvasWindow : public WinForm {

public:
		CanvasWindow(const Widget parent);

  Widget	DrawingArea()	{ return(drawA); }
  void		SetStartVolt(float v);
  void		SetEndVolt(float v);

  float		GetStartVolt();
  float		GetEndVolt();

private:
  Widget	drawA;
  Widget	startVoltW, endVoltW;

};	/* END CANVASWINDOW.H */

#endif
