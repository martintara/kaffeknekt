#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sensorAnalyticsdialog.h"


#include <QMainWindow>

#include "ui_mainwindow.h"

//neste steg er sub-sub side meny for "esnsor analytics/btnTimescale NB. skalfikses!"
#include <QFrame>             // ADDED: QFrame for sidemenyen
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class tullegrafer;
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
    void on_btnInstruction_clicked();
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
    Ui::tullegrafer *ui;

    bool sideMenuVisible;
};

#endif // MAINWINDOW_H


/*
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"

#include <QMainWindow>
//neste steg er sub-sub side meny for "ensor analytics/btnTimescale NB. skalfikses!"
#include <QFrame>             // ADDED: QFrame for sidemenyen
#include <QPushButton>        // ADDED: QPushButton for valgene

QT_BEGIN_NAMESPACE
namespace Ui {
class tullegrafer;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    //for første framen:
    void on_btnHamburger_clicked();
    void on_btnHome_clicked();
    void on_btnSettings_clicked();
    void on_btnInfo_clicked();

    //for andre framen:
    void on_btnInstruction_clicked();
    void on_btnSensorAnalytics_clicked();
    void on_btnStatistics_clicked();
    //void on_btnTimescale_clicked();



    //knappene for frame_3:
    void on_btnWaterPressure_clicked();
    void on_btnPowerConsumption_clicked();
    void on_btnWaterTemp_clicked();

protected:
    //void resizeEvetimescalent(QResizeEvent *event) override;


    void showEvent(QShowEvent *event) override;

private:
    Ui::tullegrafer *ui;


    bool sideMenuVisible;
    //bool timescaleMenuVisible = false;     // ADDED: tilstand for timescale-submenyen -> bytt med SensorAnalytics!!

    bool sensorAnalyticsMenuVisible = false;     // ADDED: tilstand for timescale-submenyen
/*
    // ADDED: widgets for timescale-submenyen
    QFrame* sensorAnalyticsFrame = nullptr;
    QPushButton* btnWaterTemp = nullptr;
    QPushButton* btnWaterPressure = nullptr;
    QPushButton* btnPowerConsumption = nullptr;

    //legger til frame_3:
    //QFrame *frame_3;

};
#endif // MAINWINDOW_H
*/
