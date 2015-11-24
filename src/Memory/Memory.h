#ifndef MEMORY_H
#define MEMORY_H

#include "../Queue/Queue.h"
#include "../Request/Request.h"

class Memory
{
private:
    Queue<Request> queue;
    int requestDropNumber;      // число потерянных заявок
public:
    Memory() : requestDropNumber(0) {}
    Memory(int maxMemorySize): queue(maxMemorySize), requestDropNumber(0) {}
    ~Memory() {}
    void SetMaximumSize(int maxSize) {queue.SetMaximumSize(maxSize);}

    bool isEmpty() const {return queue.isEmpty();}
    bool isFull() const {return queue.isFull();}
    Request GetRequest() {return queue.Get();}
    void PutRequest(const Request& request);
    int Size() const {return queue.Size();}

    int GetDropRequestNumber() const {return requestDropNumber;}
};

#endif // MEMORY_H
