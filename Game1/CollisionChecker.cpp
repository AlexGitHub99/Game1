#include "CollisionChecker.h"

float* CollisionChecker::check(sf::FloatRect &primary, sf::FloatRect &secondary)
{
	float xDisplacement = 0;
	float yDisplacement = 0;

	float primaryLeft = primary.left;
	float primaryRight = primary.left + primary.width;
	float primaryBottom = primary.top;
	float primaryTop = primary.top + primary.height;

	float secondaryLeft = secondary.left;
	float secondaryRight = secondary.left + secondary.width;
	float secondaryBottom = secondary.top;
	float secondaryTop = secondary.top + secondary.height;

	if (
		primaryLeft < secondaryRight and // primary left is left of secondary right             left bottom corner
		primaryLeft > secondaryLeft and // primary left is right of secondary left      
		primaryBottom < secondaryTop and // primary bottom is under secondary top
		primaryBottom > secondaryBottom // primary bottom is above secondary bottom 
		)
	{
		xDisplacement = (secondaryRight)-(primaryLeft);
		yDisplacement = (secondaryTop)-(primaryBottom);
	}
	else if (
		primaryRight > secondaryLeft and // primary right is right of secondary left			right bottom corner
		primaryRight < secondaryRight and // primary right is left of secondary right     
		primaryBottom < secondaryTop and // primary bottom is under secondary top      
		primaryBottom > secondaryBottom // primary bottom is above secondary bottom  
		)
	{

		xDisplacement = (secondaryLeft)-(primaryRight);
		yDisplacement = (secondaryTop)-(primaryBottom);
	}
	else if (
		primaryLeft < secondaryRight and // primary left is left of secondary right				left top corner
		primaryLeft > secondaryLeft and // primary left is right of secondary left      
		primaryTop > secondaryBottom and // primary top is above secondary bottom      
		primaryTop < secondaryTop // primary top is under secondary top        
		)
	{

		xDisplacement = (secondaryRight)-(primaryLeft);
		yDisplacement = (secondaryBottom)-(primaryTop);
	}
	else if (
		primaryRight > secondaryLeft and // primary right is right of secondary left			right top corner
		primaryRight < secondaryRight and // primary right is left of secondary right     
		primaryTop > secondaryBottom and // primary top is above secondary bottom      
		primaryTop < secondaryTop // primary top is under secondary top  
		)
	{

		xDisplacement = (secondaryLeft)-(primaryRight);
		yDisplacement = (secondaryBottom)-(primaryTop);
	}
	else if (
		primaryLeft < secondaryRight and // primary left is left of secondary right				entire left
		primaryLeft > secondaryLeft and // primary left is right of secondary left 
		primaryBottom < secondaryBottom and // primary bottom is below secondary bottom
		primaryTop > secondaryTop // primary top is above secondary top
		)
	{
		xDisplacement = (secondaryRight)-(primaryLeft);
		yDisplacement = 999; //arbitrary
	}
	else if (
		primaryRight > secondaryLeft and // primary right is right of secondary left			entire right
		primaryRight < secondaryRight and // primary right is left of secondary right
		primaryBottom < secondaryBottom and // primary bottom is below secondary bottom
		primaryTop > secondaryTop // primary top is above secondary top
		)
	{
		xDisplacement = (secondaryLeft)-(primaryRight);
		yDisplacement = 999; //arbitrary
	}
	else if (
		primaryRight > secondaryRight and // primary right is right of secondary right			entire bottom
		primaryLeft < secondaryLeft and // primary left is left of secondary left
		primaryBottom < secondaryTop and // primary bottom is below secondary top
		primaryBottom > secondaryBottom // primary bottom is above secondary bottom
		)
	{
		xDisplacement = 999; //arbitrary
		yDisplacement = (secondaryTop)-(primaryBottom);
	}
	else if (
		primaryRight > secondaryRight and // primary right is right of secondary right			entire top
		primaryLeft < secondaryLeft and // primary left is left of secondary left
		primaryTop > secondaryBottom and // primary top is above secondary bottom
		primaryTop < secondaryTop // primary top is below secondary top
		)
	{
		xDisplacement = 999; //arbitrary
		yDisplacement = (secondaryBottom)-(primaryTop);
	}

	return new float[2]{ xDisplacement, yDisplacement };
}

bool CollisionChecker::checkBool(sf::FloatRect &primary, sf::FloatRect &secondary)
{
	float* displacement = check(primary, secondary);
	if (*displacement == 0 and *(displacement + 1) == 0) {
		return false;
	}
	return true;
}
