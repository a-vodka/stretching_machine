#include "graphic.h"

graphic::graphic()
{

}

graphic::graphic(QCustomPlot *Graphic)
{
    setQCustomPlot(Graphic);
}

graphic::~graphic()
{

}

void graphic::setQCustomPlot(QCustomPlot *Graphic)
{
    this->Graphic = Graphic;

    this->plot = new QCPCurve(this->Graphic->xAxis, this->Graphic->yAxis);

    this->plot->setPen(QPen(Qt::black));
    this->Graphic->setInteraction(QCP::iRangeDrag, true);
    this->Graphic->setInteraction(QCP::iRangeZoom, true);
    this->Graphic->axisRect()->setupFullAxesBox();

    this->Graphic->xAxis->setLabel("Delta L");
    this->Graphic->yAxis->setLabel("Force");
    this->Graphic->rescaleAxes();
}

void graphic::setAxesLabels(const QString &xLabel, const QString &yLabel)
{
    if (this->Graphic)
    {
        this->Graphic->xAxis->setLabel(xLabel);
        this->Graphic->yAxis->setLabel(yLabel);
        this->Graphic->replot();
    }
}

void graphic::Add(double x, double f)
{
    if (!plot || !Graphic)
        return;

    QCPCurveData recentData = QCPCurveData(dotIndex++, x, f);
    plot->data()->add(recentData);
    this->Graphic->rescaleAxes();
}

void graphic::Replot()
{
    if (Graphic)
        Graphic->replot();
}

void graphic::clear()
{
    dotIndex = 0;
    if (Graphic)
    {
        Graphic->clearPlottables();
        this->plot = new QCPCurve(this->Graphic->xAxis, this->Graphic->yAxis);
        this->plot->setPen(QPen(Qt::black));
        this->Graphic->rescaleAxes();
        this->Graphic->replot();
    }
}
