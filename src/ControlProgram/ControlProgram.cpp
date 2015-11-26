#include "ControlProgram.h"

ControlProgram::ControlProgram(): firstReceptionistQueue(MAX_QUEUE_SIZE), secondReceptionistQueue(MAX_QUEUE_SIZE)
{
    informationSource = NULL;

    firstReceptionist = NULL;
    secondReceptionist = NULL;
    dentist = NULL;
    surgeon = NULL;
    physician = NULL;
    ophthalmologist = NULL;
    treatmentRoom = NULL;
    xRayRoom = NULL;
    statisticsBlock = NULL;

    currentModelingTime = 0.0;
    endModelingTime = 0.0;

    dropPatientNumber = 0;
    servicedPatientNumber = 0;

    firstIntervalPatientsNumber = 0;
    secondIntervalPatientsNumber = 0;
    thirdIntervalPatientsNumber = 0;
    fourthIntervalPatientsNumber = 0;

    prevGenValueForPatientServiceType = QDateTime::currentMSecsSinceEpoch();

    CleanTimeArray();
}

ControlProgram::~ControlProgram()
{
    FreeSystem();
}

void ControlProgram::ConfigureSystem(double aFirstReceptionist, double bFirstReceptionist, double aSecondReceptionist,
                                     double bSecondReceptionist, double aDentist, double bDentist,
                                     double aSurgeon, double bSurgeon, double aPhysician,
                                     double bPhysician, double aOphthalmologist, double bOphthalmologist,
                                     double aXRayRoom, double bXRayRoom, double aTreatRoom, double bTreatRoom,
                                     double sigma, double matExp, double maxBorder, double endModelingTime)
{
    try
    {
        if (sigma < 0 || (int)sigma == (int)maxBorder)
            throw ErrorInputDataException("Error input parameters in ControlProgram::ConfigureSystem");

        FreeSystem();

        informationSource = new InformationSourceUniform(1, 2);
        firstReceptionist = new MedicalReceptionist(aFirstReceptionist, bFirstReceptionist);
        secondReceptionist = new MedicalReceptionist(aSecondReceptionist, bSecondReceptionist);

        dentist = new Dentist(aDentist, bDentist, MAX_QUEUE_SIZE);
        surgeon = new Surgeon(aSurgeon, bSurgeon, MAX_QUEUE_SIZE);
        physician = new Physician(aPhysician, bPhysician, MAX_QUEUE_SIZE);
        ophthalmologist = new Ophthalmologist(aOphthalmologist, bOphthalmologist, MAX_QUEUE_SIZE);

        xRayRoom = new XRayRoom(aXRayRoom, bXRayRoom, MAX_QUEUE_SIZE);
        treatmentRoom = new TreatmentRoom(aTreatRoom, bTreatRoom, MAX_QUEUE_SIZE);
        statisticsBlock = new StatisticsBlock();

        currentModelingTime = 0.0;
        this->endModelingTime = endModelingTime;
        firstIntervalPatientsNumber = 0;
        secondIntervalPatientsNumber = 0;
        thirdIntervalPatientsNumber = 0;
        fourthIntervalPatientsNumber = 0;

        prevGenValueForPatientServiceType = QDateTime::currentMSecsSinceEpoch();

        connect(statisticsBlock, SIGNAL(CollectStatisticsSignal(int,int,bool,bool,bool,bool,bool,bool,int,int)), this,
                SLOT(StatisticsCollected(int,int,bool,bool,bool,bool,bool,bool,int,int)));

        timeArray[INFORMATION_SOURSE_INDEX] = informationSource->GetProcessTime();
        timeArray[FIRST_RECEPTIONIST_INDEX] = IDLE_TIME;
        timeArray[SECOND_RECEPTIONIST_INDEX] = IDLE_TIME;
        timeArray[SURGEON_INDEX] = IDLE_TIME;
        timeArray[OPHTHALMOLOGIST_INDEX] = IDLE_TIME;
        timeArray[PHYSICIAN_INDEX] = IDLE_TIME;
        timeArray[DENTIST_INDEX] = IDLE_TIME;
        timeArray[XRAYROOM_INDEX] = IDLE_TIME;
        timeArray[TREATMENTROOM_INDEX] = IDLE_TIME;
    }
    catch (std::bad_alloc& exception)
    {
        throw AllocMemoryException("Erro allocate memory in ControlProgram::ConfigureSystem");
    }
}

