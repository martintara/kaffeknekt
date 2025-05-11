#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "infodetaildialog.h"
#include "sensoranalyticsdialog.h"
#include "settingsdialog.h"
#include "optionsdialog.h"
#include "coffeeinstructionsdialog.h"
#include "warningdialog.h"

#include "QDialog"
#include "QIcon"
//nye bibilioteker for sub-sub side meny:
#include <QVBoxLayout>  // ADDED: for layout i timescaleFrame/SensorAnalyticsFrame



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Kaffeknekt)
{
    ui->setupUi(this);

// 1) Create the dialog but don’t show yet:
    m_graphDialog = new graphDialog(this);

// 2) Start your WebSocket client thread:
    m_ws = new WebSocketClient(this);
    connect(m_ws, &WebSocketClient::dataReceived,
        this, [this](double p, double t, const QString& flag){
    if (flag == "1") {
        if (!m_graphDialog->isVisible())
            m_graphDialog->show();
        // append and redraw:
        m_graphDialog->appendData(p, t);
    }
    });
    m_ws->start();

    // --- Test‐knapp for å prøve warning‐dialogen nå ---
    auto *btnTest = new QPushButton(tr("Test Warning"), this);
    ui->statusbar->addPermanentWidget(btnTest);
    connect(btnTest, &QPushButton::clicked,
            this,   &MainWindow::on_btnTestWarning_clicked);


    //jeg velger denne layout typen:
    auto *gl = ui->gridLayout;



    // --- 1) Sett gridLayout uten marger og spacing ---
    ui->gridLayout->setContentsMargins(0, 0, 0, 0);
    ui->gridLayout->setSpacing(5);


    // --- 2) Kolonne‐strekkfaktorer: venstre fast, midt expanderer, høyre fast ---
    ui->gridLayout->setColumnStretch(0, 0);
    ui->gridLayout->setColumnStretch(1, 1);
    ui->gridLayout->setColumnStretch(2, 0);




    // --- 3) Rad‐strekkfaktorer: hovedinnhold expanderer, bunn er fast ---
    ui->gridLayout->setRowStretch(1, 1);
    ui->gridLayout->setRowStretch(1, 0);

    // 2) Ensure your graphFrame actually does expand:
    ui->graphFrame->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding
        );

    // --- 2) Kolonne‐strekkfaktorer: kun midtkolonnen (graphFrame) vokser ---
    ui->gridLayout->setColumnStretch(0, 0);
    ui->gridLayout->setColumnStretch(1, 1);
    ui->gridLayout->setColumnStretch(2, 0);

    // --- 3) Rad‐strekkfaktorer: kun rad 0 (hovedinnhold) vokser ---
    ui->gridLayout->setRowStretch(0, 1);
    ui->gridLayout->setRowStretch(1, 0);


    // --- 4) Fiks størrelser på venstre logo og midt‐graf (uendret) ---
    ui->kk_logo->setFixedSize(90, 90);

    // --- 5) Juster QR‐koden (label_2) litt opp ---
    ui->label_2->setFixedSize(100, 100);
    // Flytt den til toppen av bunnraden:
    ui->gridLayout->setAlignment(ui->label_2,
                                 Qt::AlignTop   |
                                     Qt::AlignRight);




    // --- Gjør frame_3 responsiv med tekst og knapp ---
    // (btw!!frame_3 inneholder textEdit_2 og btnHere)
    // Make sure frame_3 itself can grow when the window grows: altså når jeg "ekspanderer vinduet"
    ui->frame_3->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding
        );

    // Oppretter et fresh QVBoxLayout for frame_3:
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


}

void MainWindow::on_btnSensorAnalytics_clicked()
{


    SensorAnalyticsDialog *dialog = new SensorAnalyticsDialog(this);
    dialog->show();
    qDebug() << "Åpner dialog nå!";


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
   //ui->infoFrame->setVisible(true);
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
{
    // ui->statusbar->showMessage("Statistics button clicked!");
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

