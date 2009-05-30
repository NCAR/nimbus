/*
-------------------------------------------------------------------------
OBJECT NAME:    MainCanvas.cc
 
FULL NAME:      Main canvas
 
COPYRIGHT:	University Corporation for Atmospheric Research, 1997-2006
-------------------------------------------------------------------------
*/

#include "MainCanvas.h"
#include "Enchilada.h"
#include "Histogram.h"
#include "Colors.h"
#include <raf/XFonts.h>
#include <raf/XPen.h>

extern Enchilada	*enchiladaWin; 
extern Histogram	*histogramWin; 
extern Colors	*color;
extern XFonts	*fonts;
extern XPen	*pen;

#include <algorithm>
#include <sstream>

static const int	TOP_OFFSET = 35;
static const int	LEFT_MARGIN = 5;
// RAF HVPS, 40 pixels masked at each end of diode array.
static const int	HVPS_MASKED = 40;

static bool debug = false;

/* -------------------------------------------------------------------- */
MainCanvas::MainCanvas(Widget w) : Canvas(w)
{
  SetDisplayMode(NORMAL);

  wrap = false;
  timingWord = true;

  reset(NULL);

}       /* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void MainCanvas::SetDisplayMode(int mode)
{
  displayMode = mode;

  switch (displayMode)
    {
    case NORMAL:
    case DIAGNOSTIC:
      PIX_PER_Y = 70;
      break;

    case ENCHILADA:
      PIX_PER_Y = 600;
      break;
    }

  maxRecs = (Height() - TOP_OFFSET) / PIX_PER_Y;

}	/* END SETDISPLAYMODE */

/* -------------------------------------------------------------------- */
void MainCanvas::reset(ADS_DataFile *file)
{
  maxRecs = (Height() - TOP_OFFSET) / PIX_PER_Y;
  y = TOP_OFFSET;

  if (file)
    {
    std::stringstream title;
    title << file->ProjectNumber() << ", " << file->FlightNumber() << " - "
	<< file->FlightDate();

    pen->SetFont(fonts->Font(0));
    pen->DrawText(Surface(), 300, 25, title.str().c_str());
    }

}	/* END RESET */

