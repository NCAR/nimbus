/*
-------------------------------------------------------------------------
OBJECT NAME:	XColorMap.h

FULL NAME:	X Color map

TYPE:		

DESCRIPTION:	Provide a series of Colors that are always the same.

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1997
-------------------------------------------------------------------------
*/

#ifndef XCOLORMAP_H
#define XCOLORMAP_H

#include <Xm/Xm.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#define N_COLORS	120


/* -------------------------------------------------------------------- */
class XColorMap {

public:
	XColorMap(const Widget w);

  unsigned long	pixel[N_COLORS];

private:

};	/* END XCOLORMAP.H */

#endif
