#include "StatisticsBlock.h"

void StatisticsBlock::CollectStatistics(int patientsNumberFirstQueue, int patientsNumberSecondQueue,
                                        bool surgeonStatus, bool dentistStatus, bool physicianStatus, bool ophtStatus,
                                        bool xRayStatus, bool treatStatus, int dropPatientsNumber, int servicedPatientsNumber)
{
    emit CollectStatisticsSignal(patientsNumberFirstQueue, patientsNumberSecondQueue,
                                 surgeonStatus, dentistStatus, physicianStatus, ophtStatus,
                                 xRayStatus, treatStatus, dropPatientsNumber, servicedPatientsNumber);
}
