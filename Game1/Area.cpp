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
	if (objects->empty()) {
		objects->push_back(newObj);
		return;
	}
	for (std::list<GameObject*>::iterator it = objects->begin(); it != objects->end(); it++) {
		float yPosNew = *(newObj->getPosition() + 1) + newObj->getTextureHeight() / 2;
		float yPos = *((*it)->getPosition() + 1) + (*it)->getTextureHeight() / 2;
		if (yPosNew < yPos) {
			objects->insert(it, newObj);
			return;
		}
	}
	objects->push_back(newObj);
}

std::list<GameObject*>* Area::getObjects()
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
