#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>

// Simple struct for a time-value sample
struct DataPoint {
    qreal timestamp;  // seconds since epoch, or relative t=0
    qreal value;
};

class GraphWidget : public QGraphicsView {
    Q_OBJECT
public:
    explicit GraphWidget(QWidget* parent = nullptr);
    ~GraphWidget();

    // Start the periodic updates
    void start();

private slots:
    void fetchAndRedraw();

private:
    QGraphicsScene* m_scene;
    QTimer*         m_timer;

    // Your three series of points
    QVector<DataPoint> m_temp, m_pressure, m_power;

    // Helpers
    void drawAxes();
    void drawSeries(const QVector<DataPoint>& series, const QColor& color, qreal yOffset);

    // These should call into your existing Influx-HTTP functions:
    QVector<DataPoint> fetchTemp();
    QVector<DataPoint> fetchPressure();
    QVector<DataPoint> fetchPower();
};

#endif // GRAPHWIDGET_H
