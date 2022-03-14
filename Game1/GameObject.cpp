#include "GameObject.h"

GameObject::GameObject(std::shared_ptr<sf::Texture> newTexture, float newTextureWidth, float newBoundBoxWidth, float newBoundBoxHeight, std::string newTextureName) {
	id = rand();
	texture = newTexture;
	sprite = std::shared_ptr<sf::Sprite>(new sf::Sprite(*texture));
	textureSize[0] = newTextureWidth;
	textureSize[1] = newTextureWidth * sprite->getTexture()->getSize().y / sprite->getTexture()->getSize().x;
	boundBox[0] = newBoundBoxWidth;
	boundBox[1] = newBoundBoxHeight;
	position[0] = 0;
	position[1] = 0;
	boundBoxOffset[0] = 0;
	boundBoxOffset[1] = 0;
	type = "object";
	textureName = newTextureName;
}

GameObject::GameObject(std::shared_ptr<sf::Texture> newTexture, float newTextureWidth, float newTextureHeight, float newBoundBoxWidth, float newBoundBoxHeight, std::string newTextureName) {
	id = rand();
	texture = newTexture;
	sprite = std::shared_ptr<sf::Sprite>(new sf::Sprite(*texture));
	textureSize[0] = newTextureWidth;
	textureSize[1] = newTextureHeight;
	boundBox[0] = newBoundBoxWidth;
	boundBox[1] = newBoundBoxHeight;
	position[0] = 0;
	position[1] = 0;
	boundBoxOffset[0] = 0;
	boundBoxOffset[1] = 0;
	type = "object";
	textureName = newTextureName;
}

int GameObject::getId()
{
	return id;
}

void GameObject::setPosition(float x, float y)
{
	position[0] = x;
	position[1] = y;
	sprite->setPosition(x, y);
}

void GameObject::setBoundBox(float newWidth, float newHeight)
{
	boundBox[0] = newWidth;
	boundBox[1] = newHeight;
}

void GameObject::setBoundBox(float newWidth)
{
	boundBox[0] = newWidth;
	boundBox[1] = newWidth * sprite->getTexture()->getSize().y / sprite->getTexture()->getSize().x;
}

void GameObject::setBoundBoxOffset(float x, float y)
{
	boundBoxOffset[0] = x;
	boundBoxOffset[1] = y;
}

void GameObject::setBoundBoxOffsetToBottom()
{
	boundBoxOffset[1] = textureSize[1] / 2 - boundBox[1] / 2;
}

void GameObject::setTextureSize(float newWidth, float newHeight)
{
	textureSize[0] = newWidth;
	textureSize[1] = newHeight;
}

void GameObject::setTextureSize(float newWidth)
{
	textureSize[0] = newWidth;
	textureSize[1] = newWidth * sprite->getTexture()->getSize().y / sprite->getTexture()->getSize().x;
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
	return textureSize[1];
}

float GameObject::getTextureWidth()
{
	return textureSize[0];
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
	return boundBox[0];
}

float GameObject::getBoundBoxHeight()
{
	return boundBox[1];
}

float GameObject::getBoundBoxOffsetX()
{
	return boundBoxOffset[0];
}

float GameObject::getBoundBoxOffsetY()
{
	return boundBoxOffset[1];
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

std::string GameObject::getTextureName()
{
	return textureName;
}


void GameObject::setLightLevel(float newLightLevel)
{
}

float GameObject::getLightLevel()
{
	return -1;
}