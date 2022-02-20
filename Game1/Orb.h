#pragma once
#include "Creature.h"
class Orb :
    public Creature
{
public:
    void move(Area& area) override;
};

