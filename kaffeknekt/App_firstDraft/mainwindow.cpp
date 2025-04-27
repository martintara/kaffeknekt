#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "timescaledialog.h"
#include "settingsdialog.h"
#include "optionsdialog.h"

#include "QDialog"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //funksjonen for å skjule ting liksom....
    ui->frame_2->setVisible(false);

    sideMenuVisible = false;   // <-- Legg til denne

    ui->frame_2->setVisible(false);

    // bakgrunnsfarge for MainWindow
    this->setStyleSheet("background-color: #A67B5B;");

    ui->frame_2->setStyleSheet("background-color: #DCC6B0; border-radius: 10px;");

    QString buttonStyle = "QPushButton { "
                          "background-color: #A67B5B; "
                          "color: #FFFFFF; "
                          "border: none; "
                          "padding: 8px; "
                          "border-radius: 5px; "
                          "} "
                          "QPushButton:hover { "
                          "background-color: #5C4033; "
                          "}";

    ui->btnHamburger->setStyleSheet(buttonStyle);
    ui->btnInstructions->setStyleSheet(buttonStyle);
    ui->btnTimescale->setStyleSheet(buttonStyle);
    ui->btnStatistics->setStyleSheet(buttonStyle);

    //ny_fro frame_2:

    ui->frame_2->setVisible(false);    // Side-meny
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_btnSettings_clicked()
{
    SettingsDialog dialog(this);
    dialog.exec();
    ui->statusbar->showMessage("Setting button clicked!");

}


void MainWindow::on_btnTimescale_clicked()
{
    timescaleDialog dialog(this);
    dialog.exec();
}
void MainWindow::on_btnHamburger_clicked(){
    sideMenuVisible = !sideMenuVisible;
    ui->frame_2->setVisible(sideMenuVisible);
    ui->statusbar->showMessage("Hamburger menu toggled!");
}



void MainWindow::on_btnInfo_clicked()
{
    ui->statusbar->showMessage("Info button clicked!");
}

void MainWindow::on_btnInstruction_clicked()
{
    ui->statusbar->showMessage("Instruction button clicked!");
}

void MainWindow::on_btnStatistics_clicked()
{
    ui->statusbar->showMessage("Statistics button clicked!");
}

void MainWindow::on_btnHome_clicked()
{

    sideMenuVisible = false;
    ui->statusbar->showMessage("Home clicked! Showing sub menu.");
}

