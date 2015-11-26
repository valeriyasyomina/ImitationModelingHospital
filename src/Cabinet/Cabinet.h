#ifndef CABINET_H
#define CABINET_H

#include "../MedicalEntity/MedicalEntityQueued.h"

class Cabinet : public MedicalEntityQueued
{
public:
    Cabinet(): MedicalEntityQueued(){}
    Cabinet(double a, double b) : MedicalEntityQueued(a, b) {}
    Cabinet(double a, double b, int maxQueueSize) : MedicalEntityQueued(a, b, maxQueueSize) {}
    ~Cabinet() {}
};

#endif // CABINET_H
