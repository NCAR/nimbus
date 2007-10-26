/*
-------------------------------------------------------------------------
OBJECT NAME:	MainCanvas.h

FULL NAME:	Main canvas

COPYRIGHT:	University Corporation for Atmospheric Research, 1997-2006
-------------------------------------------------------------------------
*/

#ifndef MAINCANVAS_H
#define MAINCANVAS_H

#include <define.h>
#include <raf/header.h>

#include <raf/Canvas.h>
#include <DataFile.h>
#include <raf/PostScript.h>

#include <sys/types.h>
#include <netinet/in.h>


/* -------------------------------------------------------------------- */
class MainCanvas : public Canvas {

public:
	MainCanvas(Widget w);

  void	SetDisplayMode(int mode);
  void	SetWrapDisplay()	{ wrap = 1 - wrap; }
  void	SetTimingWords()	{ timingWord = 1 - timingWord; }

  void	reset(ADS_DataFile *file);
  void	draw(P2d_rec *record, struct recStats &stats, float hdrVer, int probeNum, PostScript *ps);
  int	maxRecords()		{ return(maxRecs); }
  int	SpaceAvailable()	{ return(Height() - y); }


private:
  void	drawSlice(PostScript *ps, int i, unsigned long slice);
  void	enchiladaLineItem(PostScript *ps, int i, int cnt, Particle *cp);

  int	y, maxRecs;
  int	displayMode;
  int	PIX_PER_Y;

  bool	wrap,		// Wrap display around (HVPS mostly)
	timingWord;	// Toggle timing words on/off.

};	/* END MAINCANVAS.H */

#endif
