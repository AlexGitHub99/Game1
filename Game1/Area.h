#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Entity.h"
#include <list>


class Area
{
public:
	Area(std::shared_ptr<sf::Texture> newBackground, float newWidth, float newHeight) {
		background = std::shared_ptr<sf::Sprite>(new sf::Sprite(*newBackground));
		width = newWidth;
		height = newHeight;
		objects = std::shared_ptr<std::list<std::shared_ptr<GameObject>>>(new std::list<std::shared_ptr<GameObject>>);
		entities = std::shared_ptr<std::vector<std::shared_ptr<Entity>>>(new std::vector<std::shared_ptr<Entity>>);
	}

	void setSize(float newWidth, float newHeight);
	void addObject(std::shared_ptr<GameObject> newObj);
	void sortObject(std::shared_ptr<GameObject> obj);
	void addEntity(std::shared_ptr<Entity> newEnt);
	std::shared_ptr<std::list<std::shared_ptr<GameObject>>> getObjects();
	std::shared_ptr<std::vector<std::shared_ptr<Entity>>> getEntities();
	std::shared_ptr<sf::Sprite> getBackground();
	float getWidth();
	float getHeight();
private:
	std::shared_ptr<std::list<std::shared_ptr<GameObject>>> objects;
	std::shared_ptr<std::vector<std::shared_ptr<Entity>>> entities;
	std::shared_ptr<sf::Sprite> background;
	float width;
	float height;
};

