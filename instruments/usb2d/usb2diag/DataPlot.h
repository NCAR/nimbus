#include "define.h"
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
