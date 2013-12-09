#include <QtGui>

#include "renderarea.h"
#include "window.h"

const int IdRole = Qt::UserRole;

Window::Window()
{
  renderArea = new RenderArea;
  setWindowTitle(tr("Weather Radar"));
/*
  penWidthSpinBox = new QSpinBox;
  penWidthSpinBox->setRange(0, 20);
  penWidthSpinBox->setSpecialValueText(tr("0 (cosmetic pen)"));

  penWidthLabel = new QLabel(tr("Pen &Width:"));
  penWidthLabel->setBuddy(penWidthSpinBox);

  connect(penWidthSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(penChanged()));
*/
  QGridLayout *mainLayout = new QGridLayout;
  mainLayout->setColumnStretch(0, 1);
  mainLayout->setColumnStretch(3, 1);
  mainLayout->addWidget(renderArea, 0, 0, 1, 4);
  mainLayout->setRowMinimumHeight(1, 6);
//  mainLayout->addWidget(penWidthSpinBox, 3, 2);
  setLayout(mainLayout);
//  penChanged();
}

void Window::penChanged()
{
  int width = penWidthSpinBox->value();

  renderArea->setPen(QPen(Qt::blue, width));
}

