#include <qwt/qwt_plot.h>

#include "define.h"
#include "adsIOrt.h"


/* -------------------------------------------------------------------- */
class DataPlot : public QwtPlot
{
public:
        DataPlot(QWidget *, ADS_rtFile *);

  void	ToggleFreeze();

protected:
  virtual void timerEvent(QTimerEvent *e);

private:
  long		crv1, crv2;
  bool		freeze;
  ADS_rtFile	*file;

};

