#ifndef MEDICALRECEPTIONIST_H
#define MEDICALRECEPTIONIST_H

#include "../MedicalOfficer/MedicalOfficer.h"
#include "../Patient/Patient.h"

class MedicalReceptionist : public MedicalOfficer
{
protected:
    Patient patient;
public:
    MedicalReceptionist(): MedicalOfficer(){}
    MedicalReceptionist(double a, double b) : MedicalOfficer(a, b) {}

    ~MedicalReceptionist() {}

    virtual void StartServicingPatient(const Patient& patient)
    {
        this->patient = patient;
        this->StartServicePatient();
    }
    virtual Patient StopServicingPatient()
    {
        this->StopServicePatient();
        return patient;
    }
};


#endif // MEDICALRECEPTIONIST_H
