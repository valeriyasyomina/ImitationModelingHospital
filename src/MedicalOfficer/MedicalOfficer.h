#ifndef MEDICALOFFICER_H
#define MEDICALOFFICER_H

#include "../ProcessingUnit/ProcessingUnit.h"
#include "../Patient/Patient.h"

class MedicalOfficer : public ProcessingUnit
{
protected:
    bool isServicingPatient;
public:
    MedicalOfficer(): ProcessingUnit(), isServicingPatient(false) {}
    MedicalOfficer(double a, double b) : ProcessingUnit(a, b), isServicingPatient(false) {}
    ~MedicalOfficer() {}

    virtual void StartServicePatient() {isServicingPatient = true;}
    virtual void StopServicePatient() {isServicingPatient = false;}
    virtual bool IsServicingPatient() const {return isServicingPatient;}
};

#endif // MEDICALOFFICER_H
