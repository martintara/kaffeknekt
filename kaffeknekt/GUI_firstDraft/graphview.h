#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>

#include <QResizeEvent>
#include <QGraphicsSimpleTextItem>


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
    // Legg til:
    /*
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(500, 300);    // juster etter behov*/

    // Start the periodic updates
    void start();

    // append one point at a time
    void appendPressurePoint(const DataPoint& p);
    void appendTempPoint    (const DataPoint& t);

       // redraw everything (clear + axes + both series)
    void refresh();

       // you probably already have these:
    void drawAxes();
    void drawSeries(const QVector<DataPoint>& series,
                       const QColor& penColor,
                       qreal yOffset);
private slots:
    void fetchAndRedraw();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    QGraphicsScene* m_scene;
    QTimer*         m_timer;

    // Your three series of points
    //QVector<DataPoint> m_temp, m_pressure, m_power;

    //2 parametere somm skal vises:
    QVector<DataPoint> m_pressure;
    QVector<DataPoint> m_temp;
    /*
    // Helpers
    void drawAxes();
    void drawSeries(const QVector<DataPoint>& series, const QColor& color, qreal yOffset);
*/
    // These should call into your existing Influx-HTTP functions:
    QVector<DataPoint> fetchTemp();
    QVector<DataPoint> fetchPressure();
    //QVector<DataPoint> fetchPower();
};

#endif // GRAPHWIDGET_H
