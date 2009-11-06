/*
-------------------------------------------------------------------------
OBJECT NAME:	Colors.h

FULL NAME:	Colors

DESCRIPTION:	Provide a series of Colors that are always the same for
		both X & PostScript.

COPYRIGHT:	University Corporation for Atmospheric Research, 1997-2000
-------------------------------------------------------------------------
*/

#ifndef COLORS_H
#define COLORS_H

#include <define.h>

#define BLACK		0
#define MAROON		1
#define BLUE		2
#define RED		3
#define GREEN		4
#define VIOLET		5
#define PURPLE		6
#define YELLOW		7
#define BACKGROUND	8

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#ifdef PNG
#include <png.h>
#endif

/* -------------------------------------------------------------------- */
class Colors {

public:
	Colors(const Widget w);

  int	NumberOfColors()	{ return(numberColors); }
  void	ResetColors()		{ colorIndex = 0; }

  unsigned long	GetColor(int indx);
  unsigned long	NextColor();
  unsigned long	CurrentColor();

  float	*GetColorPS(int indx);
  float	*NextColorPS();
  float	*CurrentColorPS();

#ifdef PNG
  void	SavePNG(const char outFile[], XImage *image);
  unsigned short	*GetColorRGB_X(int idx);
#endif


private:
#ifdef PNG
  void	checkByteSwap(XImage *image);
  int	getColorIndex(unsigned long pixel);
#endif

  int	colorIndex, numberColors;
  bool	Color;

};	/* END COLORS.H */

#endif
