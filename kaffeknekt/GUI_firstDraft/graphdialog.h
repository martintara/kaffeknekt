#ifndef GRAPHDIALOG_H
#define GRAPHDIALOG_H

#include <QDialog>
#include "graphview.h"

namespace Ui { class GraphDialog; }
class GraphDialog : public QDialog {
    Q_OBJECT
public:
    explicit GraphDialog(QWidget *parent = nullptr);
    ~GraphDialog();

public slots:
    void appendData(double pressure, double temperature);

private:
    Ui::GraphDialog *ui;
    GraphWidget     *m_graph;
};

#endif // GRAPHDIALOG_H
