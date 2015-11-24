#ifndef DOCTOR_H
#define DOCTOR_H

#include "../MedicalOfficer/MedicalOfficer.h"
#include "../Queue/Queue.h"
#include "../Patient/Patient.h"

class Doctor : public MedicalOfficer
{
protected:
    Queue<Patient> patientQueue;
public:
    Doctor(): MedicalOfficer(){}
    Doctor(double a, double b) : MedicalOfficer(a, b) {}
    Doctor(double a, double b, int maxQueueSize) : MedicalOfficer(a, b), patientQueue(maxQueueSize) {}
    ~Doctor() {}

    bool IsAnyPatientInQueue() const {return !patientQueue.isEmpty(); }
    bool IsPatientQueueFull() const {return patientQueue.isFull(); }

    void AddPatientToQueue(const Patient& patient) {patientQueue.Add(patient);}
    void GetPatientToExamination() {patientQueue.Get();}
};

#endif // DOCTOR_H
