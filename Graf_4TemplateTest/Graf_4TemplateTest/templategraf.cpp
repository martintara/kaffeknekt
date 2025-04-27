#include "templategraf.h"
#include "graphwidget.h"
#include "ui_templategraf.h"

TemplateGraf::TemplateGraf(QWidget *parent) : QMainWindow(parent), ui(new Ui::TemplateGraf)
{
    ui->setupUi(this);

    // Opprett GraphWidget og legg til i vinduet
    GraphWidget *graph = new GraphWidget(this);
    graph->setGeometry(50, 50, 500, 300);  // Juster posisjon/størrelse etter behov

    resize(600, 400);
}

TemplateGraf::~TemplateGraf()
{
    delete ui;
}






/*
void TemplateGraf::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int margin = 50;
    int graphWidth = width() - 2 * margin;
    int graphHeight = height() - 2 * margin;

    // Tegn akser
    painter.drawLine(margin, margin, margin, margin + graphHeight); // y-akse
    painter.drawLine(margin, margin + graphHeight, margin + graphWidth, margin + graphHeight); // x-akse

    // Tegn temperaturdata
    painter.setPen(Qt::red);
    float xStep = float(graphWidth) / (temperatureData.size() - 1);
    float minTemp = 89.0;
    float maxTemp = 96.0;

    QPoint lastPoint;
    for (int i = 0; i < temperatureData.size(); ++i) {
        float temp = temperatureData[i];
        float x = margin + i * xStep;
        float y = margin + graphHeight - ((temp - minTemp) / (maxTemp - minTemp)) * graphHeight;

        QPoint point(x, y);
        painter.drawEllipse(point, 3, 3);

        if (i > 0)
            painter.drawLine(lastPoint, point);

        lastPoint = point;
    }

    // Y-akse merking
    painter.setPen(Qt::black);
    for (int t = 89; t <= 96; ++t) {
        int y = margin + graphHeight - ((t - minTemp) / (maxTemp - minTemp)) * graphHeight;
        painter.drawText(10, y + 5, QString::number(t) + "°C");
    }
}*/
