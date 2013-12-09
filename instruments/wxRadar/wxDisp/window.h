#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class QSpinBox;
class QLabel;

class RenderArea;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private slots:
    void penChanged();

private:
    RenderArea *renderArea;
    QLabel *penWidthLabel;
    QSpinBox *penWidthSpinBox;

};

#endif
