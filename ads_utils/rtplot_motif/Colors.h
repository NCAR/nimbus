/*
-------------------------------------------------------------------------
OBJECT NAME:	Colors.h

FULL NAME:	Colors

TYPE:		

DESCRIPTION:	Provide a series of Colors that are always the same for
		both X & PostScript.

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1997-8
-------------------------------------------------------------------------
*/

#ifndef COLORS_H
#define COLORS_H

#include "define.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>


/* -------------------------------------------------------------------- */
class Colors {

public:
	Colors(const Widget w);

  void	ResetColors();
  ulong	GetColor(int indx);
  ulong	NextColor();
  ulong	CurrentColor();

  float	*GetColorPS(int indx);
  float	*NextColorPS();
  float	*CurrentColorPS();

private:
  int	colorIndex;
  bool	Color;

};	/* END COLORS.H */

#endif
