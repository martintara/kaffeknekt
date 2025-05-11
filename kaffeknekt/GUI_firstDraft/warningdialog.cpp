#include "warningdialog.h"
#include "ui_warningdialog.h"
#include "mainwindow.h"


#include <QStyle>
#include <QPixmap>
#include <QFont>

WarningDialog::WarningDialog(double temp, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WarningDialog)
{
    ui->setupUi(this);

    setModal(true);
    setWindowTitle(tr("Warning"));
    // Rød bakgrunn, svart tekst
    setStyleSheet("background-color: #8B0000; color: white;");

    // Ikon: bruk labelen fra .ui
    QPixmap pix = style()
                      ->standardIcon(QStyle::SP_MessageBoxWarning)
                      .pixmap(48,48);
    ui->warning_icon->setPixmap(pix);
    ui->warning_icon->setAlignment(Qt::AlignCenter);

    // Tekst: bruk messageLabel fra .ui
    ui->warningMessage->setText(tr(
                                  "Warning: Water temperature is outside the normal range (90–96 °C)!\n"
                                  "Current: %1 °C"
                                  ).arg(temp));
    ui->warningMessage->setWordWrap(true);
    QFont f = ui->warningMessage->font();
    f.setPointSize(10);  // medium
    ui->warningMessage->setFont(f);

    // Koble knapper:
    connect(ui->btnOK,     &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->btnIgnore, &QPushButton::clicked, this, &QDialog::reject);
}

WarningDialog::~WarningDialog()
{
    delete ui;
}

void WarningDialog::on_btnIgnore_clicked()
{

    // Lukker dialogen og returnerer QDialog::Rejected
    reject();

}


void WarningDialog::on_btnOK_clicked()
{
    // Lukker dialogen og returnerer QDialog::Accepted
    accept();

}

void MainWindow::on_btnTestWarning_clicked()
{
    // Bruk en dummy-temperatur utenfor 90–96 °C
    WarningDialog dlg(100.5, this);
    int result = dlg.exec();
    if (result == QDialog::Accepted) {
        qDebug() << "User clicked OK";
    }
    else {
        qDebug() << "User clicked Ignorer";
    }
}
