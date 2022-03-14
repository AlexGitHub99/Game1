#pragma once
#include "Entity.h"
#include "Area.h"
class Creature :
    public Entity

{
public:
    Creature(std::shared_ptr<sf::Texture> newTexture, float newTextureWidth, float newBoundBoxWidth, float newBoundBoxHeight, std::string newTextureName);
    void setDamage(float newDamage);
    float getDamage();
    
protected:
    float damage;
    float damageRadius;
};

