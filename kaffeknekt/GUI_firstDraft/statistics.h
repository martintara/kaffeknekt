#ifndef STATISTICS_H
#define STATISTICS_H

#include <QDialog>

/**
 * @namespace Ui
 * @brief Qt namespace containing UI classes generated from .ui files.
 */
namespace Ui {
class Statistics;
}

/**
 * @class Statistics
 * @brief Dialog for displaying usage statistics.
 *
 * This class provides a Qt dialog that displays statistical information,
 * such as the number of cups made by the espresso machine.
 */
class Statistics : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the Statistics dialog.
     * @param parent The parent widget (optional).
     */
    explicit Statistics(QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~Statistics();

    /**
     * @brief Sets the number of cups to display in the statistics.
     * @param num The number of cups.
     */
    void setCupCount(int num);

private:
    Ui::Statistics *ui; ///< Pointer to the UI components.
    int             m_cupCount = 0; ///< Number of cups tracked for statistics.
};

#endif // STATISTICS_H
