#include "Entity.h"

Entity::Entity(std::shared_ptr<sf::Texture> newTexture, float newTextureWidth, float newBoundBoxWidth, float newBoundBoxHeight, std::string newTextureName) : GameObject(newTexture, newTextureWidth, newBoundBoxWidth, newBoundBoxHeight, newTextureName) {
	speed = 100;
	pushPlayer = false;
	type = "entity";
}

void Entity::setSpeed(float newSpeed)
{
	speed = newSpeed;
}

void Entity::setPushPlayer(bool value)
{
	pushPlayer = value;
}

float Entity::getSpeed()
{
	return speed;
}

bool Entity::doesPushPlayer()
{
	return pushPlayer;
}

void Entity::update(std::shared_ptr<Area>& area, Player &player, float ms)
{
}
