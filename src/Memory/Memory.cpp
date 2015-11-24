#include "Memory.h"

void Memory::PutRequest(const Request &request)
{
    if (!queue.isFull())
        queue.Add(request);
    else
        requestDropNumber++;
}
