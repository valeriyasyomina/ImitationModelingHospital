#ifndef PROCESSINGUNIT_H
#define PROCESSINGUNIT_H

#include <QDateTime>
#include "../Generator/LKG.h"

class ProcessingUnit
{
protected:
    double a;
    double b;
    double previousTime;
    double workingTime;
    bool requestWasAssigned;
public:
    ProcessingUnit(): a(0.0), b(0.0), previousTime(QDateTime::currentMSecsSinceEpoch()),
        workingTime(0.0), requestWasAssigned(false) {}
    ProcessingUnit(double a, double b)
    {
        this->a = a;
        this->b = b;
        previousTime = QDateTime::currentMSecsSinceEpoch();
        workingTime = 0.0;
        requestWasAssigned = false;
    }
    virtual ~ProcessingUnit() {}

    virtual void SetA(double a) {this->a = a;}
    virtual void SetB(double b) {this->b = b;}
    virtual double GetProcessTime();
    virtual bool HasAssignedRequest() const {return requestWasAssigned;}
    virtual void SetRequestAssigned(bool requestAssigned) {requestWasAssigned = requestAssigned;}
    virtual double GetWorkingTime() const {return workingTime;}
};

#endif // PROCESSINGUNIT_H
