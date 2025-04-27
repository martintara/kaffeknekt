#ifndef TIMESCALEDIALOG_H
#define TIMESCALEDIALOG_H

#include <QDialog>

namespace Ui {
class timescaleDialog;
}

class timescaleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit timescaleDialog(QWidget *parent = nullptr);
    ~timescaleDialog();

private:
    Ui::timescaleDialog *ui;
};

#endif // TIMESCALEDIALOG_H
