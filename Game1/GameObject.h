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
	void setBoundBox(float newWidth);
	void setTextureSize(float newWidth, float newHeight);
	void setTextureSize(float newWidth);
	sf::Texture* getTexture();
	sf::Sprite* getSprite();
	float* getPosition();
	float getTextureHeight();
	float getTextureWidth();
	float getWidth();
	float getHeight();
	float getX();
	float getY();
	std::string getType();

private:
	float position[2];
	float width;
	float height;
	float textureWidth;
	float textureHeight;
	sf::Texture* texture;
	sf::Sprite* sprite;

};

