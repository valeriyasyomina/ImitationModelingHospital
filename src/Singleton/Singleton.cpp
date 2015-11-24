#include "Singleton.h"
#include <stdio.h>

Singleton Singleton::instance;

Singleton::Singleton()
{
    try
    {
        controlProgram = new ControlProgram();
    }
    catch (std::bad_alloc& exception)
    {
        throw AllocMemoryException("Error allocate memory in Singleton::Singleton()");
    }
}

Singleton::~Singleton()
{
    if (controlProgram)
    {
        delete controlProgram;
        controlProgram = NULL;
    }
}

ControlProgram* Singleton::GetControlProgram()
{
    try
    {
        if (!controlProgram)
            controlProgram = new ControlProgram();
        return controlProgram;
    }
    catch (std::bad_alloc& exception)
    {
        throw AllocMemoryException("Error allocate memory in Singleton::GetControlProgram()");
    }
}


