#ifndef WARNINGDIALOG_H
#define WARNINGDIALOG_H

#include <QDialog>

/**
 * @namespace Ui
 * @brief Qt namespace containing UI classes generated from .ui files.
 */
namespace Ui { class WarningDialog; }

/**
 * @class WarningDialog
 * @brief Dialog for displaying a temperature-related warning to the user.
 *
 * This dialog notifies the user when a critical temperature threshold is reached.
 * It provides options to ignore or acknowledge the warning.
 */
class WarningDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructs the WarningDialog with a specific*
