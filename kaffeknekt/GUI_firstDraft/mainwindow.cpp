#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "infodetaildialog.h"
#include "sensoranalyticsdialog.h"
#include "settingsdialog.h"
#include "optionsdialog.h"
#include "coffeeinstructionsdialog.h"
#include "warningdialog.h"
#include "graphdialog.h"
//#include "graphview.h"
#include <QDateTime>
#include <QMessageBox>
#include "QDialog"
#include "QIcon"
//nye bibilioteker for sub-sub side meny:
#include <QVBoxLayout>  // ADDED: for layout i timescaleFrame/SensorAnalyticsFrame



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Kaffeknekt)
{
    ui->setupUi(this);

//    GraphWidget*      m_graph;
//    WebSocketClient*  m_ws;




    // 1) Embed your GraphWidget into the QFrame named "frameGraph"
    m_graph = new GraphWidget(ui->frameGraph1);
    auto *graphLay = new QVBoxLayout(ui->frameGraph1);
    graphLay->setContentsMargins(0, 0, 0, 0);
    graphLay->setSpacing(0);
    graphLay->addWidget(m_graph);

    // 2) Set initial window and start its internal timer
    m_graph->setWindowSeconds(600.0);  // 10 minutes
    m_graph->start();

    // 3) Create the analytics dialog before wiring it
    m_saDialog = new SensorAnalyticsDialog(this);

    // 4) When an interval is selected, update the graph
    connect(m_saDialog, &SensorAnalyticsDialog::intervalSelected,
            this, [this](qreal secs){
        auto seriesP = DataFetcher::fetchPressureWindow(
                static_cast<int>(secs), "http://localhost:8086/api/v2/query?org=Kaffeknekt", "tbktmYjFgL9K3hQ4z0kLF58s-mNcfSILP9nbH9FuSJkdHcHd9fAy0HYBzNyo3zbDhTT-n4eTy7zL7nmw0Mgoug==","sensor_data"
            );
            auto seriesT = DataFetcher::fetchTempWindow(
                static_cast<int>(secs), "http://localhost:8086/api/v2/query?org=Kaffeknekt", "tbktmYjFgL9K3hQ4z0kLF58s-mNcfSILP9nbH9FuSJkdHcHd9fAy0HYBzNyo3zbDhTT-n4eTy7zL7nmw0Mgoug==","sensor_data"
            );

            m_graph->clearData();
            for (auto &pt : seriesP) m_graph->appendPressurePoint(pt);
            for (auto &pt : seriesT) m_graph->appendTempPoint(pt);
            m_graph->refresh();
            m_saDialog->hide();
    });

    // 5) Show the analytics dialog when clicking the Analytics button
    connect(ui->btnSensorAnalytics, &QPushButton::clicked, this, [this](){
        ui->frame_2->setVisible(true);
        m_saDialog->setModal(false);
        m_saDialog->show();
    });



// 1) Create the dialog but don’t show yet:
    m_graphDialog = new graphDialog(this);

     m_statsDialog = new Statistics(this);
// ─── Hide/Show the in‐window frameGraph around the GraphWidget ───
    connect(m_graphDialog, &graphDialog::dialogShown, this, [this]() {
        ui->frameGraph1->hide();
    });
    connect(m_graphDialog, &graphDialog::dialogHidden, this, [this]() {
        ui->frameGraph1->show();
    });

    // --- Test‐knapp for å prøve warning‐dialogen nå ---
    auto *btnTest = new QPushButton(tr("Test Warning"), this);

    //ny
    btnTest->setObjectName("btnTestWarning");

    ui->statusbar->addPermanentWidget(btnTest);
    connect(btnTest, &QPushButton::clicked,
            this,   &MainWindow::on_btnTestWarning_clicked);
    // … after  m_graphDialog = new graphDialog(this);

    connect(m_graphDialog, &graphDialog::flagsent,
            this,         &MainWindow::on_flagsent);



    auto *f3lay = new QVBoxLayout(ui->frame_3);
    f3lay->setContentsMargins(10,10,10,10);
    f3lay->setSpacing(10);

    // Let the text edit expand to fill all extra space:
    ui->textEdit_2->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding
        );
    f3lay->addWidget(ui->textEdit_2, /*stretch=*/1);

    // Add the “HERE” button below, no stretch so it stays at its size:
    ui->btnHere->setSizePolicy(
        QSizePolicy::Fixed,
        QSizePolicy::Fixed
        );



    // Skjul menyene ved oppstart
    ui->frame->setVisible(true);               // venstre-meny
    ui->frame_2->setVisible(false);  // hamburger-meny

    sideMenuVisible = true;

    //infoFrame blir usynlig påp starten:
    ui->frame_3->setVisible(false);

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


    // Hamburger-knappen
    ui->btnHamburger->setText("");
    ui->btnHamburger->setIcon(QIcon(":/Bilder_ikoner/menu.svg"));
    ui->btnHamburger->setIconSize(QSize(28, 28));
    ui->btnHamburger->setMinimumSize(40, 40);
    ui->btnHamburger->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_flagsent(){
    ++m_cupCount;
        // if stats dialog is visible, update immediately:
        if (m_statsDialog && m_statsDialog->isVisible()) {
            // whenever you detect flag==1, emit this
               ++m_cupCount;
               m_statsDialog->setCupCount(m_cupCount);
        }

        if (m_cupCount > 20 && !m_warningShown) {
            QMessageBox::warning(
                        this,
                        tr("Time to Wash"),
                        tr("You’ve brewed %1 cups!\nTime to clean the machine.").arg(m_cupCount)
                    );
            m_warningShown = true;
        }

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


}

void MainWindow::on_btnSensorAnalytics_clicked()
{


    SensorAnalyticsDialog *dialog = new SensorAnalyticsDialog(this);
    dialog->show();

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

    //TOGGLE SYNLIGHet:
    bool visible = ui->frame_3->isVisible();
    ui->frame_3->setVisible(!visible);
}

void MainWindow::on_btnInstructions_clicked()
{
    // ui->statusbar->showMessage("Instruction button clicked!");
    CoffeeInstructionsDialog *dialog = new CoffeeInstructionsDialog(this);
    dialog->exec(); //
    //ui->beans->setPixmap(QPixmap(":/icons/beans1.png"))
}

void MainWindow::on_btnStatistics_clicked()
{// modal popup
    // whenever you detect flag==1, emit this
       m_statsDialog->setCupCount(m_cupCount);
       m_statsDialog->exec();
}

void MainWindow::on_btnHome_clicked()
{
    sideMenuVisible = true;
    ui->frame_2->setVisible(true);

}


void MainWindow::on_btnHere_clicked()
{
    InfoDetailDialog *dialog = new InfoDetailDialog(this);
    dialog->exec();
}

void MainWindow::hideInfoFrame(){
    ui->frame_3->setVisible(false);
}

