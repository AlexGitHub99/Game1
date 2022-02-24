#include "Orb.h"
#include <iostream>

void Orb::update(Area& area, Player &player, float ms)
{
	float playerPos[2] = { player.getX(), player.getY() };
	float relativePos[2] = {playerPos[0] - position[0], playerPos[1] - position[1]};
	float distance = sqrt(pow(relativePos[0], 2) + pow(relativePos[1], 2));
	if (distance <= damageRadius) {
		player.setHealth(player.getHealth() - damage * ms / 1000);
	}
	if(distance > 5) {
		float moveX = relativePos[0] / distance * speed * ms / 1000;
		float moveY = relativePos[1] / distance * speed * ms / 1000;
		setPosition(position[0] + moveX, position[1] + moveY);
	}

	CollisionChecker collisionChecker;

	sf::FloatRect orbBox;
	orbBox.left = position[0] - boundBoxWidth / 2;
	orbBox.top = position[1] - boundBoxHeight / 2;
	orbBox.width = boundBoxWidth;
	orbBox.height = boundBoxHeight;

	shared_ptr<list<shared_ptr<GameObject>>> objects = area.getObjects();

	for (std::list<shared_ptr<GameObject>>::iterator it = objects->begin(); it != objects->end(); it++) {
		shared_ptr<GameObject> currentObj = *it;

		sf::FloatRect objectBox;
		objectBox.left = currentObj->getX() - currentObj->getBoundBoxWidth() / 2 + currentObj->getBoundBoxOffsetX();
		objectBox.top = currentObj->getY() - currentObj->getBoundBoxHeight() / 2 + currentObj->getBoundBoxOffsetY();
		objectBox.width = currentObj->getBoundBoxWidth();
		objectBox.height = currentObj->getBoundBoxHeight();

		float* displacement = collisionChecker.check(orbBox, objectBox);
		float xDisplacement = *displacement;
		float yDisplacement = *(displacement + 1);

		if (xDisplacement != 0 and yDisplacement != 0) {
			if (abs(xDisplacement) < abs(yDisplacement)) {
				setPosition(position[0] + xDisplacement, position[1]);;
			}
			else {
				setPosition(position[0], position[1] + yDisplacement);
			}
		}

		free(displacement);
	}
	
	area.sortObject(shared_from_this());
}
