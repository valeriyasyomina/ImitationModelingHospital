#ifndef SURGEON_H
#define SURGEON_H

#include "Doctor.h"

class Surgeon : public Doctor
{
public:
    Surgeon(): Doctor(){}
    Surgeon(double a, double b) : Doctor(a, b) {}
    Surgeon(double a, double b, int maxQueueSize) : Doctor(a, b, maxQueueSize) {}
    ~Surgeon() {}
};

#endif // SURGEON_H
