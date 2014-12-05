/*
-------------------------------------------------------------------------
OBJECT NAME:	MainCanvas.h

FULL NAME:	Main canvas

COPYRIGHT:	University Corporation for Atmospheric Research, 1997-2014
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
  size_t maxRecords() const	{ return(maxRecs); }
  int	SpaceAvailable() const	{ return(Height() - y); }


protected:
  void	drawPMS2D(P2d_rec *record, struct recStats &stats, float hdrVer, int probeNum, PostScript *ps);
  void	drawHVPS(P2d_rec *record, struct recStats &stats, float hdrVer, int probeNum, PostScript *ps);
  void	drawFast2D(P2d_rec *record, struct recStats &stats, float hdrVer, int probeNum, PostScript *ps);
  void	drawCIP(P2d_rec *record, struct recStats &stats, float hdrVer, int probeNum, PostScript *ps);

  void	drawSlice(PostScript *ps, int i, uint32_t slice);
  void	drawSlice(PostScript *ps, int i, unsigned long long slice);
  void	enchiladaLineItem(PostScript *ps, int i, int cnt, Particle *cp);

  /**
   * Count all shadowed diodes across the flight track and display histogram.
   */
  void drawDiodeHistogram(P2d_rec *record);

  void drawAccumHistogram(struct recStats &stats);

  size_t uncompressCIP(unsigned char *dest, const unsigned char src[], int nbytes);

  int	y, maxRecs;
  int	displayMode;
  int	PIX_PER_Y;

  bool	wrap,		// Wrap display around (HVPS mostly)
	timingWord;	// Toggle timing words on/off.

};	/* END MAINCANVAS.H */

#endif
