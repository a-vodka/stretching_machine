#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QInputDialog>


/*

    ПРОПУШИЛ

*/


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Tensile-Test");

    //Graphic
    Graphic.setQCustomPlot(ui->wGraphic);

    //Com Ports
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort port;
        port.setPort(info);
        if(port.open(QIODevice::ReadWrite))
        {
            portList.append(info.portName());
        }
    }
    comBox();

    //Timers
    graphFromDB = new QTimer();

    //connect
    connect(timerDraw, &ThreadedTimer::signalForMainWindow, this, &MainWindow::clockDraw);
    connect(timerSerials, &ThreadedTimer::signalForMainWindow, this, &MainWindow::clockSerials);
    connect(graphFromDB, &QTimer::timeout, this, &MainWindow::clockExistingData);


    //vfd
//    connect(ui->forwardButton, &QPushButton::clicked, &VFD, &vfd::forward);
//    connect(ui->reverseButton, &QPushButton::clicked, &VFD, &vfd::reverse);
    connect(ui->forwardButton, &QPushButton::clicked, &vfd2, &VFD2::forward);
    connect(ui->reverseButton, &QPushButton::clicked, &vfd2, &VFD2::reverse);
        //v
      
    ui->spinFrequency->setRange(0, 599);
    ui->spinFrequency->setSingleStep(5);
    ui->spinFrequency->setSuffix(" Hz"); 
      
    db = new DBConnector(this);
}


MainWindow::~MainWindow()
{
    //delete timerDraw;
    delete ui;
}

void MainWindow::serialRecieve()
{

}



//PLUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



void MainWindow::comBox()
{
    ui->comLForce->clear();
    ui->comLForce->addItems(portList);
    ui->comLLength->clear();
    ui->comLLength->addItems(portList);
    ui->comLFrequency->clear();
    ui->comLFrequency->addItems(portList);
}

void MainWindow::on_openBForce_clicked()
{
    QString name = ui->comLForce->currentText();
    //int index = ui->comLForce->currentIndex();

    portList.removeOne(name);
    usedPorts.append(name);

    comBox();

    //set oportunity to click on the buttons
    ui->openBForce->setEnabled(false);
    ui->closeBForce->setEnabled(true);

    //set style and text of labels
    ui->labelPortForce->setText(name);
    ui->labelPortForce->setStyleSheet("color: rgb(0, 170, 0)");
    ui->labelConditionForce->setText("CONNECTED");
    ui->labelConditionForce->setStyleSheet("color: rgb(0, 170, 0)");

    //serial
    Force.setName(name);
    Force.switchConection();
}

void MainWindow::on_openBLength_clicked()
{
    QString name = ui->comLLength->currentText();
    //int index = ui->comLLength->currentIndex();

    portList.removeOne(name);
    usedPorts.append(name);

    comBox();

    //set oportunity to click on the buttons
    ui->openBLength->setEnabled(false);
    ui->closeBLength->setEnabled(true);

    //set style and text of labels
    ui->labelPortLength->setText(name);
    ui->labelPortLength->setStyleSheet("color: rgb(0, 170, 0)");
    ui->labelConditionLength->setText("CONNECTED");
    ui->labelConditionLength->setStyleSheet("color: rgb(0, 170, 0)");

    //serial
    Length.setName(name);
    Length.switchConection();
}

void MainWindow::on_openBFrequency_clicked()
{
    QString name = ui->comLFrequency->currentText();
    //int index = ui->comLLength->currentIndex();

    portList.removeOne(name);
    usedPorts.append(name);

    comBox();

    //set oportunity to click on the buttons
    ui->openBFrequency->setEnabled(false);
    ui->closeBFrequency->setEnabled(true);

    //set style and text of labels
    ui->labelPortFrequency->setText(name);
    ui->labelPortFrequency->setStyleSheet("color: rgb(0, 170, 0)");
    ui->labelConditionFrequency->setText("CONNECTED");
    ui->labelConditionFrequency->setStyleSheet("color: rgb(0, 170, 0)");

    //serial
   // VFD.setName(name);
    vfd2.setName(name);
    vfd2.switchConection();
        //v
}

