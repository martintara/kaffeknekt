#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sensoranalyticsdialog.h"


#include <QMainWindow>

#include "ui_mainwindow.h"

//neste steg er sub-sub side meny for "esnsor analytics/btnTimescale NB. skalfikses!"
#include <QFrame>             // ADDED: QFrame for sidemenyen
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class Kaffeknekt;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // For hovedmeny (frame_1)
    void on_btnHamburger_clicked();
    void on_btnHome_clicked();
    void on_btnSettings_clicked();
    void on_btnInfo_clicked();

    // For undermeny (frame_2)
    void on_btnInstructions_clicked();
    void on_btnSensorAnalytics_clicked();
    void on_btnStatistics_clicked();
    /*
    //For sub-side meny (frame_3 - som heter ikke deet lenger :)-basically Qdialogen! ):
    void on_btnWaterTemp_clicked();
    void on_btnWaterPressure_clicked();
    void on_btnPowerConsumption_clicked();

*/




protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::Kaffeknekt *ui;

    bool sideMenuVisible;
};

#endif // MAINWINDOW_H