/* -------------------------------------------------------------------- */
void MainCanvas::draw(P2d_rec *record, struct recStats &stats, float version, int probeNum, PostScript *ps)
{
  char		buffer[256];
  Particle	*cp;

  static short  prevOverLoad = 0;

//  if (record->msec == 397) debug = true;

  // Ok, now go back and display the data.
  pen->SetFont(fonts->Font(3));


  if (ps) ps->SetColor(color->GetColorPS(BLACK));
  else pen->SetColor(color->GetColor(BLACK));

  if (ps) { ps->MoveTo(35, 750-(y-2)); ps->LineTo(1060, 750-(y-2)); }
  else pen->DrawLine(Surface(), 0, y-2, Width(), y-2);

  if (ps) { ps->MoveTo(547, 750-(y-5)); ps->LineTo(547, 750-y); }
  else pen->DrawLine(Surface(), 523, y-5, 523, y);


  char * p = (char *)record;
  if (p[0] == 'C' && (p[1] == '4' || p[1] == '6'))	// 64 bit fast 2DC.
    drawFast2DC(record, stats, version, probeNum, ps);
  else
  if (((char *)&record->id)[0] == 'H')	// HVPS
    drawHVPS(record, stats, version, probeNum, ps);
  else
    drawPMS2D(record, stats, version, probeNum, ps);


 
  if (stats.duplicate)
    {
    strcpy(buffer, " DUPLICATE RECORD! ");
    if (ps)
      {
      ps->SetColor(color->GetColorPS(RED));
      ps->MoveTo(10, 750-(y+15)); ps->ShowStr(buffer);
      ps->MoveTo(300, 750-(y+15)); ps->ShowStr(buffer);
      ps->MoveTo(600, 750-(y+15)); ps->ShowStr(buffer);
      }
    else
      {
      pen->SetColor(color->GetColor(RED));
      pen->DrawText(Surface(), 10, y+15, buffer);
      pen->DrawText(Surface(), 300, y+15, buffer);
      pen->DrawText(Surface(), 600, y+15, buffer);
      }
    }

  // Draw text info at bottom of record.
  if (ps) ps->SetColor(color->GetColorPS(BLACK));
  else pen->SetColor(color->GetColor(BLACK));

  if (stats.thisTime < stats.prevTime) {
    if (ps) ps->SetColor(color->GetColorPS(RED));
    else pen->SetColor(color->GetColor(RED));
    }

  sprintf(buffer, "%02d:%02d:%02d.%03d",
	record->hour, record->minute, record->second, record->msec);

  if (ps) {
    ps->MoveTo(200, 750-(y+50)); ps->ShowStr(buffer);
    ps->SetColor(color->GetColorPS(BLACK));
    }
  else {
    pen->DrawText(Surface(), 200, y+50, buffer);
    pen->SetColor(color->GetColor(BLACK));
    }

  sprintf(buffer, ", TAS=%5.1f, overLoad=", stats.tas);
  if (ps) { ps->MoveTo(283, 750-(y+50)); ps->ShowStr(buffer); }
  else pen->DrawText(Surface(), 283, y+50, buffer);

  if (prevOverLoad > 90) {
    if (ps) ps->SetColor(color->GetColorPS(RED));
    else pen->SetColor(color->GetColor(RED));
    }

  sprintf(buffer, "%.3f,", (float)prevOverLoad / 1000);
  if (ps) {
    ps->MoveTo(445, 750-(y+50)); ps->ShowStr(buffer);
    ps->SetColor(color->GetColorPS(BLACK));
    }
  else {
    pen->DrawText(Surface(), 445, y+50, buffer);
    pen->SetColor(color->GetColor(BLACK));
    }

  sprintf(buffer, "nParticles = %d, elapsed time = ", stats.nTimeBars);
  if (ps) { ps->MoveTo(490, 750-(y+50)); ps->ShowStr(buffer); }
  else pen->DrawText(Surface(), 490, y+50, buffer);

  if (stats.DASelapsedTime / 1000 > 36000) {
    if (ps) ps->SetColor(color->GetColorPS(RED));
    else pen->SetColor(color->GetColor(RED));

    sprintf(buffer, "%6ld", stats.DASelapsedTime / 1000);
    }
  else {
    sprintf(buffer, "%6.3f", (float)stats.DASelapsedTime / 1000);
    }

  if (ps) { ps->MoveTo(706, 750-(y+50)); ps->ShowStr(buffer); }
  else pen->DrawText(Surface(), 706, y+50, buffer);

  if (ps) ps->SetColor(color->GetColorPS(BLACK));
  else pen->SetColor(color->GetColor(BLACK));

  sprintf(buffer,", timeBarTotal = %6.3f", (float)stats.tBarElapsedtime / 1000);
  if (ps) { ps->MoveTo(750, 750-(y+50)); ps->ShowStr(buffer); }
  else pen->DrawText(Surface(), 750, y+50, buffer);


  if (stats.DASelapsedTime == 0.0)
    sprintf(buffer, "%.1f%% %.1f%%", 0.0, 0.0);
  else
    {
//    float  olap =	(float)(stats.tBarElapsedtime+record->overld) /
    float  olap =	(float)(stats.tBarElapsedtime+prevOverLoad) /
			stats.DASelapsedTime * 100;

    if (olap < 75.0 || olap > 125.0) {
      if (ps) ps->SetColor(color->GetColorPS(RED));
      else pen->SetColor(color->GetColor(RED));
      }

    sprintf(buffer, "%.1f%% %.1f%%",
	(float)stats.tBarElapsedtime / stats.DASelapsedTime * 100, olap);
    }

  if (ps) { ps->MoveTo(950, 750-(y+50)); ps->ShowStr(buffer); }
  else pen->DrawText(Surface(), 950, y+50, buffer);
  if (ps) ps->SetColor(color->GetColorPS(BLACK));
  else pen->SetColor(color->GetColor(BLACK));


{
  int nCols, colWidth;
  float		res2;

nCols = 5;
colWidth = Width() / nCols;

  res2 = (float)stats.resolution * stats.resolution / 1000000;

  switch (displayMode)
    {
    case ENCHILADA:
    case NORMAL:
      sprintf(buffer,
	"sv: act = %.2lfL, used = %.3lfL, area = %.2fmm2, conc = %.3lfN/L, lw = %.3lfg/M3, z = %.3lfdb",
	stats.SampleVolume / 1000, stats.DOFsampleVolume / 1000,
	res2 * stats.area, stats.concentration, stats.lwc, stats.dbz);

      if (ps) { ps->MoveTo(315, 750-(y+62)); ps->ShowStr(buffer); }
      else pen->DrawText(Surface(), 315, y+62, buffer);

      for (size_t i = 0; (cp = (Particle *)stats.particles.DeQueue()); ++i)
        delete cp;
//printf("Normal: %d unremoved particles\n", i);
      break;

    case DIAGNOSTIC:
      sprintf(buffer, "freq = %f,  mean=%lu min=%lu max=%lu",
	stats.frequency * 1000, stats.meanBar, stats.minBar, stats.maxBar);

      if (ps) { ps->MoveTo(505, 750-(y+62)); ps->ShowStr(buffer); }
      else pen->DrawText(Surface(), 505, y+62, buffer);

      for (size_t i = 0; (cp = (Particle *)stats.particles.DeQueue()) ; ++i)
        delete cp;
//printf("Diagnostic: %d unremoved particles\n", i);
      break;
    }
}

  prevOverLoad = record->overld;
  y += PIX_PER_Y;

  if (histogramWin)
    histogramWin->AddLineItem(record, stats);

}       /* END DRAW */
 