void MainWindow::on_closeBForce_clicked()
{
    usedPorts.removeOne(Force.getName());
    portList.append(Force.getName());

    comBox();

    //set opportunity to click on the buttons
    ui->openBForce->setEnabled(true);
    ui->closeBForce->setEnabled(false);

    //set style and text of labels
    ui->labelPortForce->setText("None");
    ui->labelPortForce->setStyleSheet("color: rgb(255, 0, 0)");
    ui->labelConditionForce->setText("None");
    ui->labelConditionForce->setStyleSheet("color: rgb(255, 0, 0)");

    Force.switchConection();
}

void MainWindow::on_closeBLength_clicked()
{
    usedPorts.removeOne(Length.getName());
    portList.append(Length.getName());

    comBox();

    //set opportunity to click on the buttons
    ui->openBLength->setEnabled(true);
    ui->closeBLength->setEnabled(false);

    //set style and text of labels
    ui->labelPortLength->setText("None");
    ui->labelPortLength->setStyleSheet("color: rgb(255, 0, 0)");
    ui->labelConditionLength->setText("None");
    ui->labelConditionLength->setStyleSheet("color: rgb(255, 0, 0)");

    Length.switchConection();
}

void MainWindow::on_closeBFrequency_clicked()
{
    usedPorts.removeOne(vfd2.getName());
    portList.append(vfd2.getName());

    comBox();

    //set opportunity to click on the buttons
    ui->openBFrequency->setEnabled(true);
    ui->closeBFrequency->setEnabled(false);

    //set style and text of labels
    ui->labelPortFrequency->setText("None");
    ui->labelPortFrequency->setStyleSheet("color: rgb(255, 0, 0)");
    ui->labelConditionFrequency->setText("None");
    ui->labelConditionFrequency->setStyleSheet("color: rgb(255, 0, 0)");
}


//MODE NEW/EXISTING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



void MainWindow::on_radioNew_clicked()
{
    ui->lineSeries->setEnabled(true);

    //other widgets
    ui->comSeries->setEnabled(false);
    ui->comExperiment->setEnabled(false);
}

void MainWindow::on_radioExisting_clicked()
{
    ui->comSeries->setEnabled(true);
    ui->comExperiment->setEnabled(true);

    //another widget
    ui->lineSeries->setEnabled(false);

    ui->comSeries->clear();

    QSqlQuery qData = db->getSeriesNames();
    while(qData.next())
    {
        ui->comSeries->addItem(qData.value(0).toString());
    }
}

void MainWindow::on_comSeries_currentIndexChanged(int index)
{
    ui->comExperiment->clear();

    QSqlQuery qData = db->getExperiments(index + 1);
    while(qData.next())
    {
        ui->comExperiment->addItem(qData.value(0).toString());
    }
}


//TIME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



void MainWindow::on_drawB_clicked()
{
    //graph
    Graphic.clear();

    drawingTime = 0;
    if (ui->radioNew->isChecked())
    {
        if (Force.getConection())
        {
            QMessageBox::information(this, "Drawing", "New");

            //DataBase
            QString currentSeriesName = ui->lineSeries->text();
            currentSeriesNum = db->getSeries(currentSeriesName);

            if(!currentSeriesNum)
            {
                currentSeriesNum = db->countSeries() + 1;
                QString material = QInputDialog::getText(this, "New Series", "What's the material?");
                int length = QInputDialog::getInt(this, "New Series", "What's the length?");
                int width = QInputDialog::getInt(this, "New Series", "What's the width?");
                int height = QInputDialog::getInt(this, "New Series", "What's the height?");
                QStringList properties = QInputDialog::getText(this, "New Series", "Write the properties you want to calculate using ONLY comas").split(',');
                db->createSeries(currentSeriesNum, currentSeriesName, material, height, width, length, properties);
            }

            currentExperiment = QInputDialog::getInt(this, "New Experiment", "What's the number of the experiment?");

            // Setup axis labels and ports based on whether length sensor is connected
            if (Length.getConection())
            {
                Graphic.setAxesLabels("Delta L", "Force");
                Length.addPort();
                Length.portOpen();
            }
            else
            {
                Graphic.setAxesLabels("Time (s)", "Force");
            }

            Force.addPort();
            Force.portOpen();

            // VFD is optional (only if connected)
            if (vfd2.getConection())
            {
                vfd2.addPort();
                vfd2.portOpen();
                vfd2.run();
            }

            //timers
            timerDraw->startTimer(1000);
            timerSerials->startTimer(100);

            //style button
            ui->closeBForce->setEnabled(false);
            ui->closeBLength->setEnabled(false);
            ui->drawB->setEnabled(false);
            ui->stopDB->setEnabled(true);
        }
        else
        {
            QMessageBox::information(this, "CAUTION!!!", "No connection to Force sensor");
        }
    }
    else if(ui->radioExisting->isChecked())
    {
        qDebug () << "EXSST";
        currentSeriesNum = ui->comSeries->currentIndex() + 1;
        currentExperiment = ui->comExperiment->currentText().toInt();

        // Check if delta_l was recorded for this experiment or if it was F-t test
        QSqlQuery sample = db->getData(currentSeriesNum, currentExperiment, 0.5);
        if (sample.next() && sample.value(1).toDouble() != 0)
        {
            Graphic.setAxesLabels("Delta L", "Force");
        }
        else
        {
            Graphic.setAxesLabels("Time (s)", "Force");
        }

        QMessageBox::information(this, "Drawing", "Existing");

        //Timer
        graphFromDB->start(100);

        //style button
        ui->drawB->setEnabled(false);
        ui->stopDB->setEnabled(true);
    }
    else
    {
        QMessageBox::information(this, "CAUTION!!!", "Chose the radiobutton");
    }
}

