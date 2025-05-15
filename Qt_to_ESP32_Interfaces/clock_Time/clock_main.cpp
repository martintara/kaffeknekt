#include "clock_time.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    clock_time w;
    w.show();
    return a.exec();
}
