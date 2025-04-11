/*
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphview.h"  // Included to ensure the GraphView class is available

// Constructor for MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);  // Set up the user interface generated from mainwindow.ui

    graphView = new GraphView(this);  // Create a new GraphView object
    setCentralWidget(graphView);     // Set GraphView as the central widget of the window
}

// Destructor for MainWindow
MainWindow::~MainWindow() {
    delete ui;  // Clean up UI resources to prevent memory leaks
}

//Koden som kjører!!

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "influx_fetcher.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fetcher = new InfluxFetcher(this);

    connect(ui->FetchButton, &QPushButton::clicked, this, [=]() {
        fetcher->fetchTemperatureData();
    });

    connect(fetcher, &InfluxFetcher::dataReady, this, [=](const QByteArray &data) {
        ui->textOutput->setText(QString::fromUtf8(data));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
*/
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

