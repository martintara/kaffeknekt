//mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sensoranalyticsdialog.h"
#include "coffeeinstructionsdialog.h"
#include "graphview.h"
#include "warningdialog.h"
#include "websocketclient.h"
#include "graphdialog.h"
#include <QElapsedTimer>
#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "statistics.h"
#include "ui_mainwindow.h"
#include "infodetaildialog.h"
#include "datafetcher.h"
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
    void hideInfoFrame();
    void on_flagsent();

private slots:
    // For hovedmeny (frame_1) som egt frame
    void on_btnHamburger_clicked();
   // void on_btnHome_clicked();
    void on_btnSettings_clicked();
    void on_btnInfo_clicked();

    // For undermeny (frame_2)
    void on_btnInstructions_clicked();
    void on_btnSensorAnalytics_clicked();
    void on_btnStatistics_clicked();
    void on_btnHere_clicked();//i frame_3

    //funksjonen for testing av Warning dialogen/n poppuppen.
    void on_btnTestWarning_clicked();


protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::Kaffeknekt *ui;
    GraphWidget    *m_graph;
    Statistics          *m_statsDialog;
    SensorAnalyticsDialog *m_saDialog;// our real-time graph view
    WebSocketClient       *m_ws;
    bool sideMenuVisible;
    int    m_cupCount        = 0;
    QElapsedTimer m_lastwarningTime;
    bool   m_warningShown     = false;
    //WebSocketClient *m_ws;
    graphDialog     *m_graphDialog;

   bool analytics_clicked;
};

#endif // MAINWINDOW_H
