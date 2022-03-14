#pragma once
#include "GameObject.h"
#include "Player.h"

class Area;

class Entity :
    public GameObject
{
public:
    Entity(std::shared_ptr<sf::Texture> texture, float newTextureWidth, float newBoundBoxWidth, float newBoundBoxHeight, std::string newTexturePath);

    void setSpeed(float newSpeed);
    void setPushPlayer(bool value);
    float getSpeed();
    bool doesPushPlayer();
    virtual void update(std::shared_ptr<Area>& area, Player &player, float ms);
protected:
    float speed; //coords per second
    bool pushPlayer;
};

