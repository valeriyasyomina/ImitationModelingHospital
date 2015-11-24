#ifndef QUEUEELEMENT_H
#define QUEUEELEMENT_H

#include <stdio.h>

template <class TypeName>
struct QueueElement
{
    TypeName value;
    struct QueueElement* next;

    QueueElement(TypeName value)
    {
        this->value = value;
        next = NULL;
    }
};

#endif // QUEUEELEMENT_H
