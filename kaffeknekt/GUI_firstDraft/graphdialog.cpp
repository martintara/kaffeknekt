
#include "graphdialog.h"
#include "ui_graphdialog.h"

#include <QDateTime>
#include <QDebug>
#include <QGraphicsView>



// capture startTime exactly once, when this translation unit is first loaded
static const qreal g_startTime = QDateTime::currentMSecsSinceEpoch() / 1000.0;


graphDialog::graphDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::graphDialog)
{
    ui->setupUi(this);
    // Sett en fornuftig startstørrelse på dialogen:
    resize(800, 600);


    // The UI file already created a GraphWidget* called frameGraph:
    m_graph = ui->frameGraph;

    {
        m_graph->scene()->setSceneRect(0,
                                       -m_graph->viewport()->height(),
                                        m_graph->viewport()->width(),
                                        m_graph->viewport()->height());
        }

    m_graph->refresh();
    qDebug() << "graphDialog: initial refresh() called";

    // Start the WebSocket client thread
    m_wsClient = new WebSocketClient(this);
    connect(m_wsClient, &WebSocketClient::dataReceived,
            this,       &graphDialog::onDataReceived);
    m_wsClient->start();

    setWindowModality(Qt::NonModal);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

graphDialog::~graphDialog() {
    delete ui;
}

void graphDialog::onDataReceived(double pressure,
                                 double temperature,
                                 const QString& flag)
{
    qDebug() << " graphDialog::onDataReceived() called";

    qDebug() << "[WebSocket flag]" << flag;
    //static qreal startTime =
        //QDateTime::currentMSecsSinceEpoch() / 1000.0;



    // Pop up on first "1"
    /*
    if (flag == "1" && !isVisible())
        show();*/

    // Pop up as soon as we receive any data
    // show dialog on first data

    if (!isVisible()) show();

    //qreal t = QDateTime::currentMSecsSinceEpoch() / 1000.0;

    // at top of file, add:
    //static qreal startTime = QDateTime::currentMSecsSinceEpoch() / 1000.0;

    // then in onDataReceived:
    qreal now = QDateTime::currentMSecsSinceEpoch() / 1000.0;
    qreal t   = now - g_startTime;   // seconds since dialog opened


    m_graph->appendPressurePoint({ t, pressure });
    m_graph->appendTempPoint    ({ t, temperature });
    m_graph->refresh();
}

void graphDialog::appendData(double pressure, double temperature) {
    // (No change needed here if you still use this helper)
    qreal t = QDateTime::currentMSecsSinceEpoch() / 1000.0;
    m_graph->appendPressurePoint({ t, pressure });
    m_graph->appendTempPoint    ({ t, temperature });
}




/*#include "graphdialog.h"
#include "ui_graphdialog.h"

#include <QDateTime>

graphDialog::graphDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::graphDialog)
{
    ui->setupUi(this);

/*
        // --- Opprett og start WebSocketClient ---
    m_wsClient = new WebSocketClient(this);
    connect(m_wsClient, &WebSocketClient::dataReceived,
                this,       &graphDialog::onDataReceived);
    m_wsClient->start();



    setWindowModality(Qt::NonModal);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // Embed GraphWidget inside the placeholder frame:
    auto *lay = new QVBoxLayout(ui->frameGraph);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    m_graph = new GraphWidget(ui->frameGraph);
    lay->addWidget(m_graph);
    // We will call m_graph->start() only after we hook up data
}

graphDialog::~graphDialog() {
    delete ui;
}

void graphDialog::onDataReceived(double pressure,
                                 double temperature,
                                 const QString& flag)
{

    qDebug() << "[WebSocket flag]" << flag;
    qreal t = QDateTime::currentMSecsSinceEpoch() / 1000.0;
    m_graph->appendPressurePoint({ t, pressure });
    m_graph->appendTempPoint    ({ t, temperature });
    m_graph->refresh();

    if (flag == "1" && !isVisible())
        show();


}

void graphDialog::appendData(double pressure, double temperature) {
    // Each call, push new points into GraphWidget’s internal vectors
    // We treat timestamp as QDateTime::currentMSecsSinceEpoch() / 1000.0
    qreal t = QDateTime::currentMSecsSinceEpoch() / 1000.0;
   /* m_graph->m_pressure.push_back({t, pressure});
    m_graph->m_temp.push_back({t, temperature});

    m_graph->appendPressurePoint({ t, pressure });
    m_graph->appendTempPoint({ t, temperature });
    // then clear and redraw immediately:


    //m_graph->scene()->clear();
    //m_graph->drawAxes();
    //m_graph->drawSeries(m_graph->m_temp,     Qt::blue,  /*yOffset=*/
//0);
    //m_graph->drawSeries(m_graph->m_pressure, Qt::red,   /*yOffset=*/0);


//}*/
