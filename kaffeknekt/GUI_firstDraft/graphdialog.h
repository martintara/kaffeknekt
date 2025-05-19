//graphdialog.h
#ifndef GRAPHDIALOG_H
#define GRAPHDIALOG_H

#include <QDialog>
#include <QSizePolicy>

#include "graphview.h"
#include "websocketclient.h"

/**
 * @namespace Ui
 * @brief Qt namespace for UI classes generated from .ui files.
 */
namespace Ui { class graphDialog; }

/**
 * @class graphDialog
 * @brief Dialog window for displaying real-time pressure and temperature graphs.
 *
 * This class handles data visualization using a GraphWidget, and receives
 * real-time updates via WebSocketClient. It also emits signals on show/hide
 * events and when a specific flag is sent.
 */
class graphDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructs a graphDialog window.
     * @param parent The parent widget.
     */
    explicit graphDialog(QWidget *parent = nullptr);

    /**
     * @brief Destroys the graphDialog instance.
     */
    ~graphDialog();

    /**
     * @brief Sets the time window (in seconds) to display on the graph.
     * @param seconds Length of the time window.
     */
    void setWindowSeconds(qreal seconds) { m_windowSeconds = seconds; }

signals:
    /**
     * @brief Emitted when the dialog is shown.
     */
    void dialogShown();

    /**
     * @brief Emitted when the dialog is hidden.
     */
    void dialogHidden();

    /**
     * @brief Emitted when a flag is sent.
     */
    void flagsent();

public slots:
    /**
     * @brief Appends pressure and temperature data to the internal buffer.
     * @param pressure The new pressure reading.
     * @param temperature The new temperature reading.
     */
    void appendData(double pressure, double temperature);

    /**
     * @brief Handles data received from the WebSocketClient.
     * @param pressure The received pressure value.
     * @param temperature The received temperature value.
     * @param flag A string flag received with the data.
     */
    void onDataReceived(double pressure,
                        double temperature,
                        const QString& flag);

private:
    Ui::graphDialog *ui;               ///< Pointer to the UI components.
    GraphWidget     *m_graph;          ///< Widget used to plot the data.
    WebSocketClient *m_wsClient;       ///< WebSocket client for real-time data.

    QVector<DataPoint> m_pressure;     ///< Buffer for pressure data points.
    QVector<DataPoint> m_temp;         ///< Buffer for temperature data points.
    qreal               m_windowSeconds = 600.0; ///< Display window size in seconds.

protected:
    /**
     * @brief Handles the event when the dialog is shown.
     * @param ev Pointer to the QShowEvent.
     */
    void showEvent(QShowEvent* ev) override;

    /**
     * @brief Handles the event when the dialog is hidden.
     * @param ev Pointer to the QHideEvent.
     */
    void hideEvent(QHideEvent* ev) override;
};

#endif // GRAPHDIALOG_H
