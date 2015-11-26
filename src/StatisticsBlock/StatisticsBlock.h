#ifndef STATISTICSBLOCK_H
#define STATISTICSBLOCK_H

#include <QObject>

class StatisticsBlock : public QObject
{
    Q_OBJECT
public:
    StatisticsBlock() {}
    ~StatisticsBlock() {}

    void CollectStatistics(int patientsNumberFirstQueue, int patientsNumberSecondQueue,
                           bool surgeonStatus, bool dentistStatus, bool physicianStatus, bool ophtStatus,
                           bool xRayStatus, bool treatStatus, int dropPatientsNumber, int servicedPatientsNumber);
signals:
    void CollectStatisticsSignal(int patientsNumberFirstQueue, int patientsNumberSecondQueue,
                                 bool surgeonStatus, bool dentistStatus, bool physicianStatus, bool ophtStatus,
                                 bool xRayStatus, bool treatStatus, int dropPatientsNumber, int servicedPatientsNumber);
};

#endif // STATISTICSBLOCK_H
