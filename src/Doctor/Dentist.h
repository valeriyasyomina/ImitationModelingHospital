#ifndef DENTIST_H
#define DENTIST_H

#include "Doctor.h"

class Dentist : public Doctor
{
public:
    Dentist(): Doctor(){}
    Dentist(double a, double b) : Doctor(a, b) {}
    Dentist(double a, double b, int maxQueueSize) : Doctor(a, b, maxQueueSize) {}
    ~Dentist() {}
};

#endif // DENTIST_H
