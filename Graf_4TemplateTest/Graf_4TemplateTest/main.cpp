/*
#include "templategraf.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TemplateGraf w;
    w.show();
    return a.exec();
}
*/
#include "templategraf.h"
#include "graphwidget.h"


#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 🔽 LAST INN QSS-STILFIL
    QFile file("/home/sokcher/Documents/kaffeknekt/kaffeknekt/SpyBot.qss"); // <- bytt ut med din faktiske path
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        QString styleSheet = stream.readAll();
        a.setStyleSheet(styleSheet);
        file.close();
    } else {
        qWarning("Kunne ikke åpne .qss-filen. Sjekk filstien.");
    }

    TemplateGraf w;
    w.show();
    return a.exec();
}
