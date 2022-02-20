#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Entity.h"
#include <list>

class Area
{
public:
	Area() {
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

