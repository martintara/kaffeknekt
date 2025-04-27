#include "timescaledialog.h"
#include "ui_timescaledialog.h"

timescaleDialog::timescaleDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::timescaleDialog)
{
    ui->setupUi(this);
}

timescaleDialog::~timescaleDialog()
{
    delete ui;
}
