#include "define.h"
#include <qwidget.h>

/* -------------------------------------------------------------------- */
class DataPlot : public QWidget
{
public:
        DataPlot(QWidget * parent, FILE * fp);
  virtual ~DataPlot() { }

  virtual void plot();

  void	ToggleFreeze();

protected:
  virtual void timerEvent(QTimerEvent *e);

  FILE * _fp;
  bool _freeze;

};
