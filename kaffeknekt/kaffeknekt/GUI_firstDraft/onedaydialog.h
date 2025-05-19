#ifndef ONEDAYDIALOG_H
#define ONEDAYDIALOG_H

#include <QDialog>

namespace Ui {
class OneDayDialog;
}

class OneDayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OneDayDialog(QWidget *parent = nullptr);
    ~OneDayDialog();

private:
    Ui::OneDayDialog *ui;
};

#endif // ONEDAYDIALOG_H
