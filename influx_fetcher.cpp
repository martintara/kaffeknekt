#include "influx_fetcher.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>



InfluxFetcher::InfluxFetcher(QObject *parent)
    : QObject(parent)
{
    connect(&networkManager, &QNetworkAccessManager::finished, this, &InfluxFetcher::handleReply);
}
/*
void InfluxFetcher::fetchTemperature()
{
    QUrl url("http://localhost:8086/api/v2/query");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Token m2c2aZ6p9gIrV1_B-unhWO9Jdue0WxEInVLde3OauCi9blskPEbXbbr6kC7rFd3EMYEqwEV6ypDc4C4UqRhMGA==");

    QByteArray body = R"RAW(
    {
      "query": "from(bucket: \"sensor_data\") |> range(start: -10m) |> filter(fn: (r) => r._measurement == \"Esp32Metrics\" and r._field == \"temperature_C\") |> last()",
      "dialect": {
        "annotations": ["group", "datatype", "default"],
        "delimiter": ",",
        "header": true
      },
      "org": "Kaffeknekt"
    }
    )RAW";

    qDebug() << " Sender forespørsel til InfluxDB...";
    qDebug() << "Request body:";
    qDebug().noquote() << body;


    networkManager.post(request, body);
}*/

void InfluxFetcher::handleReply(QNetworkReply *reply)
{/*
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        qDebug() << "✅ InfluxDB response mottatt:\n";
        qDebug().noquote() << response;

        // Finn siste temperaturverdi (CSV-format)
        QList<QByteArray> lines = response.split('\n');
        if (lines.size() > 4) {
            QStringList fields = QString(lines.last()).split(',');
            if (fields.size() > 5) {
                QString temp = fields.last();
                emit dataReady("Siste temperatur: " + temp + " °C");
            }
        } else {
            emit dataReady("Ingen data funnet.");
        }
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        qDebug() << "Svar mottatt fra InfluxDB:";
        qDebug().noquote() << response;
        emit dataReady("Svar mottatt, se konsoll");
    } /*else {
        qWarning() << "Feil ved henting:" << reply->errorString();
        QByteArray response = reply->readAll();
        qDebug().noquote() << response;  // ← vis hele feilmeldingen fra InfluxDB!
        emit dataReady("Feil ved henting av data.");
    }///ny kode(((
    if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            qDebug().noquote() << "✅ Svar mottatt fra InfluxDB:\n" << response;

            // Split opp CSV til linjer
            QList<QByteArray> lines = response.split('\n');

            if (lines.size() > 4) {
                QByteArray lastLine = lines.last();
                if (lastLine.trimmed().isEmpty() && lines.size() > 5)
                    lastLine = lines.at(lines.size() - 2);  // Hopp over tom linje

                QStringList fields = QString(lastLine).split(',');

                if (fields.size() >= 8) {
                    QString tempValue = fields.at(6).trimmed();  // Verdi for temperatur
                    emit dataReady("Siste temperatur: " + tempValue + " °C");
                } else {
                    emit dataReady("⚠️ Klarte ikke hente verdi fra linjen.");
                }
            }
    }*/
    QByteArray response = reply->readAll();
    qDebug().noquote() << " Svar mottatt fra InfluxDB:\n" << response;

    QList<QByteArray> lines = response.split('\n');
    // Gå baklengs og finn første linje som ikke er tom og ikke starter med '#'
    QByteArray dataLine;
    for (int i = lines.size() - 1; i >= 0; --i) {
        QByteArray line = lines.at(i).trimmed();
        if (!line.isEmpty() && !line.startsWith('#')) {
            dataLine = line;
            break;
        }
    }

    if (!dataLine.isEmpty()) {
        QStringList fields = QString(dataLine).split(',');

        if (fields.size() >= 7) {
            QString tempValue = fields.at(6).trimmed();  // _value ligger på index 6
            emit dataReady("Siste temperatur: " + tempValue + " °C");
        } else {
            emit dataReady(" Klarte ikke tolke verdier i linjen.");
        }
    } else {
        emit dataReady("Ingen datalinje funnet i svaret.");
    }/*
    else {
        qWarning() << "Feil ved henting:" << reply->errorString();
        emit dataReady("Feil ved henting av data.");
    }*/
    reply->deleteLater();
}
//DEN NYE FETCH INFLUX:
void InfluxFetcher::fetchTemperature()
{
    //QUrl url("http://localhost:8086/api/v2/query");
    QUrl url("http://localhost:8086/api/v2/query?org=Kaffeknekt");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");  //""
    request.setRawHeader("Authorization", "Token gYOa4uSF29eaZZHM2lu7gklxe_gGa-IcWO1RLSK4JRP05nCImvw-ZMcT71wO9Gb8LyhseQ2B_LAiExHl-lOAGQ==");

    // Bygg JSON korrekt
    QJsonObject dialect;
    dialect["annotations"] = QJsonArray({"group", "datatype", "default"});
    dialect["delimiter"] = ",";
    dialect["header"] = true;

    QJsonObject json;
    json["query"] = R"(from(bucket: "sensor_data") |> range(start: -2880m) |> filter(fn: (r) => r._measurement == "Esp32Metrics" and r._field == "temperature_C") |> last())";
    json["dialect"] = dialect;
    json["org"] = "Kaffeknekt";
    json["type"] = "flux";

    QJsonDocument doc(json);
    QByteArray body = doc.toJson();

    qDebug() << "Sender forespørsel til InfluxDB...\n";
    qDebug().noquote() << body;

    networkManager.post(request, body);
}
