#ifndef COFFEEINSTRUCTIONSDIALOG_H
#define COFFEEINSTRUCTIONSDIALOG_H

#include <QDialog>



namespace Ui {
class CoffeeInstructionsDialog;
}

class CoffeeInstructionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CoffeeInstructionsDialog(QWidget *parent = nullptr);
    ~CoffeeInstructionsDialog();

private:
    Ui::CoffeeInstructionsDialog *ui;
};

#endif // OPTIONSDIALOG_H
