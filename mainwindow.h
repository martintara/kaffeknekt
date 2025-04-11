<<<<<<< HEAD
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graphview.h"  // Make sure the filename matches exactly
=======
/*#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "influx_fetcher.h"
>>>>>>> 2f35e43 (koden B&K, men ikke nor data RTS)

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
<<<<<<< HEAD
    MainWindow(QWidget *parent = nullptr);  // Constructor
    ~MainWindow();                          // Destructor

private:
    Ui::MainWindow *ui;       // Pointer to UI elements
    GraphView *graphView;     // Pointer to the custom graph view
=======
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void fetchTemperatureData();

private:
    Ui::MainWindow *ui;
    Influx_fetcher *fetcher;
};

#endif // MAINWINDOW_H
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "influx_fetcher.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    InfluxFetcher *fetcher;
>>>>>>> 2f35e43 (koden B&K, men ikke nor data RTS)
};

#endif // MAINWINDOW_H
