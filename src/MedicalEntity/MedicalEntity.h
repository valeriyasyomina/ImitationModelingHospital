#ifndef MEDICALENTITY_H
#define MEDICALENTITY_H

#include "../ProcessingUnit/ProcessingUnit.h"
#include "../Patient/Patient.h"

class MedicalEntity : public ProcessingUnit
{
protected:
    bool isServicingPatient;
    Patient patient;
public:
    MedicalEntity(): ProcessingUnit(), isServicingPatient(false) {}
    MedicalEntity(double a, double b) : ProcessingUnit(a, b), isServicingPatient(false) {}
    ~MedicalEntity() {}

    virtual void StartServicePatient(const Patient& patient) {this->patient = patient; isServicingPatient = true;}
    virtual Patient StopServicePatient() {isServicingPatient = false; return patient;}
    virtual bool IsServicingPatient() const {return isServicingPatient;}
    virtual int GenerateNextPatientService(int lowBorderService, int highBorderService);
};

#endif // MEDICALENTITY_H
