#include "Creature.h"

Creature::Creature(std::shared_ptr<sf::Texture> texture, float newTextureWidth, float newBoundBoxWidth, float newBoundBoxHeight, std::string newTexturePath) : Entity(texture, newTextureWidth, newBoundBoxWidth, newBoundBoxHeight, newTexturePath) {
    damage = 100;
    damageRadius = 100;
}

void Creature::setDamage(float newDamage)
{
	damage = newDamage;
}

float Creature::getDamage()
{
	return damage;
}
