#include <QtGui>

#include <iostream>
#include <arpa/inet.h>
#include "renderarea.h"

typedef struct
{
        uint16_t type;               //Valid in all versions
        uint16_t count;              //Valid in all versions
        uint32_t timestamp;          //Valid in all versions
        uint16_t activity;           //Valid in all versions
        uint16_t datacount;          //Valid in all versions
        uint16_t data[100];          //Valid in all versions
        uint16_t extra[16];          //Valid if version >= 1
        uint16_t bitcount;           //Valid if version >= 1
        uint16_t rsvd123;            //Valid if version >= 1
        uint32_t timestamph;         //Valid if version >= 1
} SEQRECORD708;


RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
//  fp = fopen("/home/ads/wxRadar/BTIRPC/examples/BTI708/release-rpc-x64/examp4.dat", "rb");
  fp = fopen("examp4full.dat", "rb");
}

QSize RenderArea::minimumSizeHint() const
{
  return QSize(200, 200);
}

QSize RenderArea::sizeHint() const
{
  return QSize(600, 400);
}

void RenderArea::setPen(const QPen &pen)
{
  this->pen = pen;
  update();
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
  int beam, pxlCnt = 0, noIncCnt = 0;
  unsigned char *data, fr[200];
  SEQRECORD708 record;

  data = (unsigned char *) record.data;

std::cout << "painting event\n";

  QPainter painter(this);

  for (beam = 0; fread(&record, sizeof(SEQRECORD708), 1, fp); ++beam)
  {
/*
printf("type = 0x%04x\n", (record.type));
printf("count = %d\n", (record.count));
printf("timestamp = %d\n", (record.timestamp));
printf("activity = %d\n", (record.activity));
printf("datacount = %d\n", (record.datacount));
printf("bitcount = %d\n", (record.bitcount));
*/
    // Duplicate record check
//    if (memcmp(fr, data, sizeof(data)) == 0) printf("%d - dup record\n", beam);
    memcpy(fr, data, sizeof(data));

    bool right = data[0] & 0x04; // Are we on the right or left half of display (i.e. inc or dec x)?

    /* Position on bottom ceneter of screen.  We assume each record
     * in the file is a comlete scan.  No merging of records.
     */
    int x = 300; int y = 320;

    // First 10 bits is a header and there are 1600 bits in 200 bytes.
    for (int bitCnt = 10; bitCnt < 1600; bitCnt += 5)
    {
      int byteNum = bitCnt / 8;
      int bitNum = bitCnt % 8;

      unsigned char mask = 0x00;
      for (int m = 0; m < 8-bitNum; ++m)
        mask |= (0x01 << m);

      unsigned char byte = (data[byteNum] >> bitNum) & mask;
//printf("%d - %d %d - %02x %02x", bitCnt, byteNum, bitNum, data[byteNum], data[byteNum+1]);
//printf(" - %02x %2x", (byte & 0x1f), mask);

      // break if rest of record is zeros.  Just checking two consecutive bytes....
      if (data[byteNum] == 0 && data[byteNum+1] == 0) break;

      if (bitNum > 3)
      {
        byte |= data[byteNum+1] << (8-bitNum);
//printf(" %02x", (byte & 0x1f));
      }
//printf("\n");

++pxlCnt;
if ((byte & 0x01) == 0 && (byte & 0x02) == 0) ++noIncCnt;

      if (byte & 0x01) { if (right) ++x; else --x; }
      if (byte & 0x02) --y;

      unsigned color = (byte >> 2) & 0x07;
      QColor qcolor;
      if (color == 0x00) qcolor = Qt::black;
      if (color == 0x01) qcolor = Qt::green;
      if (color == 0x02) qcolor = Qt::yellow;
      if (color == 0x03) qcolor = Qt::red;
      if (color == 0x04) qcolor = Qt::magenta;
      if (color == 0x05) qcolor = Qt::blue;
      if (color == 0x06) qcolor = Qt::cyan;
      if (color == 0x07) qcolor = Qt::white;
      QPen pen1(qcolor, 1);
      painter.setPen(pen1);

      painter.drawPoint(QPoint(x,y));
    }
//exit(1);
  }
  printf("%d beams in file.\n", beam);
  printf("%d non zero pixels in file.\n", pxlCnt);
  printf("%d no increment pixels.\n", noIncCnt);
}
