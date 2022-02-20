#pragma once
#include <SFML/Graphics.hpp>

class GameObject
{
public:
	GameObject() {
		type = "object";
	}

	void setTexture(sf::Texture* newTexture);
	void setSprite(sf::Sprite* newSprite);
	void setPosition(float x, float y);
	void setScreenPosition(float x, float y);
	void setBoundBox(float newWidth, float newHeight);
	void setBoundBox(float newWidth);
	void setBoundBoxOffset(float x, float y);
	void setTextureSize(float newWidth, float newHeight);
	void setTextureSize(float newWidth);
	sf::Texture* getTexture();
	sf::Sprite* getSprite();
	float* getPosition();
	float getTextureHeight();
	float getTextureWidth();
	float getRenderWidth();
	float getRenderHeight();
	float getBoundBoxWidth();
	float getBoundBoxHeight();
	float getBoundBoxOffsetX();
	float getBoundBoxOffsetY();
	float getX();
	float getY();
	std::string getType();

	virtual void setLightLevel(float newLightLevel);
	virtual float getLightLevel();

private:
	float boundBoxWidth;
	float boundBoxHeight;
	float boundBoxOffsetX;
	float boundBoxOffsetY;
	float textureWidth;
	float textureHeight;
	sf::Texture* texture;
	sf::Sprite* sprite;
	
protected:
	std::string type;
	float position[2];

};

