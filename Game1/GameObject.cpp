#include "GameObject.h"

void GameObject::setTexture(sf::Texture* newTexture)
{
	texture = newTexture;
	sprite = new sf::Sprite(*texture);
}

void GameObject::setSprite(sf::Sprite* newSprite)
{
	sprite = newSprite;
}

void GameObject::setPosition(float x, float y)
{
	position[0] = x;
	position[1] = y;
	sprite->setPosition(x, y);
}

void GameObject::setScreenPosition(float x, float y)
{
	sprite->setPosition(x, y);
}

void GameObject::setBoundBox(float newWidth, float newHeight)
{
	width = newWidth;
	height = newHeight;
}

sf::Texture* GameObject::getTexture()
{
	return texture;
}

sf::Sprite* GameObject::getSprite()
{
	return sprite;
}

float* GameObject::getPosition() {
	return position;
}

float GameObject::getWidth()
{
	return width;
}

float GameObject::getHeight()
{
	return height;
}

float GameObject::getX()
{
	return position[0];
}

float GameObject::getY()
{
	return position[1];
}
