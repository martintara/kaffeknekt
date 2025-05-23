#ifndef DATAFETCHER_H
#define DATAFETCHER_H
#include <QDebug>
#include <QObject>
#include <QVector>
#include "graphview.h"    // for DataPoint
class DataFetcher
{
public:
    DataFetcher();
    static QVector<DataPoint> fetchPressureWindow(qreal windowSeconds,
                                                  const QString& influxUrl,
                                                  const QString& token,
                                                  const QString& bucket);
    static QVector<DataPoint> fetchTempWindow(qreal windowSeconds,
                                              const QString& influxUrl,
                                              const QString& token,
                                              const QString& bucket);
};

#endif // DATAFETCHER_H
