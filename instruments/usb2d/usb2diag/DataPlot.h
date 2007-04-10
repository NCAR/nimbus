#include "define.h"
#include <qwidget.h>

#include <nidas/linux/usbtwod/usbtwod.h>

// This needs to go in a more global include file, say nidas/usbtwod.h
typedef struct
{
  dsm_sample_time_t timetag;
  dsm_sample_length_t length;
  unsigned char data[4096];
} usb2d_rec;


/* -------------------------------------------------------------------- */
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
  virtual void displayRecord(int x, int y, usb2d_rec & rec);

  FILE * _fp;
  bool _freeze;

};
