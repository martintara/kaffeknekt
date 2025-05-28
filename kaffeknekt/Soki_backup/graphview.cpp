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
    /*
    // Hide the scrollbars so (0,0) is always top-left of the visible area
      setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);

    // inverter y‐retningen slik at høydeverdier (positive) går OPPOVER
    scale(1, -1);

    //    this makes scene‐Y=0 sit at the bottom of the widget.
    translate(0, -height());
    // sørg for at scene‐rektangelet matcher visningsområdet
    m_scene->setSceneRect(0, 0, width(), height());

// ─── Make this view expand to fill available space ───
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //setMinimumSize(300, 150);    // adjust to taste

    // 25 s interval
    m_timer->setInterval(25 * 1000);
    connect(m_timer, &QTimer::timeout, this, &GraphWidget::fetchAndRedraw);*/

        // Initialize sceneRect to current size
    //m_scene->setSceneRect(0, -height(), width(), height());
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

// ——— Drawing helpers ———
/*void GraphWidget::drawAxes() {
    QPen axisPen(Qt::black);
    axisPen.setWidth(2);
    // X-axis
    m_scene->addLine(0, 0, 800, 0, axisPen);
    // Y-axis
    m_scene->addLine(0, 0, 0, -200, axisPen);*/
void GraphWidget::drawAxes() {

    QPen pen(Qt::black); pen.setWidth(2);
    auto r = m_scene->sceneRect();
    qreal w = r.width(), h = r.height(), m = 20;

    // X‐axis at the bottom
    m_scene->addLine(0,    0, w,    0, pen);
    // Y‐axis on the left
    m_scene->addLine(0,    0, 0,    h, pen);


    //auto *lay

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
    /*
    QPen axisPen(Qt::black);
    axisPen.setWidth(3);

    // Scene‐rektangelet i positive coords
    QRectF r = m_scene->sceneRect();
    qreal w = r.width();
    qreal h = r.height();
    const qreal margin = 20;

    // — Tegn akser —
    // X‐akse fra (0,0) → (w,0) ved bunnen
    m_scene->addLine(0, 0, w, 0, axisPen);
    // Y‐akse fra (0,0) → (0,h) på venstre side
    m_scene->addLine(0, 0, 0, h, axisPen);

    // — Tekstetiketter —
    // De skal IKKE speiles av scale(1,-1), derfor ignorer transform:

    // 1) X‐etikett (“Time (s)”) under X‐aksen
    {
        auto *txt = m_scene->addSimpleText(tr("Time (s)"));
        txt->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        QRectF tb = txt->boundingRect();
        // legg under aksen (y < 0 etter flip/translate)
        txt->setPos((w - tb.width()) / 2, -margin);
    }

    // 2) Venstre Y‐etikett (“Pressure (bar)”) langs venstre
    {
        auto *txt = m_scene->addSimpleText(tr("Pressure (bar)"));
        txt->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        txt->setRotation(90);
        QRectF tb = txt->boundingRect();
        // sentrert langs y‐aksen
        txt->setPos(-tb.width(), (h - tb.width()) / 2);
    }

    // 3) Høyre Y‐etikett (“Temperature (°C)”) langs høyre
    {
        auto *txt = m_scene->addSimpleText(tr("Temperature (°C)"));
        txt->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        txt->setRotation(90);
        QRectF tb = txt->boundingRect();
        txt->setPos(w + margin - tb.width(), (h - tb.width()) / 2);
    }*/

    /*
   // Etiketter innenfor rektangelet:
    auto *xTxt = m_scene->addSimpleText(tr("Time (s)"));
    QRectF xb = xTxt->boundingRect();
    xTxt->setPos((w - xb.width())/2, -m);  // litt under x‐aksen (nå y-negativ etter scaling)

    auto *pTxt = m_scene->addSimpleText(tr("Pressure (bar)"));
    pTxt->setRotation(90);
    QRectF pb = pTxt->boundingRect();
    pTxt->setPos(-pb.width(), (h-pb.width())/2);

    auto *tTxt = m_scene->addSimpleText(tr("Temperature (°C)"));
    tTxt->setRotation(90);
    QRectF tb = tTxt->boundingRect();
    tTxt->setPos(w + m - tb.width(), (h-tb.width())/2);*/


    //den gamle koden
    /* QPen axisPen(Qt::black);
    axisPen.setWidth(2);

    QRectF r = m_scene->sceneRect();
    qreal w = r.width();
    qreal h = r.height();
    const qreal margin = 20;

    // X‐axis at y=0 from x=0→w
    m_scene->addLine(0, 0, w, 0, axisPen);
    // Y‐axis at x=0 from y=0→-h
    m_scene->addLine(0, 0, 0, -h, axisPen);

    // X‐axis label, centered under axis
    {
        auto *txt = m_scene->addSimpleText(tr("Time (s)"));
        QRectF tb = txt->boundingRect();
        txt->setPos((w - tb.width()) / 2, margin);
    }

    // Left Y‐axis label (Pressure)
    {
        auto *txt = m_scene->addSimpleText(tr("Pressure (bar)"));
        QRectF tb = txt->boundingRect();
        txt->setRotation(-90);
        // place halfway up axis, left of x=0
        txt->setPos(-h / 2 - tb.width() / 2, -margin);
    }

    // Right Y‐axis label (Temperature)
    {
        auto *txt = m_scene->addSimpleText(tr("Temperature (°C)"));
        QRectF tb = txt->boundingRect();
        txt->setRotation(-90);
        // place halfway up axis, to the right of the graph
        txt->setPos(w + margin, -h / 2 - tb.width() / 2);
    }
    */
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
    /*
    QPainterPath path;
    path.moveTo(s[0].timestamp, s[0].value + yOffset);
    for (int i = 1; i < s.size(); ++i) {
        path.lineTo(s[i].timestamp, s[i].value + yOffset);
    }
    QGraphicsPathItem* item = m_scene->addPath(path, QPen(c, 1.5));
    Q_UNUSED(item);*/
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

/*
QVector<DataPoint> GraphWidget::fetchPower() {
    QVector<DataPoint> v;
    return v;
}*/


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
    /*
    if (scene()) scene()->clear();
    drawAxes();
    //drawSeries(m_temp,     Qt::blue,  /*yOffset=*/
    //0);
    //drawSeries(m_pressure, Qt::red,   /*yOffset=*/0);*/
    //drawSeries(m_power,    Qt::green, /*yOffset=*/-100); // om du fortsatt vil ha power‐kurven


    /*
    fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
    qDebug() << "fitInView to" << m_scene->sceneRect();*/
}

void GraphWidget::resizeEvent(QResizeEvent* event) {

    // Let the base class handle scrollbars, etc.
/*
    QGraphicsView::resizeEvent(event);

    // Make our scene cover exactly the visible area,
    // with Y running negative upwards
    QSize s = event->size();
    m_scene->setSceneRect(0, -s.height(), s.width(), s.height());
    //qDebug() << "resizeEvent→sceneRect=" << m_scene->sceneRect();
    qDebug() << "GraphWidget::resizeEvent() new sceneRect =" << m_scene->sceneRect();
    */

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

