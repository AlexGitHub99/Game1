#include "Orb.h"

void Orb::move(Area& area)
{
	setPosition(position[0] + 10, position[1] + 10);
	area.sortObject(this);
}
