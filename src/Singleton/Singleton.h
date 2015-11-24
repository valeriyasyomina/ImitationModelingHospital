#ifndef SINGLETON_H
#define SINGLETON_H

#include "../ControlProgram/ControlProgram.h"

class Singleton
{
private:
    Singleton();
    ~Singleton();
public:
    static Singleton& Instance() {return instance;}
    ControlProgram* GetControlProgram();
private:
    static Singleton instance;
    ControlProgram* controlProgram;
};

#endif // SINGLETON_H
