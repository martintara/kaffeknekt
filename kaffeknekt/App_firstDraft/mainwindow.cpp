#include "mainwindow.h"
#include "ui_mainwindow.h"


#include "sensorAnalyticsdialog.h"
#include "settingsdialog.h"
#include "optionsdialog.h"

#include "QDialog"
#include "QIcon"
//nye bibilioteker for sub-sub side meny:
#include <QVBoxLayout>  // ADDED: for layout i timescaleFrame/SensorAnalyticsFrame



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::tullegrafer)
{
    ui->setupUi(this);

    // Skjul menyene ved oppstart
    ui->frame->setVisible(true);               // venstre-meny
    ui->frame_2->setVisible(true);  // hamburger-meny
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
    ui->btnTimescale->setStyleSheet(defaultButtonStyle);
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

void MainWindow::on_btnInstruction_clicked()
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





/*

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::tullegrafer)
{
    ui->setupUi(this);
    ui->frame->hide();


    ui->btnHamburger->setText("");
    ui->btnHamburger->setIcon(QIcon(":/icons/menu.svg"));
    ui->btnHamburger->setIconSize(QSize(28, 28));
    ui->btnHamburger->setMinimumSize(40, 40);
    ui->btnHamburger->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        //ny adda for subsub side meny:
    // =========== ADDED: Bygg opp Timescale-submenyen ===========
    //sensorAnalyticsFrame = new QFrame(ui->frame_2);
    //sensorAnalyticsFrame->setGeometry(0, 0, 0, 0);
    ui->frame_2->width(), ui->frame_2->height();
    //sensorAnalyticsFrame->setStyleSheet("background-color: #DCC6B0; border-radius: 10px;");
    //sensorAnalyticsFrame->setVisible(false);


    //funksjonen for å skjule ting liksom....
    ui->frame_2->setVisible(true);

    sideMenuVisible = false;   // <-- Legg til denne

    //ui->frame_2->setVisible(false);

    //"deklarere visibility" av frame_3:- dette skjuler frame_3 fra starten slik at den ikke vises til Sensor Analytics er trykke på!
    ui->frame_3->setVisible(false);

//OPPPEEE EHERRRRRRR!!!!


    // bakgrunnsfarge for MainWindow
    //this->setStyleSheet("background-color: #A67B5B;");

    this->setStyleSheet("background-color: #A67B5B;");
    ui->frame->setStyleSheet("background-color: #7B4A2F;");
    // Vertikal layout for knappene
    auto *lay = new QVBoxLayout(sensorAnalyticsFrame);
    lay->setContentsMargins(10,10,10,10);
    lay->setSpacing(8);
    /*
    // Opprett knappene
    btnWaterTemp        = new QPushButton("Water Temperature",    sensorAnalyticsFrame); // ADDED
    btnWaterPressure    = new QPushButton("Water Pressure",       sensorAnalyticsFrame); // ADDED
    btnPowerConsumption = new QPushButton("Power Consumption",    sensorAnalyticsFrame); // ADDED

    // Opprett knappene
    btnWaterTemp        = new QPushButton("Water Temperature",    sensorAnalyticsFrame); // ADDED
    btnWaterPressure    = new QPushButton("Water Pressure",       sensorAnalyticsFrame); // ADDED
    btnPowerConsumption = new QPushButton("Power Consumption",    sensorAnalyticsFrame); // ADDED
    // Gjenbruk samme stil som de andre knappene
    QString buttonStyle = ui->btnTimescale->styleSheet();
    for (auto *b : {btnWaterTemp, btnWaterPressure, btnPowerConsumption})
        b->setStyleSheet(buttonStyle);

    // Legg knappene i layout
    lay->addWidget(btnWaterTemp);
    lay->addWidget(btnWaterPressure);
    lay->addWidget(btnPowerConsumption);

    // ============================================================

    ui->frame_2->setStyleSheet("background-color: #DCC6B0; border-radius: 10px;");

    QString defaultButtonStyle = "QPushButton { "
                          "background-color: #A67B5B; "
                          "color: #FFFFFF; "
                          "border: none; "
                          "padding: 8px; "
                          "border-radius: 5px; "
                          "} "
                          "QPushButton:hover { "
                          "background-color: #5C4033; "
                          "}";

    //ui->btnHamburger->setStyleSheet(buttonStyle);
    ui->btnInstructions->setStyleSheet(defaultButtonStyle);
    ui->btnTimescale->setStyleSheet(defaultButtonStyle);
    ui->btnStatistics->setStyleSheet(defaultButtonStyle);

    //ny_fro frame_2:

    ui->frame_2->setVisible(false);    // Side-meny

    // Oppdater frame_1 til å følge høyden ved resizing
    //ui->frame->setGeometry(ui->frame->x(), 0, ui->frame->width(), this->height());
    // Tving frame til å dekke hele venstre side
    //ui->frame->setGeometry(0, 0, 100, this->height());

    /*
    // Vertikal layout for knappene
    auto *lay = new QVBoxLayout(sensorAnalyticsFrame);
    lay->setContentsMargins(10,10,10,10);
    lay->setSpacing(8);*/


/*
    // Gjenbruk samme stil som de andre knappene
    QString buttonStyle = ui->btnTimescale->styleSheet();
    for (auto *b : {btnWaterTemp, btnWaterPressure, btnPowerConsumption})
        b->setStyleSheet(buttonStyle);

    // Legg knappene i layout
    lay->addWidget(btnWaterTemp);
    lay->addWidget(btnWaterPressure);
    lay->addWidget(btnPowerConsumption);



}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    ui->frame->setMinimumHeight(this->height());
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


void MainWindow::on_btnSensorAnalytics_clicked()
{

    // Vis eller skjul frame_3 som du lagde i Designer
    bool vis = !ui->frame_3->isVisible();
    ui->frame_3->setVisible(vis);

    // Skjul/vis andre knapper hvis du ønsker
    ui->btnInstructions->setVisible(!vis);
    ui->btnStatistics->setVisible(!vis);
/*
    SensorAnalyticsDialog dialog(this);
    dialog.exec();

    sensorAnalyticsMenuVisible = !sensorAnalyticsMenuVisible;
    sensorAnalyticsFrame->setVisible(sensorAnalyticsMenuVisible);




    //når man trykker på SensorAnalytics knappen skal frame_3 vises!
    ui->frame_3->setVisible(sideMenuVisible);
// HERRRRRRR OPPEE




    // Skjul de vanlige knappene bak
    ui->btnInstructions->setVisible(!sensorAnalyticsMenuVisible);
    ui->btnStatistics->setVisible(!sensorAnalyticsMenuVisible);
    //vis knappen!

    // !!!!!!!!!!!11
    // HER NEEDEEEEEEEE
    bool vis = !ui->frame_3->isVisible();
    ui->frame_3->setVisible(vis);

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
}
void MainWindow::on_btnHamburger_clicked(){
    sideMenuVisible = !sideMenuVisible;
    ui->frame_2->setVisible(sideMenuVisible);
    //ui->statusbar->showMessage("Hamburger menu toggled!");

    // =========== ADDED: når hovedmeny stenger, skjul også timescale-submenyen ===========
    if (!sideMenuVisible && sensorAnalyticsMenuVisible) {
        sensorAnalyticsMenuVisible = false;
        sensorAnalyticsFrame->setVisible(false);
        // vis tilbake de vanlige menyknappene
        ui->btnInstructions->setVisible(true);
        ui->btnStatistics->setVisible(true);
    }
    // ===============================================================================

}



void MainWindow::on_btnInfo_clicked()
{
    //ui->statusbar->showMessage("Info button clicked!");
}

void MainWindow::on_btnInstruction_clicked()
{
    //ui->statusbar->showMessage("Instruction button clicked!");
}

void MainWindow::on_btnStatistics_clicked()
{
    //ui->statusbar->showMessage("Statistics button clicked!");
}

void MainWindow::on_btnHome_clicked()
{

    sideMenuVisible = false;
    //ui->statusbar->showMessage("Home clicked! Showing sub menu.");
}

//for å force størrelse  til frame_1:
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    // Tving høyden litt ekstra for å dekke alt
    int fullHeight = this->geometry().height();

    ui->frame->setGeometry(ui->frame->x(), 0, ui->frame->width(), fullHeight + 5);
}

void MainWindow::on_btnWaterTemp_clicked()
{

}


void MainWindow::on_btnWaterPressure_clicked()
{

}


void MainWindow::on_btnPowerConsumption_clicked()
{

}*/



