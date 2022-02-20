#pragma once
#include "GameObject.h"
class Entity :
    public GameObject
{
public:
    void setSpeed(float newSpeed);
    float getSpeed();
private:
    float speed;

};

