//graphdialog.h
#ifndef GRAPHDIALOG_H
#define GRAPHDIALOG_H

#include <QDialog>

#include <QSizePolicy>

#include "graphview.h"

#include "websocketclient.h"

namespace Ui { class graphDialog; }
class graphDialog : public QDialog {
    Q_OBJECT
public:
    explicit graphDialog(QWidget *parent = nullptr);
    ~graphDialog();

public slots:

    void appendData(double pressure, double temperature);
    void onDataReceived(double pressure,
                            double temperature,
                            const QString& flag);

private:
    Ui::graphDialog *ui;
    GraphWidget     *m_graph;
    WebSocketClient *m_wsClient;
};

#endif // GRAPHDIALOG_H