void MainWindow::clockDraw()
{
    Graphic.Replot();
}

void MainWindow::clockSerials()
{
    double f = Force.getSeria();
    double l = 0;

    if (Length.getConection())
    {
        l = Length.getSeria();
        Graphic.Add(l, f);
    }
    else
    {
        Graphic.Add(drawingTime, f);
    }

    db->insertData(currentSeriesNum, currentExperiment, drawingTime, f, l);

    //Widget
    ui->lcdN->display(drawingTime);
    ui->CurrentLengthLCD->display(l);
    ui->CurrentForceLCD->display(f);

    drawingTime += 0.1;
}

void MainWindow::clockExistingData()
{
    qDebug() << currentSeriesNum << currentExperiment << drawingTime;
    QSqlQuery data = db->getData(currentSeriesNum, currentExperiment, drawingTime);

    double F = 0;
    double delta_l = 0;

    if(data.next())
    {
        F = data.value(0).toDouble();
        delta_l = data.value(1).toDouble();
        qDebug () << F << delta_l;
    }
    else
    {
        graphFromDB->stop();
        qDebug() << 1;
        return;
    }

    double xVal = (delta_l != 0) ? delta_l : drawingTime;
    Graphic.Add(xVal, F);
    Graphic.Replot();

    //Widget
    ui->lcdN->display(drawingTime);
    ui->CurrentLengthLCD->display(delta_l);
    ui->CurrentForceLCD->display(F);
    drawingTime += 0.1;
}

void MainWindow::on_stopDB_clicked()
{
    if (ui->radioNew->isChecked())
    {
        //vfd
        if (vfd2.getConection())
        {
            vfd2.stop();
            vfd2.portClose();
        }

        //serials
        Force.portClose();
        if (Length.getConection())
        {
            Length.portClose();
        }

        //timers
        timerDraw->stopTimer();
        timerSerials->stopTimer();

        //style button
        ui->closeBForce->setEnabled(true);
        ui->closeBLength->setEnabled(true);
        ui->drawB->setEnabled(true);
        ui->stopDB->setEnabled(false);
    }
    else
    {
        //Timer
        graphFromDB->stop();

        //style button
        ui->drawB->setEnabled(true);
        ui->stopDB->setEnabled(false);
    }
}



//FOLLOWTHELINK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



void MainWindow::on_clickTheSettings_triggered()
{
    QMessageBox::information(this, "CAUTION!!!", "Chose the radiobutton");
}



//VFD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void MainWindow::on_pushButton_clicked()
{
    //VFD.setFrequency(ui->spinFrequency->value());
    vfd2.setFrequency(ui->spinFrequency->value());
    //v
}



void MainWindow::on_pushButton_2_clicked()
{
    //VFD.stop();
    vfd2.stop();
    //v
}
