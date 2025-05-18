#include "infodetaildialog.h"
#include "mainwindow.h"
#include "ui_infodetaildialog.h"

#include <QScrollArea>
#include <QVBoxLayout>


InfoDetailDialog::InfoDetailDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InfoDetailDialog)
{
    ui->setupUi(this);

    // Gjør scroll‐området om til fleksibel beholder
    ui->scrollArea->setWidgetResizable(true);

    // Pek på det interne widget‐innholdet
    QWidget *content = ui->scrollAreaWidgetContents_2;

    // Hvis det ikke allerede har en layout, oppretter du en
    if (!content->layout()) {
        auto *vlay = new QVBoxLayout(content);
        vlay->setContentsMargins(10, 10, 10, 10);
        vlay->setSpacing(20);
    }

    // Hent layouten som nå ligger på content
    auto *vlay = qobject_cast<QVBoxLayout*>(content->layout());

    // Legg til dine to widgets i riktig rekkefølge
    vlay->addWidget(ui->SD_beskrivelse);
//    vlay->addWidget(ui->sekvensDiagram);

//    vlay->addWidget(ui->AD_beskrivelse);
//    vlay->addWidget(ui->label_2);

}

InfoDetailDialog::~InfoDetailDialog()
{
    delete ui;

}



void InfoDetailDialog::on_btnBack_clicked()
{
    this->close(); //går tilbake til mainwindow.
}

void InfoDetailDialog::on_btnHome_clicked(){
    MainWindow* main = qobject_cast<MainWindow*>(parent());
    if(main){
        main->hideInfoFrame();
        main->show();
    }
    this->close();
}

