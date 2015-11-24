#ifndef QUEUEFULLEXCEPTION_H
#define QUEUEFULLEXCEPTION_H

#include "Exception.h"

class QueueFullException: public Exception
{
public:
    explicit QueueFullException(std::string messageToSet)
    {
        message = messageToSet;
    }
    ~QueueFullException() throw (){}
};

#endif // QUEUEFULLEXCEPTION_H