void ControlProgram::StartModeling()
{
    for (currentModelingTime = 0.0; currentModelingTime <= endModelingTime; currentModelingTime)
    {
        statisticsBlock->CollectStatistics(firstReceptionistQueue.Size(), secondReceptionistQueue.Size(),
                                           surgeon->IsServicingPatient(), dentist->IsServicingPatient(),
                                           physician->IsServicingPatient(), ophthalmologist->IsServicingPatient(),
                                           xRayRoom->IsServicingPatient(), treatmentRoom->IsServicingPatient(),
                                           dropPatientNumber, servicedPatientNumber);
        currentModelingTime = GetMinTime();
        if (currentModelingTime <= FIRST_INTERVAL_BORDER)  // с 8.00 до 10.00 утра
        {
            informationSource->SetA(1);
            informationSource->SetB(2);
        }
        //    informationSource->SetMatExp(FIRST_INTERVAL_MAT_EXP);
        else if (currentModelingTime > FIRST_INTERVAL_BORDER
                 && currentModelingTime <= SECOND_INTERVAL_BORDER)  // с 10.00 до 13.00
        {
            informationSource->SetA(2);
            informationSource->SetB(5);
        }
          //  informationSource->SetMatExp(SECOND_INTERVAL_MAT_EXP);
        else if (currentModelingTime > SECOND_INTERVAL_BORDER
                 && currentModelingTime <= THIRD_INTERVAL_BORDER)  // с 13.00 до 17.00
        {
            informationSource->SetA(5);
            informationSource->SetB(10);
        }
           // informationSource->SetMatExp(THIRD_INTERVAL_MAT_EXP);
        else if (currentModelingTime > THIRD_INTERVAL_BORDER)   // после 17.00
        {
            informationSource->SetA(4);
            informationSource->SetB(6);
        }
           // informationSource->SetMatExp(FOURTH_INTERVAL_MAT_EXP);

        RealizeEvents();
    }
    emit ModelingFinishedSignal();
    CountKoeffs();
}

