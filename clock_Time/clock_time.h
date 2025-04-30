#ifndef CLOCK_TIME_H
#define CLOCK_TIME_H

#include <QDateTimeEdit>
#include <QWidget>


class clock_time : public QWidget
{
    Q_OBJECT

public:
explicit clock_time(QWidget *parent = nullptr);

private slots:
void setTime();

private:
    QDateTimeEdit *dateTimeEdit;
};
#endif // CLOCK_TIME_H
