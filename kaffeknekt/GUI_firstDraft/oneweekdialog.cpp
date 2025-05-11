#include "oneweekdialog.h"
#include "ui_oneweekdialog.h"

OneWeekDialog::OneWeekDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OneWeekDialog)
{
    ui->setupUi(this);
}

OneWeekDialog::~OneWeekDialog()
{
    delete ui;
}
