#include "DataPlot.h"

#include <qevent.h>
#include <qpainter.h>

#include <cstdio>
#include <unistd.h>

const unsigned long long DataPlot::_syncWord = 0xAAAAAAA000000000LL;
const unsigned long long DataPlot::_syncMask = 0xFFFFFFF000000000LL;


/* -------------------------------------------------------------------- */
DataPlot::DataPlot(QWidget * parent, FILE * fp) : QWidget(parent), _fp(fp), _freeze(false)
{
  // Position file pointer at end of nidas header.
  char		buff[4096];

  rewind(_fp);
  fread(buff, 4096, 1, _fp);
  char * p = strstr(buff, "end header\n");
  if (p == 0)
    printf("Can't locate 'end header'\n");

  fseek(_fp, (p - buff) + 11, 0);


//  (void)startTimer(10);
}

/* -------------------------------------------------------------------- */
void DataPlot::ToggleFreeze()
{
  _freeze = 1 - _freeze;

}

/* -------------------------------------------------------------------- */
void DataPlot::timerEvent(QTimerEvent *)
{   
  if (_freeze)
    return;    

  // Check data file for new data (real-time mode).



  plot();
}

/* -------------------------------------------------------------------- */
void DataPlot::paintEvent(QPaintEvent * e)
{
  QWidget::paintEvent(e);
  plot();
}

/* -------------------------------------------------------------------- */
void DataPlot::plot()
{
  nidas_hdr	hdr;
  usb2d_rec	twod_rec;
  size_t	x, y = 10;

  for (int i = 0; i < 7; ++i)
  {
    x = 5;
    for (int j = 0; j < 2; ++j)
    {
      fread(&hdr, sizeof(nidas_hdr), 1, _fp);
      fread(&twod_rec, hdr.length, 1, _fp);
      printf("%llu %lu %lu %lx %lx\n", hdr.timetag, hdr.length,
		hdr.sample_id, twod_rec.tas, twod_rec.id);
      displayRecord64(x, y, (unsigned long long *)twod_rec.data);
      x += 513;
    }
    y += 75;
  }
}

/* -------------------------------------------------------------------- */
void DataPlot::displayRecord64(int start_x, int start_y, unsigned long long * data_p)
{
  QPainter _painter(this);
  _painter.setPen(Qt::blue);

  size_t cnt = 0;
  QPointArray pts(64*512);

  for (int i = 0; i < 512; ++i)
  {
    unsigned long long slice = flipLonglong(data_p[i]);

//    if ((slice & _syncMask) == _syncWord)
//      printf("%llu\n", slice & ~_syncMask);	// Print timing word.

    for (int j = 0; j < 64; ++j)
    {
      if ( !((slice >> j) & 0x00000001) )
      {
        pts.setPoint(cnt, start_x + i, start_y + (63-j));
        ++cnt;
      }
    }
  }
  _painter.drawPoints(pts, 0, cnt);
}
