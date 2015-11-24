#include "LKG.h"

double LKG::GenerateValue(double previousValue, int lowBorder, int highBorder)
{
    double generatedNumber = (double) Generator(previousValue);
    return TransformValueIntoBorders(generatedNumber, lowBorder, highBorder);
}

double LKG::GenerateValue(double previousValue)
{
    return Generator(previousValue);
}

double LKG::Generator(double previousValue)
{
    long long int value = (a * previousValue + b);
    return value % m;
}

double LKG::TransformValueIntoBorders(double value, int lowBorder, int highBorder)
{
    double interval = highBorder - lowBorder;
    while (value > highBorder)
        value -= interval;
    while (value < lowBorder)
        value += interval;

    return value;
}

int LKG::a = 106;
int LKG::b = 1283;
int LKG::m = 6075;


