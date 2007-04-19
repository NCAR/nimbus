#ifndef _DataPlot_h_
#define _DataPlot_h_

#include <qwidget.h>
#include "DataMng.h"
#include "DataUsb2d64.h"
 

/* -------------------------------------------------------------------- */
/**
 * Class to plot/draw PMS-2D ercords on a Qt Widget.
 */
class DataPlot: public QWidget
{
public:
  DataPlot(QWidget * parent, FILE * fp);
  virtual ~DataPlot() { if (dm !=NULL) {delete dm;}}

  void  Plot();
  void	ToggleFreeze();
  void	RstTimer(int s);

protected:
  virtual void timerEvent(QTimerEvent * e);

  virtual void paintEvent(QPaintEvent * e);


private:
  bool     _freeze;
  DataUsb2d64* dm;
};

#endif
