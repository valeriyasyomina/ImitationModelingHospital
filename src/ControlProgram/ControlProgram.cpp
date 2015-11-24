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

    currentModelingTime = 0.0;
    endModelingTime = 0.0;

    dropPatientNumber = 0;
    servicedPatientNumber = 0;

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
                                     double sigma, double matExp, double maxBorder, double endModelingTime)
{
    try
    {
        if (sigma < 0 || (int)sigma == (int)maxBorder)
            throw ErrorInputDataException("Error input parameters in ControlProgram::ConfigureSystem");

        FreeSystem();

        informationSource = new InformationSource(sigma, matExp, 0, maxBorder, 12);
        firstReceptionist = new MedicalReceptionist(aFirstReceptionist, bFirstReceptionist);
        secondReceptionist = new MedicalReceptionist(aSecondReceptionist, bSecondReceptionist);

        dentist = new Dentist(aDentist, bDentist, MAX_DENTIST_QUEUE_SIZE);
        surgeon = new Surgeon(aSurgeon, bSurgeon, MAX_SURGEON_QUEUE_SIZE);
        physician = new Physician(aPhysician, bPhysician, MAX_PHYSICIAN_QUEUE_SIZE);
        ophthalmologist = new Ophthalmologist(aOphthalmologist, bOphthalmologist, MAX_OPHTHALMOLOGIST_QUEUE_SIZE);


        currentModelingTime = 0.0;
        this->endModelingTime = endModelingTime;
        prevGenValueForPatientServiceType = QDateTime::currentMSecsSinceEpoch();



       /* firstOperator = new Operator(aFirstOperator, bFirstOperator);
        secondOperator = new Operator(aSecondOperator, bSecondOperator);
        thirdOperator = new Operator(aThirdOperator, bThirdOperator);

        statisticsBlock = new StatisticsBlock();

        firstStorage = new Memory(maxMemorySize);
        secondStorage = new Memory(maxMemorySize);

        firstComputer = new Computer(FIRST_COMPUTER_PROCESSING_TIME);
        secondComputer = new Computer(SECOND_COMPUTER_PROCESSING_TIME);

        connect(statisticsBlock, SIGNAL(CollectStatisticsSignal(double)), this,
                SLOT(StatisticsCollected(double)));

        this->requestNumberToBeProcessed = requestNumberToBeProcessed;
        requestProcessedNumber = 0;
        requestDropNumber = 0;*/

        timeArray[INFORMATION_SOURSE_INDEX] = informationSource->GenerateRequestTime();
        timeArray[FIRST_RECEPTIONIST_INDEX] = IDLE_TIME;
        timeArray[SECOND_RECEPTIONIST_INDEX] = IDLE_TIME;
        timeArray[SURGEON_INDEX] = IDLE_TIME;
        timeArray[OPHTHALMOLOGIST_INDEX] = IDLE_TIME;
        timeArray[PHYSICIAN_INDEX] = IDLE_TIME;
        timeArray[DENTIST_INDEX] = IDLE_TIME;
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
       // statisticsBlock->CollectStatistics(memory->Size(), memory->GetDropRequestNumber(), currentModelingTime,
                                         //  processingUnit->GetWorkingTime());
        currentModelingTime = GetMinTime();
        RealizeEvents();
    }
    emit ModelingFinishedSignal();
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
        timeArray[INFORMATION_SOURSE_INDEX] = informationSource->GenerateRequestTime() + currentModelingTime;
        Patient patient(DENTIST_INDEX, OPHTHALMOLOGIST_INDEX, prevGenValueForPatientServiceType);
        prevGenValueForPatientServiceType = patient.GetPreviousGeneratedValue();
        if (patient.GetServiceType() == PHYSICIAN_INDEX)
        {
            if (!firstReceptionistQueue.isFull())
                firstReceptionistQueue.Add(patient);
            else
                dropPatientNumber++;
        }
        else
        {
            if (!secondReceptionistQueue.isFull())
                secondReceptionistQueue.Add(patient);
            else
                dropPatientNumber++;
        }
    }
    if (fabs(timeArray[FIRST_RECEPTIONIST_INDEX] - currentModelingTime) < EPSILON &&
            firstReceptionist->IsServicingPatient())
    {
         // заканчиваем обслуживание пациента
        Patient patient = firstReceptionist->StopServicingPatient();
        timeArray[FIRST_RECEPTIONIST_INDEX] = IDLE_TIME;
        int patientServiceType = patient.GetServiceType();
        if (patientServiceType == DENTIST_INDEX)
        {
            if (!dentist->IsPatientQueueFull())
                dentist->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
        else if (patientServiceType == PHYSICIAN_INDEX)
        {
            if (!physician->IsPatientQueueFull())
                physician->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
        else if (patientServiceType == OPHTHALMOLOGIST_INDEX)
        {
            if (!ophthalmologist->IsPatientQueueFull())
                ophthalmologist->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
        else if (patientServiceType == SURGEON_INDEX)
        {
            if (!surgeon->IsPatientQueueFull())
                surgeon->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }

    }
    if (fabs(timeArray[SECOND_RECEPTIONIST_INDEX] - currentModelingTime) < EPSILON &&
            secondReceptionist->IsServicingPatient())
    {
        // заканчиваем обслуживание пациента
        Patient patient = secondReceptionist->StopServicingPatient();
        timeArray[SECOND_RECEPTIONIST_INDEX] = IDLE_TIME;
        int patientServiceType = patient.GetServiceType();
        if (patientServiceType == DENTIST_INDEX)
        {
            if (!dentist->IsPatientQueueFull())
                dentist->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
        else if (patientServiceType == PHYSICIAN_INDEX)
        {
            if (!physician->IsPatientQueueFull())
                physician->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
        else if (patientServiceType == OPHTHALMOLOGIST_INDEX)
        {
            if (!ophthalmologist->IsPatientQueueFull())
                ophthalmologist->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
        else if (patientServiceType == SURGEON_INDEX)
        {
            if (!surgeon->IsPatientQueueFull())
                surgeon->AddPatientToQueue(patient);
            else
                dropPatientNumber++;
        }
    }

    if (!firstReceptionistQueue.isEmpty() && fabs(timeArray[FIRST_RECEPTIONIST_INDEX] - IDLE_TIME) < EPSILON
            && !firstReceptionist->IsServicingPatient())
    {
        //берем пациента на обслуживание
        timeArray[FIRST_RECEPTIONIST_INDEX] = firstReceptionist->GetProcessTime() + currentModelingTime;
        firstReceptionist->StartServicingPatient(firstReceptionistQueue.Get());
    }
    if (!secondReceptionistQueue.isEmpty() && fabs(timeArray[SECOND_RECEPTIONIST_INDEX] - IDLE_TIME) < EPSILON
            && !secondReceptionist->IsServicingPatient())
    {
        //берем пациента на обслуживание
        timeArray[SECOND_RECEPTIONIST_INDEX] = firstReceptionist->GetProcessTime() + currentModelingTime;
        secondReceptionist->StartServicingPatient(secondReceptionistQueue.Get());
    }

    if (fabs(timeArray[DENTIST_INDEX] - currentModelingTime) < EPSILON &&
                dentist->IsServicingPatient())
    {
        // Врач заканчивает обслуживание пациента
        dentist->StopServicePatient();
        timeArray[DENTIST_INDEX] = IDLE_TIME;
        servicedPatientNumber++;
    }

    if (fabs(timeArray[SURGEON_INDEX] - currentModelingTime) < EPSILON &&
                surgeon->IsServicingPatient())
    {
        // Врач заканчивает обслуживание пациента
        surgeon->StopServicePatient();
        timeArray[SURGEON_INDEX] = IDLE_TIME;
        servicedPatientNumber++;
    }

    if (fabs(timeArray[OPHTHALMOLOGIST_INDEX] - currentModelingTime) < EPSILON &&
                ophthalmologist->IsServicingPatient())
    {
        // Врач заканчивает обслуживание пациента
        ophthalmologist->StopServicePatient();
        timeArray[OPHTHALMOLOGIST_INDEX] = IDLE_TIME;
        servicedPatientNumber++;
    }

    if (fabs(timeArray[PHYSICIAN_INDEX] - currentModelingTime) < EPSILON &&
                physician->IsServicingPatient())
    {
        // Врач заканчивает обслуживание пациента
        physician->StopServicePatient();
        timeArray[PHYSICIAN_INDEX] = IDLE_TIME;
        servicedPatientNumber++;
    }

    if (dentist->IsAnyPatientInQueue() && !dentist->IsServicingPatient() && fabs(timeArray[DENTIST_INDEX] - IDLE_TIME) < EPSILON)
    {
        timeArray[DENTIST_INDEX] = dentist->GetProcessTime() + currentModelingTime;
        dentist->StartServicePatient();
        dentist->GetPatientToExamination();
    }

    if (surgeon->IsAnyPatientInQueue() && !surgeon->IsServicingPatient() && fabs(timeArray[SURGEON_INDEX] - IDLE_TIME) < EPSILON)
    {
        timeArray[SURGEON_INDEX] = surgeon->GetProcessTime() + currentModelingTime;
        surgeon->StartServicePatient();
        surgeon->GetPatientToExamination();
    }

    if (ophthalmologist->IsAnyPatientInQueue() && !ophthalmologist->IsServicingPatient() &&
            fabs(timeArray[OPHTHALMOLOGIST_INDEX] - IDLE_TIME) < EPSILON)
    {
        timeArray[OPHTHALMOLOGIST_INDEX] = ophthalmologist->GetProcessTime() + currentModelingTime;
        ophthalmologist->StartServicePatient();
        ophthalmologist->GetPatientToExamination();
    }

    if (physician->IsAnyPatientInQueue() && !physician->IsServicingPatient() && fabs(timeArray[PHYSICIAN_INDEX] - IDLE_TIME) < EPSILON)
    {
        timeArray[PHYSICIAN_INDEX] = physician->GetProcessTime() + currentModelingTime;
        physician->StartServicePatient();
        physician->GetPatientToExamination();
    }




    /*if (fabs(timeArray[FIRST_OPERATOR_INDEX] - currentModelingTime) < EPSILON &&
                firstOperator->HasAssignedRequest())
    {
        firstOperator->SetRequestAssigned(false);
        Request request(currentModelingTime);
        firstStorage->PutRequest(request);
        timeArray[FIRST_OPERATOR_INDEX] = IDLE_TIME;
    }
    if (fabs(timeArray[SECOND_OPERATOR_INDEX] - currentModelingTime) < EPSILON &&
                secondOperator->HasAssignedRequest())
    {
        secondOperator->SetRequestAssigned(false);
        Request request(currentModelingTime);
        firstStorage->PutRequest(request);
        timeArray[SECOND_OPERATOR_INDEX] = IDLE_TIME;
    }
    if (fabs(timeArray[THIRD_OPERATOR_INDEX] - currentModelingTime) < EPSILON &&
                thirdOperator->HasAssignedRequest())
    {
        thirdOperator->SetRequestAssigned(false);
        Request request(currentModelingTime);
        secondStorage->PutRequest(request);
        timeArray[THIRD_OPERATOR_INDEX] = IDLE_TIME;
    }

    if (firstOperator->HasAssignedRequest() && fabs(timeArray[FIRST_OPERATOR_INDEX] - IDLE_TIME) < EPSILON)
        timeArray[FIRST_OPERATOR_INDEX] = firstOperator->GetProcessTime() + currentModelingTime;
    if (secondOperator->HasAssignedRequest() && fabs(timeArray[SECOND_OPERATOR_INDEX] - IDLE_TIME) < EPSILON)
        timeArray[SECOND_OPERATOR_INDEX] = secondOperator->GetProcessTime() + currentModelingTime;
    if (thirdOperator->HasAssignedRequest() && fabs(timeArray[THIRD_OPERATOR_INDEX] - IDLE_TIME) < EPSILON)
        timeArray[THIRD_OPERATOR_INDEX] = thirdOperator->GetProcessTime() + currentModelingTime;

    if (fabs(timeArray[FIRST_COMPUTER_INDEX] - currentModelingTime) < EPSILON &&
                firstComputer->HasAssignedRequest())
    {
        firstComputer->SetRequestAssigned(false);
        requestProcessedNumber++;
        timeArray[FIRST_COMPUTER_INDEX] = IDLE_TIME;
    }
    if (fabs(timeArray[SECOND_COMPUTER_INDEX] - currentModelingTime) < EPSILON &&
                secondComputer->HasAssignedRequest())
    {
        secondComputer->SetRequestAssigned(false);
        requestProcessedNumber++;
        timeArray[SECOND_COMPUTER_INDEX] = IDLE_TIME;
    }

    if (!firstStorage->isEmpty() && fabs(timeArray[FIRST_COMPUTER_INDEX] - IDLE_TIME) < EPSILON)
    {
        Request request = firstStorage->GetRequest();
        firstComputer->SetRequestAssigned(true);
        timeArray[FIRST_COMPUTER_INDEX] = firstComputer->GetProcessTime() + currentModelingTime;
    }

    if (!secondStorage->isEmpty() && fabs(timeArray[SECOND_COMPUTER_INDEX] - IDLE_TIME) < EPSILON)
    {
         Request request = secondStorage->GetRequest();
         secondComputer->SetRequestAssigned(true);
         timeArray[SECOND_COMPUTER_INDEX] = secondComputer->GetProcessTime() + currentModelingTime;
    }*/
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

}

void ControlProgram::StatisticsCollected(double requestDropKoff)
{
    emit StatisticsCollectedSignal(requestDropKoff);
}

