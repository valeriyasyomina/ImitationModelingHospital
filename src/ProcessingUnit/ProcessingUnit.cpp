#include "ProcessingUnit.h"

double ProcessingUnit::GetProcessTime()
{    
    previousTime = LKG::GenerateValue(previousTime);
    double resultTime = LKG::TransformValueIntoBorders(previousTime, a, b);
    workingTime += resultTime;
    return resultTime;
}
