#ifndef LKG_H
#define LKG_H

class LKG
{
protected:
    static int a;
    static int b;
    static int m;
public:
    static double GenerateValue(double previousValue, int lowBorder, int highBorder);
    static double GenerateValue(double previousValue);
    static double TransformValueIntoBorders(double value, int lowBorder, int highBorder);
protected:
    static double Generator(double previousValue);
};

#endif // LKG_H
