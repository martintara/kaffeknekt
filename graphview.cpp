#include "GraphView.h"
#include <QGraphicsLineItem>
#include <QPen>
#include <cstdlib>
#include <ctime>
#include <QDebug>

// Constructor for GraphView
GraphView::GraphView(QWidget *parent) : QGraphicsView(parent) {
    scene = new QGraphicsScene(this);     // Create a new graphics scene
    setScene(scene);                      // Attach the scene to the view
    setFixedSize(600, 500);               // Set the fixed window size
    scene->setSceneRect(0, 0, 600, 500);  // Set the drawing area size
}

// Handle key press events
void GraphView::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_D) {  // If 'D' key is pressed
        generatePressureGraph();      // Generate and display the graph
    }
}

// Draw the pressure graph
void GraphView::generatePressureGraph() {
    static int graphIndex = 0;     // Track which graph to display
    scene->clear();                // Clear existing items in the scene

    // Predefined sets of pressure data
    std::vector<std::vector<int>> pressureSets = {
        {6, 6, 7, 8, 9, 9, 10, 8, 6, 10, 7},  // Example 1: low/high pressure due to scale or blockages
        {8, 11, 9, 9, 9, 8, 10, 8, 9, 8, 9},  // Example 2: more stable after cleaning
        {8, 9, 10, 8, 9, 8, 10, 9, 10, 9, 8}  // Example 3: consistent good performance
    };

    std::vector<int> pressures = pressureSets[graphIndex];  // Select current dataset
    graphIndex = (graphIndex + 1) % pressureSets.size();    // Cycle to the next dataset

    std::vector<QPointF> points;  // Points for the graph
    int startX = 50, startY = 350;

    // Calculate positions for pressure points
    for (int i = 0; i < pressures.size(); ++i) {
        points.push_back(QPointF(startX + i * 45, startY - ((pressures[i] - 3) * 40)));  // Normalize pressure values
    }

    // Draw X and Y axes
    scene->addLine(50, 350, 550, 350, QPen(Qt::black));  // X-axis
    scene->addLine(50, 350, 50, 50, QPen(Qt::black));    // Y-axis

    // Draw the lines connecting pressure points
    for (size_t i = 1; i < points.size(); ++i) {
        scene->addLine(points[i - 1].x(), points[i - 1].y(),
                       points[i].x(), points[i].y(), QPen(Qt::blue, 2));
    }

    // Label the X-axis with time from 07:00 to 17:00
    for (int i = 0; i <= 10; ++i) {
        scene->addText(QString::number(7 + i) + ":00")->setPos(startX + i * 45 - 10, 355);
    }

    // Label the Y-axis with pressure values (3 to 12 bar)
    for (int p = 3; p <= 12; ++p) {
        scene->addText(QString::number(p) + " bar")->setPos(5, startY - ((p - 3) * 40) - 5);
    }
}
