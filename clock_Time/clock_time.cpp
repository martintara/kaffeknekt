#include "clock_time.h"

#include <QVBoxLayout>
#include <QProcess>
#include <QMessageBox>
#include <QTimeEdit>
#include <QPushButton>
#include <QDebug>

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

    //Send timeString to ESP32 somehow...


}

