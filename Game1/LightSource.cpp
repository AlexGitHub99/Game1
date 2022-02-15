#include "LightSource.h"

void LightSource::setLightLevel(float newLightLevel)
{
	lightLevel = newLightLevel;
}

float LightSource::getLightLevel()
{
	return lightLevel;
}

std::string getType()
{
	return "lightSource";
}