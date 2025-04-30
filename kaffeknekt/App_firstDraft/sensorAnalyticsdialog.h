#ifndef SENSORANALYTICSDIALOG_H
#define SENSORANALYTICSDIALOG_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class SensorAnalyticsDialog;
}

class SensorAnalyticsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SensorAnalyticsDialog(QWidget *parent = nullptr);
    ~SensorAnalyticsDialog();

private slots:
    void on_btnWaterTemp_clicked();

    void on_btnWaterPressure_clicked();

    void on_btnPowerConsumption_clicked();

private:
    Ui::SensorAnalyticsDialog *ui;
};

#endif // SENSORANALYTICSDIALOG_H
