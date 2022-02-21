#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include <list>
class Entity;

class Area
{
public:
	Area(sf::Texture* newBackground, float newWidth, float newHeight) {
		background = new sf::Sprite(*newBackground);
		width = newWidth;
		height = newHeight;
		objects = new std::list<GameObject*>;
		entities = new std::vector<Entity*>;
	}

	void setBackground(sf::Texture* newBackground);
	void setSize(float newWidth, float newHeight);
	void addObject(GameObject* newObj);
	void sortObject(GameObject* obj);
	void addEntity(Entity* newEnt);
	std::list<GameObject*>* getObjects();
	std::vector<Entity*>* getEntities();
	sf::Sprite* getBackground();
	float getWidth();
	float getHeight();
private:
	std::list<GameObject*>* objects;
	std::vector<Entity*>* entities;
	sf::Sprite* background;
	float width;
	float height;
};

