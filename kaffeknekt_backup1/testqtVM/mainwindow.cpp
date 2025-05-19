#include "mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // Initialiser brukergrensesnittet fra mainwindow.ui
    setupUi(this);

    // Hvis du har angitt objektets navn til knappen som "pushButton" i .ui-filen:
    connect(pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_pushButton_clicked()
{
    // Vis en melding når knappen klikkes
    QMessageBox::information(this, "Melding", "Hello, World!");
}
