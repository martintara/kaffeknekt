//graphview.h
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

    // Start the periodic updates
    void start();

    void setWindowSeconds(qreal seconds) { m_windowSeconds = seconds; }

    // append one point at a time
    void appendPressurePoint(const DataPoint& p);
    void appendTempPoint    (const DataPoint& t);

       // redraw everything (clear + axes + both series)
    void refresh();
    void clearData();

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


    //2 parametere somm skal vises:
    QVector<DataPoint> m_pressure;
    QVector<DataPoint> m_temp;


    //QVector<DataPoint> fetchPower();

    qreal                 m_windowSeconds = 600.0;  // sliding-window in seconds aka 10min

};

#endif // GRAPHWIDGET_H
