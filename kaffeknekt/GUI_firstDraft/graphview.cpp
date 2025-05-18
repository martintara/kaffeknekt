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
    qreal w = r.width(), h = r.height();

    // X‐axis at the bottom
    m_scene->addLine(0,    0, w,    0, pen);
    // Y‐axis on the left
    m_scene->addLine(0,    0, 0,    h, pen);
   QPen axisPen(Qt::black,2);

   // X-axis (horizontal) from left to right at bottom
   m_scene->addLine(0, 0, w+40, 0, axisPen);

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

    //Optional: add right Y-axis label for something else (e.g., temperature)
   QGraphicsSimpleTextItem* rightYLabel = makeLabel("Temperature (°C)");
   rightYLabel->setRotation(90);
   QRectF ryLabelRect = rightYLabel->boundingRect();
   qDebug() << "temp place" << yLabelRect.width()/2<< w/2;
    rightYLabel->setPos(yLabelRect.width() + 850, w/2 -30);  // right of graph

        // 1) Draw the two axes
        //    left at x=0 for pressure, right at x=w for temperature
        m_scene->addLine(0, 0, 0, h, axisPen);
        m_scene->addLine(w, 0, w, h, axisPen);



        // 2) Compute min/max for each series (with a little padding)
        qreal pMin=std::numeric_limits<qreal>::max(),
              pMax=std::numeric_limits<qreal>::lowest();
        for (auto &pt : m_pressure) {
          pMin = qMin(pMin, pt.value);
          pMax = qMax(pMax, pt.value);
        }
        if (pMin>pMax) { pMin=0; pMax=1; }
        qreal pPad = (pMax-pMin)*0.1;
        pMin -= pPad;  pMax += pPad;

        qreal tMin=std::numeric_limits<qreal>::max(),
              tMax=std::numeric_limits<qreal>::lowest();
        for (auto &pt : m_temp) {
          tMin = qMin(tMin, pt.value);
          tMax = qMax(tMax, pt.value);
        }
        if (tMin>tMax) { tMin=0; tMax=1; }
        qreal tPad = (tMax-tMin)*0.1;
        tMin -= tPad;  tMax += tPad;

        // 3) Draw tick‐marks + labels
        auto drawYAxis = [&](qreal minV, qreal maxV, qreal x, const QString &label){
          int steps = 5;
          qreal range = maxV - minV;
          for (int i = 0; i <= steps; ++i) {
            qreal v = minV + (range/steps)*i;
            qreal y = (v - minV) / range * h;
            m_scene->addLine(x, y, x + (x==0? 5 : -5), y, axisPen);
            auto *lbl = makeLabel(QString::number(v, 'f', 1));
            QRectF br = lbl->boundingRect();
            lbl->setPos(x + (x==0? 8 : -br.width()-8),
                        y - br.height()/2);
          }
          // axis title
          auto *title = makeLabel(label);
          title->setRotation(90);
          QRectF tr = title->boundingRect();
          title->setPos(x + (x==0? 8 : -tr.width()-8),
                        h/2 + tr.width()/2);
        };

        drawYAxis(pMin, pMax, /*x=*/0,   QStringLiteral("Pressure (bar)"));
        drawYAxis(tMin, tMax, /*x=*/w,   QStringLiteral("Temperature (°C)"));

        // 4) X‐axis remains time at bottom
        m_scene->addLine(0, 0, w, 0, axisPen);
        int numXTicks = 6;
        qreal now = QDateTime::currentMSecsSinceEpoch()/1000.0;
        qreal t0  = now - m_windowSeconds;
        for (int i = 0; i <= numXTicks; ++i) {
          qreal t = t0 + (now-t0)*i/numXTicks;
          qreal x = w * (qreal(i)/numXTicks);
          m_scene->addLine(x, 0, x, -5, axisPen);
          auto *lbl = makeLabel(
            QDateTime::fromSecsSinceEpoch(qint64(t))
              .toString("hh:mm:ss")
          );
          QRectF br = lbl->boundingRect();
          lbl->setPos(x - br.width()/2, 8);
        }
    }



void GraphWidget::drawSeries(const QVector<DataPoint>& s, const QColor& c, qreal yOffset) {
    if (s.size() < 2)
        return;


   /*
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
    qreal yMax = (c == Qt::red)   // red = pressure
                 ? 20            // pressure rarely exceeds ~20bar
                 : 100;          // blue = temperature up to ~100°C
     // red = pressure, blue = temp

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

void GraphWidget::clearData(){
        m_pressure.clear();
        m_temp.clear();


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

