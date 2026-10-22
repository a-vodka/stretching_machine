#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <qcustomplot.h>
#include <qvector.h>
#include <QDebug>

class graphic
{
    QCustomPlot* Graphic = nullptr;   //it's a widget where you display a graphic
    QCPCurve* plot = nullptr;         //it's a curve which displays on the widget

    int dotIndex = 0;                 //it's a number descriming dots order
public:
    graphic();
    graphic(QCustomPlot *Graphic);
    ~graphic();
    void setQCustomPlot(QCustomPlot *Graphic);
    void setAxesLabels(const QString &xLabel, const QString &yLabel);
    void Add(double x, double f);
    void Replot();
    void clear();
};

#endif // GRAPHIC_H
