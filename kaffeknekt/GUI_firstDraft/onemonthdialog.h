#ifndef ONEMONTHDIALOG_H
#define ONEMONTHDIALOG_H

#include <QDialog>

namespace Ui {
class OneMonthDialog;
}

class OneMonthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OneMonthDialog(QWidget *parent = nullptr);
    ~OneMonthDialog();

private:
    Ui::OneMonthDialog *ui;
};

#endif // ONEMONTHDIALOG_H
