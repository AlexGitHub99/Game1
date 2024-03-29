#pragma once
#include "Creature.h"
#include "Path.h"

class Orb :
    public std::enable_shared_from_this<Orb>,
    public Creature
{
public:
    Orb(std::shared_ptr<sf::Texture> newTexture, float newTextureWidth, float newBoundBoxWidth, float newBoundBoxHeight, std::string newTextureName);
    void update(std::shared_ptr<Area>& area, Player &player, float ms) override;
};

