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
