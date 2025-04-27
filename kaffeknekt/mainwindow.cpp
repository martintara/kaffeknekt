
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "influx_fetcher.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto *fetcher = new InfluxFetcher(this);

    connect(ui->FetchButton, &QPushButton::clicked, this, [=]() {
        fetcher->fetchTemperature();
    });

    connect(fetcher, &InfluxFetcher::dataReady, this, [=](const QString &result) {
        ui->textOutput->setText(result);
    });
}
MainWindow::~MainWindow()
{
    delete ui;
}

