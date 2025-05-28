#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H
#include "klokke.h"

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

private:
    Ui::SettingsDialog *ui;
    clock_time *clockWidget;
};

#endif // SETTINGSDIALOG_H
