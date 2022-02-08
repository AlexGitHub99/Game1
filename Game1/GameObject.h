#pragma once
#include <SFML/Graphics.hpp>

class GameObject
{
public:
	void setTexture(sf::Texture* newTexture);
	void setSprite(sf::Sprite* newSprite);
	void setPosition(float x, float y);
	void setScreenPosition(float x, float y);
	void setBoundBox(float newWidth, float newHeight);
	sf::Texture* getTexture();
	sf::Sprite* getSprite();
	float* getPosition();
	float getWidth();
	float getHeight();
	float getX();
	float getY();

private:
	float position[2];
	float width;
	float height;
	sf::Texture* texture;
	sf::Sprite* sprite;

};

