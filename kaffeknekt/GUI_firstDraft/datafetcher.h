#ifndef DATAFETCHER_H
#define DATAFETCHER_H
#include <QDebug>
#include <QObject>
#include <QVector>
#include "graphview.h"    // for DataPoint

/**
 * @class DataFetcher
 * @brief Utility class for fetching sensor data from InfluxDB.
 *
 * This class provides static methods for retrieving temperature and pressure data
 * over a given time window from an InfluxDB source.
 */
class DataFetcher
{
public:
    /**
     * @brief Default constructor for DataFetcher.
     */
    DataFetcher();

    /**
     * @brief Fetch pressure data points within a given time window.
     * 
     * @param windowSeconds The size of the time window in seconds.
     * @param influxUrl URL of the InfluxDB server.
     * @param token Authentication token for accessing the InfluxDB.
     * @param bucket Name of the bucket to query data from.
     * @return QVector<DataPoint> A vector of pressure data points.
     */
    static QVector<DataPoint> fetchPressureWindow(qreal windowSeconds,
                                                  const QString& influxUrl,
                                                  const QString& token,
                                                  const QString& bucket);

    /**
     * @brief Fetch temperature data points within a given time window.
     * 
     * @param windowSeconds The size of the time window in seconds.
     * @param influxUrl URL of the InfluxDB server.
     * @param token Authentication token for accessing the InfluxDB.
     * @param bucket Name of the bucket to query data from.
     * @return QVector<DataPoint> A vector of temperature data points.
     */
    static QVector<DataPoint> fetchTempWindow(qreal windowSeconds,
                                              const QString& influxUrl,
                                              const QString& token,
                                              const QString& bucket);
};

#endif // DATAFETCHER_H
