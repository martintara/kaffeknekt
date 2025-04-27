#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    //for første framen:
    void on_btnHamburger_clicked();
    void on_btnHome_clicked();
    void on_btnSettings_clicked();
    void on_btnInfo_clicked();

    //for andre framen:
    void on_btnInstruction_clicked();
    void on_btnTimescale_clicked();
    void on_btnStatistics_clicked();



private:
    Ui::MainWindow *ui;
    bool sideMenuVisible;
};
#endif // MAINWINDOW_H
