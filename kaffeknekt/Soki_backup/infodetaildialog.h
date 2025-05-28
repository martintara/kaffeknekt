#ifndef INFODETAILDIALOG_H
#define INFODETAILDIALOG_H

#include <QDialog>

namespace Ui {
class InfoDetailDialog;
}

class InfoDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InfoDetailDialog(QWidget *parent = nullptr);
    ~InfoDetailDialog();

private slots:
    void on_btnBack_clicked();

    void on_btnHome_clicked();

private:
    Ui::InfoDetailDialog *ui;
};

#endif // INFODETAILDIALOG_H
