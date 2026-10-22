#include "serialports.h"

SerialPorts::SerialPorts()
{
    serial = new QSerialPort(this);
    //connect(serial, &QSerialPort::readyRead, this, &SerialPorts::getSeria);
}

SerialPorts::SerialPorts(QString name)
{
    this->name = name;
    serial = new QSerialPort(this);
    //connect(serial, &QSerialPort::readyRead, this, &SerialPorts::getSeria);
}

SerialPorts::~SerialPorts()
{
    serial->close();
}

void SerialPorts::addPort()
{
    serial->setPortName(this->name);
    portSettings();
}

void SerialPorts::portSettings()
{
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
}

//void SerialPorts::portSettings(int a)
//{
//    a+=a;
//    serial->setBaudRate(QSerialPort::Baud9600);
//    serial->setDataBits(QSerialPort::Data7);
//    serial->setParity(QSerialPort::NoParity);
//    serial->setStopBits(QSerialPort::TwoStop);
//    serial->setFlowControl(QSerialPort::NoFlowControl);
//}

void SerialPorts::setName(QString name)
{
    this->name = name;
}

void SerialPorts::changePort(QString name)
{
    setName(name);
    serial->setPortName(this->name);
}

void SerialPorts::portOpen()
{
    serial->open(QIODevice::ReadWrite);
}

void SerialPorts::portClose()
{
    serial->close();
}

void SerialPorts::writeData(QString row)
{
    Q_UNUSED(row);
    //serial->write(row, Q_INT64_C());
}

void SerialPorts::switchConection()
{
    conection = !conection;
}

bool SerialPorts::getConection()
{
    return conection;
}

const QString SerialPorts::getName()
{
    return name;
}

QSerialPort *SerialPorts::getPort()
{
    return serial;
}

double SerialPorts::getSeria()
{
    if (!serial || !serial->isOpen())
        return 0.0;

    QByteArray raw = serial->readLine();
    if (raw.isEmpty())
        return lastResult;

    QString ba1 = QString::fromLatin1(raw).trimmed();
    if (ba1.length() >= 7)
    {
        QString numPart = ba1.mid(1, 7).trimmed();
        bool ok = false;
        double val = numPart.toDouble(&ok);
        if (ok)
        {
            lastResult = std::abs(val);
            return lastResult;
        }
    }

    return lastResult;
}


