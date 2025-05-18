                                                                                                                                                                                                                                                                            #include "graphview.h"
#include <QPen>
#include <QtMath>
#include <QPainterPath>
#include <QtGlobal>
#include <QSizePolicy>
#include <QDateTime>


// ——— Constructor & Destructor ———
GraphWidget::GraphWidget(QWidget* parent)
    : QGraphicsView(parent),
    m_scene(new QGraphicsScene(this)),
    m_timer(new QTimer(this))
{
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy  (Qt::ScrollBarAlwaysOff);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_timer->setInterval(20*1000);
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
   // m_power    = fetchPower();



    // 2) Clear & redraw
    m_scene->clear();
    drawAxes();
    drawSeries(m_temp,     Qt::red,    /*yOffset=*/ 0);
    drawSeries(m_pressure, Qt::blue,   /*yOffset=*/ -50);
    //drawSeries(m_power,    Qt::green,  /*yOffset=*/ -100);
}

void GraphWidget::drawAxes() {

    QPen pen(Qt::black); pen.setWidth(2);
    auto r = m_scene->sceneRect();
    qreal w = r.width(), h = r.height(),margin = 30;

    // X‐axis at the bottom
    m_scene->addLine(0,    0, w,    0, pen);
    // Y‐axis on the left
    m_scene->addLine(0,    0, 0,    h, pen);
   QPen axisPen(Qt::black,2);

   // X-axis (horizontal) from left to right at bottom
   m_scene->addLine(0, 0, w, 0, axisPen);

   // Y-axis (vertical) from bottom to top
   m_scene->addLine(0, 0, 0, h, axisPen);

   // Helper lambda to make text labels that are not affected by flipping
   auto makeLabel = [&](const QString& text) {
       QGraphicsSimpleTextItem* label = m_scene->addSimpleText(text);
       label->setFlag(QGraphicsItem::ItemIgnoresTransformations);  // Important for readable text
       return label;
   };

   // Add X-axis label (centered below axis)
   QGraphicsSimpleTextItem* xLabel = makeLabel("Time (s)");
   QRectF xLabelRect = xLabel->boundingRect();
    xLabel->setPos((w - xLabelRect.width()) / 2, 30);

   // Y-axis label (vertical text, rotated)
   QGraphicsSimpleTextItem* yLabel = makeLabel("Pressure (bar)");
   yLabel->setRotation(90);  // rotate vertical
   QRectF yLabelRect = yLabel->boundingRect();
   //qDebug() << "pressure place" << yLabelRect.width()/2 -10 << w/2 -10;
   yLabel->setPos(yLabelRect.width()/2 -10 , w/2 -10);  // left of Y-axis

   // Optional: add right Y-axis label for something else (e.g., temperature)
   QGraphicsSimpleTextItem* rightYLabel = makeLabel("Temperature (°C)");
   rightYLabel->setRotation(90);
   QRectF ryLabelRect = rightYLabel->boundingRect();
//   qDebug() << "temp place" << yLabelRect.width()/2<< w/2;
   rightYLabel->setPos(yLabelRect.width()/2 + 7, w/2 );  // right of graph

   int numFineTicks = 15;   // 0 to 15 (inclusive)
   int numCoarseTicks = 5;  // 20, 40, 60, 80, 100
   qreal totalTicks = numFineTicks + numCoarseTicks;  // 21 total levels
   qreal tickSpacing = h / totalTicks;

   for (int i = 0; i <= numFineTicks; ++i) {
       qreal y = i * tickSpacing;
       m_scene->addLine(0, y, 5, y, QPen(Qt::black));

       auto* label = m_scene->addSimpleText(QString::number(i));  // 0 to 15
       label->setFlag(QGraphicsItem::ItemIgnoresTransformations);
       QRectF br = label->boundingRect();
       label->setPos(br.width(), y + br.height() / 2);  // Left of Y-axis
   }

   for (int j = 1; j <= numCoarseTicks; ++j) {
       qreal y = (numFineTicks + j) * tickSpacing;
       int value = j * 20;  // 20, 40, ..., 100
       m_scene->addLine(0, y, 5, y, QPen(Qt::black));

       auto* label = m_scene->addSimpleText(QString::number(value));
       label->setFlag(QGraphicsItem::ItemIgnoresTransformations);
       QRectF br = label->boundingRect();
       label->setPos(br.width(), y + br.height() / 2);  // Left of Y-axis
   }


int numXTicks = 12;
qreal now = QDateTime::currentMSecsSinceEpoch() / 1000.0;
qreal windowStart = now - 10;
qreal windowEnd = now;

for (int i = 0; i <= numXTicks; ++i) {
    qreal t = windowStart + i * (windowEnd - windowStart) / numXTicks;
    qreal x = i * (w / numXTicks);

    // Draw the tick
    m_scene->addLine(x, 0, x, -5, QPen(Qt::black));

    // Convert t (in seconds) to readable time string
    QDateTime time = QDateTime::fromSecsSinceEpoch(static_cast<qint64>(t));
    QString timeText = time.toString("hh:mm:ss");  // or "mm:ss"

    // Create the label
    auto* label = m_scene->addSimpleText(timeText);
    label->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    QRectF br = label->boundingRect();
    label->setPos(x - br.width() / 2, 20);  // Below tick
}

}

