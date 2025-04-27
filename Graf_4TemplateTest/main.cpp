
/*
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
//

#include <QApplication>

#include "graphwidget.h"
#include "templategraf.h"
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 👉 Last inn custom style
    QFile file("style.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        a.setStyleSheet(styleSheet);
    }

    TemplateGraf w;   // Du brukte TemplateGraf, ikke MainWindow
    w.show();
    return a.exec();
}*/
#include <QApplication>   // 🔹 Denne må være med!
#include "graphwidget.h"
#include "templategraf.h"
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Laster inn custom style
    QFile file("style.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        a.setStyleSheet(styleSheet);
    }

    TemplateGraf w;
    w.show();
    return a.exec();
}

