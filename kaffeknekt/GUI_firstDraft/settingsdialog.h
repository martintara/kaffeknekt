#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

/**
 * @namespace Ui
 * @brief Qt namespace containing UI classes generated from .ui files.
 */
namespace Ui {
class SettingsDialog;
}

/**
 * @class SettingsDialog
 * @brief Dialog window for configuring application settings.
 *
 * This class provides a Qt-based dialog interface where users can
 * view and modify application-specific settings.
 */
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the SettingsDialog.
     * @param parent The parent widget (optional).
     */
    explicit SettingsDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~SettingsDialog();

private:
    Ui::SettingsDialog *ui; ///< Pointer to the UI components.
};

#endif // SETTINGSDIALOG_H
