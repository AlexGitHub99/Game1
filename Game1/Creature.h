#pragma once
#include "Entity.h"
#include "Area.h"
class Creature :
    public Entity

{
public:
    Creature(std::shared_ptr<sf::Texture> texture, float newTextureWidth, float newBoundBoxWidth, float newBoundBoxHeight) : Entity(texture, newTextureWidth, newBoundBoxWidth, newBoundBoxHeight) {
        damage = 100;
        damageRadius = 100;
    }
    void setDamage(float newDamage);
    float getDamage();
    
protected:
    float damage;
    float damageRadius;
};

