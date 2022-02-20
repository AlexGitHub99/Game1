#pragma once
#include "Entity.h"
#include "Area.h"
class Creature :
    public Entity

{
public:
    void setDamage(float newDamage);
    float getDamage();
    virtual void move(Area &area);
private:
    float damage;

};

