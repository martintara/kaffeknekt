// statistics.cpp
#include "statistics.h"
#include "ui_statistics.h"

Statistics::Statistics(QWidget *parent)
    : QDialog(parent), ui(new Ui::Statistics)
{
    ui->setupUi(this);
    ui->countLabel->setText(QString::number(m_cupCount));
}

Statistics::~Statistics()
{
    delete ui;
}

void Statistics::setCupCount(int num)
{
    m_cupCount = num;
    ui->countLabel->setText(QString::number(m_cupCount));
}
