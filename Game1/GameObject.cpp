#include "GameObject.h"

GameObject::GameObject(std::shared_ptr<sf::Texture> texture, float newTextureWidth, float newBoundBoxWidth, float newBoundBoxHeight) {
	sprite = std::shared_ptr<sf::Sprite>(new sf::Sprite(*texture));
	textureWidth = newTextureWidth;
	textureHeight = newTextureWidth * sprite->getTexture()->getSize().y / sprite->getTexture()->getSize().x;
	boundBoxWidth = newBoundBoxWidth;
	boundBoxHeight = newBoundBoxHeight;
	position[0] = 0;
	position[1] = 0;
	boundBoxOffsetX = 0;
	boundBoxOffsetY = 0;
	type = "object";
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
	boundBoxWidth = newWidth;
	boundBoxHeight = newHeight;
}

void GameObject::setBoundBox(float newWidth)
{
	boundBoxWidth = newWidth;
	boundBoxHeight = newWidth * sprite->getTexture()->getSize().y / sprite->getTexture()->getSize().x;
}

void GameObject::setBoundBoxOffset(float x, float y)
{
	boundBoxOffsetX = x;
	boundBoxOffsetY = y;
}

void GameObject::setTextureSize(float newWidth, float newHeight)
{
	textureWidth = newWidth;
	textureHeight = newHeight;
}

void GameObject::setTextureSize(float newWidth)
{
	textureWidth = newWidth;
	textureHeight = newWidth * sprite->getTexture()->getSize().y / sprite->getTexture()->getSize().x;
}

std::shared_ptr<sf::Sprite> GameObject::getSprite()
{
	return sprite;
}

float* GameObject::getPosition() {
	return position;
}

float GameObject::getTextureHeight()
{
	return textureHeight;
}

float GameObject::getTextureWidth()
{
	return textureWidth;
}

float GameObject::getRenderWidth()
{
	return sprite->getTexture()->getSize().x * sprite->getScale().x;
}

float GameObject::getRenderHeight()
{
	return sprite->getTexture()->getSize().y * sprite->getScale().y;
}

float GameObject::getBoundBoxWidth()
{
	return boundBoxWidth;
}

float GameObject::getBoundBoxHeight()
{
	return boundBoxHeight;
}

float GameObject::getBoundBoxOffsetX()
{
	return boundBoxOffsetX;
}

float GameObject::getBoundBoxOffsetY()
{
	return boundBoxOffsetY;
}

float GameObject::getX()
{
	return position[0];
}

float GameObject::getY()
{
	return position[1];
}

std::string GameObject::getType()
{
	return type;
}


void GameObject::setLightLevel(float newLightLevel)
{
}

float GameObject::getLightLevel()
{
	return -1;
}