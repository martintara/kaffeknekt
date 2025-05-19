#ifndef SENSORANALYTICSDIALOG_H
#define SENSORANALYTICSDIALOG_H

#include <QDialog>
#include <QDebug>

/**
 * @namespace Ui
 * @brief Qt namespace containing UI classes generated from .ui files.
 */
namespace Ui {
class SensorAnalyticsDialog;
}

/**
 * @class SensorAnalyticsDialog
 * @brief Dialog for displaying and selecting sensor analytics metrics.
 *
 * This dialog allows the user to view different sensor-related data
 * (e.g., water temperature, pressure, power consumption) and to select
 * the time interval for data analysis.
 */
class SensorAnalyticsDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the SensorAnalyticsDialog.
     * @param parent The parent widget (optional).
     */
    explicit SensorAnalyticsDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~SensorAnalyticsDialog();

private slots:
    /**
     * @brief Slot triggered when the water temperature button is clicked.
     */
    void on_btnWaterTemp_clicked();

    /**
     * @brief Slot triggered when the water pressure button is clicked.
     */
    void on_btnWaterPressure_clicked();

    /**
     * @brief Slot triggered when the power consumption button is clicked.
     */
    void on_btnPowerConsumption_clicked();

    /**
     * @brief Slot triggered when the "day" interval is selected.
     */
    void on_day_clicked();

    /**
     * @brief Slot triggered when the "week" interval is selected.
     */
    void on_week_clicked();

    /**
     * @brief Slot triggered when the "month" interval is selected.
     */
    void on_month_clicked();

signals:
    /**
     * @brief Emitted when the user selects a time interval.
     * @param seconds The selected interval in seconds.
     */
    void intervalSelected(qreal seconds);

private:
    Ui::SensorAnalyticsDialog *ui; ///< Pointer to the UI components.
};

#endif // SENSORANALYTICSDIALOG_H
