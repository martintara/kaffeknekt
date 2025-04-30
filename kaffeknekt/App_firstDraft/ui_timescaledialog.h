/********************************************************************************
** Form generated from reading UI file 'timescaledialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TIMESCALEDIALOG_H
#define UI_TIMESCALEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_timescaleDialog
{
public:

    void setupUi(QDialog *timescaleDialog)
    {
        if (timescaleDialog->objectName().isEmpty())
            timescaleDialog->setObjectName("timescaleDialog");
        timescaleDialog->resize(400, 300);

        retranslateUi(timescaleDialog);

        QMetaObject::connectSlotsByName(timescaleDialog);
    } // setupUi

    void retranslateUi(QDialog *timescaleDialog)
    {
        timescaleDialog->setWindowTitle(QCoreApplication::translate("timescaleDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class timescaleDialog: public Ui_timescaleDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIMESCALEDIALOG_H
