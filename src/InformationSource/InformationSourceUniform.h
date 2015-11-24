#ifndef INFORMATIONSOURCEUNIFORM_H
#define INFORMATIONSOURCEUNIFORM_H

#include "../ProcessingUnit/ProcessingUnit.h"

class InformationSourceUniform : public ProcessingUnit
{
public:
    InformationSourceUniform(): ProcessingUnit(){}
    InformationSourceUniform(double a, double b) : ProcessingUnit(a, b) {}
    ~InformationSourceUniform() {}
};

#endif // INFORMATIONSOURCEUNIFORM_H
