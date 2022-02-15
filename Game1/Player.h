#pragma once

#include <SFML/Graphics.hpp>

class Player
{
public:
	void setTexture(sf::Texture* newTexture);
	void setSprite(sf::Sprite* newSprite);
	void setPosition(float x, float y);
	void setBoundBox(float newWidth, float newHeight);
	void movePosition(float x, float y);
	void setScreenPosition(float x, float y);
	void setLightLevel(float newLightLevel);
	float* getPosition();
	sf::Texture* getTexture();
	sf::Sprite* getSprite();
	float getWidth();
	float getHeight();
	float getX();
	float getY();
	float getLightLevel();

private:
	float position[2];
	float width;
	float height;
	float lightLevel;

	sf::Texture* texture;
	sf::Sprite* sprite;
};

