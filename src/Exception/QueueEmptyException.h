#ifndef QUEUEEMPTYEXCEPTION_H
#define QUEUEEMPTYEXCEPTION_H

#include "Exception.h"

class QueueEmptyException: public Exception
{
public:
    explicit QueueEmptyException(std::string messageToSet)
    {
        message = messageToSet;
    }
    ~QueueEmptyException() throw (){}
};

#endif // QUEUEEMPTYEXCEPTION_H
