/*
-------------------------------------------------------------------------
OBJECT NAME:	CanvasWindow.h

FULL NAME:	Create Main Canvas Window

TYPE:		

DESCRIPTION:	

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1997-9
-------------------------------------------------------------------------
*/

#ifndef CANWINDOW_H
#define CANWINDOW_H

#include "define.h"

#include "Window.h"

#include <Xm/CascadeB.h>
#include <Xm/DrawingA.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/List.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Separator.h>


/* -------------------------------------------------------------------- */
class CanvasWindow : public WinForm {

public:
		CanvasWindow(const Widget parent);

  Widget	DrawingArea()	{ return(drawA); }
  Widget	List()		{ return(varList); }


private:
  Widget	drawA, varList;

};	/* END CANVASWINDOW.H */

#endif
