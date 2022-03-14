#include "Creature.h"

Creature::Creature(std::shared_ptr<sf::Texture> newTexture, float newTextureWidth, float newBoundBoxWidth, float newBoundBoxHeight, std::string newTextureName) : Entity(newTexture, newTextureWidth, newBoundBoxWidth, newBoundBoxHeight, newTextureName) {
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
