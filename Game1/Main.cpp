#define _USE_MATH_DEFINES

#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>
#include <windows.h>
#include <WinUser.h>
#include <thread>
#include <fstream>
#include <time.h>
#include "wtypes.h"
#include "GameObject.h"
#include "Area.h"
#include "Player.h"
#include "LightSource.h"

int PLAYER_SPEED = 1000; //coord per second

const int lightMapWidth = 40;
const int lightMapHeight = 40;

float* relativePosition(float primaryX, float primaryY, float secondaryX, float secondaryY);
void setSquare(sf::VertexArray* lightPoints, int x, int y, int alpha, int lightMapWidth, int lightMapHeight);
void setPoints(sf::VertexArray* lightPoints, int lightMap[][lightMapHeight + 1], float baseLevel);
void getDesktopResolution(int& horizontal, int& vertical);


int main() {

	int screenWidth;
	int screenHeight;
	getDesktopResolution(screenWidth, screenHeight);
	int baseLightLevel = 10;
	float FOVRatio = 16 / 10;
	int FOVW = 2000;
	int FOVH = (float)FOVW / FOVRatio;

	if ((float)screenWidth / (float)screenHeight > (float)FOVW / (float)FOVH) {
		FOVRatio = (float)screenWidth / (float)screenHeight;
		FOVH = (float)FOVW / FOVRatio;
	}
	else if ((float)screenWidth / (float)screenHeight < (float)FOVW / (float)FOVH) {
		FOVRatio = ((float)screenWidth / (float)screenHeight);
		FOVW = (float)FOVH * FOVRatio;
	}


	sf::RenderWindow mainWindow(sf::VideoMode(screenWidth, screenHeight), "Game", sf::Style::Fullscreen);

	sf::Texture *myTexture = new sf::Texture();
	if (!myTexture->loadFromFile("resources/temprock.png")) {
		return -1;
	}

	sf::Texture *playerTexture = new sf::Texture();
	if (!playerTexture->loadFromFile("resources/player.png")) {
		return -1;
	}

	sf::Texture* backgroundTexture = new sf::Texture();
	if (!backgroundTexture->loadFromFile("resources/area_1.png")) {
		return -1;
	}

	sf::Texture lightTexture;
	if (!lightTexture.loadFromFile("resources/light.png")) {
		return -1;
	}

	sf::Texture* lampTexture = new sf::Texture();
	if (!lampTexture->loadFromFile("resources/lamp.png")) {
		return -1;
	}
	
	Player player;
	player.setPosition(250, 250);
	player.setTexture(playerTexture);
	player.setBoundBox(50);
	player.setLightLevel(600); //level that light reaches
	player.setTextureSize(50);

	//temporary stuff-----------------------------------------------------------
	GameObject* myObj = new GameObject();
	myObj->setTexture(myTexture);
	myObj->setPosition(250, 250);
	myObj->setTextureSize(100, 50);
	myObj->setBoundBox(100, 100);

	GameObject* myObj2 = new GameObject();
	myObj2->setTexture(myTexture);
	myObj2->setPosition(1000, 400);
	myObj2->setTextureSize(600);
	myObj2->setBoundBox(600);

	LightSource* lamp = new LightSource();
	lamp->setTexture(lampTexture);
	lamp->setPosition(200, 800);
	lamp->setTextureSize(10, 50);
	lamp->setBoundBox(10, 50);
	lamp->setLightLevel(500);

	LightSource* lamp2 = new LightSource();
	lamp2->setTexture(lampTexture);
	lamp2->setPosition(900, 800);
	lamp2->setTextureSize(60);
	lamp2->setBoundBox(50);
	lamp2->setLightLevel(600);

	sf::Sprite lightSprite(lightTexture);
	lightSprite.setPosition(screenWidth / 2, screenHeight / 2);

	Area area;
	area.setBackground(backgroundTexture);
	area.addObject(myObj);
	area.addObject(myObj2);
	area.addObject(lamp);
	area.addObject(lamp2);

	/*for (int i = 0; i < 20; i++) {
		LightSource* lamp3 = new LightSource();
		lamp3->setTexture(lampTexture);
		lamp3->setPosition(100 + i*100, 700);
		lamp3->setTextureSize(50);
		lamp3->setBoundBox(50);
		lamp3->setLightLevel(500);
		area.addObject(lamp3);
	}*/

	/*for (int i = 0; i < 80; i++) {
		GameObject* myObj3 = new GameObject();
		myObj3->setTexture(myTexture);
		myObj3->setPosition(100 + 50*i, 700);
		myObj3->setTextureSize(300);
		myObj3->setBoundBox(300);
		area.addObject(myObj3);
	}*/


	//end of temporary stuff--------------------------------------------

	area.setSize(5000, 5000);

	clock_t t = clock();

	int lightMap[lightMapWidth + 1][lightMapHeight + 1]; //extra width because of edge points

	sf::VertexArray* lightPoints = new sf::VertexArray(sf::Quads, lightMapWidth* lightMapHeight * 4);

	float squareWidth = (float)screenWidth / (float)lightMapWidth;
	float squareHeight = (float)screenHeight / (float)lightMapHeight;

	//fill vertex array with points
	for (int x = 0; x < lightMapWidth; x++) {
		for (int y = 0; y < lightMapHeight; y++) {

			(*lightPoints)[y * lightMapWidth * 4 + x * 4].position = sf::Vector2f((float)x * squareWidth, (float)y * squareHeight);
			(*lightPoints)[y * lightMapWidth * 4 + x * 4 + 1].position = sf::Vector2f((float)(x + 1) * squareWidth, (float)y * squareHeight);
			(*lightPoints)[y * lightMapWidth * 4 + x * 4 + 2].position = sf::Vector2f((float)(x + 1) * squareWidth, (float)(y + 1) * squareHeight);
			(*lightPoints)[y * lightMapWidth * 4 + x * 4 + 3].position = sf::Vector2f((float)x * squareWidth, (float)(y + 1) * squareHeight);
		}
	}

	//main loop
	while (mainWindow.isOpen()) {
		float ms = (clock() - t)*1000/CLOCKS_PER_SEC;
		t = clock();
		sf::Event event;
		
		while (mainWindow.pollEvent(event)) {

			if (event.type == sf::Event::MouseWheelMoved) {
				FOVW += -(float)FOVW*(float)event.mouseWheel.delta/20;
				FOVH = (float)FOVW / FOVRatio;
			}

			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::Escape) {
					mainWindow.close();
				}
			}
			if (event.type == sf::Event::Closed) {
				mainWindow.close();
			}
		}

		std::vector<GameObject*>* objects = area.getObjects();
		//get keyboard input
		float angle = 0.0;
		bool UP = false;
		bool DOWN = false;
		bool RIGHT = false;
		bool LEFT = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			LEFT = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			RIGHT = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			UP = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			DOWN = true;
		}


		//handle movement
		if (UP == true or RIGHT == true or DOWN == true or LEFT == true) {
			float magnitude = ms / 1000 * PLAYER_SPEED;
			if (UP == true and RIGHT == true) {
				angle = M_PI / 4;
			}
			else if (UP == true and LEFT == true) {
				angle = M_PI * 3 / 4;
			}
			else if (DOWN == true and RIGHT == true) {
				angle = -M_PI / 4;
			}
			else if (DOWN == true and LEFT == true) {
				angle = -M_PI * 3 / 4;
			}
			else if (UP == true) {
				angle = M_PI / 2;
			}
			else if (DOWN == true) {
				angle = -M_PI / 2;
			}
			else if (RIGHT == true) {
				angle = 0;
			}
			else if (LEFT == true) {
				angle = M_PI;
			}

			
			player.movePosition(cos(angle) * magnitude, sin(angle) * -magnitude);

			//collissions

			//object collisions
			for (int i = 0; i < objects->size(); i++) {
				GameObject currentObj = *objects->at(i);

				float xDisplacement = 0;
				float yDisplacement = 0;

				float playerLeft = player.getX() - player.getWidth() / 2;
				float playerRight = player.getX() + player.getWidth() / 2;
				float playerBottom = player.getY() - player.getHeight() / 2;
				float playerTop = player.getY() + player.getHeight() / 2;

				float objectLeft = currentObj.getX() - currentObj.getWidth() / 2;
				float objectRight = currentObj.getX() + currentObj.getWidth() / 2;
				float objectBottom = currentObj.getY() - currentObj.getHeight() / 2;
				float objectTop = currentObj.getY() + currentObj.getHeight() / 2;

				if (
					playerLeft < objectRight and // player left is left of object right             left bottom corner
					playerLeft > objectLeft and // player left is right of object left      
					playerBottom < objectTop and // player bottom is under object top
					playerBottom > objectBottom // player bottom is above object bottom 
					) 
				{ 
					xDisplacement = (objectRight) - (playerLeft);
					yDisplacement = (objectTop) - (playerBottom);
				}
				else if (
					playerRight > objectLeft and // player right is right of object left			right bottom corner
					playerRight < objectRight and // player right is left of object right     
					playerBottom < objectTop and // player bottom is under object top      
					playerBottom > objectBottom // player bottom is above object bottom  
					) 
				{

					xDisplacement = (objectLeft) - (playerRight);
					yDisplacement = (objectTop) - (playerBottom);
				}
				else if (
					playerLeft < objectRight and // player left is left of object right				left top corner
					playerLeft > objectLeft and // player left is right of object left      
					playerTop > objectBottom and // player top is above object bottom      
					playerTop < objectTop // player top is under object top        
					) 
				{ 

					xDisplacement = (objectRight) - (playerLeft);
					yDisplacement = (objectBottom) - (playerTop);
				}
				else if (
					playerRight > objectLeft and // player right is right of object left			right top corner
					playerRight < objectRight and // player right is left of object right     
					playerTop > objectBottom and // player top is above object bottom      
					playerTop < objectTop // player top is under object top  
					) 
				{      

					xDisplacement = (objectLeft) - (playerRight);
					yDisplacement = (objectBottom) - (playerTop);
				} 
				else if (																																		
					playerLeft < objectRight and // player left is left of object right				entire left
					playerLeft > objectLeft and // player left is right of object left 
					playerBottom < objectBottom and // player bottom is below object bottom
					playerTop > objectTop // player top is above object top
					)
				{
					xDisplacement = (objectRight) - (playerLeft);
					yDisplacement = 999; //arbitrary
				} 
				else if (
					playerRight > objectLeft and // player right is right of object left			entire right
					playerRight < objectRight and // player right is left of object right
					playerBottom < objectBottom and // player bottom is below object bottom
					playerTop > objectTop // player top is above object top
					)
				{
					xDisplacement = (objectLeft) - (playerRight);
					yDisplacement = 999; //arbitrary
				} 
				else if (
					playerRight > objectRight and // player right is right of object right			entire bottom
					playerLeft < objectLeft and // player left is left of object left
					playerBottom < objectTop and // player bottom is below object top
					playerBottom > objectBottom // player bottom is above object bottom
					)
				{
					xDisplacement = 999; //arbitrary
					yDisplacement = (objectTop) - (playerBottom);
				}
				else if (
					playerRight > objectRight and // player right is right of object right			entire top
					playerLeft < objectLeft and // player left is left of object left
					playerTop > objectBottom and // player top is above object bottom
					playerTop < objectTop // player top is below object top
					)
				{
					xDisplacement = 999; //arbitrary
					yDisplacement = (objectBottom) - (playerTop);
				}


				if (xDisplacement != 0 and yDisplacement != 0) {
					if (abs(xDisplacement) < abs(yDisplacement)) {
						player.movePosition(xDisplacement, 0.0);
					}
					else {
						player.movePosition(0.0, yDisplacement);
					}
				}
			}

			//border collisions
			if (player.getX() - player.getWidth() / 2 < 0) {
				player.movePosition(0 - (player.getX() - player.getWidth() / 2), 0.0);
			}
			if (player.getX() + player.getWidth() / 2 > area.getWidth()) {
				player.movePosition(area.getWidth() - (player.getX() + player.getWidth() / 2), 0.0);
			}
			if (player.getY() - player.getHeight() / 2 < 0) {
				player.movePosition(0.0, 0 - (player.getY() - player.getHeight() / 2));
			}
			if (player.getY() + player.getHeight() / 2 > area.getHeight()) {
				player.movePosition(0.0, area.getHeight() - (player.getY() + player.getHeight() / 2));
			}

		}

		mainWindow.clear();

		//draw background
		sf::Sprite* background = area.getBackground();
		sf::Vector2u backgroundSize = background->getTexture()->getSize();
		float playerPos[2] = { *player.getPosition(), *(player.getPosition() + 1) };

		float errorX = (playerPos[0] - (float)FOVW / 2) / area.getWidth() * (float)backgroundSize.x - 1 - (int)((playerPos[0] - (float)FOVW / 2) / area.getWidth() * (float)backgroundSize.x - 1);
		float errorY = (playerPos[1] - (float)FOVH / 2) / area.getHeight() * (float)backgroundSize.y - 1 - (int)((playerPos[1] - (float)FOVH / 2) / area.getHeight() * (float)backgroundSize.y - 1);
		background->setTextureRect(sf::IntRect((playerPos[0] - (float)FOVW/2)/area.getWidth()* (float)backgroundSize.x - 1, (playerPos[1] - (float)FOVH/2)/area.getHeight() * (float)backgroundSize.y - 1, (float)FOVW / area.getWidth() * (float)backgroundSize.x + 2, (float)FOVH / area.getHeight() * (float)backgroundSize.y + 2));
		background->setScale((float)screenWidth/((float)background->getTextureRect().width - 2), (float)screenHeight/((float)background->getTextureRect().height - 2));

		float backgroundX = (-errorX - 1) * ((float)screenWidth / (float)background->getTextureRect().width);
		float backgroundY = (-errorY - 1) * ((float)screenHeight / (float)background->getTextureRect().height);
		background->setPosition(backgroundX, backgroundY);
		mainWindow.draw(*area.getBackground());

		//draw black area outside of background border
		float backgroundScreenEdgeLeft = -playerPos[0] / (float)FOVW * (float)screenWidth + (float)screenWidth / 2;
		float backgroundScreenEdgeRight = (area.getWidth() - playerPos[0]) / (float)FOVW * (float)screenWidth + (float)screenWidth / 2;
		float backgroundScreenEdgeTop = -playerPos[1] / (float)FOVH * (float)screenHeight + (float)screenHeight / 2;
		float backgroundScreenEdgeBottom = (area.getHeight() - playerPos[1]) / (float)FOVH * (float)screenHeight + (float)screenHeight / 2;

		if (backgroundScreenEdgeLeft > 0) {
			sf::RectangleShape blackRect(sf::Vector2f(backgroundScreenEdgeLeft, screenHeight));
			blackRect.setPosition(0, 0);
			blackRect.setFillColor(sf::Color(0, 0, 0, 255));
			mainWindow.draw(blackRect);
		}
		if (backgroundScreenEdgeRight < screenWidth) {
			sf::RectangleShape blackRect(sf::Vector2f(backgroundScreenEdgeRight, screenHeight));
			blackRect.setPosition(backgroundScreenEdgeRight, 0);
			blackRect.setFillColor(sf::Color(0, 0, 0, 255));
			mainWindow.draw(blackRect);
		}
		if (backgroundScreenEdgeTop > 0) {
			sf::RectangleShape blackRect(sf::Vector2f(screenWidth, backgroundScreenEdgeTop));
			blackRect.setPosition(0, 0);
			blackRect.setFillColor(sf::Color(0, 0, 0, 255));
			mainWindow.draw(blackRect);
		}
		if (backgroundScreenEdgeBottom < screenHeight) {
			sf::RectangleShape blackRect(sf::Vector2f(screenWidth, backgroundScreenEdgeBottom));
			blackRect.setPosition(0, backgroundScreenEdgeBottom);
			blackRect.setFillColor(sf::Color(0, 0, 0, 255));
			mainWindow.draw(blackRect);
		}
		
		//Add player to light source list
		float playerLightDistX = player.getLightLevel() / (float)FOVW * (float)screenWidth;

		std::vector<std::pair<sf::Vector2f, float>> sourcePositions;
		sourcePositions.push_back(std::pair<sf::Vector2f, float>(sf::Vector2f((float)screenWidth / 2, (float)screenHeight / 2), playerLightDistX));

		//draw objects
		for (int i = 0; i < objects->size(); i++) {
			GameObject* currentObj = objects->at(i);
			float objectPos[2] = { *currentObj->getPosition(), *(currentObj->getPosition() + 1) };
			float* temp = relativePosition(playerPos[0], playerPos[1], objectPos[0], objectPos[1]);
			float relativePosition[2] = { *temp, *(temp + 1) };
			currentObj->setScreenPosition((relativePosition[0] - currentObj->getTextureWidth()/2)/(float)FOVW* (float)screenWidth + (float)screenWidth / 2, (relativePosition[1] - currentObj->getTextureHeight() / 2)/(float)FOVH* (float)screenHeight + (float)screenHeight / 2);
			sf::Sprite* currentSprite = currentObj->getSprite();
			currentSprite->setScale(currentObj->getTextureWidth() / (float)FOVW * (float)screenWidth / currentObj->getTexture()->getSize().x, currentObj->getTextureHeight() / (float)FOVH * (float)screenHeight / currentObj->getTexture()->getSize().y);
			mainWindow.draw(*currentSprite);
			
			if (currentObj->getType().compare("lightSource") == 0) {
				float objectLightDistX = currentObj->getLightLevel() / (float)FOVW * (float)screenWidth;
				sourcePositions.push_back(std::pair<sf::Vector2f, float>(currentSprite->getPosition(), objectLightDistX));
			}
		}

		//create lightMap
		float xRatio = (float)screenWidth / (float)lightMapWidth;
		float yRatio = (float)screenHeight / (float)lightMapHeight;
		for (int x = 0; x <= lightMapWidth; x++) {
			for (int y = 0; y <= lightMapHeight; y++) {
				float xPos = xRatio * (float)x;
				float yPos = yRatio * (float)y;
				float alphaSquared = 0;
				for (int i = 0; i < sourcePositions.size(); i++) {
					sf::Vector2f currentSource = sourcePositions.at(i).first;
					float distance = sqrt(pow(currentSource.x - xPos, 2) + pow(currentSource.y - yPos, 2));
					float lightDist = sourcePositions.at(i).second;
					float nextAlpha = distance / lightDist * 255;
					if (nextAlpha < 255) {
						alphaSquared += pow(255 - nextAlpha, 2);
					}
				}
				float alpha = sqrt(alphaSquared);
				if(alpha > 255) {
					alpha = 255;
				}
				alpha = 255 - alpha;
				lightMap[x][y] = alpha;
			}
		}
		
		//draw player
		player.setScreenPosition((float)screenWidth/2 - (player.getTextureWidth()/(float)FOVW*(float)screenWidth)/2, (float)screenHeight / 2 - (player.getTextureHeight() / (float)FOVH * (float)screenHeight) / 2);
		player.getSprite()->setScale(player.getTextureWidth() * (float)screenWidth / (float)FOVW / player.getSprite()->getTexture()->getSize().x, player.getTextureHeight() * (float)screenHeight / (float)FOVH / player.getSprite()->getTexture()->getSize().y);
		mainWindow.draw(*player.getSprite());

		//draw lightMap
		setPoints(lightPoints, lightMap, baseLightLevel);
		mainWindow.draw(*lightPoints);


		sf::Font courier;
		if (!courier.loadFromFile("resources/fonts/CourierPrime-Regular.ttf")) {
			return -1;
		}
		sf::Text coords;
		coords.setFont(courier);
		coords.setString("X: " + std::to_string(playerPos[0]) + " Y: " + std::to_string(playerPos[1]));
		mainWindow.draw(coords);

		//mainWindow.setActive();

		mainWindow.display();
	}

	return 0;
}

