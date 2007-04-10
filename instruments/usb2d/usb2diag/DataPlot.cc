#include "DataPlot.h"

#include <qevent.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <qpaintdevice.h>

#include <cstdio>
#include <unistd.h>


/* -------------------------------------------------------------------- */
DataPlot::DataPlot(QWidget * parent, FILE * fp) : QWidget(parent), _fp(fp), _freeze(false)
{

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
  usb2d_rec twod_rec;

  size_t	y = 10;

  fseek(_fp, 314, 0);

  for (int i = 0; i < 7; ++i)
  {
    for (int j = 0; j < 2; ++j)
    {
      fread(&twod_rec, sizeof(usb2d_rec), 1, _fp);
      printf("%ld %lu\n", twod_rec.timetag, twod_rec.length);
      displayRecord(10 + 512 * j, y, twod_rec);
    }
    y += 75;
  }
}

/* -------------------------------------------------------------------- */
void DataPlot::displayRecord(int x, int y, usb2d_rec & rec)
{
  unsigned long long * p = (unsigned long long *)rec.data;
  unsigned long long slice;

  QPainter _painter(this);
  _painter.setPen(Qt::red);
  _painter.setBrush(Qt::red);

  size_t cnt = 0;
  QPointArray pts(64*512);

  for (int i = 0; i < 512; ++i)
  {
    slice = p[i];

    for (int j = 0; j < 64; ++j)
    {
      if ( !((slice >> j) & 0x00000001) )
      {
        pts.setPoint(cnt, i + x + 10, y + (63-j));
        ++cnt;
      }
    }
  }
  _painter.drawPoints(pts, 0, cnt);
}
