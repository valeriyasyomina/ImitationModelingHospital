#ifndef TREATMENTROOM_H
#define TREATMENTROOM_H

#include "Cabinet.h"

class TreatmentRoom : public Cabinet
{
public:
    TreatmentRoom(): Cabinet(){}
    TreatmentRoom(double a, double b) : Cabinet(a, b) {}
    TreatmentRoom(double a, double b, int maxQueueSize) : Cabinet(a, b, maxQueueSize) {}
    ~TreatmentRoom() {}
};

#endif // TREATMENTROOM_H
