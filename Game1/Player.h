#pragma once

#include <SFML/Graphics.hpp>

class Player
{
public:
	void setTexture(sf::Texture* newTexture);
	void setSprite(sf::Sprite* newSprite);
	void setPosition(float x, float y);
	void setBoundBox(float newWidth, float newHeight);
	void setBoundBox(float newWidth);
	void setBoundBoxOffset(float x, float y);
	void movePosition(float x, float y);
	void setScreenPosition(float x, float y);
	void moveScreenPosition(float x, float y);
	void setLightLevel(float newLightLevel);
	void setTextureSize(float newWidth, float newHeight);
	void setTextureSize(float newWidth);
	void setFaceLeft(bool isTrue);
	float* getPosition();
	sf::Texture* getTexture();
	sf::Sprite* getSprite();
	float getWidth();
	float getHeight();
	float getBoundBoxOffsetX();
	float getBoundBoxOffsetY();
	float getX();
	float getY();
	float getLightLevel();
	float getTextureHeight();
	float getTextureWidth();
	bool isFacingLeft();

private:
	float position[2];
	float width;
	float height;
	float boundBoxOffsetX;
	float boundBoxOffsetY;
	float lightLevel;
	float textureWidth;
	float textureHeight;
	bool facingLeft;

	sf::Texture* texture;
	sf::Sprite* sprite;
};

