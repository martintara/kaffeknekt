#include "graphwidget.h"
#include <QPainter>

GraphWidget::GraphWidget(QWidget *parent) : QWidget(parent)
{
    // Eksempeldata
    temperatureData = {89.5, 90.2, 91.8, 93.4, 94.1, 95.2, 96.0};
}

void GraphWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor(245, 240, 225));

    int margin = 30;
    int graphWidth = width() - 2 * margin;
    int graphHeight = height() - 2 * margin;

    // Tegn akser
    painter.setPen(Qt::black);
    painter.drawLine(margin, margin, margin, margin + graphHeight); // y-akse
    painter.drawLine(margin, margin + graphHeight, margin + graphWidth, margin + graphHeight); // x-akse

    // Tegn temperaturdata
    painter.setPen(Qt::red);
    float xStep = float(graphWidth) / (temperatureData.size() - 1);
    float minTemp = 89.0;
    float maxTemp = 96.0;

    QPoint lastPoint;
    for(std::size_t i = 0; i < temperatureData.size(); ++i) {
        float temp = temperatureData[i];
        float x = margin + i * xStep;
        float y = margin + graphHeight - ((temp - minTemp) / (maxTemp - minTemp)) * graphHeight;

        QPoint point(x, y);
        painter.drawEllipse(point, 3, 3);

        if (i > 0)
            painter.drawLine(lastPoint, point);

        lastPoint = point;
    }
}
