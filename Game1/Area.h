#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Entity.h"
#include <list>


class Area
{
public:
	Area(std::shared_ptr<sf::Texture> newBackground, float newWidth, float newHeight, std::string newTexturePath);

	void setSize(float newWidth, float newHeight);
	void addObject(std::shared_ptr<GameObject> newObj);
	void sortObject(std::shared_ptr<GameObject> obj);
	void addEntity(std::shared_ptr<Entity> newEnt);
	std::shared_ptr<std::list<std::shared_ptr<GameObject>>> getObjects();
	std::shared_ptr<std::vector<std::shared_ptr<Entity>>> getEntities();
	std::shared_ptr<sf::Sprite> getBackground();
	float getWidth();
	float getHeight();
	std::string getTexturePath();
private:
	std::shared_ptr<std::list<std::shared_ptr<GameObject>>> objects;
	std::shared_ptr<std::vector<std::shared_ptr<Entity>>> entities;
	std::shared_ptr<sf::Sprite> background;
	float width;
	float height;
	std::string texturePath;
};

