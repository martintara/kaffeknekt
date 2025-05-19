#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sensoranalyticsdialog.h"
#include "coffeeinstructionsdialog.h"
#include "graphview.h"
#include "warningdialog.h"
#include "websocketclient.h"
#include "graphdialog.h"
#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "statistics.h"
#include "ui_mainwindow.h"
#include "infodetaildialog.h"
#include "datafetcher.h"
#include <QFrame>
#include <QPushButton>

QT_BEGIN_NAMESPACE
/**
 * @namespace Ui
 * @brief Qt namespace containing UI classes generated from .ui files.
 */
namespace Ui {
class Kaffeknekt;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief The main application window for the Kaffeknekt dashboard.
 *
 * This class controls the main GUI, manages views such as graphs, settings, and instructions,
 * and connects to real-time data via WebSocket. It also handles various UI interactions.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the main window.
     * @param parent Optional parent widget.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~MainWindow();

    /**
     * @brief Hides the informational side frame (if visible).
     */
    void hideInfoFrame();

    /**
     * @brief Slot to handle when a flag is sent (e.g., for a warning or state change).
     */
    void on_flagsent();

private slots:
    // Frame 1 (main menu)
    /**
     * @brief Slot triggered when the hamburger menu button is clicked.
     */
    void on_btnHamburger_clicked();

    /**
     * @brief Slot triggered when the home button is clicked.
     */
    void on_btnHome_clicked();

    /**
     * @brief Slot triggered when the settings button is clicked.
     */
    void on_btnSettings_clicked();

    /**
     * @brief Slot triggered when the info button is clicked.
     */
    void on_btnInfo_clicked();

    // Frame 2 (submenu)
    /**
     * @brief Slot triggered when the instructions button is clicked.
     */
    void on_btnInstructions_clicked();

    /**
     * @brief Slot triggered when the sensor analytics button is clicked.
     */
    void on_btnSensorAnalytics_clicked();

    /**
     * @brief Slot triggered when the statistics button is clicked.
     */
    void on_btnStatistics_clicked();

    /**
     * @brief Slot triggered when the "Here" button inside info frame is clicked.
     */
    void on_btnHere_clicked();

    /**
     * @brief Slot triggered when the test warning button is clicked.
     * Used for testing the warning popup.
     */
    void on_btnTestWarning_clicked();

protected:
    /**
     * @brief Handles the event when the main window is shown.
     * @param event Pointer to the QShowEvent.
     */
    void showEvent(QShowEvent *event) override;

private:
    Ui::Kaffeknekt *ui;                         ///< Pointer to UI components.
    GraphWidget    *m_graph;                    ///< Graph widget for data display.
    Statistics          *m_statsDialog;         ///< Dialog for showing statistics.
    SensorAnalyticsDialog *m_saDialog;          ///< Dialog for real-time analytics.
    WebSocketClient       *m_ws;                ///< WebSocket client for real-time data.
    bool sideMenuVisible;                       ///< Tracks visibility of side menu.
    int    m_cupCount        = 0;               ///< Tracks the number of cups (usage metric).
    bool   m_warningShown     = false;          ///< Whether a warning dialog has been shown.
    graphDialog     *m_graphDialog;             ///< Dialog for graph display.
};

#endif // MAINWINDOW_H
