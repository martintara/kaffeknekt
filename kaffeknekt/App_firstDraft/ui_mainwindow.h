/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_tullegrafer
{
public:
    QWidget *centralwidget;
    QLabel *label_velkommen;
    QFrame *frame;
    QPushButton *btnHamburger;
    QPushButton *btnHome;
    QPushButton *btnSettings;
    QPushButton *btnInfo;
    QFrame *frame_2;
    QPushButton *btnTimescale;
    QPushButton *btnInstructions;
    QPushButton *btnStatistics;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *kaffeknekt_logo;
    QPushButton *pushButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *tullegrafer)
    {
        if (tullegrafer->objectName().isEmpty())
            tullegrafer->setObjectName("tullegrafer");
        tullegrafer->resize(800, 600);
        centralwidget = new QWidget(tullegrafer);
        centralwidget->setObjectName("centralwidget");
        label_velkommen = new QLabel(centralwidget);
        label_velkommen->setObjectName("label_velkommen");
        label_velkommen->setGeometry(QRect(330, 200, 171, 41));
        label_velkommen->setFrameShape(QFrame::Shape::Box);
        label_velkommen->setTextFormat(Qt::TextFormat::MarkdownText);
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(-11, -1, 151, 581));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        btnHamburger = new QPushButton(frame);
        btnHamburger->setObjectName("btnHamburger");
        btnHamburger->setGeometry(QRect(40, 10, 61, 26));
        QIcon icon;
        icon.addFile(QString::fromUtf8("menu.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnHamburger->setIcon(icon);
        btnHamburger->setIconSize(QSize(35, 35));
        btnHome = new QPushButton(frame);
        btnHome->setObjectName("btnHome");
        btnHome->setGeometry(QRect(40, 70, 61, 41));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/home.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnHome->setIcon(icon1);
        btnHome->setIconSize(QSize(32, 32));
        btnSettings = new QPushButton(frame);
        btnSettings->setObjectName("btnSettings");
        btnSettings->setGeometry(QRect(40, 140, 61, 41));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/settings.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnSettings->setIcon(icon2);
        btnSettings->setIconSize(QSize(32, 32));
        btnInfo = new QPushButton(frame);
        btnInfo->setObjectName("btnInfo");
        btnInfo->setGeometry(QRect(40, 500, 61, 41));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/info.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnInfo->setIcon(icon3);
        btnInfo->setIconSize(QSize(32, 32));
        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName("frame_2");
        frame_2->setEnabled(true);
        frame_2->setGeometry(QRect(140, 10, 171, 161));
        frame_2->setFrameShape(QFrame::Shape::NoFrame);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        btnTimescale = new QPushButton(frame_2);
        btnTimescale->setObjectName("btnTimescale");
        btnTimescale->setGeometry(QRect(10, 60, 51, 41));
        btnTimescale->setToolTipDuration(4);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/calendar.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnTimescale->setIcon(icon4);
        btnTimescale->setIconSize(QSize(32, 32));
        btnInstructions = new QPushButton(frame_2);
        btnInstructions->setObjectName("btnInstructions");
        btnInstructions->setGeometry(QRect(10, 10, 51, 41));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/coffee.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnInstructions->setIcon(icon5);
        btnInstructions->setIconSize(QSize(32, 32));
        btnStatistics = new QPushButton(frame_2);
        btnStatistics->setObjectName("btnStatistics");
        btnStatistics->setGeometry(QRect(10, 110, 51, 41));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/pie-chart.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnStatistics->setIcon(icon6);
        btnStatistics->setIconSize(QSize(32, 32));
        label = new QLabel(frame_2);
        label->setObjectName("label");
        label->setGeometry(QRect(70, 20, 91, 18));
        label_2 = new QLabel(frame_2);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(70, 120, 91, 18));
        label_3 = new QLabel(frame_2);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(70, 70, 91, 18));
        kaffeknekt_logo = new QPushButton(centralwidget);
        kaffeknekt_logo->setObjectName("kaffeknekt_logo");
        kaffeknekt_logo->setGeometry(QRect(500, 190, 61, 61));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icons/kaffeknekt_logo.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        kaffeknekt_logo->setIcon(icon7);
        kaffeknekt_logo->setIconSize(QSize(60, 60));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(200, 280, 471, 211));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/icons/tullegraf.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton->setIcon(icon8);
        pushButton->setIconSize(QSize(450, 450));
        tullegrafer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(tullegrafer);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 23));
        tullegrafer->setMenuBar(menubar);
        statusbar = new QStatusBar(tullegrafer);
        statusbar->setObjectName("statusbar");
        tullegrafer->setStatusBar(statusbar);

        retranslateUi(tullegrafer);

        QMetaObject::connectSlotsByName(tullegrafer);
    } // setupUi

    void retranslateUi(QMainWindow *tullegrafer)
    {
        tullegrafer->setWindowTitle(QCoreApplication::translate("tullegrafer", "MainWindow", nullptr));
        label_velkommen->setText(QCoreApplication::translate("tullegrafer", "Welcome To Kaffeknekt ", nullptr));
        btnHamburger->setText(QString());
        btnHome->setText(QString());
        btnSettings->setText(QString());
        btnInfo->setText(QString());
        btnTimescale->setText(QString());
        btnInstructions->setText(QString());
        btnStatistics->setText(QString());
        label->setText(QCoreApplication::translate("tullegrafer", "Instructions", nullptr));
        label_2->setText(QCoreApplication::translate("tullegrafer", "Statistics", nullptr));
        label_3->setText(QCoreApplication::translate("tullegrafer", "Timescale", nullptr));
        kaffeknekt_logo->setText(QString());
        pushButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class tullegrafer: public Ui_tullegrafer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
