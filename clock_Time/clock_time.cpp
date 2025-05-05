#include "clock_time.h"

#include <QVBoxLayout>
#include <QProcess>
#include <QMessageBox>
#include <QTimeEdit>
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QSerialPort>


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

void clock_time::setTime(){
    QDateTime dateTime = dateTimeEdit->dateTime();
    QString timeString = dateTime.toString("yyyy-MM-dd HH:mm:ss");
    //QProcess::execute("sudo", QStringList() << "date" << "-s" << timeString); //Not in use for now
    qDebug() << "timeString:" << timeString;

    QStringList list = timeString.split(" ");
    QStringList dateList = list[0].split("-");
    QString yearString = dateList[0];
    QString monthString = dateList[1];
    QString dayString = dateList[2];
    QStringList timeList = list[1].split(":");
    QString hourString = timeList[0];
    QString minuteString = timeList[1];
    QString secondString = timeList[2];

    QString timeformat = "{'set_time': True ,'year': %1 ,'month': %2 ,'day': %3 ,'hour': %4 ,'minute': %5 ,'secound': %6 }";
    QString sentTime = timeformat.arg(yearString).arg(monthString).arg(dayString).arg(hourString).arg(minuteString).arg(secondString);

    qDebug() << "format:" << sentTime;

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

    QByteArray bytemessage = sentTime.toUtf8();
    serial.write(bytemessage);
    serial.flush();
    serial.close();

}

