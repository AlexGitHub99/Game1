#pragma once

#include <SFML/Graphics.hpp>

class Player
{
public:
	Player(sf::Texture* newTexture, float newTextureWidth, float newBoundBoxWidth, float newBoundBoxHeight, float newHealth) {
		texture = newTexture;
		sprite = new sf::Sprite(*texture);
		textureWidth = newTextureWidth;
		textureHeight = newTextureWidth * sprite->getTexture()->getSize().y / sprite->getTexture()->getSize().x;
		boundBoxWidth = newBoundBoxWidth;
		boundBoxHeight = newBoundBoxHeight;
		health = newHealth;
		lightLevel = 0;
		position[0] = 0;
		position[1] = 0;
		facingLeft = false;
		boundBoxOffsetX = 0;
		boundBoxOffsetY = 0;

	}
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
	void setHealth(float newHealth);
	float* getPosition();
	sf::Texture* getTexture();
	sf::Sprite* getSprite();
	float getBoundBoxWidth();
	float getBoundBoxHeight();
	float getBoundBoxOffsetX();
	float getBoundBoxOffsetY();
	float getX();
	float getY();
	float getLightLevel();
	float getTextureHeight();
	float getTextureWidth();
	float getRenderWidth();
	float getRenderHeight();
	bool isFacingLeft();
	float getHealth();

private:
	float position[2];
	float boundBoxWidth;
	float boundBoxHeight;
	float boundBoxOffsetX;
	float boundBoxOffsetY;
	float lightLevel;
	float textureWidth;
	float textureHeight;
	bool facingLeft;
	float health;

	sf::Texture* texture;
	sf::Sprite* sprite;
};

