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
#include "LightSource.h"

float* relativePosition(float primaryX, float primaryY, float secondaryX, float secondaryY);

int PLAYER_SPEED = 1000; //coord per second

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
	player.setBoundBox(100, 100);
	player.setLightLevel(400); //level that light reaches

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
	lamp->setTextureSize(50);
	lamp->setBoundBox(50);


	sf::Sprite lightSprite(lightTexture);
	lightSprite.setPosition(screenWidth / 2, screenHeight / 2);

	Area area;
	area.setBackground(backgroundTexture);
	area.addObject(myObj);
	area.addObject(myObj2);
	area.addObject(lamp);

	area.setSize(5000, 5000);

	clock_t t = clock();

	//main loop
	while (mainWindow.isOpen()) {
		float ms = (clock() - t)*1000/CLOCKS_PER_SEC;
		t = clock();
		sf::Event event;
		
		while (mainWindow.pollEvent(event)) {

			if (event.type == sf::Event::MouseWheelMoved) {
				FOV += -(float)FOV*(float)event.mouseWheel.delta/20;
			}
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

		sf::Sprite* background = area.getBackground();
		sf::Vector2u backgroundSize = background->getTexture()->getSize();
		float playerPos[2] = { *player.getPosition(), *(player.getPosition() + 1) };

		float errorX = (playerPos[0] - (float)FOV / 2) / area.getWidth() * (float)backgroundSize.x - 1 - (int)((playerPos[0] - (float)FOV / 2) / area.getWidth() * (float)backgroundSize.x - 1);
		float errorY = (playerPos[1] - (float)FOV / 2) / area.getHeight() * (float)backgroundSize.y - 1 - (int)((playerPos[1] - (float)FOV / 2) / area.getHeight() * (float)backgroundSize.y - 1);
		background->setTextureRect(sf::IntRect((playerPos[0] - (float)FOV/2)/area.getWidth()* (float)backgroundSize.x - 1, (playerPos[1] - (float)FOV/2)/area.getHeight() * (float)backgroundSize.y - 1, (float)FOV / area.getWidth() * (float)backgroundSize.x + 2, (float)FOV / area.getHeight() * (float)backgroundSize.y + 2));
		background->setScale((float)screenWidth/((float)background->getTextureRect().width - 2), (float)screenHeight/((float)background->getTextureRect().height - 2));

		float backgroundX = (-errorX - 1) * ((float)screenWidth / (float)background->getTextureRect().width);
		float backgroundY = (-errorY - 1) * ((float)screenHeight / (float)background->getTextureRect().height);
		background->setPosition(backgroundX, backgroundY);
		mainWindow.draw(*area.getBackground());
		float backgroundScreenEdgeLeft = -playerPos[0] / (float)FOV * (float)screenWidth + (float)screenWidth / 2;
		float backgroundScreenEdgeRight = (area.getWidth() - playerPos[0]) / (float)FOV * (float)screenWidth + (float)screenWidth / 2;
		float backgroundScreenEdgeTop = -playerPos[1] / (float)FOV * (float)screenHeight + (float)screenHeight / 2;
		float backgroundScreenEdgeBottom = (area.getHeight() - playerPos[1]) / (float)FOV * (float)screenHeight + (float)screenHeight / 2;

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
		
		//draw objects
		for (int i = 0; i < objects->size(); i++) {
			GameObject currentObj = *objects->at(i);
			float objectPos[2] = { *currentObj.getPosition(), *(currentObj.getPosition() + 1) };
			float* temp = relativePosition(playerPos[0], playerPos[1], objectPos[0], objectPos[1]);
			float relativePosition[2] = { *temp, *(temp + 1) };
			currentObj.setScreenPosition((relativePosition[0] - currentObj.getTextureWidth()/2)/(float)FOV* (float)screenWidth + (float)screenWidth / 2, (relativePosition[1] - currentObj.getTextureHeight() / 2)/(float)FOV* (float)screenHeight + (float)screenHeight / 2);
			sf::Sprite currentSprite = *(currentObj.getSprite());
			currentSprite.setScale(currentObj.getTextureWidth() / (float)FOV * (float)screenWidth / currentObj.getTexture()->getSize().x, currentObj.getTextureHeight() / (float)FOV * (float)screenHeight / currentObj.getTexture()->getSize().y);
			mainWindow.draw(currentSprite);
			if (currentObj.getType().compare("lightSource")) {
				
			}
		}
		
		player.setScreenPosition((float)screenWidth/2 - (player.getWidth()/(float)FOV*(float)screenWidth)/2, (float)screenHeight / 2 - (player.getHeight() / (float)FOV * (float)screenHeight) / 2);
		player.getSprite()->setScale((float)screenWidth / (float)FOV, (float)screenHeight / (float)FOV);
		mainWindow.draw(*player.getSprite());


		sf::FloatRect lightBox((float)screenWidth / 2 - (player.getLightLevel() * 2 / (float)FOV * (float)screenWidth) / 2, 
								(float)screenHeight / 2 - (player.getLightLevel() * 2 / (float)FOV * (float)screenHeight) / 2, 
								player.getLightLevel() * 2 / (float)FOV * (float)screenWidth, 
								player.getLightLevel() * 2 / (float)FOV * (float)screenHeight);

		sf::RectangleShape blackRect;
		blackRect.setFillColor(sf::Color(0, 0, 0, 220));
		blackRect.setSize(sf::Vector2f(screenWidth, lightBox.top));
		blackRect.setPosition(0, 0);
		mainWindow.draw(blackRect);

		blackRect.setSize(sf::Vector2f(screenWidth, screenHeight - (lightBox.top + lightBox.height)));
		blackRect.setPosition(0, lightBox.top + lightBox.height);
		mainWindow.draw(blackRect);

		blackRect.setSize(sf::Vector2f(lightBox.left, lightBox.height));
		blackRect.setPosition(0, lightBox.top);
		mainWindow.draw(blackRect);

		blackRect.setSize(sf::Vector2f(screenWidth - (lightBox.left + lightBox.width), lightBox.height));
		blackRect.setPosition((lightBox.left + lightBox.width), lightBox.top);
		mainWindow.draw(blackRect);

		sf::Vector2u lightSpriteSize = lightSprite.getTexture()->getSize();
		lightSprite.setPosition(lightBox.left, lightBox.top);
		lightSprite.setScale( lightBox.width / lightSpriteSize.x,  lightBox.height / lightSpriteSize.y);

		lightSprite.setColor(sf::Color(255, 255, 255, 220));
		mainWindow.draw(lightSprite);

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