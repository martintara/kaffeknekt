 HEAD
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);  // Initialize the Qt application
    MainWindow mainWindow;         // Create the main window
    mainWindow.setWindowTitle("Trykkgraf - Espresso");  // Set window title (optional)
    mainWindow.show();             // Show the main window
    return app.exec();             // Start the Qt event loop

#include "influx_fetcher.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

}
