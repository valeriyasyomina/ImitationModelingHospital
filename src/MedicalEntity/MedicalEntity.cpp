#include "MedicalEntity.h"

int MedicalEntity::GenerateNextPatientService(int lowBorderService, int highBorderService)
{
    double previousServiceType = LKG::GenerateValue(previousServiceType);
    double serviceType = LKG::TransformValueIntoBorders(previousServiceType, lowBorderService, highBorderService);
    return (int) serviceType;
}
