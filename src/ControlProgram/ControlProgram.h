#ifndef CONTROLPROGRAM_H
#define CONTROLPROGRAM_H

#include "../StatisticsBlock/StatisticsBlock.h"
#include "../InformationSource/InformationSource.h"
#include "../Doctor/Dentist.h"
#include "../Doctor/Ophthalmologist.h"
#include "../Doctor/Physician.h"
#include "../Doctor/Surgeon.h"
#include "../MedicalReceptionist/MedicalReceptionist.h"
#include "../Cabinet/TreatmentRoom.h"
#include "../Cabinet/XRayRoom.h"
#include "../InformationSource/InformationSourceUniform.h"
#include <math.h>

const int MAX_QUEUE_SIZE = 200;

const int ARRAY_SIZE = 9;

const int INFORMATION_SOURSE_INDEX = 0;
const int FIRST_RECEPTIONIST_INDEX = 1;
const int SECOND_RECEPTIONIST_INDEX = 2;
const int DENTIST_INDEX = 3;
const int SURGEON_INDEX = 4;
const int PHYSICIAN_INDEX = 5;
const int OPHTHALMOLOGIST_INDEX = 6;
const int XRAYROOM_INDEX = 7;
const int TREATMENTROOM_INDEX = 8;
const int EXIT_HOSPITAL_INDEX = 9;

const double EPSILON = 1e-3;
const double IDLE_TIME = -1.0;

const int MAX_DENTIST_QUEUE_SIZE = 30;
const int MAX_SURGEON_QUEUE_SIZE = 20;
const int MAX_PHYSICIAN_QUEUE_SIZE = 40;
const int MAX_OPHTHALMOLOGIST_QUEUE_SIZE = 35; //35
const int MAX_XRAYROOM_QUEUE_SIZE = 10;
const int MAX_TREATMENTROOM_QUEUE_SIZE = 15;

const double FIRST_INTERVAL_BORDER = 120;
const double SECOND_INTERVAL_BORDER = 300;
const double THIRD_INTERVAL_BORDER = 540;

const int MINUTEST_IN_HOUR = 60;
const int BEGIN_WORKING_TIME = 8;

const double FIRST_INTERVAL_MAT_EXP = 1;
const double SECOND_INTERVAL_MAT_EXP = 3;
const double THIRD_INTERVAL_MAT_EXP = 10;
const double FOURTH_INTERVAL_MAT_EXP = 5;

class ControlProgram : public QObject
{
    Q_OBJECT
private:
    ProcessingUnit* informationSource;
    MedicalEntity* firstReceptionist;
    MedicalEntity* secondReceptionist;
    MedicalEntityQueued* dentist;
    MedicalEntityQueued* surgeon;
    MedicalEntityQueued* physician;
    MedicalEntityQueued* ophthalmologist;
    MedicalEntityQueued* xRayRoom;
    MedicalEntityQueued* treatmentRoom;
    Queue<Patient> firstReceptionistQueue;
    Queue<Patient> secondReceptionistQueue;
    StatisticsBlock* statisticsBlock;

    double timeArray[ARRAY_SIZE];
    double currentModelingTime;
    double endModelingTime;

    int dropPatientNumber;
    int servicedPatientNumber;
    double prevGenValueForPatientServiceType;

    int firstIntervalPatientsNumber;
    int secondIntervalPatientsNumber;
    int thirdIntervalPatientsNumber;
    int fourthIntervalPatientsNumber;
public:
    ControlProgram();
    ~ControlProgram();

    void ConfigureSystem(double aFirstReceptionist, double bFirstReceptionist, double aSecondReceptionist,
                         double bSecondReceptionist, double aDentist, double bDentist,
                         double aSurgeon, double bSurgeon, double aPhysician,
                         double bPhysician, double aOphthalmologist, double bOphthalmologist,
                         double aXRayRoom, double bXRayRoom, double aTreatRoom, double bTreatRoom,
                         double sigma, double matExp, double maxBorder, double endModelingTime);
    void StartModeling();
private:
    double GetMinTime();
    void RealizeEvents();
    void CleanTimeArray();
    void FreeSystem();
    void CountKoeffs();
    QString GetCurrentTime();
public slots:
    void StatisticsCollected(int patientsNumberFirstQueue, int patientsNumberSecondQueue,
                             bool surgeonStatus, bool dentistStatus, bool physicianStatus, bool ophtStatus,
                             bool xRayStatus, bool treatStatus, int dropPatientsNumber, int servicedPatientsNumber);
signals:
    void StatisticsCollectedSignal(int patientsNumberFirstQueue, int patientsNumberSecondQueue,
                                   bool surgeonStatus, bool dentistStatus, bool physicianStatus, bool ophtStatus,
                                   bool xRayStatus, bool treatStatus, int dropPatientsNumber, int servicedPatientsNumber,
                                   const QString& currentTime);
    void ModelingFinishedSignal();
    void PatientKoffsSignal(double, double, double, double);
};

#endif // CONTROLPROGRAM_H
