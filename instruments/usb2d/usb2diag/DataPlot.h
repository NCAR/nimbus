#ifndef _DataPlot_h_
#define _DataPlot_h_

#include <qwidget.h>
 
const unsigned  short  TBYTE    = 4096;
const unsigned  short  BIT_N2D  = 64;
const unsigned  short  BYTE_N2D = 8;
const unsigned  short  SLIDE_N2D= TBYTE/BYTE_N2D;
const unsigned  short  ROW_N2D  = 8;
const unsigned  short  RCDPR_N2D= 2; 
const unsigned  short  BYTE_USB2D_REC = 8 +TBYTE;

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
class DataPlot : public QWidget
{
public:
        DataPlot(QWidget * parent, FILE * fp);
  virtual ~DataPlot() { }

  virtual void plot();
  void	ToggleFreeze();
  void	RstTimer(int s);

protected:
  virtual void timerEvent(QTimerEvent * e);

  virtual void paintEvent(QPaintEvent * e);

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
  virtual void displayRecord64(int x, int y, unsigned long long * data_p);
  static const unsigned long long _syncMask, _syncWord;

private:
  FILE * _fp;
  bool   _freeze;
  int    _fsize;

  bool   _posfp();
  

};

#endif
