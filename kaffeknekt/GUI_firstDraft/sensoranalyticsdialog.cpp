#include "sensoranalyticsdialog.h"
#include "ui_sensoranalyticsdialog.h"
#include <QDebug>

SensorAnalyticsDialog::SensorAnalyticsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SensorAnalyticsDialog)
{
    ui->setupUi(this);
    qDebug() << "SensorAnalyticsDialog åpnet!";
    // Skjul timescale-frame ved oppstart
    ui->frameTimescale->setVisible(false);
}

SensorAnalyticsDialog::~SensorAnalyticsDialog()
{
    delete ui;
}

void SensorAnalyticsDialog::on_btnWaterTemp_clicked()
{
    qDebug() << "Water Temperature clicked";
    ui->frameTimescale->setVisible(true);

}


void SensorAnalyticsDialog::on_btnWaterPressure_clicked()
{
    qDebug() << "water pressure clickedd";
    ui->frameTimescale->setVisible(true);

}

void SensorAnalyticsDialog::on_btnPowerConsumption_clicked()
{
    qDebug() << "Power cons.. knappen er clicked";
    ui->frameTimescale->setVisible(true);

}
