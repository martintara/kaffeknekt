// WebSocketClient.h
#pragma once
#include <QThread>
#include <QString>

class WebSocketClient : public QThread {
    Q_OBJECT
public:
    explicit WebSocketClient(QObject* parent = nullptr);
    void run() override;

signals:
    void dataReceived(double pressure, double temperature, const QString& flag);
};
