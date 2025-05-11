#ifndef ONEWEEKDIALOG_H
#define ONEWEEKDIALOG_H

#include <QDialog>

namespace Ui {
class OneWeekDialog;
}

class OneWeekDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OneWeekDialog(QWidget *parent = nullptr);
    ~OneWeekDialog();

private:
    Ui::OneWeekDialog *ui;
};

#endif // ONEWEEKDIALOG_H
