#include "StatisticsBlock.h"

void StatisticsBlock::CollectStatistics(int requestProcessedNumber, int requestDropNumber)
{
    double requestDropKoff = requestDropNumber + requestProcessedNumber == 0 ?
                0.0 : (double) requestDropNumber / (requestDropNumber + requestProcessedNumber);
    emit CollectStatisticsSignal(requestDropKoff);
}
