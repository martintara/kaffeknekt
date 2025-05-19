// WebSocketClient.h
#pragma once

#include <QThread>
#include <QString>

/**
 * @class WebSocketClient
 * @brief Threaded client for receiving real-time data via WebSocket.
 *
 * This class runs in a separate thread and is responsible for
 * connecting to a WebSocket server to receive live pressure,
 * temperature, and flag data.
 */
class WebSocketClient : public QThread {
    Q_OBJECT

public:
    /**
     * @brief Constructs the WebSocketClient.
     * @param parent Optional parent QObject.
     */
    explicit WebSocketClient(QObject* parent = nullptr);

    /**
     * @brief Runs the thread logic.
     * Override this method to implement the WebSocket communication.
     */
    void run() override;

signals:
    /**
     * @brief Emitted when new sensor data is received.
     * @param pressure The pressure value.
     * @param temperature The temperature value.
     * @param flag A string-based flag or status indicator.
     */
    void dataReceived(double pressure, double temperature, const QString& flag);
};