/* -------------------------------------------------------------------- */
void MainCanvas::drawPMS2D(P2d_rec * record, struct recStats &stats, float version, int probeNum, PostScript *ps)
{
  int		nextColor, cntr = 0;
  unsigned long *p, slice, pSlice, ppSlice, syncWord;
  bool		colorIsBlack = false;
  Particle	*cp;
  char		buffer[256];

  static unsigned long  prevSlice[2];
  static unsigned long  prevTime;
  static P2d_rec	prevRec;

  p = (unsigned long *)record->data;
  ppSlice = prevSlice[0];
  pSlice = prevSlice[1];

  if (memcmp((void *)&record, (void *)&prevRec, sizeof(P2d_rec)) == 0)
    stats.duplicate = true;

  if (version < 3.35)
    syncWord = SyncWordMask;
  else
    syncWord = StandardSyncWord;

  if (displayMode == RAW_RECORD)
    {
    p = (unsigned long *)record->data;
    for (size_t i = 0; i < RecordLen; ++i, ++p)		/* 2DC and/or 2DP	*/
      drawSlice(ps, i, *p);

    y += 34;
    }

  p = (unsigned long *)record->data;
  for (size_t i = 0; i < RecordLen; ++p)		/* 2DC and/or 2DP	*/
    {
    slice = *p;

    /* If sync word and word before is not -1, then sum up timing words
     * into milliseconds.
     */
    if (slice == syncWord && ppSlice == 0xffffffff && pSlice != 0xffffffff)
      {
      if ((cp = (Particle *)stats.particles.DeQueue()) == NULL)
        break;

if (debug) { if (cp) printf("dq: %06lx %lu %lu\n", cp->timeWord, cp->h, cp->w); else printf("NULL\n"); }

      unsigned long timeWord = (unsigned long)((float)cp->timeWord * stats.frequency);

      if (timeWord >= stats.DASelapsedTime)
        {
        sprintf(buffer, "%lu", timeWord);

        if (ps) {
          ps->SetColor(color->GetColorPS(RED));
          ps->MoveTo(i+10, 750-(y+(PIX_PER_Y-30)));
          ps->ShowStr(buffer);
          }
        else {
          pen->SetColor(color->GetColor(RED));
          pen->DrawText(Surface(), i+10, y+(PIX_PER_Y-30), buffer);
          }
        }


      // Draw timing & sync words in yellow (or some other color).
      if (cp && cp->reject) {
        if (cp->h == 1 && cp->w == 1)
          if (ps) ps->SetColor(color->GetColorPS(YELLOW));
          else pen->SetColor(color->GetColor(YELLOW));
        else
          if (ps) ps->SetColor(color->GetColorPS(RED));
          else pen->SetColor(color->GetColor(RED));

        nextColor = 0;
        }
      else {	// Green for good particle
        if (ps) ps->SetColor(color->GetColorPS(GREEN));
        else pen->SetColor(color->GetColor(GREEN));

        nextColor = probeNum;
        }

      if (timingWord)
        {
        drawSlice(ps, i++, pSlice);
        drawSlice(ps, i++, slice);
        }
      else
        i += 2;

      if (ps) ps->SetColor(color->GetColorPS(nextColor));
      else pen->SetColor(color->GetColor(nextColor));

      if (nextColor == 0)	// if Black
        colorIsBlack = true;
      else
        colorIsBlack = false;

      for (++p; i < RecordLen && *p != 0xffffffff; )
        drawSlice(ps, i++, *p++);

      if (enchiladaWin)
        enchiladaWin->AddLineItem(cntr++, cp);

      delete cp;
      }
    else
    if (p[1] != syncWord)
      {
      if (!colorIsBlack)
        {
        if (ps) ps->SetColor(color->GetColorPS(BLACK));
        else pen->SetColor(color->GetColor(BLACK));

        colorIsBlack = true;
        }

      drawSlice(ps, i++, slice);
      }
    else
      ++i;

    ppSlice = pSlice;
    pSlice = slice;
    }

  p = (unsigned long *)record->data;
  prevSlice[0] = p[1022];
  prevSlice[1] = p[1023];
  stats.prevTime = prevTime;
  prevTime = stats.thisTime;
  memcpy((void *)&prevRec, (void *)&record, sizeof(P2d_rec));
}

