#include "mainwindow.h"
#include "ui_mainwindow.h"


#include "sensoranalyticsdialog.h"
#include "settingsdialog.h"
#include "optionsdialog.h"

#include "QDialog"
#include "QIcon"
//nye bibilioteker for sub-sub side meny:
#include <QVBoxLayout>  // ADDED: for layout i timescaleFrame/SensorAnalyticsFrame



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Kaffeknekt)
{
    ui->setupUi(this);

    // Skjul menyene ved oppstart
    ui->frame->setVisible(true);               // venstre-meny
    ui->frame_2->setVisible(false);  // hamburger-meny
    //ui->frame_2->setVisible(false);  // timescale-meny (sensor analytics)

    sideMenuVisible = true;

    // Stilsetting
    this->setStyleSheet("background-color: #A67B5B;");
    ui->frame->setStyleSheet("background-color: #7B4A2F;");
    ui->frame_2->setStyleSheet("background-color: #DCC6B0; border-radius: 10px;");
    //ui->frame_2->setStyleSheet("background-color: #C8B29C; border-radius: 10px;");

    QString defaultButtonStyle = R"(
        QPushButton {
            background-color: #A67B5B;
            color: #FFFFFF;
            border: none;
            padding: 8px;
            border-radius: 5px;
        }
        QPushButton:hover {
            background-color: #5C4033;
        }
    )";

    ui->btnInstructions->setStyleSheet(defaultButtonStyle);
    ui->btnSensorAnalytics->setStyleSheet(defaultButtonStyle);
    ui->btnStatistics->setStyleSheet(defaultButtonStyle);
    //????bro den er deklarert riktig aæærrghhhh
    /*
    ui->btnWaterTemp->setStyleSheet(defaultButtonStyle);
    ui->btnWaterPressure->setStyleSheet(defaultButtonStyle);
    ui->btnPowerConsumption->setStyleSheet(defaultButtonStyle); */

    // Hamburger-knappen
    ui->btnHamburger->setText("");
    ui->btnHamburger->setIcon(QIcon(":/icons/menu.svg"));
    ui->btnHamburger->setIconSize(QSize(28, 28));
    ui->btnHamburger->setMinimumSize(40, 40);
    ui->btnHamburger->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    ui->frame->setMinimumHeight(this->height());
}

void MainWindow::on_btnHamburger_clicked()
{
    sideMenuVisible = !sideMenuVisible;
    ui->frame_2->setVisible(sideMenuVisible);

    // Hvis meny lukkes, lukk også timescale-meny
    /*
    if (!sideMenuVisible) {
        ui->frame_3->setVisible(false);
        ui->btnInstructions->setVisible(true);
        ui->btnStatistics->setVisible(true);
    }*/
}

void MainWindow::on_btnSensorAnalytics_clicked()
{
    /*
    SensorAnalyticsDialog dialog(this);
    dialog.exec();*/

    SensorAnalyticsDialog *dialog = new SensorAnalyticsDialog(this);
    dialog->show();
    qDebug() << "Åpner dialog nå!";

    // Toggle timescale-submeny (frame_3)
    //bool vis = !ui->frame_2->isVisible();
    //ui->frame_2->setVisible(vis);

    // Skjul de andre knappene i frame_2 når submeny vises
    //ui->btnInstructions->setVisible(!vis);
    //ui->btnStatistics->setVisible(!vis);
}

void MainWindow::on_btnSettings_clicked()
{
    SettingsDialog dialog(this);
    dialog.exec();
    ui->statusbar->showMessage("Settings button clicked!");
}

void MainWindow::on_btnInfo_clicked()
{
    // ui->statusbar->showMessage("Info button clicked!");
}

void MainWindow::on_btnInstructions_clicked()
{
    // ui->statusbar->showMessage("Instruction button clicked!");
}

void MainWindow::on_btnStatistics_clicked()
{
    // ui->statusbar->showMessage("Statistics button clicked!");
}

void MainWindow::on_btnHome_clicked()
{
    sideMenuVisible = true;
    ui->frame_2->setVisible(true);
}