void GraphWidget::drawSeries(const QVector<DataPoint>& s, const QColor& c, qreal yOffset) {
    if (s.size() < 2)
        return;

   /* qDebug() << "   drawSeries() first point at"
             << s[0].timestamp << s[0].value
             << "offset=" <<yOffset;
|rv
    if (s.size() < 2)
        return;
    QPainterPath path;
    path.moveTo(s[0].timestamp, s[0].value + yOffset);
    for (int i = 1; i < s.size(); ++i)
        path.lineTo(s[i].timestamp, s[i].value + yOffset);
    m_scene->addPath(path, QPen(c, 1.5));*/
    qreal w = m_scene->width();
    qreal h = m_scene->height();

    qreal xMin = s.first().timestamp;
    qreal xMax = s.last().timestamp;
    qreal xRange = xMax - xMin;

    // Safety
    if (xRange == 0) xRange = 1;

    // Use full scene height for scaling Y (example ranges)
    qreal yMin = 0;
    qreal yMax = (c == Qt::red) ? 20 : 120;  // red = pressure, blue = temp

    auto mapX = [&](qreal t) {
        return (t - xMin) / xRange * w;
    };
    auto mapY = [&](qreal v) {
        return (v - yMin) / (yMax - yMin) * h;
    };

    QPainterPath path;
    path.moveTo(mapX(s[0].timestamp), mapY(s[0].value + yOffset));
    for (int i = 1; i < s.size(); ++i)
        path.lineTo(mapX(s[i].timestamp), mapY(s[i].value + yOffset));

    m_scene->addPath(path, QPen(c, 1.5));

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




//de nye endringeen siden jeg fhjerna de fra grapgdiaialog og slkengte de direkte i graphview ++ forklar mer senere :)

// ─── NEW PUBLIC API ───

// Legger til ett data‐punkt i m_pressure
void GraphWidget::appendPressurePoint(const DataPoint& p) {
    m_pressure.push_back(p);
// remove any points older than our current window
// remove any points older than our current window
    qreal cutoff = p.timestamp - m_windowSeconds;
    while (!m_pressure.isEmpty() && m_pressure.first().timestamp < cutoff) {
        m_pressure.removeFirst();
    }
}
// Legger til ett data‐punkt i m_temp
void GraphWidget::appendTempPoint(const DataPoint& t) {
    m_temp.push_back(t);
    qreal cutoff = t.timestamp - m_windowSeconds;
    while (!m_temp.isEmpty() && m_temp.first().timestamp < cutoff) {
        m_temp.removeFirst();
    }
}

// Tegner alt på nytt (clear + aksser + alle kurver)
void GraphWidget::refresh() {

    m_scene->clear();
    drawAxes();
    drawSeries(m_temp,     Qt::blue,  0);
    drawSeries(m_pressure, Qt::red,   0);

}

void GraphWidget::resizeEvent(QResizeEvent* event) {



// 1) clear out any old scale/translate
    resetTransform();
    // 2) flip the vertical axis
    scale(1, -1);
    // 3) move the origin down to the bottom of the view
    translate(0, -viewport()->height());
    // 4) make the scene cover exactly the widget’s area, now in positive coords
    QSize s = event->size();
    m_scene->setSceneRect(0, 0, s.width(), s.height());
}

