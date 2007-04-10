/*
-------------------------------------------------------------------------
OBJECT NAME:	XColorMap.cc

FULL NAME:	X Color map values.

ENTRY POINTS:	XColorMap()

STATIC FNS:		

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1997
-------------------------------------------------------------------------
*/

#include "XColorMap.h"
#include "temperature.h"

#include <cstdio>
#include <cstdlib>
 
/* -------------------------------------------------------------------- */
XColorMap::XColorMap(const Widget w)
{
  int		i, screenNum;
  Colormap      defaultCMap;
  Display	*dpy;
  XColor	exactDef;

  dpy = XtDisplay(w);
  screenNum = XScreenNumberOfScreen(XtScreen(w));

  if (DefaultDepth(dpy, screenNum) == 1)
    {
    fprintf(stderr, "XColorMap: No color available.\n");
    exit(1);
    }


  defaultCMap = DefaultColormap(dpy, screenNum);

  exactDef.flags = DoRed | DoGreen | DoBlue;

  for (i = 0; i < N_COLORS; ++i)
    {
    exactDef.red = rgbs[i].r;
    exactDef.green = rgbs[i].g;
    exactDef.blue = rgbs[i].b;

    if (!XAllocColor(dpy, defaultCMap, &exactDef))
      {
      fprintf(stderr, "Couldn't allocate color, #%d\n", i);
      exit(1);
      }

    pixel[i] = exactDef.pixel;
    }

}	/* END CONSTRUCTOR */

/* END XCOLORMAP.CC */
