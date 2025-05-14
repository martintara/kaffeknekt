#include "clock_time.h"
#include <QVBoxLayout>
#include <QTimeEdit>
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <QSerialPort>
#include <QJsonObject>
#include <QJsonDocument>

/// @brief 
/// @param parent 
clock_time::clock_time(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    dateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    QPushButton *setButton = new QPushButton("Set Time", this);
    layout->addWidget(dateTimeEdit);
    layout->addWidget(setButton);
    connect(setButton, &QPushButton::clicked, this, &clock_time::setTime);
}

/// @brief 
void clock_time::setTime(){
    QDateTime dateTime = dateTimeEdit->dateTime();

    QJsonObject timeObject;
    timeObject["set_time"] = true;
    timeObject["year"] = dateTime.toString("yyyy");
    timeObject["month"] = dateTime.toString("MM");
    timeObject["day"] = dateTime.toString("dd");
    timeObject["hour"] = dateTime.toString("HH");
    timeObject["minute"] = dateTime.toString("mm");
    timeObject["second"] = dateTime.toString("ss");

    QJsonDocument doc(timeObject);
    QString jsonString = doc.toJson(QJsonDocument::Compact);
    jsonString.append("\n");

    qDebug() << "Sending JSON:" << jsonString;

    QSerialPort serial;
    serial.setPortName("/dev/ttyUSB0");
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.setStopBits(QSerialPort::OneStop);

    if(!serial.open(QIODevice::WriteOnly)){
        qDebug() << "Error: Port Not Open";
        return;
    }

    QByteArray bytemessage = jsonString.toUtf8();
    serial.write(bytemessage);
    serial.flush();
    serial.close();
}
