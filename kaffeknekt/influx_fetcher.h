#ifndef INFLUXFETCHER_H
#define INFLUXFETCHER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class InfluxFetcher : public QObject
{
    Q_OBJECT

public:
    explicit InfluxFetcher(QObject *parent = nullptr);
    void fetchTemperature();

signals:
    void dataReady(const QString &temperatureText);

private slots:
    void handleReply(QNetworkReply *reply);

private:
    QNetworkAccessManager networkManager;
};

#endif // INFLUXFETCHER_H
