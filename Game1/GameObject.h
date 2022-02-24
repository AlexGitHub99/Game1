#pragma once
#include <SFML/Graphics.hpp>

class GameObject
{
public:
	GameObject(std::shared_ptr<sf::Texture> texture, float newTextureWidth, float newBoundBoxWidth, float newBoundBoxHeight);

	int getId();
	void setPosition(float x, float y);
	void setBoundBox(float newWidth, float newHeight);
	void setBoundBox(float newWidth);
	void setBoundBoxOffset(float x, float y);
	void setTextureSize(float newWidth, float newHeight);
	void setTextureSize(float newWidth);
	std::shared_ptr<sf::Sprite> getSprite();
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

protected:
	int id;
	float boundBoxWidth;
	float boundBoxHeight;
	float boundBoxOffsetX;
	float boundBoxOffsetY;
	float textureWidth;
	float textureHeight;
	std::shared_ptr<sf::Sprite> sprite;
	std::string type;
	float position[2];

};

