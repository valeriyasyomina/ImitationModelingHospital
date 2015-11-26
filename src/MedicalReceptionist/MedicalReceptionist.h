#ifndef MEDICALRECEPTIONIST_H
#define MEDICALRECEPTIONIST_H

#include "../MedicalEntity/MedicalEntity.h"
#include "../Patient/Patient.h"

class MedicalReceptionist : public MedicalEntity
{
public:
    MedicalReceptionist(): MedicalEntity(){}
    MedicalReceptionist(double a, double b) : MedicalEntity(a, b) {}

    ~MedicalReceptionist() {}

  /*  virtual void StartServicingPatient(const Patient& patient)
    {
        this->patient = patient;
        this->StartServicePatient();
    }
    virtual Patient StopServicingPatient()
    {
        this->StopServicePatient();
        return patient;
    }*/
};


#endif // MEDICALRECEPTIONIST_H
