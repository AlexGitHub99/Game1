#pragma once
#include "Entity.h"
#include "Area.h"
class Creature :
    public Entity

{
public:
    Creature() {
        damage = 100;
        damageRadius = 100;
    }
    void setDamage(float newDamage);
    float getDamage();
    
protected:
    float damage;
    float damageRadius;
};

