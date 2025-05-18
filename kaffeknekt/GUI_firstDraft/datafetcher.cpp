// DataFetcher.cpp
#include "datafetcher.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QObject>

QVector<DataPoint> DataFetcher::fetchPressureWindow(qreal windowSeconds,
                                                    const QString& influxUrl,
                                                    const QString& token,
                                                    const QString& bucket)
{
    // 1) Build your GET URL (bucket + range)
    QUrl url(influxUrl
             + QString("&bucket=%1").arg(QUrl::toPercentEncoding(bucket))
             + QString("&range=%1s").arg(windowSeconds));
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", "Token " + token.toUtf8());
    req.setHeader( QNetworkRequest::ContentTypeHeader,
                   "application/vnd.flux; charset=utf-8" );
    req.setRawHeader("Accept", "application/csv");

    // 2) Flux query: only ESp32Metrics measurement, pressure field
    QByteArray fluxQuery = (R"(
    option start = -1h
option stop  = now()
option windowPeriod = %1s
from(bucket:"sensor_data")
  |> range(start: start, stop: stop)
  |> filter(fn: (r) => r._measurement == "Esp32Metrics")
  |> filter(fn: (r) => r._field == "pressure")
  |> aggregateWindow(every: windowPeriod, fn: mean, createEmpty: false)
  |> yield()

)");

    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.post(req, fluxQuery);

    // 3) Wait synchronously
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // 4) Read + dump
    QByteArray body = reply->readAll();


    QVector<DataPoint> out;

    // after you read all into `body`:
    QStringList lines = QString::fromUtf8(body).split('\n', Qt::SkipEmptyParts);
    if (lines.size() < 2) {
        qWarning() << "No data rows!";
        return out;
    }

    // 1) parse the header from line 0
    QStringList header = lines[0].split(',', Qt::KeepEmptyParts);

    // 2) find the indexes
    int idxTime  = header.indexOf("_time");
    int idxValue = header.indexOf("_value");
    if (idxTime < 0 || idxValue < 0) {
        qWarning() << "Couldn't find _time/_value in header:" << header;
        return out;
    }

    // 3) parse each subsequent row
    for (int i = 1; i < lines.size(); ++i) {
        const QString& line = lines[i].trimmed();
        if (line.isEmpty()) continue;

        QStringList cols = line.split(',', Qt::KeepEmptyParts);
        if (cols.size() <= qMax(idxTime, idxValue)) continue;

        // parse ISO timestamps
        QDateTime dt = QDateTime::fromString(cols[idxTime], Qt::ISODate);
        if (!dt.isValid()) {
            qWarning() << "Bad date parse:" << cols[idxTime];
            continue;
        }
        qreal t = dt.toMSecsSinceEpoch() / 1000.0;
        qreal v = cols[idxValue].toDouble();

        qDebug() << "[DataFetcher] point → t =" << t << ", v =" << v;
        out.append({ t, v });
    }

    reply->deleteLater();
    return out;

}

QVector<DataPoint> DataFetcher::fetchTempWindow(qreal windowSeconds,
                                                const QString& influxUrl,
                                                const QString& token,
                                                const QString& bucket) {

    // 1) Build your GET URL (bucket + range)
    QUrl url(influxUrl
             + QString("&bucket=%1").arg(QUrl::toPercentEncoding(bucket))
             + QString("&range=%1s").arg(windowSeconds));
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", "Token " + token.toUtf8());
    req.setHeader( QNetworkRequest::ContentTypeHeader,
                   "application/vnd.flux; charset=utf-8" );
    req.setRawHeader("Accept", "application/csv");

    // 2) Flux query: only ESp32Metrics measurement, pressure field
    QByteArray fluxQuery = (R"(
    option start = -1h
option stop  = now()
option windowPeriod = 1m

  from(bucket:"sensor_data")
  |> range(start: start, stop: stop)
  |> filter(fn: (r) => r._measurement == "Esp32Metrics")
  |> filter(fn: (r) => r._field == "temperature")
  |> aggregateWindow(every: windowPeriod, fn: mean, createEmpty: false)
  |> yield()

)");

    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.post(req, fluxQuery);

    // 3) Wait synchronously
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // 4) Read + dump
    QByteArray body = reply->readAll();
    QVector<DataPoint> out;
    qDebug() << "[DataFetcher] raw CSV response:\n" << body;

    QStringList lines = QString::fromUtf8(body).split('\n', Qt::SkipEmptyParts);
    if (lines.size() < 2) {
        qWarning() << "No data rows!";
        return out;
    }

    // 1) parse the header from line 0
    QStringList header = lines[0].split(',', Qt::KeepEmptyParts);

    // 2) find the indexes
    int idxTime  = header.indexOf("_time");
    int idxValue = header.indexOf("_value");
    if (idxTime < 0 || idxValue < 0) {
        qWarning() << "Couldn't find _time/_value in header:" << header;
        return out;
    }

    // 3) parse each subsequent row
    for (int i = 1; i < lines.size(); ++i) {
        const QString& line = lines[i].trimmed();
        if (line.isEmpty()) continue;

        QStringList cols = line.split(',', Qt::KeepEmptyParts);
        if (cols.size() <= qMax(idxTime, idxValue)) continue;

        // parse ISO timestamps
        QDateTime dt = QDateTime::fromString(cols[idxTime], Qt::ISODate);
        if (!dt.isValid()) {
            qWarning() << "Bad date parse:" << cols[idxTime];
            continue;
        }
        qreal t = dt.toMSecsSinceEpoch() / 1000.0;
        qreal v = cols[idxValue].toDouble();

        qDebug() << "[DataFetcher] point → t =" << t << ", v =" << v;
        out.append({ t, v });
    }

    reply->deleteLater();
    return out;


}








