#ifndef QUEUE_H
#define QUEUE_H

#include "QueueElement.h"
#include "Exception/AllocMemoryException.h"
#include "Exception/QueueFullException.h"
#include "Exception/QueueEmptyException.h"
#include "Exception/ErrorInputDataException.h"

#define USE_EXCEPTIONS
#undef USE_EXCEPTIONS

template <class TypeName>
class Queue
{
private:
    QueueElement<TypeName>* head;
    int size;
    int maxSize;
public:
    Queue();
    Queue(int maxSize);
    Queue(const Queue& queue);
    Queue operator=(const Queue& queue);
    TypeName operator[](int index) const;
    ~Queue();

    void Add(TypeName value);
    TypeName Get();
    bool isEmpty() const {return !size ? true : false;}
    bool isFull() const {return size >= maxSize ? true : false;}
    void SetMaximumSize(int maxSize);
    int Size() const {return size;}
    int MaxSize() const {return maxSize;}
private:
    void Free();
};

template <class TypeName>
Queue<TypeName>::Queue()
{
    maxSize = 0;
    size = 0;
    head = NULL;
}

template <class TypeName>
Queue<TypeName>::Queue(int maxSize)
{
#ifdef USE_EXCEPTIONS
    if (maxSize <= 0)
        throw ErrorInputDataException("Error maxSize parameter in Queue<TypeName>::Queue(int)");
#endif
    if (maxSize > 0)
    {
        this->maxSize = maxSize;
        size = 0;
        head = NULL;
    }
}

template <class TypeName>
void Queue<TypeName>::SetMaximumSize(int maxSize)
{
#ifdef USE_EXCEPTIONS
    if (maxSize <= 0)
        throw ErrorInputDataException("Error maxSize parameter in Queue<TypeName>::SetMaximumSize(int)");
#endif
    if (maxSize > 0)
        this->maxSize = maxSize;
}

template <class TypeName>
Queue<TypeName>::Queue(const Queue<TypeName>& queue)
{
    size = queue.size;
    maxSize = queue.maxSize;
    for (int i = 0; i < queue.size; i++)
        this->Add(queue[i]);
}

template <class TypeName>
Queue<TypeName> Queue<TypeName>::operator=(const Queue<TypeName>& queue)
{    
    if (!head)
        return queue;
    if (this == &queue)
        return *this;
    Free();
    maxSize = queue.maxSize;
    for (int i = 0; i < queue.size; i++)
        Add(queue[i]);
    return *this;
}

template <class TypeName>
TypeName Queue<TypeName>::operator[](int index) const
{
    if (index < 0 || index >= size)
        throw ErrorInputDataException("Error index in Queue<TypeName>::operator[](int)");

    QueueElement<TypeName>* curPointer = head;
    for (int i = 0; i < index; i++)
        curPointer = curPointer->next;
    return curPointer->value;
}

template <class TypeName>
Queue<TypeName>::~Queue()
{
    Free();
}

template <class TypeName>
void Queue<TypeName>::Free()
{
    if (head)
    {
        QueueElement<TypeName>* curPointer = head;
        for (; curPointer; curPointer = head)
        {
            head = head->next;
            delete head;
        }
        size = 0;
        maxSize = 0;
    }
}

template <class TypeName>
void Queue<TypeName>::Add(TypeName value)
{
    try
    {
#ifdef USE_EXCEPTIONS
        if (size >= maxSize)
            throw QueueFullException("Error add! Queue is full");
#endif
        if (size < maxSize)
        {
            if (!head)
            {
                head = new QueueElement<TypeName>(value);
                size++;
            }
            else
            {
                QueueElement<TypeName>* curPointer = head;
                for (; curPointer->next; curPointer = curPointer->next)
                    ;
                QueueElement<TypeName>* element = new QueueElement<TypeName>(value);
                curPointer->next = element;
                size++;
            }
        }
    }
    catch (std::bad_alloc& exception)
    {
        throw AllocMemoryException("Error allocate memory in Queue<TypeName>::Add(TypeName)");
    }
}

template <class TypeName>
TypeName Queue<TypeName>::Get()
{
#ifdef USE_EXCEPTIONS
    if (!size)
        throw QueueEmptyException("Error get value! Queue is empty!");
#endif
    if (size > 0)
    {
        TypeName value = head->value;
        QueueElement<TypeName>* currentPointer = head;
        head = head->next;
        delete currentPointer;
        size--;
        return value;
    }
}


#endif // QUEUE_H
