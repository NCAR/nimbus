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
  _fsize=0;
  plot();
  startTimer(2000);
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

  

  this->erase(this->rect());

  plot();
}

/* -------------------------------------------------------------------- */
void DataPlot::RstTimer(int s)
{
  killTimers();
  startTimer(s*1000);

}
/* -------------------------------------------------------------------- */
void DataPlot::paintEvent(QPaintEvent * e)
{
  QWidget::paintEvent(e);
  this->erase(this->rect());
  plot();
}

bool DataPlot::_posfp() {
  //get filesize
  fseek (_fp, 0, SEEK_END);
  int size=ftell(_fp);
  //if (size==_fsize) {
  //  printf("\n Same file size!");
  //  return false;
  //}

  //_fsize = size;
  // get file pointer at end of nidas header.
  rewind(_fp);
  char buff[TBYTE];
  fread(buff, TBYTE, 1, _fp);
  char * p = strstr(buff, "end header\n");
  if (p == 0) {
    printf("Can't locate 'end header'\n");
    exit(-1);
  }

  //check if the record is more than we can display
  int s    = ROW_N2D*RCDPR_N2D*(BYTE_USB2D_REC+sizeof(nidas_hdr));
  int offs =(p - buff) + 11;

  if ((size-offs)>s) {offs= size-s;}
  fseek(_fp, offs, 0);
  return true;
}

/* -------------------------------------------------------------------- */
void DataPlot::plot()
{
  nidas_hdr	hdr;
  usb2d_rec	twod_rec;
  size_t	x, y = 10;

  if (!_posfp()){return;}
  for (int i = 0; i < ROW_N2D; ++i)
  {
    x = 5;
    for (int j = 0; j < RCDPR_N2D; ++j)
    {
      fread(&hdr, sizeof(nidas_hdr), 1, _fp);
      fread(&twod_rec, hdr.length, 1, _fp);
      printf("%llu %lu %lu %lx %lx\n", hdr.timetag, hdr.length,
		hdr.sample_id, twod_rec.tas, twod_rec.id);
      displayRecord64(x, y, (unsigned long long *)twod_rec.data);
      x += SLIDE_N2D+1;
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
  QPointArray pts(BIT_N2D*SLIDE_N2D);

  for (int i = 0; i < SLIDE_N2D; ++i)
  {
    unsigned long long slice = flipLonglong(data_p[i]);

//    if ((slice & _syncMask) == _syncWord)
//      printf("%llu\n", slice & ~_syncMask);	// Print timing word.

    for (int j = 0; j < BIT_N2D; ++j)
    {
      if ( !((slice >> j) & 0x00000001) )
      {
        pts.setPoint(cnt, start_x + i, start_y + ((BIT_N2D-1)-j));
        ++cnt;
      }
    }
  }
  _painter.drawPoints(pts, 0, cnt);
}
