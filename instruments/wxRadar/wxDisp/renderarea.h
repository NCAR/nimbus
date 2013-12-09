#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>

#include <cstdio>

class RenderArea : public QWidget
{
    Q_OBJECT

public:
    RenderArea(QWidget *parent = 0);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:
    void setPen(const QPen &pen);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPen pen;

    FILE *fp;
};

#endif
