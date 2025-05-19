#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

/**
 * @namespace Ui
 * @brief Qt namespace containing UI classes generated from .ui files.
 */
namespace Ui {
class OptionsDialog;
}

/**
 * @class OptionsDialog
 * @brief Dialog window for displaying and modifying application options.
 *
 * This class provides a simple Qt dialog interface for setting or displaying
 * configuration options within the application.
 */
class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the OptionsDialog.
     * @param parent The parent widget (optional).
     */
    explicit OptionsDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~OptionsDialog();

private:
    Ui::OptionsDialog *ui; ///< Pointer to the UI components.
};

#endif // OPTIONSDIALOG_H
