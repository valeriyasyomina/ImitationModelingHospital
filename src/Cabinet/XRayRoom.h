#ifndef XRAYROOM_H
#define XRAYROOM_H

#include "Cabinet.h"

class XRayRoom : public Cabinet
{
public:
    XRayRoom(): Cabinet(){}
    XRayRoom(double a, double b) : Cabinet(a, b) {}
    XRayRoom(double a, double b, int maxQueueSize) : Cabinet(a, b, maxQueueSize) {}
    ~XRayRoom() {}
};

#endif // XRAYROOM_H
