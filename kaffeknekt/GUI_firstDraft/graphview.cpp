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
    QPen axisPen(Qt::black); axisPen.setWidth(2);
    auto r = m_scene->sceneRect();
    qreal w = r.width(), h = r.height();
    qDebug() << "width and height" << w << h;
    m_scene->addLine(0, 0, w, 0, axisPen);     // X-axis (bottom)
    m_scene->addLine(0, 0, 0, h, axisPen);     // Left Y-axis (pressure)
    m_scene->addLine(w, 0, w, h, axisPen);     // Right Y-axis (temperature)

       // X AXIS
    auto* xLabel = m_scene->addSimpleText("Time (s)");
        xLabel->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        QRectF xRect = xLabel->boundingRect();
        xLabel->setPos(w / 2, -10);


        // --- Y-axis left label (Pressure) ---
        auto* yLabelLeft = m_scene->addSimpleText("Pressure (bar)");
        yLabelLeft->setBrush(Qt::red);
        yLabelLeft->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        yLabelLeft->setRotation(90);
        QRectF yLeftRect = yLabelLeft->boundingRect();
        yLabelLeft->setPos(-2, h / 2);  // x=10 (inside left axis)

        // --- Y-axis right label (Temperature) ---
            auto* yLabelRight = m_scene->addSimpleText("Temperature (°C)");
            yLabelRight->setBrush(Qt::blue);
            yLabelRight->setFlag(QGraphicsItem::ItemIgnoresTransformations);
            yLabelRight->setRotation(90);
            QRectF yRightRect = yLabelRight->boundingRect();
            yLabelRight->setPos(w - 40, h/ 2);  // near right edge


            for (int i = 0; i <= 12; ++i) {
                qreal y = (h / 12.0) * i;

                // Left Y ticks
                m_scene->addLine(0, y, 5, y, axisPen);
                auto* tickLeft = m_scene->addSimpleText(QString::number(i));
                tickLeft->setFlag(QGraphicsItem::ItemIgnoresTransformations);
                tickLeft->setPos(8, y - 8);

                // Right Y ticks
                /*
                m_scene->addLine(w, y, w - 5, y, axisPen);
                auto* tickRight = m_scene->addSimpleText(QString::number(i * 10 + 20));  // just example values
                tickRight->setFlag(QGraphicsItem::ItemIgnoresTransformations);
                tickRight->setPos(w - 40, y - 8);*/

                int tempValue = 0 + i * 10;  // 10, 20, ..., 120
                m_scene->addLine(w, y, w - 5, y, axisPen);
                auto* tickRight = m_scene->addSimpleText(QString::number(tempValue));
                tickRight->setFlag(QGraphicsItem::ItemIgnoresTransformations);
                tickRight->setPos(w - 40, y - 8);
            }

            int numXTicks = 12;
                qreal now = QDateTime::currentMSecsSinceEpoch() / 1000.0;
                qreal t0 = now - m_windowSeconds;

                for (int i = 0; i <= numXTicks; ++i) {
                    qreal t = t0 + (now - t0) * i / numXTicks;
                    qreal x = w * (qreal(i) / numXTicks);
                    m_scene->addLine(x, 0, x, -5, axisPen);

                    QString timeStr = QDateTime::fromSecsSinceEpoch(qint64(t)).toString("hh:mm:ss");
                    auto* lbl = m_scene->addSimpleText(timeStr);
                    lbl->setFlag(QGraphicsItem::ItemIgnoresTransformations);
                    QRectF br = lbl->boundingRect();
                    lbl->setPos(x - br.width() / 2, -20);
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
    //m_scene->setSceneRect(0, 0, s.width(), s.height());
    //m_scene->setSceneRect(0, -40, s.width(), s.height() + 40);  // add 40px space below

    // Add 60px padding: left, bottom, right
    m_scene->setSceneRect(-10, -40, s.width()-100, s.height());

}

