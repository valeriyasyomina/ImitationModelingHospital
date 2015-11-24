#ifndef PHYSICIAN_H
#define PHYSICIAN_H

#include "Doctor.h"

class Physician : public Doctor
{
public:
    Physician(): Doctor(){}
    Physician(double a, double b) : Doctor(a, b) {}
    Physician(double a, double b, int maxQueueSize) : Doctor(a, b, maxQueueSize) {}
    ~Physician() {}
};

#endif // PHYSICIAN_H
