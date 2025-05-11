#include "onemonthdialog.h"
#include "ui_onemonthdialog.h"

OneMonthDialog::OneMonthDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OneMonthDialog)
{
    ui->setupUi(this);
}

OneMonthDialog::~OneMonthDialog()
{
    delete ui;
}