double ControlProgram::GetMinTime()
{
    double minTime = timeArray[0];
    for (int i = 1; i < ARRAY_SIZE; i++)
    {
        if (timeArray[i] < minTime && timeArray[i] != IDLE_TIME)
            minTime = timeArray[i];
    }
    return minTime;
}
void ControlProgram::RealizeEvents()
{
    if (fabs(timeArray[INFORMATION_SOURSE_INDEX] - currentModelingTime) < EPSILON)
    {
        timeArray[INFORMATION_SOURSE_INDEX] = informationSource->GetProcessTime() + currentModelingTime;
        Patient patient(DENTIST_INDEX, TREATMENTROOM_INDEX + 1, prevGenValueForPatientServiceType);
        prevGenValueForPatientServiceType = patient.GetPreviousGeneratedValue();

        if (patient.GetServiceType() == XRAYROOM_INDEX || patient.GetServiceType() == TREATMENTROOM_INDEX)
        {
            if (!secondReceptionistQueue.isFull())
                secondReceptionistQueue.Add(patient);
            else
                dropPatientNumber++;
        }
        else
        {
            if (!firstReceptionistQueue.isFull())
                firstReceptionistQueue.Add(patient);
            else
                dropPatientNumber++;
        }

        if (currentModelingTime <= FIRST_INTERVAL_BORDER)  // с 8.00 до 10.00 утра
            firstIntervalPatientsNumber++;
        else if (currentModelingTime > FIRST_INTERVAL_BORDER
                 && currentModelingTime <= SECOND_INTERVAL_BORDER)  // с 10.00 до 13.00
            secondIntervalPatientsNumber++;
        else if (currentModelingTime > SECOND_INTERVAL_BORDER
                 && currentModelingTime <= THIRD_INTERVAL_BORDER)  // с 13.00 до 17.00
            thirdIntervalPatientsNumber++;
        else if (currentModelingTime > THIRD_INTERVAL_BORDER)   // после 17.00
            fourthIntervalPatientsNumber++;

    }
    if (fabs(timeArray[FIRST_RECEPTIONIST_INDEX] - currentModelingTime) < EPSILON &&
            firstReceptionist->IsServicingPatient())
    {
         // заканчиваем обслуживание пациента
        Patient patient = firstReceptionist->StopServicePatient();
        timeArray[FIRST_RECEPTIONIST_INDEX] = IDLE_TIME;
        int patientServiceType = patient.GetServiceType();
        if (patientServiceType == DENTIST_INDEX)
        {
            if (dentist->GetServedPatientsNumber() < MAX_DENTIST_QUEUE_SIZE)
                dentist->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
        else if (patientServiceType == PHYSICIAN_INDEX)
        {
            if (physician->GetServedPatientsNumber() < MAX_PHYSICIAN_QUEUE_SIZE)
                physician->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
        else if (patientServiceType == OPHTHALMOLOGIST_INDEX)
        {
            if (ophthalmologist->GetServedPatientsNumber() < MAX_OPHTHALMOLOGIST_QUEUE_SIZE)
                ophthalmologist->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
        else if (patientServiceType == SURGEON_INDEX)
        {
            if (surgeon->GetServedPatientsNumber() < MAX_SURGEON_QUEUE_SIZE)
                surgeon->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
        else if (patientServiceType == TREATMENTROOM_INDEX)
        {
            if (treatmentRoom->GetServedPatientsNumber() < MAX_TREATMENTROOM_QUEUE_SIZE)
                treatmentRoom->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
        else if (patientServiceType == XRAYROOM_INDEX)
        {
            if (xRayRoom->GetServedPatientsNumber() < MAX_XRAYROOM_QUEUE_SIZE)
                xRayRoom->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }

    }
    if (fabs(timeArray[SECOND_RECEPTIONIST_INDEX] - currentModelingTime) < EPSILON &&
            secondReceptionist->IsServicingPatient())
    {
        // заканчиваем обслуживание пациента
        Patient patient = secondReceptionist->StopServicePatient();
        timeArray[SECOND_RECEPTIONIST_INDEX] = IDLE_TIME;
        int patientServiceType = patient.GetServiceType();
        if (patientServiceType == DENTIST_INDEX)
        {
            if (dentist->GetServedPatientsNumber() < MAX_DENTIST_QUEUE_SIZE)
                dentist->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
        else if (patientServiceType == PHYSICIAN_INDEX)
        {
            if (physician->GetServedPatientsNumber() < MAX_PHYSICIAN_QUEUE_SIZE)
                physician->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
        else if (patientServiceType == OPHTHALMOLOGIST_INDEX)
        {
            if (ophthalmologist->GetServedPatientsNumber() < MAX_OPHTHALMOLOGIST_QUEUE_SIZE)
                ophthalmologist->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
        else if (patientServiceType == SURGEON_INDEX)
        {
            if (surgeon->GetServedPatientsNumber() < MAX_SURGEON_QUEUE_SIZE)
                surgeon->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
        else if (patientServiceType == TREATMENTROOM_INDEX)
        {
            if (treatmentRoom->GetServedPatientsNumber() < MAX_TREATMENTROOM_QUEUE_SIZE)
                treatmentRoom->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
        else if (patientServiceType == XRAYROOM_INDEX)
        {
            if (xRayRoom->GetServedPatientsNumber() < MAX_XRAYROOM_QUEUE_SIZE)
                xRayRoom->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
    }

    if (!firstReceptionistQueue.isEmpty() && fabs(timeArray[FIRST_RECEPTIONIST_INDEX] - IDLE_TIME) < EPSILON
            && !firstReceptionist->IsServicingPatient())
    {
        //берем пациента на обслуживание
        timeArray[FIRST_RECEPTIONIST_INDEX] = firstReceptionist->GetProcessTime() + currentModelingTime;
        firstReceptionist->StartServicePatient(firstReceptionistQueue.Get());
    }
    if (!secondReceptionistQueue.isEmpty() && fabs(timeArray[SECOND_RECEPTIONIST_INDEX] - IDLE_TIME) < EPSILON
            && !secondReceptionist->IsServicingPatient())
    {
        //берем пациента на обслуживание
        timeArray[SECOND_RECEPTIONIST_INDEX] = firstReceptionist->GetProcessTime() + currentModelingTime;
        secondReceptionist->StartServicePatient(secondReceptionistQueue.Get());
    }

    if (fabs(timeArray[DENTIST_INDEX] - currentModelingTime) < EPSILON &&
                dentist->IsServicingPatient())
    {
        // Врач заканчивает обслуживание пациента
        Patient patient = dentist->StopServicePatient();
        timeArray[DENTIST_INDEX] = IDLE_TIME;
        int nextPatientServiceType = dentist->GenerateNextPatientService(XRAYROOM_INDEX, EXIT_HOSPITAL_INDEX + 1);
        patient.SetServiceType(nextPatientServiceType);
        servicedPatientNumber++;
        dentist->IncServedPatientsNumber();
        if (nextPatientServiceType == XRAYROOM_INDEX)
        {
            if (xRayRoom->GetServedPatientsNumber() < MAX_XRAYROOM_QUEUE_SIZE)
                xRayRoom->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
        else if (nextPatientServiceType == TREATMENTROOM_INDEX)
        {
            if (treatmentRoom->GetServedPatientsNumber() < MAX_TREATMENTROOM_QUEUE_SIZE)
                treatmentRoom->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }     
    }

    if (fabs(timeArray[SURGEON_INDEX] - currentModelingTime) < EPSILON &&
                surgeon->IsServicingPatient())
    {
        // Врач заканчивает обслуживание пациента
        Patient patient = surgeon->StopServicePatient();
        timeArray[SURGEON_INDEX] = IDLE_TIME;
        int nextPatientServiceType = surgeon->GenerateNextPatientService(XRAYROOM_INDEX, EXIT_HOSPITAL_INDEX);
        patient.SetServiceType(nextPatientServiceType);
        servicedPatientNumber++;
        surgeon->IncServedPatientsNumber();
        if (nextPatientServiceType == XRAYROOM_INDEX)
        {
            if (xRayRoom->GetServedPatientsNumber() < MAX_XRAYROOM_QUEUE_SIZE)
                xRayRoom->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
        else if (nextPatientServiceType == TREATMENTROOM_INDEX)
        {
            if (treatmentRoom->GetServedPatientsNumber() < MAX_TREATMENTROOM_QUEUE_SIZE)
                treatmentRoom->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }       
    }

    if (fabs(timeArray[OPHTHALMOLOGIST_INDEX] - currentModelingTime) < EPSILON &&
                ophthalmologist->IsServicingPatient())
    {
        // Врач заканчивает обслуживание пациента
        ophthalmologist->StopServicePatient();
        timeArray[OPHTHALMOLOGIST_INDEX] = IDLE_TIME;
        servicedPatientNumber++;
        ophthalmologist->IncServedPatientsNumber();
    }

    if (fabs(timeArray[PHYSICIAN_INDEX] - currentModelingTime) < EPSILON &&
                physician->IsServicingPatient())
    {
        // Врач заканчивает обслуживание пациента
        Patient patient = physician->StopServicePatient();
        timeArray[PHYSICIAN_INDEX] = IDLE_TIME;
        int nextPatientServiceType = physician->GenerateNextPatientService(XRAYROOM_INDEX, EXIT_HOSPITAL_INDEX);
        patient.SetServiceType(nextPatientServiceType);
        servicedPatientNumber++;
        physician->IncServedPatientsNumber();
        if (nextPatientServiceType == XRAYROOM_INDEX)
        {
            if (xRayRoom->GetServedPatientsNumber() < MAX_XRAYROOM_QUEUE_SIZE)
                xRayRoom->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
        else if (nextPatientServiceType == TREATMENTROOM_INDEX)
        {
            if (treatmentRoom->GetServedPatientsNumber() < MAX_TREATMENTROOM_QUEUE_SIZE)
                treatmentRoom->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }       
    }

    if (fabs(timeArray[TREATMENTROOM_INDEX] - currentModelingTime) < EPSILON &&
                treatmentRoom->IsServicingPatient())
    {
        // Врач заканчивает обслуживание пациента
        treatmentRoom->StopServicePatient();
        timeArray[TREATMENTROOM_INDEX] = IDLE_TIME;
        servicedPatientNumber++;
        treatmentRoom->IncServedPatientsNumber();
    }

    if (fabs(timeArray[XRAYROOM_INDEX] - currentModelingTime) < EPSILON &&
                xRayRoom->IsServicingPatient())
    {
        // Врач заканчивает обслуживание пациента
        xRayRoom->StopServicePatient();
        timeArray[XRAYROOM_INDEX] = IDLE_TIME;
        servicedPatientNumber++;
        xRayRoom->IncServedPatientsNumber();
    }

    if (dentist->IsAnyPatientInQueue() && !dentist->IsServicingPatient() && fabs(timeArray[DENTIST_INDEX] - IDLE_TIME) < EPSILON)
    {
        timeArray[DENTIST_INDEX] = dentist->GetProcessTime() + currentModelingTime;
        dentist->StartServicePatient(dentist->GetPatientFromQueue());
    //    dentist->GetPatientToExamination();
    }

    if (surgeon->IsAnyPatientInQueue() && !surgeon->IsServicingPatient() && fabs(timeArray[SURGEON_INDEX] - IDLE_TIME) < EPSILON)
    {
        timeArray[SURGEON_INDEX] = surgeon->GetProcessTime() + currentModelingTime;
        surgeon->StartServicePatient(surgeon->GetPatientFromQueue());
        //surgeon->GetPatientToExamination();
    }

    if (ophthalmologist->IsAnyPatientInQueue() && !ophthalmologist->IsServicingPatient() &&
            fabs(timeArray[OPHTHALMOLOGIST_INDEX] - IDLE_TIME) < EPSILON)
    {
        timeArray[OPHTHALMOLOGIST_INDEX] = ophthalmologist->GetProcessTime() + currentModelingTime;
        ophthalmologist->StartServicePatient(ophthalmologist->GetPatientFromQueue());
      //  ophthalmologist->GetPatientToExamination();
    }

    if (physician->IsAnyPatientInQueue() && !physician->IsServicingPatient() && fabs(timeArray[PHYSICIAN_INDEX] - IDLE_TIME) < EPSILON)
    {
        timeArray[PHYSICIAN_INDEX] = physician->GetProcessTime() + currentModelingTime;
        physician->StartServicePatient(physician->GetPatientFromQueue());
      //  physician->GetPatientToExamination();
    }

    if (treatmentRoom->IsAnyPatientInQueue() && !treatmentRoom->IsServicingPatient() &&
            fabs(timeArray[TREATMENTROOM_INDEX] - IDLE_TIME) < EPSILON)
    {
        timeArray[TREATMENTROOM_INDEX] = treatmentRoom->GetProcessTime() + currentModelingTime;
        treatmentRoom->StartServicePatient(treatmentRoom->GetPatientFromQueue());
    }

    if (xRayRoom->IsAnyPatientInQueue() && !xRayRoom->IsServicingPatient() && fabs(timeArray[XRAYROOM_INDEX] - IDLE_TIME) < EPSILON)
    {
        timeArray[XRAYROOM_INDEX] = xRayRoom->GetProcessTime() + currentModelingTime;
        xRayRoom->StartServicePatient(xRayRoom->GetPatientFromQueue());
    }
}

void ControlProgram::CleanTimeArray()
{
    for (int i = 0; i < ARRAY_SIZE; i++)
        timeArray[i] = 0.0;
}

void ControlProgram::FreeSystem()
{
    if (informationSource)
    {
        delete informationSource;
        informationSource = NULL;
    }

    if (firstReceptionist)
    {
        delete firstReceptionist;
        firstReceptionist = NULL;
    }
    if (secondReceptionist)
    {
        delete secondReceptionist;
        secondReceptionist = NULL;
    }

    if (dentist)
    {
        delete dentist;
        dentist = NULL;
    }
    if (ophthalmologist)
    {
        delete ophthalmologist;
        ophthalmologist = NULL;
    }
    if (physician)
    {
        delete physician;
        physician = NULL;
    }
    if (surgeon)
    {
        delete surgeon;
        surgeon = NULL;
    }

    if (xRayRoom)
    {
        delete xRayRoom;
        xRayRoom = NULL;
    }
    if (treatmentRoom)
    {
        delete treatmentRoom;
        treatmentRoom = NULL;
    }

    if (statisticsBlock)
    {
        delete statisticsBlock;
        statisticsBlock = NULL;
    }

}

QString ControlProgram::GetCurrentTime()
{
    int hours = currentModelingTime / MINUTEST_IN_HOUR + BEGIN_WORKING_TIME;
    int minutes = (int) currentModelingTime % MINUTEST_IN_HOUR;

    return QString::number(hours) + ":" + QString::number(minutes);
}

void ControlProgram::CountKoeffs()
{
    int allPatientsNumber = firstIntervalPatientsNumber + secondIntervalPatientsNumber + thirdIntervalPatientsNumber +
            fourthIntervalPatientsNumber;
    double firstIntervalKoff = (double) firstIntervalPatientsNumber / allPatientsNumber;
    double secondIntervalKoff = (double) secondIntervalPatientsNumber / allPatientsNumber;
    double thirdIntervalKoff = (double) thirdIntervalPatientsNumber / allPatientsNumber;
    double fourthIntervalKoff = (double) fourthIntervalPatientsNumber / allPatientsNumber;

    emit PatientKoffsSignal(firstIntervalKoff, secondIntervalKoff, thirdIntervalKoff, fourthIntervalKoff);
}

void ControlProgram::StatisticsCollected(int patientsNumberFirstQueue, int patientsNumberSecondQueue,
                                         bool surgeonStatus, bool dentistStatus, bool physicianStatus, bool ophtStatus,
                                         bool xRayStatus, bool treatStatus, int dropPatientsNumber, int servicedPatientsNumber)
{
    emit StatisticsCollectedSignal(patientsNumberFirstQueue, patientsNumberSecondQueue,
                                   surgeonStatus, dentistStatus, physicianStatus, ophtStatus,
                                   xRayStatus, treatStatus, dropPatientsNumber, servicedPatientsNumber, GetCurrentTime());
}

