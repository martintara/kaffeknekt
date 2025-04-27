/*
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

*/
//med .qss Qt templaten:

#include "influx_fetcher.h"
#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // === Last inn .qss fil ===
    QFile file("/home/sokcher/Downloads/SpyBot.qss"); // <-- Endre dette til riktig path
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        QString style = stream.readAll();
        a.setStyleSheet(style);
        file.close();
    } else {
        qDebug() << "Kunne ikke åpne stilarket!";
    }

    // Start GUI
    MainWindow w;
    w.show();
    return a.exec();
}
r
