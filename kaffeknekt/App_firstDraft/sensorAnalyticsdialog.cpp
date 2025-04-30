#include "sensorAnalyticsdialog.h"
#include "ui_sensorAnalyticsdialog.h"
#include <QDebug>

SensorAnalyticsDialog::SensorAnalyticsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SensorAnalyticsDialog)
{
    ui->setupUi(this);
    qDebug() << "SensorAnalyticsDialog åpnet!";
}

SensorAnalyticsDialog::~SensorAnalyticsDialog()
{
    delete ui;
}

void SensorAnalyticsDialog::on_btnWaterTemp_clicked()
{

}


void SensorAnalyticsDialog::on_btnWaterPressure_clicked()
{

}

void SensorAnalyticsDialog::on_btnPowerConsumption_clicked()
{

}



