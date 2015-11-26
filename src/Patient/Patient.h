#ifndef PATIENT_H
#define PATIENT_H

#include "../Generator/LKG.h"
#include <QDateTime>

class Patient
{
protected:
    int service;
    double previousServiceType;
public:
    Patient(): service(0), previousServiceType(0.0) {}
    Patient(int lowBorderService, int highBorderService, double previousServiceType)
    {
        this->previousServiceType = previousServiceType;
        service = GetServiceType(lowBorderService, highBorderService);
    }
    Patient(const Patient& patient)
    {
        service = patient.service;
        previousServiceType = patient.previousServiceType;
    }
    Patient operator=(const Patient& patient)
    {
        service = patient.service;
        previousServiceType = patient.previousServiceType;
        return *this;
    }
    ~Patient() {}

    int GetServiceType() const {return service;}
    void SetServiceType(int serviceType) {service = serviceType;}
    double GetPreviousGeneratedValue() const {return previousServiceType;}
protected:
    virtual int GetServiceType(int lowBorderService, int highBorderService);
};

#endif // PATIENT_H
