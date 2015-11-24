#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "Singleton/Singleton.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_btnStartModeling_clicked();

public slots:
    void StatisticsCollected(double requestDropKoff);
    void ModelingFinished();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
