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
    void on_btnSum_clicked();

    void on_spinRowsA_valueChanged(int arg1);

    void on_spinColsA_valueChanged(int arg1);

    void on_spinRowsB_valueChanged(int arg1);

    void on_spinColsB_valueChanged(int arg1);

    void on_btnSubtract_clicked();

    void on_btnMultiply_clicked();

    void on_btMultiplyA_clicked();

    void on_btMultiplyB_clicked();

    void on_btTransposeA_clicked();

    void on_btTransposeB_clicked();

    void on_btDeterminantA_clicked();

    void on_btDeterminantB_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
