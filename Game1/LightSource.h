#pragma once
#include "GameObject.h"

class LightSource :
    public GameObject
{
public:
    void setLightLevel(float newLightLevel);
    float getLightLevel();
private:
    float lightLevel;
};

