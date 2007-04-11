#ifndef _DataPlot_h_
#define _DataPlot_h_

#include <qwidget.h>

// This needs to go in a more global include file, say nidas/usbtwod.h
typedef struct
{
  unsigned long long timetag;
  unsigned long length;
  unsigned long id;
  unsigned char data[4096];
} usb2d_rec;


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

protected:
  virtual void timerEvent(QTimerEvent * e);

  virtual void paintEvent(QPaintEvent * e);

  /**
   * Display 1 64 bit 2D record.
   * @param x is the starting x coordiante for the record (upper left).
   * @param y is the starting y coordiante for the record (upper left).
   * @param data_p is the pointer to the 2D data record (sans header).
   */
  virtual void displayRecord64(int x, int y, unsigned long long * data_p);

  FILE * _fp;
  bool _freeze;

};

#endif
