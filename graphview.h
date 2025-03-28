#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <vector>

// Custom graphics view for displaying espresso pressure graphs
class GraphView : public QGraphicsView {
    Q_OBJECT

public:
    explicit GraphView(QWidget *parent = nullptr);  // Constructor

protected:
    void keyPressEvent(QKeyEvent *event) override;  // Handle key press (e.g. 'D' key)

private:
    QGraphicsScene *scene;              // Scene to hold and draw graphics
    void generatePressureGraph();       // Generate and draw pressure graph
};

#endif // GRAPHVIEW_H
