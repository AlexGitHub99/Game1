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
#include "GameObject.h"
#include "Area.h"
#include "Player.h"

float* relativePosition(float primaryX, float primaryY, float secondaryX, float secondaryY);

int PLAYER_SPEED = 400; //coord per second

int main() {

	int screenWidth = 1000;
	int screenHeight = 1000;
	int FOV = 1500;

	sf::RenderWindow mainWindow(sf::VideoMode(screenWidth, screenHeight), "Game");

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


	
	Player player;
	player.setPosition(500, 500);
	player.setTexture(playerTexture);
	player.setBoundBox(100, 100);
	GameObject* myObj = new GameObject();
	myObj->setTexture(myTexture);
	myObj->setPosition(250, 250);
	myObj->setBoundBox(330, 289);

	GameObject* myObj2 = new GameObject();
	myObj2->setTexture(myTexture);
	myObj2->setPosition(350, 750);
	myObj2->setBoundBox(330, 289);

	Area area;
	area.setBackground(backgroundTexture);
	area.addObject(myObj);
	area.addObject(myObj2);
	area.setSize(10000, 10000);

	clock_t t = clock();

	//main loop
	while (mainWindow.isOpen()) {
		float ms = (clock() - t)*1000/CLOCKS_PER_SEC;
		t = clock();
		sf::Event event;
		
		while (mainWindow.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				mainWindow.close();
			}
		}

		std::vector<GameObject*>* objects = area.getObjects();

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

			for (int i = 0; i < objects->size(); i++) {
				GameObject currentObj = *objects->at(i);

				float xDisplacement = 0;
				float yDisplacement = 0;

				if (player.getX() - player.getWidth() / 2 < currentObj.getX() + currentObj.getWidth() / 2 and // player left is left of object right             left bottom
					player.getX() - player.getWidth() / 2 > currentObj.getX() - currentObj.getWidth() / 2 and // player left is right of object left      
					player.getY() - player.getHeight() / 2 < currentObj.getY() + currentObj.getHeight() / 2 and // player bottom is under object top
					player.getY() - player.getHeight() / 2 > currentObj.getY() - currentObj.getHeight() / 2) { // player bottom is above object bottom 

					xDisplacement = (currentObj.getX() + currentObj.getWidth() / 2) - (player.getX() - player.getWidth() / 2);
					yDisplacement = (currentObj.getY() + currentObj.getHeight() / 2) - (player.getY() - player.getHeight() / 2);
				}
				else if (player.getX() + player.getWidth() / 2 > currentObj.getX() - currentObj.getWidth() / 2 and // player right is right of object left       right bottom
					player.getX() + player.getWidth() / 2 < currentObj.getX() + currentObj.getWidth() / 2 and // player right is left of object right     
					player.getY() - player.getHeight() / 2 < currentObj.getY() + currentObj.getHeight() / 2 and // player bottom is under object top      
					player.getY() - player.getHeight() / 2 > currentObj.getY() - currentObj.getHeight() / 2) {  // player bottom is above object bottom  

					xDisplacement = (currentObj.getX() - currentObj.getWidth() / 2) - (player.getX() + player.getWidth() / 2);
					yDisplacement = (currentObj.getY() + currentObj.getHeight() / 2) - (player.getY() - player.getHeight() / 2);
				}
				else if (player.getX() - player.getWidth() / 2 < currentObj.getX() + currentObj.getWidth() / 2 and // player left is left of object right        left top
					player.getX() - player.getWidth() / 2 > currentObj.getX() - currentObj.getWidth() / 2 and // player left is right of object left      
					player.getY() + player.getHeight() / 2 > currentObj.getY() - currentObj.getHeight() / 2 and // player top is above object bottom      
					player.getY() + player.getHeight() / 2 < currentObj.getY() + currentObj.getHeight() / 2) { // player top is under object top        

					xDisplacement = (currentObj.getX() + currentObj.getWidth() / 2) - (player.getX() - player.getWidth() / 2);
					yDisplacement = (currentObj.getY() - currentObj.getHeight() / 2) - (player.getY() + player.getHeight() / 2);
				}
				else if (player.getX() + player.getWidth() / 2 > currentObj.getX() - currentObj.getWidth() / 2 and // player right is right of object left       right top
					player.getX() + player.getWidth() / 2 < currentObj.getX() + currentObj.getWidth() / 2 and // player right is left of object right     
					player.getY() + player.getHeight() / 2 > currentObj.getY() - currentObj.getHeight() / 2 and // player top is above object bottom      
					player.getY() + player.getHeight() / 2 < currentObj.getY() + currentObj.getHeight() / 2) { // player top is under object top       

					xDisplacement = (currentObj.getX() - currentObj.getWidth() / 2) - (player.getX() + player.getWidth() / 2);
					yDisplacement = (currentObj.getY() - currentObj.getHeight() / 2) - (player.getY() + player.getHeight() / 2);
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
		}

		mainWindow.clear();

		sf::Sprite* background = area.getBackground();
		sf::Vector2u backgroundSize = background->getTexture()->getSize();
		float playerPos[2] = { *player.getPosition(), *(player.getPosition() + 1) };

		float errorX = (playerPos[0] - (float)FOV / 2) / area.getWidth() * (float)backgroundSize.x - (int)((playerPos[0] - (float)FOV / 2) / area.getWidth() * (float)backgroundSize.x);
		float errorY = (playerPos[1] - (float)FOV / 2) / area.getHeight() * (float)backgroundSize.y - (int)((playerPos[1] - (float)FOV / 2) / area.getHeight() * (float)backgroundSize.y);
		background->setTextureRect(sf::IntRect((playerPos[0] - (float)FOV/2)/area.getWidth()* (float)backgroundSize.x, (playerPos[1] - (float)FOV/2)/area.getHeight() * (float)backgroundSize.y, (float)FOV / area.getWidth() * (float)backgroundSize.x + 2, (float)FOV / area.getHeight() * (float)backgroundSize.y + 2));
		background->setScale((float)screenWidth/((float)background->getTextureRect().width - 2), (float)screenHeight/((float)background->getTextureRect().height - 2));

		background->setPosition(( -errorX - 1)* ((float)screenWidth / (float)background->getTextureRect().width), ( -errorY - 1)* ((float)screenHeight / (float)background->getTextureRect().height));
		mainWindow.draw(*area.getBackground());

		

		for (int i = 0; i < objects->size(); i++) {
			GameObject currentObj = *objects->at(i);
			float objectPos[2] = { *currentObj.getPosition(), *(currentObj.getPosition() + 1) };
			float* temp = relativePosition(playerPos[0], playerPos[1], objectPos[0], objectPos[1]);
			float relativePosition[2] = { *temp, *(temp + 1) };
			currentObj.setScreenPosition((relativePosition[0] - currentObj.getWidth()/2)/(float)FOV* (float)screenWidth + (float)screenWidth / 2, (relativePosition[1] - currentObj.getHeight() / 2)/(float)FOV* (float)screenHeight + (float)screenHeight / 2);
			sf::Sprite currentSprite = *(currentObj.getSprite());
			currentSprite.setScale((float)screenWidth / (float)FOV, (float)screenHeight / (float)FOV);
			mainWindow.draw(currentSprite);
		}
		
		player.setScreenPosition((float)screenWidth/2 - (player.getWidth()/(float)FOV*(float)screenWidth)/2, (float)screenHeight / 2 - (player.getHeight() / (float)FOV * (float)screenHeight) / 2);
		sf::Sprite playerSprite = *player.getSprite();
		playerSprite.setScale((float)screenWidth / (float)FOV, (float)screenHeight / (float)FOV);
		mainWindow.draw(playerSprite);

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