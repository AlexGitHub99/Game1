#include "Orb.h"
#include <iostream>

void Orb::update(Area& area, Player &player, float ms)
{
	float playerPos[2] = { *(player.getPosition()), *(player.getPosition() + 1) };
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
	area.sortObject(shared_from_this());
}
