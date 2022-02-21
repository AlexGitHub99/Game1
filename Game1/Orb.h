#pragma once
#include "Creature.h"

class Orb :
    public Creature
{
public:
    Orb() {
        speed = 700;
        damage = 50;
        damageRadius = 60;
    }
    void update(Area& area, Player &player, float ms) override;
};

