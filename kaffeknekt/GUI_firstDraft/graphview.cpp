                                                                                                                                                                                                                                                                            #include "graphview.h"
#include <QPen>
#include <QtMath>
#include <QPainterPath>
#include <QtGlobal>
#include <QSizePolicy>

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

    m_timer->setInterval(25*1000);
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
    qreal w = r.width(), h = r.height(), m = 20;

    // X‐axis at the bottom
    m_scene->addLine(0,    0, w,    0, pen);
    // Y‐axis on the left
    m_scene->addLine(0,    0, 0,    h, pen);




    // Labels must ignore the flip transform:
    auto makeLabel = [&](const QString &text){
        auto *t = m_scene->addSimpleText(text);
        t->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        return t;
    };

    // X‐label
    {
      auto *t = makeLabel(tr("Time (s)"));
      auto br = t->boundingRect();
      t->setPos((w - br.width())/2, -m);
    }
    // Left Y‐label
    {
      auto *t = makeLabel(tr("Pressure (bar)"));
      t->setRotation(90);
      auto br = t->boundingRect();
      t->setPos(-br.width(), (h - br.width())/2);
    }
    // Right Y‐label
    {
      auto *t = makeLabel(tr("Temperature (°C)"));
      t->setRotation(90);
      auto br = t->boundingRect();
      t->setPos(w + m - br.width(), (h - br.width())/2);
    }

}

void GraphWidget::drawSeries(const QVector<DataPoint>& s, const QColor& c, qreal yOffset) {
    if (s.size() < 2)
        return;

    qDebug() << "   drawSeries() first point at"
             << s[0].timestamp << s[0].value
             << "offset=" <<yOffset;

    if (s.size() < 2)
        return;
    QPainterPath path;
    path.moveTo(s[0].timestamp, s[0].value + yOffset);
    for (int i = 1; i < s.size(); ++i)
        path.lineTo(s[i].timestamp, s[i].value + yOffset);
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
}

// Legger til ett data‐punkt i m_temp
void GraphWidget::appendTempPoint(const DataPoint& t) {
    m_temp.push_back(t);
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

        qDebug() << "GraphWidget::resizeEvent → sceneRect =" << m_scene->sceneRect();
}

