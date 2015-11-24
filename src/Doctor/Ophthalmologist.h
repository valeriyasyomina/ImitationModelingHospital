#ifndef OPHTHALMOLOGIST_H
#define OPHTHALMOLOGIST_H

#include "Doctor.h"

class Ophthalmologist : public Doctor
{
public:
    Ophthalmologist(): Doctor(){}
    Ophthalmologist(double a, double b) : Doctor(a, b) {}
    Ophthalmologist(double a, double b, int maxQueueSize) : Doctor(a, b, maxQueueSize) {}
    ~Ophthalmologist() {}
};

#endif // OPHTHALMOLOGIST_H
