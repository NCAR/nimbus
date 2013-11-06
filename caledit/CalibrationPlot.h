#ifndef _CalibrationPlot_h_
#define _CalibrationPlot_h_

//#include <QDataWidgetMapper>
//#include <QItemDelegate>
#include <QtGui>

class QColor;
class QwtPlotCurve;

#include <list>

using namespace std;

#include "ui_CalibrationPlot.h"

/**
 * @class CalibrationCurve
 * This is a simple class that associates a row of calibration data
 * to a pair of plotted curves.  The 'actual' curve is rendered as
 * line segments from set point to set point.  The 'fitted' is a
 * single straight line that is least square fitted to the set points
 * based upon the coefficients set in the calibration row.
 */
class CalibrationCurve
{
public:
    QString rid;
    QwtPlotCurve* actual;
    QwtPlotCurve* fitted;
};

/**
 * @class CalibrationPlot
 * A container widget that stores the curves and maintains their colors. 
 */
class CalibrationPlot : public QWidget, public Ui::CalibrationPlot
{
    Q_OBJECT

public:
    CalibrationPlot(QWidget* parent = 0);
    void dropColors();
    void setupColors();

    QList<CalibrationCurve *> curves;
    list<QColor> colors;
};

#endif