/* -------------------------------------------------------------------- */
void MainCanvas::drawHVPS(P2d_rec * record, struct recStats &stats, float version, int probeNum, PostScript *ps)
{
  size_t	y1 = 0, cntr = 0, shaded, unshaded, line = LEFT_MARGIN;
  unsigned short	*sp = (unsigned short *)record->data;
  Particle	*cp;

  static unsigned long prevTime;
  static P2d_rec	prevRec;

  if (memcmp((void *)&record, (void *)&prevRec, sizeof(P2d_rec)) == 0)
    stats.duplicate = true;

  if (*sp == 0xcaaa)
    {
//      printf("MainCanvas:draw: diagnostic record.\n");
    return;
    }

//printf("--------------------------------\n");
  pen->SetColor(color->GetColor(RED));
//    pen->DrawLine(Surface(), line, y+HVPS_MASKED, 1600, y+HVPS_MASKED);
  pen->DrawLine(Surface(), line, y+(128-HVPS_MASKED), 1600, y+(128-HVPS_MASKED));
//    pen->DrawLine(Surface(), line, y+256-HVPS_MASKED, 1600, y+256-HVPS_MASKED);

  for (size_t i = 0; i < 2048; )
    {
    if (wrap && line >= Width()-(LEFT_MARGIN<<1))
      {
      line = LEFT_MARGIN;
      y += 190 + PIX_PER_Y - (HVPS_MASKED<<1);

      if (ps) {
        ps->SetColor(color->GetColorPS(RED));
        }
      else {
        pen->SetColor(color->GetColor(RED));
        }

      pen->DrawLine(Surface(), line, y+(128-HVPS_MASKED), line+1500, y+(128-HVPS_MASKED));
      }

    if (sp[i] & 0x8000)
      {
//      if (!(sp[i+1] & 0x8000))
//        printf("Missaligned timing words.\n");

      if ((cp = (Particle *)stats.particles.DeQueue()) == NULL)
        break;

      line += 2;

      if (timingWord)
        {
        if (cp && cp->reject) {
          if (ps) ps->SetColor(color->GetColorPS(RED));
          else pen->SetColor(color->GetColor(RED));
          }
        else {
          if (ps) ps->SetColor(color->GetColorPS(GREEN));
          else pen->SetColor(color->GetColor(GREEN));
          }

        pen->DrawLine(Surface(), line, y, line, y+256-(HVPS_MASKED<<1));
        }

      if (ps) ps->SetColor(color->GetColorPS(BLUE));
      else pen->SetColor(color->GetColor(BLUE));

//printf("\ntiming: %d, ovrld=%x - %x", (((unsigned long)sp[i] << 15) & 0x3fff0000)+(unsigned long)(sp[i+1] & 0x3fff), sp[i] & 0xc000, sp[i+1] & 0xc000);

      i += 2;

      for (; !(sp[i] & 0x8000) && i < 2048; ++i)
        {
        shaded = (sp[i] & 0x3f80) >> 7;
        unshaded = sp[i] & 0x007f;

        if (sp[i] & 0x4000)
          {
          y1 = y;

          if (sp[i] == 0x4000)
            y1 += 128;

          line += 1;	// Max airspeed is 45 m/s.  Cheesy multiply by 2.
          unshaded -= HVPS_MASKED;
//printf("\n %4d", i);
          }

//if (sp[i] == 0x4000)
//  printf(" 128   0");
//else
//  printf(" %3d %3d", unshaded, shaded);

        if (shaded > 0)
          {	// Max airspeed is 45 m/s.  Cheesy multiply by 2.  Draw lines twice.
          pen->DrawLine(Surface(), line, y1 + unshaded,
			line, y1 + unshaded + shaded);
//          pen->DrawLine(Surface(), line + 1, y1 + unshaded,
//			line + 1, y1 + unshaded + shaded);
          }

        y1 += unshaded + shaded;
        }

      if (enchiladaWin)
        enchiladaWin->AddLineItem(cntr++, cp);

      delete cp;
      }
    else
      ++i;
    }

  y += 224 - (HVPS_MASKED<<1);
  stats.prevTime = prevTime;
  prevTime = stats.thisTime;
  memcpy((void *)&prevRec, (void *)&record, sizeof(P2d_rec));
}

