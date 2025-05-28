#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "klokke.h"

#include <QVBoxLayout>

/*
SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    clockWidget = new clock_time(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(clockWidget);
    setLayout(layout);
//kobler the signal i accept()
    connect(clockWidget, &clock_time::timeSet, this, &SettingsDialog::accept);
}*/


SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    // Åpne klokke-dialogen direkte og vis den som modal
    clock_time timeDialog(this);
    timeDialog.exec(); // <-- når bruker trykker “Set Time”, lukkes denne
    accept();   // <-- valgfritt: lukk settingsdialog etterpå
    //kobler the signal i accept()
    //connect(clockWidget, &clock_time::timeSet, this, &SettingsDialog::accept);
}


SettingsDialog::~SettingsDialog()
{
    delete ui;
}
