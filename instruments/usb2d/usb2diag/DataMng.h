#ifndef _DataMng_h_
#define _DataMng_h_

#include <iostream>
#include <qwidget.h>
 
const unsigned  short  TBYTE    = 4096;
const unsigned  short  BIT_N2D  = 64;
const unsigned  short  BYTE_N2D = 8;
const unsigned  short  SLIDE_N2D= TBYTE/BYTE_N2D;
const unsigned  short  ROW_N2D  = 8;
const unsigned  short  RCDPR_N2D= 2; 
const unsigned  short  BYTE_USB2D_REC = 8 +TBYTE;
const unsigned  int    TPTS     = TBYTE*8*ROW_N2D*RCDPR_N2D;

// This needs to go in a more global include file, say nidas/usbtwod.h
typedef struct
{
  unsigned long long timetag;
  unsigned long length;
  unsigned long sample_id;
} nidas_hdr;

typedef struct		// Data, length = 4104
{
  unsigned long id;	// Data or Housekeeping.
  unsigned long tas;
  unsigned char data[TBYTE];
} usb2d_rec;

typedef struct		// length = 8
{
  unsigned long id;
  unsigned long particle_count;
} usb2d_hskp;


/* -------------------------------------------------------------------- */
/**
 * Class to plot/draw PMS-2D ercords on a Qt Widget.
 */
class DataMng
{
public:
  DataMng( FILE * fp);
  virtual ~DataMng() { }
  virtual bool         posfp();
  virtual QPointArray* getPoints();
  int                  getP();

protected:

inline long long flipLonglongIn(const void* p)
{
    union {
      long long v;
      char b[BYTE_N2D];
    } u;
    const char* cp = (const char*)p;
    for (int i = BYTE_N2D-1; i >= 0; i--) u.b[i] = *cp++;
    return u.v;
}
inline long long flipLonglong(const long long& p)
{
    return flipLonglongIn(&p);
}
  /**
   * Display 1 64 bit 2D record.
   * @param x is the starting x coordiante for the record (upper left).
   * @param y is the starting y coordiante for the record (upper left).
   * @param data_p is the pointer to the 2D data record (sans header).
   */
  virtual void getRecord64(int x, int y, unsigned long long * data_p);
  static const unsigned long long _syncMask, _syncWord;

private:
  FILE * _fp;
  int    _fsize;
  QPointArray *_pts;
  size_t _cnt;

};

#endif
