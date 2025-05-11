#include "graphview.h"
#include <QPen>
#include <QtMath>
#include <QPainterPath>
#include <QtGlobal>


// ——— Constructor & Destructor ———
GraphWidget::GraphWidget(QWidget* parent)
    : QGraphicsView(parent),
    m_scene(new QGraphicsScene(this)),
    m_timer(new QTimer(this))
{
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);

    // 25 s interval
    m_timer->setInterval(25 * 1000);
    connect(m_timer, &QTimer::timeout, this, &GraphWidget::fetchAndRedraw);
}

GraphWidget::~GraphWidget() = default;

void GraphWidget::start() {
    fetchAndRedraw();
    m_timer->start();
}

// ——— Data fetch & redraw ———
void GraphWidget::fetchAndRedraw() {
    // 1) Fetch
    m_temp     = fetchTemp();
    m_pressure = fetchPressure();
    m_power    = fetchPower();

    // 2) Clear & redraw
    m_scene->clear();
    drawAxes();
    drawSeries(m_temp,     Qt::red,    /*yOffset=*/ 0);
    drawSeries(m_pressure, Qt::blue,   /*yOffset=*/ -50);
    drawSeries(m_power,    Qt::green,  /*yOffset=*/ -100);
}

// ——— Drawing helpers ———
void GraphWidget::drawAxes() {
    QPen axisPen(Qt::black);
    axisPen.setWidth(2);
    // X-axis
    m_scene->addLine(0, 0, 800, 0, axisPen);
    // Y-axis
    m_scene->addLine(0, 0, 0, -200, axisPen);
}

void GraphWidget::drawSeries(const QVector<DataPoint>& s, const QColor& c, qreal yOffset) {
    if (s.size() < 2) return;

    QPainterPath path;
    path.moveTo(s[0].timestamp, s[0].value + yOffset);
    for (int i = 1; i < s.size(); ++i) {
        path.lineTo(s[i].timestamp, s[i].value + yOffset);
    }
    QGraphicsPathItem* item = m_scene->addPath(path, QPen(c, 1.5));
    Q_UNUSED(item);
}

// ——— Stubbed fetch-functions ———
// Replace these bodies with calls into your HTTP/Influx code:
QVector<DataPoint> GraphWidget::fetchTemp() {
    QVector<DataPoint> v;
    // … call your Influx client, parse into DataPoint list …
    return v;
}
QVector<DataPoint> GraphWidget::fetchPressure() {
    QVector<DataPoint> v;
    return v;
}
QVector<DataPoint> GraphWidget::fetchPower() {
    QVector<DataPoint> v;
    return v;
}
