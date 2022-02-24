#include "Path.h"

vector<float[2]> Path::find(Area& area, Entity &entity, Player& player) //main pathfinding function
{
	list<shared_ptr<GameObject>> objects = *(area.getObjects());

	int width = floor(area.getWidth() / entity.getBoundBoxWidth());
	int height = floor(area.getHeight() / entity.getBoundBoxHeight());
	vector<unique_ptr<vector<bool>>> tileMap;
	for (int x = 0; x < width; x++) {
		tileMap.push_back(unique_ptr<vector<bool>>(new vector<bool>()));
		for (int y = 0; y < height; y++) {
			bool value = isFilled(objects, entity, x, y);
			tileMap.back()->push_back(value);
		}
	}
	
	cout << tileMap.size() << endl;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			assert(x < tileMap.size());
			assert(y < tileMap.at(x)->size());

			cout << (tileMap.at(x)->at(y) ? "X" : " ");
		}
		cout << endl;
	}
	
	vector<float[2]> points;
	return points;

}

//check if a given tile defined by integer coordinates and a tile size defined by creature bound box size interesects with any objects
bool Path::isFilled(list<shared_ptr<GameObject>> &objects, Entity &entity, int x, int y) {
	CollisionChecker collisionChecker;

	sf::FloatRect tileBox;
	tileBox.left = x* entity.getBoundBoxWidth();
	tileBox.width = entity.getBoundBoxWidth();
	tileBox.top = y* entity.getBoundBoxHeight();
	tileBox.height = entity.getBoundBoxHeight();

	for (std::list<shared_ptr<GameObject>>::iterator it = objects.begin(); it != objects.end(); it++) {
		shared_ptr<GameObject> currentObj = *it;

		if (currentObj->getId() == entity.getId()) {
			continue;
		}

		sf::FloatRect objectBox;
		objectBox.left = currentObj->getX() - currentObj->getBoundBoxWidth() / 2 + currentObj->getBoundBoxOffsetX();
		objectBox.top = currentObj->getY() - currentObj->getBoundBoxHeight() / 2 + currentObj->getBoundBoxOffsetY();
		objectBox.width = currentObj->getBoundBoxWidth();
		objectBox.height = currentObj->getBoundBoxHeight();
		
		if (collisionChecker.checkBool(tileBox, objectBox)) {
			return true;
		}
	}
	return false;
}