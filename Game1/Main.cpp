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

int PLAYER_SPEED = 800;//pixels per second

int main() {

	int screenWidth = 1000;
	int screenHeight = 1000;

	sf::RenderWindow mainWindow(sf::VideoMode(screenWidth, screenHeight), "Game");

	sf::Texture *myTexture = new sf::Texture();
	if (!myTexture->loadFromFile("resources/temprock.png")) {
		return -1;
	}

	sf::Texture* playerTexture = new sf::Texture();
	if (!playerTexture->loadFromFile("resources/player.png")) {
		return -1;
	}

	sf::Vector2u playerTextureSize = playerTexture->getSize();


	Player player;
	player.setPosition(500, 500);
	player.setTexture(playerTexture);
	player.setBoundBox(100, 100);
	player.setScreenPosition(round((screenWidth - playerTextureSize.x)/2) , round((screenHeight - playerTextureSize.y)/2));
	GameObject myObj;
	myObj.setTexture(myTexture);
	myObj.setPosition(250, 250);
	myObj.setBoundBox(350, 350);

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

			w
			player.movePosition(cos(angle) * magnitude, sin(angle) * -magnitude);

			float xDisplacement = 0;
			float yDisplacement = 0;

			if (player.getX() - player.getWidth() / 2 < myObj.getX() + myObj.getWidth() / 2 and // player left is left of object right             left bottom
				player.getX() - player.getWidth() / 2 > myObj.getX() - myObj.getWidth() / 2 and // player left is right of object left      
				player.getY() - player.getHeight() / 2 < myObj.getY() + myObj.getHeight() / 2 and // player bottom is under object top
				player.getY() - player.getHeight() / 2 > myObj.getY() - myObj.getHeight() / 2) { // player bottom is above object bottom 

				xDisplacement = (myObj.getX() + myObj.getWidth() / 2) - (player.getX() - player.getWidth() / 2);
				yDisplacement = (myObj.getY() + myObj.getHeight() / 2) - (player.getY() - player.getHeight() / 2);
			}
			else if (player.getX() + player.getWidth() / 2 > myObj.getX() - myObj.getWidth() / 2 and // player right is right of object left       right bottom
				player.getX() + player.getWidth() / 2 < myObj.getX() + myObj.getWidth() / 2 and // player right is left of object right     
				player.getY() - player.getHeight() / 2 < myObj.getY() + myObj.getHeight() / 2 and // player bottom is under object top      
				player.getY() - player.getHeight() / 2 > myObj.getY() - myObj.getHeight() / 2) {  // player bottom is above object bottom  

				xDisplacement = (myObj.getX() - myObj.getWidth() / 2) - (player.getX() + player.getWidth() / 2);
				yDisplacement = (myObj.getY() + myObj.getHeight() / 2) - (player.getY() - player.getHeight() / 2);
			}
			else if (player.getX() - player.getWidth() / 2 < myObj.getX() + myObj.getWidth() / 2 and // player left is left of object right        left top
				player.getX() - player.getWidth() / 2 > myObj.getX() - myObj.getWidth() / 2 and // player left is right of object left      
				player.getY() + player.getHeight() / 2 > myObj.getY() - myObj.getHeight() / 2 and // player top is above object bottom      
				player.getY() + player.getHeight() / 2 < myObj.getY() + myObj.getHeight() / 2) { // player top is under object top        

				xDisplacement = (myObj.getX() + myObj.getWidth() / 2) - (player.getX() - player.getWidth() / 2);
				yDisplacement = (myObj.getY() - myObj.getHeight() / 2) - (player.getY() + player.getHeight() / 2);
			}
			else if	(player.getX() + player.getWidth() / 2 > myObj.getX() - myObj.getWidth() / 2 and // player right is right of object left       right top
				player.getX() + player.getWidth() / 2 < myObj.getX() + myObj.getWidth() / 2 and // player right is left of object right     
				player.getY() + player.getHeight() / 2 > myObj.getY() - myObj.getHeight() / 2 and // player top is above object bottom      
				player.getY() + player.getHeight() / 2 < myObj.getY() + myObj.getHeight() / 2) { // player top is under object top       

				xDisplacement = (myObj.getX() - myObj.getWidth() / 2) - (player.getX() + player.getWidth() / 2);
				yDisplacement = (myObj.getY() - myObj.getHeight() / 2) - (player.getY() + player.getHeight() / 2);
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

		mainWindow.clear();
		//sf::Sprite mySprite = *myObj.getSprite();
		float playerPos[2] = { *player.getPosition(), *(player.getPosition() + 1) };
		float objectPos[2] = { *myObj.getPosition(), *(myObj.getPosition() + 1) };
		float* temp = relativePosition(playerPos[0], playerPos[1], objectPos[0], objectPos[1]);
		float relativePosition[2] = { *temp, *(temp + 1) };
		/*mySprite.setPosition(relativePosition[0] + screenWidth/2, relativePosition[1] + screenHeight/2);*/
		sf::Vector2u textureSize = myTexture->getSize();
		myObj.setScreenPosition(relativePosition[0] + screenWidth / 2 - textureSize.x / 2, relativePosition[1] + screenHeight / 2 - textureSize.y / 2);
		mainWindow.draw(*myObj.getSprite());
		mainWindow.draw(*player.getSprite());
		mainWindow.display();

		Sleep(16);

		//mainWindow.setActive();

		mainWindow.display();
	}

	return 0;
}

float* relativePosition(float primaryX, float primaryY, float secondaryX, float secondaryY) {
	float relPos[2] = { secondaryX - primaryX, secondaryY - primaryY };
	return relPos;
}