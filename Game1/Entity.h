#pragma once
#include "GameObject.h"
#include "Player.h"
class Area;

class Entity :
    public GameObject
{
public:
    Entity(std::shared_ptr<sf::Texture> texture, float newTextureWidth, float newBoundBoxWidth, float newBoundBoxHeight);

    void setSpeed(float newSpeed);
    float getSpeed();
    virtual void update(Area& area, Player &player, float ms);
protected:
    float speed; //coords per second

};

