#ifndef NORMALGENERATOR_H
#define NORMALGENERATOR_H

class NormalGenerator
{
protected:
    double sigma;
    double matExp;
    double minBorder;
    double maxBorder;
    int iterationNumber;
    int currentNumber;
public:
    NormalGenerator();
    NormalGenerator(double sigma, double matExp, int minBorder, int maxBorder, int iterationNumber);
    ~NormalGenerator() {}

    double GenerateValue();
protected:
    double TransformValueIntoBorders(double value);

};

#endif // NORMALGENERATOR_H