/* -------------------------------------------------------------------- */
void MainCanvas::drawFast2DC(P2d_rec * record, struct recStats &stats, float version, int probeNum, PostScript *ps)
{
  Particle	*cp;
  int		nextColor, cntr = 0;
  bool		colorIsBlack = false;
  unsigned long long *p = (unsigned long long *)record->data;

  static unsigned long prevTime;
  static P2d_rec prevRec;

  if (memcmp((void *)&record, (void *)&prevRec, sizeof(P2d_rec)) == 0)
    stats.duplicate = true;

  if ((cp = (Particle *)stats.particles.Front()) == NULL)
    return;

  for (size_t i = 0; i < 512; )
  {
    if (cp == 0 || cp->reject)
      nextColor = 0;	// black.
    else
      nextColor = probeNum;

    if ((*p & Fast2DC_Mask) == Fast2DC_Sync)
    {
      // Draw timing & sync words in yellow (or some other color).
      if (cp && cp->reject) {
        if (cp->h == 0 || cp->w == 0)
          if (ps) ps->SetColor(color->GetColorPS(YELLOW));
          else pen->SetColor(color->GetColor(YELLOW));
        else
          if (ps) ps->SetColor(color->GetColorPS(RED));
          else pen->SetColor(color->GetColor(RED));
      }
      else {	// Green for good particle
        if (ps) ps->SetColor(color->GetColorPS(GREEN));
        else pen->SetColor(color->GetColor(GREEN));
      }

      drawSlice(ps, i++, *p++);

      // Get next particle.
      cp = (Particle *)stats.particles.DeQueue();
      delete cp;
      cp = (Particle *)stats.particles.Front();
    }
    else
    {
      if (ps) ps->SetColor(color->GetColorPS(nextColor));
      else pen->SetColor(color->GetColor(nextColor));

      if (nextColor == 0)	// if Black
        colorIsBlack = true;
      else
        colorIsBlack = false;

      for (; i < 512 && (*p & Fast2DC_Mask) != Fast2DC_Sync; ++p)
        drawSlice(ps, i++, *p);

      if (enchiladaWin)
        enchiladaWin->AddLineItem(cntr++, cp);
    }
  }

/*
// For diagnostics, display record as is on 2nd half of screen/window.
  for (size_t i = 0; i < 512; ++i)
{ if (((unsigned long long *)record->data)[i] == 0xffffffffffffffffLL) { pen->SetColor(color->GetColor(YELLOW)); ((unsigned long long *)record->data)[i] = 0; }
    drawSlice(ps, 512+i, ((unsigned long long *)record->data)[i]);
pen->SetColor(color->GetColor(0)); }
*/

  y += 32;
  stats.prevTime = prevTime;
  prevTime = stats.thisTime;
  memcpy((void *)&prevRec, (void *)&record, sizeof(P2d_rec));
}

