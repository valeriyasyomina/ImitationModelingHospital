#ifndef MEDICALENTITYQUEUED_H
#define MEDICALENTITYQUEUED_H

#include "../Queue/Queue.h"
#include "MedicalEntity.h"

class MedicalEntityQueued : public MedicalEntity
{
protected:
    Queue<Patient> patientQueue;
    int servicedPatientsNumber;
    int maxQueueSize;
public:
    MedicalEntityQueued(): MedicalEntity(){}
    MedicalEntityQueued(double a, double b) : MedicalEntity(a, b), servicedPatientsNumber(0),
                                              maxQueueSize(0) {}
    MedicalEntityQueued(double a, double b, int maxQueueSize) : MedicalEntity(a, b), patientQueue(maxQueueSize),
                                                                servicedPatientsNumber(0), maxQueueSize(0) {}
    ~MedicalEntityQueued() {}

    virtual bool IsAnyPatientInQueue() const {return !patientQueue.isEmpty(); }
    virtual bool IsPatientQueueFull() const {return patientQueue.isFull(); }

    virtual void AddPatientToQueue(const Patient& patient) {patientQueue.Add(patient);}
    virtual Patient GetPatientFromQueue() {return patientQueue.Get();}

    virtual int GetPatientsNumberInQueue() const {return patientQueue.Size();}
    virtual void IncServedPatientsNumber() {servicedPatientsNumber++;}
    virtual int GetServedPatientsNumber() const {return servicedPatientsNumber;}

    virtual int GetMaxQueueSize() const {return maxQueueSize;}
    virtual void SetMaxQueueSize(int maxSize) {maxQueueSize = maxSize;}
};

#endif // MEDICALENTITYQUEUED_H
