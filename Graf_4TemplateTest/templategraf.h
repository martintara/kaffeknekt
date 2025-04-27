#ifndef TEMPLATEGRAF_H
#define TEMPLATEGRAF_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class TemplateGraf;
}
QT_END_NAMESPACE

class TemplateGraf : public QMainWindow
{
    Q_OBJECT

public:
    TemplateGraf(QWidget *parent = nullptr);
    ~TemplateGraf();

private:
    Ui::TemplateGraf *ui;
};

#endif // TEMPLATEGRAF_H
