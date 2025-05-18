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
    void setWindowSeconds(qreal seconds) { m_windowSeconds = seconds; }


signals:
    void dialogShown();
    void dialogHidden();
    void flagsent();

public slots:

    void appendData(double pressure, double temperature);
    void onDataReceived(double pressure,
                            double temperature,
                            const QString& flag);


private:
    Ui::graphDialog *ui;
    GraphWidget     *m_graph;
    WebSocketClient *m_wsClient;

    //
    QVector<DataPoint> m_pressure;
    QVector<DataPoint> m_temp;
    qreal               m_windowSeconds = 600.0; // default 10 min

protected:
    void showEvent(QShowEvent* ev) override;
    void hideEvent(QHideEvent* ev) override;


};

#endif // GRAPHDIALOG_H
