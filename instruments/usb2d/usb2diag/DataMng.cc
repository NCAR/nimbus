#include "DataMng.h"

#include <qevent.h>
#include <qpainter.h>

#include <cstdio>
#include <unistd.h>

const unsigned long long DataMng::_syncWord = 0xAAAAAAA000000000LL;
const unsigned long long DataMng::_syncMask = 0xFFFFFFF000000000LL;


/* -------------------------------------------------------------------- */
DataMng::DataMng(FILE * fp) : _fp(fp)
{
  _fsize=0;
  _pts = new QPointArray(TBYTE*8*ROW_N2D*RCDPR_N2D);
}


bool DataMng::posfp() {
  //get filesize
  fseek (_fp, 0, SEEK_END);
  int size=ftell(_fp);
 
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

  //check if the same file size
  if (_fsize!=0 && size==_fsize) {
    printf("\n Same file size!");
    return false;
  }
  _fsize = size;
 
  return true;
}

/* -------------------------------------------------------------------- */
QPointArray* DataMng::getPoints()
{
  nidas_hdr	hdr;
  usb2d_rec	twod_rec;
  size_t	x, y = 10;

  _cnt = 0;
  for (int i = 0; i < ROW_N2D; ++i)
  {
    x = 5;
    for (int j = 0; j < RCDPR_N2D; ++j)
    {
      fread(&hdr, sizeof(nidas_hdr), 1, _fp);
      fread(&twod_rec, hdr.length, 1, _fp);
      printf("%llu %lu %lu %lx %lx\n", hdr.timetag, hdr.length,
		hdr.sample_id, twod_rec.tas, twod_rec.id);
      getRecord64(x, y, (unsigned long long *)twod_rec.data);
      x += SLIDE_N2D+1;
    }
    y += 75;
  }
  return _pts;
}

/* -------------------------------------------------------------------- */
void DataMng::getRecord64(int start_x, int start_y, unsigned long long * data_p)
{
  //QPainter _painter(this);
  //_painter.setPen(Qt::blue);
 
  for (int i = 0; i < SLIDE_N2D; ++i)
  {
    unsigned long long slice = flipLonglong(data_p[i]);

//    if ((slice & _syncMask) == _syncWord)
//      printf("%llu\n", slice & ~_syncMask);	// Print timing word.

    for (int j = 0; j < BIT_N2D; ++j)
    {
      if ( !((slice >> j) & 0x00000001) )
      {
        _pts->setPoint(_cnt, start_x + i, start_y + ((BIT_N2D-1)-j));
        ++_cnt;
      }
    }
  }
 
 // _painter.drawPoints(pts, 0, cnt);
}


int DataMng::getP()
{
  return TBYTE*8*ROW_N2D*RCDPR_N2D;
}
