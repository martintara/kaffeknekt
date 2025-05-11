#include "onedaydialog.h"
#include "ui_onedaydialog.h"

OneDayDialog::OneDayDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OneDayDialog)
{
    ui->setupUi(this);
}

OneDayDialog::~OneDayDialog()
{
    delete ui;
}
