#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

class Area
{
public:
	Area() {
		objects = new std::vector<GameObject*>;
	}

	void setBackground(sf::Texture* newBackground);
	void setSize(float newWidth, float newHeight);
	void addObject(GameObject* newObj);
	std::vector<GameObject*>* getObjects();
	sf::Sprite* getBackground();
	float getWidth();
	float getHeight();
private:
	std::vector<GameObject*>* objects;
	sf::Sprite* background;
	float width;
	float height;
};

