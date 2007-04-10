#include "DataPlot.h"

#include <qevent.h>
#include <qpixmap.h>
#include <qpainter.h>

#include <cstdio>
#include <unistd.h>

#include <nidas/linux/usbtwod/usbtwod.h>

typedef struct
{
  dsm_sample_time_t timetag;
  dsm_sample_length_t length;
  unsigned char data[4096];
} usb2d_rec;

usb2d_rec twod_rec;


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
void DataPlot::plot()
{   
  unsigned long long * p = (unsigned long long *)twod_rec.data;
  unsigned long long slice;
  size_t	y = 10;

  QPainter _painter(this);
  QPen _pen(red, 1);
//  QPixmap _pix(525, 300);
//  _pix = QPixmap::grabWidget(this);

  fseek(_fp, 314, 0);

  fread(&twod_rec, sizeof(usb2d_rec), 1, _fp);
  printf("%ld %lu\n", twod_rec.timetag, twod_rec.length);

  size_t cnt = 0;
  QPointArray pts(64*512);
  for (int i = 0; i < 512; ++i)
  {
    slice = p[i];

    for (int j = 0; j < 64; ++j)
    {
      if ( !((slice >> j) & 0x00000001) )
      {
        pts.setPoint(cnt, i + 10, y + (63-j));
        ++cnt;
      }
    }
  }

printf("cnt = %d\n", cnt);
  _painter.drawPoints(pts, 0, cnt);
//  _painter.drawPixmap(0, 0, _pix);
  show();



}
