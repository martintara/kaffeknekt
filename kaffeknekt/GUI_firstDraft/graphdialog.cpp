#include "graphdialog.h"
#include "ui_graphdialog.h"

GraphDialog::GraphDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GraphDialog)
{
    ui->setupUi(this);
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

GraphDialog::~GraphDialog() {
    delete ui;
}

void GraphDialog::appendData(double pressure, double temperature) {
    // Each call, push new points into GraphWidget’s internal vectors
    // We treat timestamp as QDateTime::currentMSecsSinceEpoch() / 1000.0
    qreal t = QDateTime::currentMSecsSinceEpoch() / 1000.0;
    m_graph->m_pressure.push_back({t, pressure});
    m_graph->m_temp.push_back({t, temperature});

    // then clear and redraw immediately:
    m_graph->scene()->clear();
    m_graph->drawAxes();
    m_graph->drawSeries(m_graph->m_temp,     Qt::blue,  /*yOffset=*/0);
    m_graph->drawSeries(m_graph->m_pressure, Qt::red,   /*yOffset=*/0);
}
