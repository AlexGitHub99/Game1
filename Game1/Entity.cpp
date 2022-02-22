#include "Entity.h"

Entity::Entity(std::shared_ptr<sf::Texture> texture, float newTextureWidth, float newBoundBoxWidth, float newBoundBoxHeight) : GameObject(texture, newTextureWidth, newBoundBoxWidth, newBoundBoxHeight) {
	speed = 100;
}

void Entity::setSpeed(float newSpeed)
{
	speed = newSpeed;
}

float Entity::getSpeed()
{
	return speed;
}

void Entity::update(Area& area, Player &player, float ms)
{
}
