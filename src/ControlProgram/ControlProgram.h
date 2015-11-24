#ifndef CONTROLPROGRAM_H
#define CONTROLPROGRAM_H

#include "../StatisticsBlock/StatisticsBlock.h"
#include "../InformationSource/InformationSource.h"
#include "../Doctor/Dentist.h"
#include "../Doctor/Ophthalmologist.h"
#include "../Doctor/Physician.h"
#include "../Doctor/Surgeon.h"
#include "../MedicalReceptionist/MedicalReceptionist.h"
#include <math.h>

const int MAX_QUEUE_SIZE = 200;

const int ARRAY_SIZE = 7;

const int INFORMATION_SOURSE_INDEX = 0;
const int FIRST_RECEPTIONIST_INDEX = 1;
const int SECOND_RECEPTIONIST_INDEX = 2;
const int DENTIST_INDEX = 3;
const int SURGEON_INDEX = 4;
const int PHYSICIAN_INDEX = 5;
const int OPHTHALMOLOGIST_INDEX = 6;

const double EPSILON = 1e-3;
const double IDLE_TIME = -1.0;

const int MAX_DENTIST_QUEUE_SIZE = 30;
const int MAX_SURGEON_QUEUE_SIZE = 20;
const int MAX_PHYSICIAN_QUEUE_SIZE = 40;
const int MAX_OPHTHALMOLOGIST_QUEUE_SIZE = 35;

class ControlProgram : public QObject
{
    Q_OBJECT
private:
    InformationSource* informationSource;
    MedicalReceptionist* firstReceptionist;
    MedicalReceptionist* secondReceptionist;
    Doctor* dentist;
    Doctor* surgeon;
    Doctor* physician;
    Doctor* ophthalmologist;
    Queue<Patient> firstReceptionistQueue;
    Queue<Patient> secondReceptionistQueue;

    double timeArray[ARRAY_SIZE];
    double currentModelingTime;
    double endModelingTime;

    int dropPatientNumber;
    int servicedPatientNumber;

    double prevGenValueForPatientServiceType;
public:
    ControlProgram();
    ~ControlProgram();

    void ConfigureSystem(double aFirstReceptionist, double bFirstReceptionist, double aSecondReceptionist,
                         double bSecondReceptionist, double aDentist, double bDentist,
                         double aSurgeon, double bSurgeon, double aPhysician,
                         double bPhysician, double aOphthalmologist, double bOphthalmologist,
                         double sigma, double matExp, double maxBorder, double endModelingTime);
    void StartModeling();
private:
    double GetMinTime();
    void RealizeEvents();
    void CleanTimeArray();
    void FreeSystem();
public slots:
    void StatisticsCollected(double requestDropKoff);
signals:
    void StatisticsCollectedSignal(double requestDropKoff);
    void ModelingFinishedSignal();
};

#endif // CONTROLPROGRAM_H
