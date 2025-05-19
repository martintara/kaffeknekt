#ifndef INFODETAILDIALOG_H
#define INFODETAILDIALOG_H

#include <QDialog>

/**
 * @namespace Ui
 * @brief Qt namespace containing UI classes generated from .ui files.
 */
namespace Ui {
class InfoDetailDialog;
}

/**
 * @class InfoDetailDialog
 * @brief A dialog that displays detailed information with navigation options.
 *
 * This dialog provides a UI for displaying extended information and includes
 * navigation buttons such as "Back" and "Home".
 */
class InfoDetailDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the InfoDetailDialog.
     * @param parent The parent widget (optional).
     */
    explicit InfoDetailDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~InfoDetailDialog();

private slots:
    /**
     * @brief Slot triggered when the "Back" button is clicked.
     */
    void on_btnBack_clicked();

    /**
     * @brief Slot triggered when the "Home" button is clicked.
     */
    void on_btnHome_clicked();

private:
    Ui::InfoDetailDialog *ui; ///< Pointer to the UI components.
};

#endif // INFODETAILDIALOG_H
