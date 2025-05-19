#include "clock_time.h"
#include <QVBoxLayout>
#include <QTimeEdit>
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <QSerialPort>
#include <QJsonObject>
#include <QJsonDocument>

///@brief Constructor for the clock_time class.
///Constructor that initializes the layout for the clock_time widget. (Placeholder version) 
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

///@brief Takes date values from user and sends them to the ESP32.
///This function takes the date values from the QDateTimeEdit widget, formats it to interface specifications,
///and sends it to the ESP32 via a serial port.
void clock_time::setTime(){
    QDateTime dateTime = dateTimeEdit->dateTime();

    //Adds time values as a QJsonObject in correct format
    QJsonObject timeObject;
    timeObject["set_time"] = true;
    timeObject["year"] = dateTime.toString("yyyy");
    timeObject["month"] = dateTime.toString("MM");
    timeObject["day"] = dateTime.toString("dd");
    timeObject["hour"] = dateTime.toString("HH");
    timeObject["minute"] = dateTime.toString("mm");
    timeObject["second"] = dateTime.toString("ss");

    //Adds the QJsonObject to a QJsonDocument and converts it to a string
    QJsonDocument doc(timeObject);
    QString jsonString = doc.toJson(QJsonDocument::Compact);
    jsonString.append("\n");

    //Create a QSerialPort and defines the settings
    QSerialPort serial;
    serial.setPortName("/dev/ttyUSB0");
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.setStopBits(QSerialPort::OneStop);

    //Opens the serial port and check if it opended correctly
    if(!serial.open(QIODevice::WriteOnly)){ 
        qDebug() << "Error: Port Not Open";
        return;
    }

    //Turns the QString int QByteArray and sends it to the ESP32
    QByteArray bytemessage = jsonString.toUtf8();
    serial.write(bytemessage); 
    serial.flush(); //Is used to force the data to be sendt immediately
    serial.close();
}
