#ifndef STATISTICSBLOCK_H
#define STATISTICSBLOCK_H

#include <QObject>

class StatisticsBlock : public QObject
{
    Q_OBJECT
public:
    StatisticsBlock() {}
    ~StatisticsBlock() {}

    void CollectStatistics(int requestProcessedNumber, int requestDropNumber);
signals:
    void CollectStatisticsSignal(double requestDropKoff);
};

#endif // STATISTICSBLOCK_H
