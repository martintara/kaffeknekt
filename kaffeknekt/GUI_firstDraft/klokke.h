#ifndef CLOCK_TIME_H
#define CLOCK_TIME_H

#include <QDateTimeEdit>
#include <QWidget>
#include <QDialog>


class clock_time : public QDialog
{
    Q_OBJECT

public:
explicit clock_time(QWidget *parent = nullptr);

private slots:
void setTime();

private:
    QDateTimeEdit *dateTimeEdit;

signals:
    void timeSet();

};
#endif // CLOCK_TIME_H
