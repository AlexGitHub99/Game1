#include "Player.h"

void Player::setPosition(float x, float y)
{
	position[0] = x;
	position[1] = y;
}

void Player::setTexture(sf::Texture* newTexture)
{
	texture = newTexture;
	sprite = new sf::Sprite(*texture);
}

void Player::setSprite(sf::Sprite* newSprite)
{
	sprite = newSprite;
}

void Player::setScreenPosition(float x, float y)
{
	sprite->setPosition(x, y);
}

void Player::setLightLevel(float newLightLevel)
{
	lightLevel = newLightLevel;
}

void Player::setBoundBox(float newWidth, float newHeight) {
	width = newWidth;
	height = newHeight;
}

void Player::movePosition(float x, float y) {
	position[0] += x;
	position[1] += y;
}

float* Player::getPosition()
{
	return position;
}

sf::Texture* Player::getTexture()
{
	return texture;
}

sf::Sprite* Player::getSprite()
{
	return sprite;
}

float Player::getWidth() {
	return width;
}

float Player::getHeight() {
	return height;
}

float Player::getX() {
	return position[0];
}

float Player::getY() {
	return position[1];
}

float Player::getLightLevel()
{
	return lightLevel;
}
