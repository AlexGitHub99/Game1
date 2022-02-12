#include "Area.h"

void Area::setBackground(sf::Texture* newBackground)
{
	background = new sf::Sprite(*newBackground);
}

void Area::setSize(float newWidth, float newHeight)
{
	width = newWidth;
	height = newHeight;
}

void Area::addObject(GameObject* newObj)
{
	objects->push_back(newObj);
}

std::vector<GameObject*>* Area::getObjects()
{
	return objects;
}

sf::Sprite* Area::getBackground()
{
	return background;
}

float Area::getWidth()
{
	return width;
}

float Area::getHeight()
{
	return width;
}
