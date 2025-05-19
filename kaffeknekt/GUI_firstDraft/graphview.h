#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>

#include <QResizeEvent>
#include <QGraphicsSimpleTextItem>

/**
 * @struct DataPoint
 * @brief Represents a single data sample with a timestamp and a value.
 */
struct DataPoint {
    qreal timestamp;  ///< Time of the data point (seconds since epoch or relative).
    qreal value;      ///< Value of the data point (e.g., temperature or pressure).
};

/**
 * @class GraphWidget
 * @brief A custom graphics view for plotting pressure and temperature data in real-time.
 *
 * This widget displays two time-series graphs (pressure and temperature) and
 * periodically updates to reflect new incoming data.
 */
class GraphWidget : public QGraphicsView {
    Q_OBJECT
public:
    /**
     * @brief Constructs a GraphWidget.
     * @param parent The parent widget.
     */
    explicit GraphWidget(QWidget* parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~GraphWidget();

    /**
     * @brief Starts the periodic data refresh timer.
     */
    void start();

    /**
     * @brief Sets the size of the time window (in seconds) for graph display.
     * @param seconds Length of the display window in seconds.
     */
    void setWindowSeconds(qreal seconds) { m_windowSeconds = seconds; }

    /**
     * @brief Appends a new pressure data point.
     * @param p The pressure data point to append.
     */
    void appendPressurePoint(const DataPoint& p);

    /**
     * @brief Appends a new temperature data point.
     * @param t The temperature data point to append.
     */
    void appendTempPoint(const DataPoint& t);

    /**
     * @brief Redraws the entire graph: clears scene, draws axes and data series.
     */
    void refresh();

    /**
     * @brief Clears all stored data points and the graph display.
     */
    void clearData();

    /**
     * @brief Draws the axes on the graph.
     */
    void drawAxes();

    /**
     * @brief Draws a time series on the graph with a given color and Y offset.
     * @param series The data series to draw.
     * @param penColor The color of the graph line.
     * @param yOffset Vertical offset for the series.
     */
    void drawSeries(const QVector<DataPoint>& series,
                    const QColor& penColor,
                    qreal yOffset);

private slots:
    /**
     * @brief Called periodically to fetch new data and update the graph.
     */
    void fetchAndRedraw();

protected:
    /**
     * @brief Handles resizing of the widget to adjust the graph accordingly.
     * @param event Resize event.
     */
    void resizeEvent(QResizeEvent* event) override;

private:
    QGraphicsScene* m_scene;     ///< Scene for rendering the graph.
    QTimer*         m_timer;     ///< Timer for triggering periodic updates.

    QVector<DataPoint> m_pressure; ///< Stored pressure data points.
    QVector<DataPoint> m_temp;     ///< Stored temperature data points.

    qreal m_windowSeconds = 600.0; ///< Time window for visible data (default 10 minutes).
};

#endif // GRAPHWIDGET_H
