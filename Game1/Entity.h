#pragma once
#include "GameObject.h"
#include "Area.h"
#include "Player.h"

class Entity :
    public GameObject
{
public:
    Entity() {
        speed = 100;
    }
    void setSpeed(float newSpeed);
    float getSpeed();
    virtual void update(Area& area, Player &player, float ms);
protected:
    float speed; //coords per second

};

