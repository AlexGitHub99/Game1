#pragma once
#include "Area.h"

class CollisionChecker
{
public:
	float* check(sf::FloatRect &primary, sf::FloatRect &secondary);
	bool checkBool(sf::FloatRect &primary, sf::FloatRect &secondary);
};

