#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graphview.h"  // Make sure the filename matches exactly

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);  // Constructor
    ~MainWindow();                          // Destructor

private:
    Ui::MainWindow *ui;       // Pointer to UI elements
    GraphView *graphView;     // Pointer to the custom graph view
};

#endif // MAINWINDOW_H
