#ifndef COFFEEINSTRUCTIONSDIALOG_H
#define COFFEEINSTRUCTIONSDIALOG_H

#include <QDialog>

/**
 * @namespace Ui
 * @brief Qt namespace for UI classes generated from .ui files.
 */
namespace Ui {
class CoffeeInstructionsDialog;
}

/**
 * @class CoffeeInstructionsDialog
 * @brief A dialog window that displays coffee brewing instructions.
 *
 * This class represents a modal dialog window built using Qt,
 * specifically for displaying instructions related to coffee preparation.
 */
class CoffeeInstructionsDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     * @param parent Pointer to the parent widget (default is nullptr).
     *
     * Creates and initializes the CoffeeInstructionsDialog.
     */
    explicit CoffeeInstructionsDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     *
     * Cleans up resources used by the dialog.
     */
    ~CoffeeInstructionsDialog();

private:
    /**
     * @brief Pointer to the UI elements of the dialog.
     *
     * This is generated automatically by Qt Designer (via .ui file).
     */
    Ui::CoffeeInstructionsDialog *ui;
};

#endif // COFFEEINSTRUCTIONSDIALOG_H
