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

// ─── START LIVE-GRAPH SETUP ───

    // 1) Embed your GraphWidget into the QFrame named "frameGraph"
    m_graph = new GraphWidget(ui->frameGraph);
    auto *graphLay = new QVBoxLayout(ui->frameGraph);
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
        m_graph->setWindowSeconds(secs);
        m_graph->refresh();
        m_saDialog->hide();
    });

    // 5) Show the analytics dialog when clicking the Analytics button
    connect(ui->btnSensorAnalytics, &QPushButton::clicked, this, [this](){
        ui->frame_2->setVisible(true);
        m_saDialog->setModal(false);
        m_saDialog->show();
    });

    // 6) Start the WebSocket client and feed live data into the member graph
    m_ws = new WebSocketClient(this);
        qDebug() << "Websocket attachement";
    connect(m_ws, &WebSocketClient::dataReceived, this, [this](double pressure, double temp, const QString& flag){
        if (flag == QLatin1String("1")) {
            qreal now = QDateTime::currentMSecsSinceEpoch()/1000.0;
            m_graph->appendPressurePoint({ now, pressure });
            m_graph->appendTempPoint    ({ now, temp     });
        }
    });
    m_ws->start();
   /*
   m_ws = new WebSocketClient(this);
   connect(m_ws, &WebSocketClient::dataReceived(),
           this, [this](double pressure, double temp, const QString& flag){
       if (flag == QLatin1String("1")) {
           qreal now = QDateTime::currentMSecsSinceEpoch()/1000.0;
           m_graph->appendPressurePoint({ now, pressure });
           m_graph->appendTempPoint    ({ now, temp     });
        }
   });*/
   m_ws->start();

// 1) Create the dialog but don’t show yet:
    m_graphDialog = new graphDialog(this);

// ─── Hide/Show the in‐window frameGraph around the GraphWidget ───
    connect(m_graphDialog, &graphDialog::dialogShown, this, [this]() {
        ui->frameGraph->hide();
    });
    connect(m_graphDialog, &graphDialog::dialogHidden, this, [this]() {
        ui->frameGraph->show();
    });

    // --- Test‐knapp for å prøve warning‐dialogen nå ---
    auto *btnTest = new QPushButton(tr("Test Warning"), this);

    //ny
    btnTest->setObjectName("btnTestWarning");

    ui->statusbar->addPermanentWidget(btnTest);
    connect(btnTest, &QPushButton::clicked,
            this,   &MainWindow::on_btnTestWarning_clicked);


//    //jeg velger denne layout typen:
//    auto *gl = ui->gridLayout;



//    // --- 1) Sett gridLayout uten marger og spacing ---
//    ui->gridLayout->setContentsMargins(0, 0, 0, 0);
//    ui->gridLayout->setSpacing(5);


//    // --- 2) Kolonne‐strekkfaktorer: venstre fast, midt expanderer, høyre fast ---
//    ui->gridLayout->setColumnStretch(0, 0);
//    ui->gridLayout->setColumnStretch(1, 1);
//    ui->gridLayout->setColumnStretch(2, 0);



/*
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
*/                                                                                                                                                                                                                                                                            #include "graphview.h"


        //de nye endringeen siden jeg fhjerna de fra grapgdiaialog og slkengte de direkte i graphview ++ forklar mer senere :)



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

