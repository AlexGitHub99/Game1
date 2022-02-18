#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include <list>

class Area
{
public:
	Area() {
		objects = new std::list<GameObject*>;
	}

	void setBackground(sf::Texture* newBackground);
	void setSize(float newWidth, float newHeight);
	void addObject(GameObject* newObj);
	std::list<GameObject*>* getObjects();
	sf::Sprite* getBackground();
	float getWidth();
	float getHeight();
private:
	std::list<GameObject*>* objects;
	sf::Sprite* background;
	float width;
	float height;
};

