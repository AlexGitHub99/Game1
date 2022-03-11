#pragma once
#include "GameObject.h"

class LightSource :
    public GameObject
{
public:
	LightSource(std::shared_ptr<sf::Texture> texture, float newTextureWidth, float newBoundBoxWidth, float newBoundBoxHeight, float lightLevel, std::string newTexturePath) : lightLevel(lightLevel), GameObject(texture, newTextureWidth, newBoundBoxWidth, newBoundBoxHeight, newTexturePath) {
		type = "lightSource";
	}

    void setLightLevel(float newLightLevel) override;
    float getLightLevel() override;
private:
    float lightLevel;
};

