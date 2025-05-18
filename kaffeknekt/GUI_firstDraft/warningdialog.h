#ifndef WARNINGDIALOG_H
#define WARNINGDIALOG_H

#include <QDialog>


namespace Ui { class WarningDialog; }
class WarningDialog : public QDialog {
    Q_OBJECT
public:

    explicit WarningDialog(double temp, QWidget *parent = nullptr);
    ~WarningDialog();
private slots:
    void on_btnIgnore_clicked();

    void on_btnOK_clicked();

private:
    Ui::WarningDialog *ui;
};

#endif // WARNINGDIALOG_H
