#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(Singleton::Instance().GetControlProgram(), SIGNAL(StatisticsCollectedSignal(double)), this,
            SLOT(StatisticsCollected(double)));
    connect(Singleton::Instance().GetControlProgram(), SIGNAL(ModelingFinishedSignal()), this,
            SLOT(ModelingFinished()));

    ui->lblRequestDropKoff->setText("");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnStartModeling_clicked()
{
    try
    {
        if (ui->edtMaxMemorySize->text() == "" || ui->edtAInfSource->text() == "" ||
                ui->edtBInfSource->text() == "" || ui->edtAFirstOperator->text() == "" ||
                ui->edtBFirstOperator->text() == "" || ui->edtASecondOperator->text() == "" ||
                ui->edtBSecondOperator->text() == "" || ui->edtAThirdOperator->text() == "" ||
                ui->edtBThirdOperator->text() == "" || ui->edtRequestToBeProcessed->text() == "")
            QMessageBox::information(this, "Error", "You have not inputed all data!", QMessageBox::Ok);
        else
        {
            int maxMemorySize = ui->edtMaxMemorySize->text().toInt();
            double aFirstOperator = ui->edtAFirstOperator->text().toDouble();
            double bFirstOperator = ui->edtBFirstOperator->text().toDouble();
            double aSecondOperator = ui->edtASecondOperator->text().toDouble();
            double bSecondOperator = ui->edtBSecondOperator->text().toDouble();
            double aThirdOperator = ui->edtAThirdOperator->text().toDouble();
            double bThirdOperator = ui->edtBThirdOperator->text().toDouble();

            double aInfSource = ui->edtAInfSource->text().toDouble();
            double bInfSource = ui->edtBInfSource->text().toDouble();

            int requestNumberToBeProcessed = ui->edtRequestToBeProcessed->text().toInt();

          /*  Singleton::Instance().GetControlProgram()->ConfigureSystem(maxMemorySize, aFirstOperator, bFirstOperator,
                                                                       aSecondOperator, bSecondOperator, aThirdOperator,
                                                                       bThirdOperator, aInfSource, bInfSource,
                                                                       requestNumberToBeProcessed);
            */

            Singleton::Instance().GetControlProgram()->ConfigureSystem(10, 20, 5, 10, 30, 60, 20, 50, 10, 30, 10, 20, 9, 2,
                                                                       10, 100);
            Singleton::Instance().GetControlProgram()->StartModeling();

        }
    }
    catch (Exception& exception)
    {
        QMessageBox::information(this, "Error", exception.GetMessage(), QMessageBox::Ok);
    }
}

void MainWindow::ModelingFinished()
{
    QMessageBox::information(this, "Процесс завершен", "Процесс моделирования завершен", QMessageBox::Ok);
}

void MainWindow::StatisticsCollected(double requestDropKoff)
{
    ui->lblRequestDropKoff->setText("Коэффициент потерь = " + QString::number(requestDropKoff));
}
