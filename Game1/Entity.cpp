#include "Entity.h"

Entity::Entity(std::shared_ptr<sf::Texture> texture, float newTextureWidth, float newBoundBoxWidth, float newBoundBoxHeight) : GameObject(texture, newTextureWidth, newBoundBoxWidth, newBoundBoxHeight) {
	speed = 100;
	pushPlayer = false;
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

void Entity::update(Area& area, Player &player, float ms)
{
}
