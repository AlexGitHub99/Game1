#include "Entity.h"

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
