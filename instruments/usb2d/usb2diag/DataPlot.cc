#include "DataPlot.h"

#include <qevent.h>
#include <qpainter.h>

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
  usb2d_rec twod_rec;

  size_t	y = 10;

  fseek(_fp, 310, 0);

  for (int i = 0; i < 7; ++i)
  {
    for (int j = 0; j < 2; ++j)
    {
      fread(&twod_rec, sizeof(usb2d_rec), 1, _fp);
      printf("%llu %lu %lu\n", twod_rec.timetag, twod_rec.length, twod_rec.id);
      displayRecord64(512 * j, y, (unsigned long long *)twod_rec.data);
    }
    y += 75;
  }
}

/* -------------------------------------------------------------------- */
void DataPlot::displayRecord64(int start_x, int start_y, unsigned long long * data_p)
{
  QPainter _painter(this);
  _painter.setPen(Qt::red);

  size_t cnt = 0;
  QPointArray pts(64*512);

  for (int i = 0; i < 512; ++i)
  {
    unsigned long long slice = data_p[i];

    for (int j = 0; j < 64; ++j)
    {
      if ( !((slice >> j) & 0x00000001) )
      {
        pts.setPoint(cnt, start_x + i + 10, start_y + (63-j));
        ++cnt;
      }
    }
  }
  _painter.drawPoints(pts, 0, cnt);
}
