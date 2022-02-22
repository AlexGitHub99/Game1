#pragma once
#include "Creature.h"

class Orb :
    public std::enable_shared_from_this<Orb>,
    public Creature
{
public:
    Orb(std::shared_ptr<sf::Texture> texture, float newTextureWidth, float newBoundBoxWidth, float newBoundBoxHeight) : Creature(texture, newTextureWidth, newBoundBoxWidth, newBoundBoxHeight) {
        speed = 700;
        damage = 50;
        damageRadius = 60;
    }
    void update(Area& area, Player &player, float ms) override;
};

