#pragma once
#include "GameObject.h"

class LightSource :
    public GameObject
{
public:
    LightSource() {
        type = "lightSource";
    }

    void setLightLevel(float newLightLevel) override;
    float getLightLevel() override;
private:
    float lightLevel;
};

