#include "Patient.h"

int Patient::GetServiceType(int lowBorderService, int highBorderService)
{
    previousServiceType = LKG::GenerateValue(previousServiceType);
    double serviceType = LKG::TransformValueIntoBorders(previousServiceType, lowBorderService, highBorderService);
    return (int) serviceType;
}

