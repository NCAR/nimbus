#ifndef _DataPlot_h_
#define _DataPlot_h_

#include <qwidget.h>
#include "DataMng.h"
 

/* -------------------------------------------------------------------- */
/**
 * Class to plot/draw PMS-2D ercords on a Qt Widget.
 */
class DataPlot: public QWidget
{
public:
  DataPlot(QWidget * parent, FILE * fp);
  virtual ~DataPlot() { }

  void  Plot();
  void	ToggleFreeze();
  void	RstTimer(int s);

protected:
  virtual void timerEvent(QTimerEvent * e);

  virtual void paintEvent(QPaintEvent * e);


private:
  bool     _freeze;
  DataMng* dm;
};

#endif
