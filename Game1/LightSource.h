#pragma once
#include "GameObject.h"

class LightSource :
    public GameObject
{
public:
	LightSource(std::shared_ptr<sf::Texture> newTexture, float newTextureWidth, float newBoundBoxWidth, float newBoundBoxHeight, float lightLevel, std::string newTextureName) : lightLevel(lightLevel), GameObject(newTexture, newTextureWidth, newBoundBoxWidth, newBoundBoxHeight, newTextureName) {
		type = "lightSource";
	}

	LightSource(std::shared_ptr<sf::Texture> newTexture, float newTextureWidth, float newTextureHeight, float newBoundBoxWidth, float newBoundBoxHeight, float lightLevel, std::string newTextureName) : lightLevel(lightLevel), GameObject(newTexture, newTextureWidth, newTextureHeight, newBoundBoxWidth, newBoundBoxHeight, newTextureName) {
		type = "lightSource";
	}

    void setLightLevel(float newLightLevel) override;
    float getLightLevel() override;
private:
    float lightLevel;
};

