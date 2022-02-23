#include "Player.h"

void Player::setPosition(float x, float y)
{
	position[0] = x;
	position[1] = y;
}

void Player::moveScreenPosition(float x, float y)
{
	sprite->setPosition(sprite->getPosition().x + x, sprite->getPosition().y + y);
}

void Player::setLightLevel(float newLightLevel)
{
	lightLevel = newLightLevel;
}

void Player::setTextureSize(float newWidth, float newHeight)
{
	textureWidth = newWidth;
	textureHeight = newHeight;
}

void Player::setTextureSize(float newWidth)
{
	textureWidth = newWidth;
	textureHeight = newWidth * sprite->getTexture()->getSize().y / sprite->getTexture()->getSize().x;
}

void Player::setFaceLeft(bool isTrue)
{
	facingLeft = isTrue;
}

void Player::setHealth(float newHealth)
{
	health = newHealth;
}

void Player::setBoundBox(float newWidth, float newHeight) {
	boundBoxWidth = newWidth;
	boundBoxHeight = newHeight;
}

void Player::setBoundBox(float newWidth)
{
	boundBoxWidth = newWidth;
	boundBoxHeight = newWidth * sprite->getTexture()->getSize().y / sprite->getTexture()->getSize().x;
}

void Player::setBoundBoxOffset(float x, float y)
{
	boundBoxOffsetX = x;
	boundBoxOffsetY = y;
}

void Player::movePosition(float x, float y) {
	position[0] += x;
	position[1] += y;
}

std::shared_ptr<sf::Sprite> Player::getSprite()
{
	return sprite;
}



float Player::getBoundBoxWidth() {
	return boundBoxWidth;
}

float Player::getBoundBoxHeight() {
	return boundBoxHeight;
}

float Player::getBoundBoxOffsetX()
{
	return boundBoxOffsetX;
}

float Player::getBoundBoxOffsetY()
{
	return boundBoxOffsetY;
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

float Player::getTextureHeight()
{
	return textureHeight;
}

float Player::getTextureWidth()
{
	return textureWidth;
}

float Player::getRenderWidth()
{
	return sprite->getTexture()->getSize().x * sprite->getScale().x;
}

float Player::getRenderHeight()
{
	return sprite->getTexture()->getSize().y * sprite->getScale().y;
}

bool Player::isFacingLeft()
{
	return facingLeft;
}

float Player::getHealth()
{
	return health;
}
