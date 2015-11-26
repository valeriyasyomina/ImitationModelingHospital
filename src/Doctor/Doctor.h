#ifndef DOCTOR_H
#define DOCTOR_H

#include "../MedicalEntity/MedicalEntityQueued.h"

class Doctor : public MedicalEntityQueued
{
public:
    Doctor(): MedicalEntityQueued(){}
    Doctor(double a, double b) : MedicalEntityQueued(a, b) {}
    Doctor(double a, double b, int maxQueueSize) : MedicalEntityQueued(a, b, maxQueueSize) {}
    ~Doctor() {}
};

#endif // DOCTOR_H
