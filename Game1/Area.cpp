#include "Area.h"

 Area::Area(std::shared_ptr<sf::Texture> newBackground, float newWidth, float newHeight, std::string newTexturePath) {
	backgroundTexture = newBackground;
	background = std::shared_ptr<sf::Sprite>(new sf::Sprite(*newBackground));
	width = newWidth;
	height = newHeight;
	objects = std::shared_ptr<std::list<std::shared_ptr<GameObject>>>(new std::list<std::shared_ptr<GameObject>>);
	entities = std::shared_ptr<std::vector<std::shared_ptr<Entity>>>(new std::vector<std::shared_ptr<Entity>>);
	texturePath = newTexturePath;
}

void Area::setSize(float newWidth, float newHeight)
{
	width = newWidth;
	height = newHeight;
}

void Area::addObject(std::shared_ptr<GameObject> newObj)
{
	if (objects->empty()) {
		objects->push_back(newObj);
		return;
	}
	for (std::list<std::shared_ptr<GameObject>>::iterator it = objects->begin(); it != objects->end(); it++) {
		float yPosNew = *(newObj->getPosition() + 1) + newObj->getTextureHeight() / 2;
		float yPos = *((*it)->getPosition() + 1) + (*it)->getTextureHeight() / 2;
		if (yPosNew < yPos) {
			objects->insert(it, newObj);
			return;
		}
	}
	objects->push_back(newObj);
}

void Area::sortObject(std::shared_ptr<GameObject> obj)
{
	objects->remove(obj);
	addObject(obj);
}

void Area::addEntity(std::shared_ptr<Entity> newEnt)
{
	entities->push_back(newEnt);
	addObject(newEnt);
}


std::shared_ptr<std::list<std::shared_ptr<GameObject>>> Area::getObjects()
{
	return objects;
}

std::shared_ptr<std::vector<std::shared_ptr<Entity>>> Area::getEntities()
{
	return entities;
}

std::shared_ptr<sf::Sprite> Area::getBackground()
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

std::string Area::getTexturePath()
{
	return texturePath;
}