float* relativePosition(float primaryX, float primaryY, float secondaryX, float secondaryY) {
	float relPos[2] = { secondaryX - primaryX, secondaryY - primaryY };
	return relPos;
}

void setSquare(sf::VertexArray* lightPoints, int x, int y, int alpha, int lightMapWidth, int lightMapHeight) { //x and y < lightMapWidth and LightMapHeight
	(*lightPoints)[y * lightMapWidth * 4 + x * 4].color = sf::Color(0, 0, 0, alpha);
	(*lightPoints)[y * lightMapWidth * 4 + x * 4 + 1].color = sf::Color(0, 0, 0, alpha);
	(*lightPoints)[y * lightMapWidth * 4 + x * 4 + 2].color = sf::Color(0, 0, 0, alpha);
	(*lightPoints)[y * lightMapWidth * 4 + x * 4 + 3].color = sf::Color(0, 0, 0, alpha);
}


//This method is WAYYY better (like 100-1000 times more efficient for same result)
void setPoints(sf::VertexArray* lightPoints, int lightMap[][lightMapHeight + 1], float baseLevel) { //x and y <= lightMapWidth and LightMapHeight
	for (int x = 0; x <= lightMapWidth; x++) {
		for (int y = 0; y <= lightMapHeight; y++) {
			int alpha = lightMap[x][y];
			alpha = alpha - baseLevel >= 0 ? alpha - baseLevel : 0;
			if (y * lightMapWidth * 4 + x * 4 < (*lightPoints).getVertexCount()) { //check under array size
				//top left
				(*lightPoints)[y * lightMapWidth * 4 + x * 4].color = sf::Color(0, 0, 0, alpha);
			}
			if (y * lightMapWidth * 4 + (x - 1) * 4 + 1 < (*lightPoints).getVertexCount() and y * lightMapWidth * 4 + (x - 1) * 4 + 1 >= 0) { //check under array size, check above -1
				//top right
				(*lightPoints)[y * lightMapWidth * 4 + (x - 1) * 4 + 1].color = sf::Color(0, 0, 0, alpha);
			}
			if ((y - 1) * lightMapWidth * 4 + x * 4 + 3 < (*lightPoints).getVertexCount() and (y - 1) * lightMapWidth * 4 + x * 4 + 3 >= 0) { //check under array size, check above -1
				//bottom left
				(*lightPoints)[(y - 1) * lightMapWidth * 4 + x * 4 + 3].color = sf::Color(0, 0, 0, alpha);
			}
			if ((y - 1) * lightMapWidth * 4 + (x - 1) * 4 + 2 >= 0) { // check above -1
				//bottom right
				(*lightPoints)[(y - 1) * lightMapWidth * 4 + (x - 1) * 4 + 2].color = sf::Color(0, 0, 0, alpha);
			}
		}
	}
}

//source: https://cppkid.wordpress.com/2009/01/07/how-to-get-the-screen-resolution-in-pixels/
void getDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}