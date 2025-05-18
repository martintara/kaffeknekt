#ifndef STATISTICS_H
#define STATISTICS_H

#include <QDialog>

namespace Ui {
class Statistics;
}

class Statistics : public QDialog
{
    Q_OBJECT

public:
    explicit Statistics(QWidget *parent = nullptr);
    ~Statistics();
    void setCupCount(int num);

private:
    Ui::Statistics *ui;
    int             m_cupCount = 0;


};

#endif // STATISTICS_H