/* -------------------------------------------------------------------- */
void MainCanvas::enchiladaLineItem(PostScript *ps, int i, int cnt,
	Particle *cp)
{
  int	h, m, s;

  if (ps) ps->SetColor(color->GetColorPS(BLACK));
  else pen->SetColor(color->GetColor(BLACK));

  if (cnt == 0)	// Print title.
    {
    strcpy(buffer, " #     Time       timeWord  rj  iy  ix  ia    dt   live");

    if (ps) {
       ps->MoveTo(5, 750); ps->ShowStr(buffer); }
    else
       pen->DrawText(Surface(), 5, 120, buffer);
    }


  h = cp->time / 3600;
  m = (cp->time - (h*3600)) / 60;
  s = cp->time - (h*3600) - (m*60);

  // Particle #, time stamp, timeword, reject, h, w, a
  sprintf(buffer, "%03d %02d:%02d:%02d.%03ld  %8ld %2d %3d %3d %3d %6ld %6ld",
	cnt, h, m, s, cp->msec, cp->timeWord, cp->reject, cp->h, cp->w,
	cp->area, cp->deltaTime, cp->liveTime);

  if (ps) {
    ps->MoveTo(5, 750-(y+80+(cnt*15))); ps->ShowStr(buffer); }
  else
    pen->DrawText(Surface(), 5, 137+(cnt*15), buffer);
}

/* -------------------------------------------------------------------- */
void MainCanvas::drawSlice(PostScript *ps, int i, unsigned long slice)
{
  if (slice == 0xffffffff)
    return;

  size_t	cnt = 0;
  XPoint	pts[32];

  if (ps)
    {
    size_t first;

    ps->MoveTo(i+37, 750-y-32);

    if (slice == 0x00000000)
      ps->rLineTo(0, 32);
    else
    for (size_t j = 0; j < 32; )
      {
      cnt = 1;
      first = (slice >> j) & 0x00000001;

      while (((slice >> ++j) & 0x00000001) == first && j < 32)
        ++cnt;

      if (first)
        ps->rMoveTo(0, cnt);
      else
        ps->rLineTo(0, cnt);
      }
    }
  else
    {
    for (size_t j = 0; j < 32; ++j)
      if ( !((slice >> j) & 0x00000001) )
        {
        pts[cnt].x = i + 12;
        pts[cnt].y = y + (31-j);
        ++cnt;
        }

    pen->DrawPoints(Surface(), pts, cnt);
    }
}

/* -------------------------------------------------------------------- */
void MainCanvas::drawSlice(PostScript *ps, int i, unsigned long long slice)
{
//  if (slice == 0xffffffffffffffffLL)
//    return;

  size_t	cnt = 0;
  XPoint	pts[64];

  if (ps)
    {
    size_t first;

    ps->MoveTo(i+37, 750-y-64);

    if (slice == 0x00000000)
      ps->rLineTo(0, 64);
    else
    for (size_t j = 0; j < 64; )
      {
      cnt = 1;
      first = (slice >> j) & 0x00000001;

      while (((slice >> ++j) & 0x00000001) == first && j < 64)
        ++cnt;

      if (first)
        ps->rMoveTo(0, cnt);
      else
        ps->rLineTo(0, cnt);
      }
    }
  else
    {
    for (size_t j = 0; j < 64; ++j)
      if ( !((slice >> j) & 0x00000001) )
        {
        pts[cnt].x = i + 12;
        pts[cnt].y = y + (63-j);
        ++cnt;
        }

    pen->DrawPoints(Surface(), pts, cnt);
    }
}

/* END MAINCANVAS.CC */
