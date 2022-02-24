#pragma once
#include "Creature.h"
#include <iostream>
#include <cassert>
#include "CollisionChecker.h"

using namespace std;
class Path
{
public:
	std::vector<float[2]> find(Area& area, Entity &entity, Player& player);

private:
	bool isFilled(list<shared_ptr<GameObject>>& objects, Entity &entity, int x, int y);
};

