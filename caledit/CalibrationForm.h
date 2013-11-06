#ifndef _CalibrationForm_h_
#define _CalibrationForm_h_

#include <QAbstractItemModel>
#include <QStringListModel>
#include <QDataWidgetMapper>
#include <QPushButton>

#include "ui_CalibrationForm.h"

#define nRows 20

/**
 * @class CalibrationForm
 * This defines the form used when editing a single calibration entry.
 * Its layout UI is based upon the one used in the AEROS calibration tool.
 */
class CalibrationForm : public QWidget, public Ui::CalibrationForm
{
  Q_OBJECT

public:
  CalibrationForm(QWidget* parent = 0);

  void setModel(QAbstractItemModel *model);

  QStringListModel* setupComboModel(QString sql_column);

  void setupMapper();

  void setRow(int row);
  int  getRow() {return _row;};

  void setEnabled(bool state);

  QList<QLineEdit*>   _setPointList;
  QList<QPushButton*> _delButtonList;
  QList<QLineEdit*>   _inputList;
  QList<QLineEdit*>   _stdDevList;
  QList<QLineEdit*>   _appliedList;
  QList<QLineEdit*>   _setDateTimeList;

  QList<QLineEdit*>   _currCalCList;
  QList<QLineEdit*>   _prevCalCList;

  QButtonGroup* _delButtonGroup;
  QButtonGroup* _curveFitGroup;

public slots:
  void commitData(QWidget* widget);
  void removeSetPoint(int index);
  void changeFitButtonClicked(int order);
  void revert();
  void submit();

signals:
  void removeSetPoint(int row, int index);
  void changeFitButtonClicked(int row, int order);
  void replot(int row);
  void initializeForm(int row);
  void submitForm(int row);

private:
  QAbstractItemModel* _model;
  QDataWidgetMapper* _mapper;
  int _row;
};

#endif
