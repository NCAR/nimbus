#include <qwt_plot.h>

#include "define.h"
#include "adsIOrt.h"


/* -------------------------------------------------------------------- */
class DataPlot : public QwtPlot
{
public:
        DataPlot(QWidget *, ADS_rtFile *);

  void	setUnits(int Units);
  void	ToggleFreeze();

protected:
  virtual void timerEvent(QTimerEvent *e);

private:
  int		units;
  bool		freeze;
  ADS_rtFile	*file;

};

