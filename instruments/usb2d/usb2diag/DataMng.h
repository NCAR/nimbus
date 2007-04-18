#ifndef _DataMng_h_
#define _DataMng_h_

#include <iostream>
#include <qwidget.h>
 
const unsigned  short  TBYTE    = 4096;


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
  DataMng( FILE * fp,  short bitn);
  virtual ~DataMng()   { }
  virtual bool         Posfp();
  virtual QPointArray* GetPoints();
  
protected:


  /**
   * Display 1 64 bit 2D record.
   * @param x is the starting x coordiante for the record (upper left).
   * @param y is the starting y coordiante for the record (upper left).
   * @param data_p is the pointer to the 2D data record (sans header).
   */
  virtual void _getRecord(int x, int y, unsigned char* data_p);
  static const unsigned long long _syncMask, _syncWord;

  short  _bit_n ;	// = 64 32;
  short  _byte_n;	// = 8 4;
  short  _slide_n;	//= TBYTE/BYTE_N2D;
  short  _row_n;	//= 8 16;
  short  _rcdpr_n;	//= 2 1; 
  short  _byte_usb2d_rcd;// = 8 +TBYTE;

  nidas_hdr*	_hdr;
  usb2d_rec*	_twod_rec;
  void           _get2drec();
  
  void          _init(short bitn);
  bool          _chkInit();


private:
  FILE * _fp;
  int    _fsize;
  QPointArray *_pts;
  size_t _cnt;
  
};

#endif
