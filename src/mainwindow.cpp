#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(Singleton::Instance().GetControlProgram(),
            SIGNAL(StatisticsCollectedSignal(int,int,bool,bool,bool,bool,bool,bool,int,int,QString)),
            this, SLOT(StatisticsCollected(int,int,bool,bool,bool,bool,bool,bool,int,int,QString)));
    connect(Singleton::Instance().GetControlProgram(), SIGNAL(ModelingFinishedSignal()), this,
            SLOT(ModelingFinished()));
    connect(Singleton::Instance().GetControlProgram(), SIGNAL(PatientKoffsSignal(double,double,double,double)), this,
            SLOT(PatientKoffs(double,double,double,double)));
    ui->lstStatistics->clear();
    ui->lblPatientsKoffs->setText("");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnStartModeling_clicked()
{
    try
    {
        if (ui->edtModelingTime->text() == "")
            QMessageBox::information(this, "Error", "You have not inputed all data!", QMessageBox::Ok);
        else
        {
            double endModelingTime = ui->edtModelingTime->text().toDouble();
            Singleton::Instance().GetControlProgram()->ConfigureSystem(10, 20, 10, 20, 10, 30, 20, 40, 10, 20, 10, 15, 10, 20,
                                                                       5, 15, 1, 1, 15, endModelingTime);
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

void MainWindow::StatisticsCollected(int patientsNumberFirstQueue, int patientsNumberSecondQueue,
                                     bool surgeonStatus, bool dentistStatus, bool physicianStatus, bool ophtStatus,
                                     bool xRayStatus, bool treatStatus, int dropPatientsNumber, int servicedPatientsNumber,
                                     const QString& currentTime)
{
    QString surgeonStatusStr = (surgeonStatus) ? "Обслуживает пациента\n" : "Не занят\n";
    QString dentistStatusStr = (dentistStatus) ? "Обслуживает пациента\n" : "Не занят\n";
    QString physicianStatusStr = (physicianStatus) ? "Обслуживает пациента\n" : "Не занят\n";
    QString ophtStatusStr = (ophtStatus) ? "Обслуживает пациента\n" : "Не занят\n";
    QString xRayStatusStr = (xRayStatus) ? "Обслуживает пациента\n" : "Не занят\n";
    QString treatStatusStr = (treatStatus) ? "Обслуживает пациента\n" : "Не занят\n";

    QString strInfo = currentTime + "\n" +
            "----------------------------------------------------------------------------\n" +
            "Количество пациентов в первой очереди = " + QString::number(patientsNumberFirstQueue) + "\n" +
            "Количество пациентов во второй очереди = " + QString::number(patientsNumberSecondQueue) + "\n" +
            "Статус хирурга = " + surgeonStatusStr +
            "Статус стоматолога = " + dentistStatusStr +
            "Статус терапевта = " + physicianStatusStr +
            "Статус окулиста = " + ophtStatusStr +
            "Статус рентген-кабинета = " + xRayStatusStr +
            "Статус процедурного кабинета = " + treatStatusStr +
            "Количество необслуженных пациентов = " + QString::number(dropPatientsNumber) + "\n" +
            "Количество обслуженных пациентов = " + QString::number(servicedPatientsNumber) + "\n";
    ui->lstStatistics->addItem(strInfo);
}

void MainWindow::PatientKoffs(double firstInterval, double secondInterval, double thirdInterval,
                                    double fourthInterval)
{
    QString strPatientsKoff = "Процент пациентов с 08.00 до 10.00 = " + QString::number(firstInterval * 100) + "\n" +
                      "Процент пациентов с 10.00 до 13.00 = " + QString::number(secondInterval * 100) + "\n" +
                      "Процент пациентов с 13.00 до 17.00 = " + QString::number(thirdInterval * 100) + "\n" +
                      "Процент пациентов после 17.00 = " + QString::number(fourthInterval * 100) + "\n";

    ui->lblPatientsKoffs->setText(strPatientsKoff);

}
