#include "NormalGenerator.h"
#include "LKG.h"
#include "Exception/ErrorInputDataException.h"
#include <QDateTime>

NormalGenerator::NormalGenerator()
{
    this->sigma = 0.0;
    this->matExp = 0.0;
    this->minBorder = 0;
    this->maxBorder = 0;
    this->iterationNumber = 0;
    this->currentNumber = 0;
}

NormalGenerator::NormalGenerator(double sigma, double matExp, int minBorder, int maxBorder, int iterationNumber)
{
    if (sigma < 0 || iterationNumber <= 0 || minBorder >= maxBorder || sigma == (double) maxBorder)
        throw ErrorInputDataException("Error parameters in NormalGenerator::NormalGenerator");
    this->sigma = sigma;
    this->matExp = matExp;
    this->minBorder = minBorder;
    this->maxBorder = maxBorder;
    this->iterationNumber = iterationNumber;
    this->currentNumber = QDateTime::currentMSecsSinceEpoch();
}

double NormalGenerator::GenerateValue()
{
    double sum = 0.0;
    for (int i = 0; i < iterationNumber; i++)
    {
        currentNumber = LKG::GenerateValue(currentNumber);
        sum += currentNumber;
    }
    double number = matExp + (sum - iterationNumber / 2) * sigma;
    return TransformValueIntoBorders(number);
}

double NormalGenerator::TransformValueIntoBorders(double value)
{
    double interval = maxBorder - minBorder;
    while (value > maxBorder)
        value -= interval;
    while (value < minBorder)
        value += interval;
    return value;
}
