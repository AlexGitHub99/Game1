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
#include "Orb.h"
#include "Path.h"
#include "CollisionChecker.h"
#include "TextBox.h"
#include "Entity.h"
#include <any>

using namespace std;
int PLAYER_SPEED = 1000; //coord per second
int CAMERA_SPEED = 3;
float menuSize = 1;

const int lightMapWidth = 40;
const int lightMapHeight = 40;

float* relativePosition(float primaryX, float primaryY, float secondaryX, float secondaryY);
void setSquare(sf::VertexArray* lightPoints, int x, int y, int alpha, int lightMapWidth, int lightMapHeight);
void setPoints(sf::VertexArray* lightPoints, int lightMap[][lightMapHeight + 1], float baseLevel);
void getDesktopResolution(int& horizontal, int& vertical);
void renderBackground(Area& area, int screenSize[2], float cameraPos[2], int FOV[2]);
void setPlayerScreenPos(Player& player, float cameraGap[2], int screenSize[2], int FOV[2]);
void renderObject(shared_ptr<GameObject>, int screenSize[2], float cameraPos[2], int FOV[2]);
bool isInside(float x, float y, sf::FloatRect& bounds);

//load textures
shared_ptr<sf::Texture> rockTexture(new sf::Texture());
shared_ptr<sf::Texture> wallTexture(new sf::Texture());
shared_ptr<sf::Texture> playerTexture(new sf::Texture());
shared_ptr<sf::Texture> backgroundTexture(new sf::Texture());
shared_ptr<sf::Texture> lampTexture(new sf::Texture());
shared_ptr<sf::Texture> orbTexture(new sf::Texture());

int main() {
	int screenSize[2];
	getDesktopResolution(screenSize[0], screenSize[1]);
	float FOVRatio = 16 / 10;
	int FOV[2];
	FOV[0] = 2000;
	FOV[1] = (int)round((float)FOV[0] / FOVRatio);

	if ((float)screenSize[0] / (float)screenSize[1] > (float)FOV[0] / (float)FOV[1]) {
		FOVRatio = (float)screenSize[0] / (float)screenSize[1];
		FOV[1] = (int)round((float)FOV[0] / FOVRatio);
	}
	else if ((float)screenSize[0] / (float)screenSize[1] < (float)FOV[0] / (float)FOV[1]) {
		FOVRatio = ((float)screenSize[0] / (float)screenSize[1]);
		FOV[0] = (int)round((float)FOV[1] * FOVRatio);
	}

	//load textures
	if (!rockTexture->loadFromFile("resources/rock_1.png")) {
		return -1;
	}
	if (!wallTexture->loadFromFile("resources/wall.png")) {
		return -1;
	}
	if (!playerTexture->loadFromFile("resources/player.png")) {
		return -1;
	}
	if (!backgroundTexture->loadFromFile("resources/area_1.png")) {
		return -1;
	}
	if (!lampTexture->loadFromFile("resources/lamp.png")) {
		return -1;
	}
	if (!orbTexture->loadFromFile("resources/orb.png")) {
		return -1;
	}

	struct MenuObject {
		virtual shared_ptr<GameObject> createObject() {
			return nullptr;
		};
		string type;
		shared_ptr<sf::Sprite> sprite;
	};

	struct Lamp : public MenuObject {
		Lamp() {
			texture = lampTexture;
			width = 50;
			lightLevel = 100;
			box[0] = 50;
			box[1] = 50;
			type = "Lamp";
			sprite = make_shared<sf::Sprite>(sf::Sprite(*texture));
		};

		shared_ptr<GameObject> createObject() override {
			shared_ptr<LightSource> obj = make_shared<LightSource>(LightSource(texture, width, box[0], box[1], lightLevel));
			obj->setBoundBoxOffsetToBottom();
			return obj;
		}

		shared_ptr<sf::Texture> texture;
		float width;
		float lightLevel;
		float box[2];
	};

	struct Wall : public MenuObject {
		Wall() {
			texture = wallTexture;
			width = 100;
			box[0] = 100;
			box[1] = 100;
			type = "Wall";
			sprite = make_shared<sf::Sprite>(sf::Sprite(*texture));
		};

		shared_ptr<GameObject> createObject() override {
			shared_ptr<GameObject> obj = make_shared<GameObject>(GameObject(texture, width, box[0], box[1]));
			obj->setBoundBoxOffsetToBottom();
			return obj;
		}

		shared_ptr<sf::Texture> texture;
		float width;
		float box[2];
	};



	if (true) {
		float BUILD_CAM_SPEED = 1500;
		sf::RenderWindow buildWindow(sf::VideoMode(screenSize[0], screenSize[1]), "Builder", sf::Style::Fullscreen);
		Area area(backgroundTexture, 4000, 4000);
		float cameraPos[2] = { 0, 0 };

		bool menuOpen = false;
		bool initializeMenu = false;
		bool blink = false;

		clock_t t = clock();
		clock_t msTimer = t;

		list<shared_ptr<TextBox>> textBoxes;
		shared_ptr<TextBox> activeBox;

		shared_ptr<Lamp> lamp = make_shared<Lamp>(Lamp());
		shared_ptr<Wall> wall = make_shared<Wall>(Wall());
		vector<shared_ptr<MenuObject>> menuObjects;
		menuObjects.push_back(lamp);
		menuObjects.push_back(wall);

		while(buildWindow.isOpen()) {
			float ms = (clock() - t) * 1000 / CLOCKS_PER_SEC;
			t = clock();

			sf::Event event;
			while (buildWindow.pollEvent(event)) {

				if (activeBox != nullptr) {
					
					switch(event.type) {
					case (sf::Event::TextEntered): {
						sf::String rawInput = event.text.unicode;
						if (rawInput.getSize() > 1) {
							break;
						}
						char chr = *(rawInput.getData());
						if ((chr - 48 >= 0 and chr - 48 <= 9) or chr == '.') {
							activeBox->addText(rawInput.toAnsiString());
						}
						else if (chr == 8) {
							activeBox->backspace();
						}
						break;
					}
					case (sf::Event::KeyReleased):
						if (event.key.code == sf::Keyboard::Enter) {
							BUILD_CAM_SPEED = stof(activeBox->getText());
							activeBox->setText("");
							activeBox.reset();
							
						}
					}
				} 
				else { //dissalowed events while textbox is active
					switch (event.type) {

					case (sf::Event::KeyReleased):

						switch (event.key.code) {

						case (sf::Keyboard::Escape):
							buildWindow.close();
							break;

						case (sf::Keyboard::M):
							menuOpen = !menuOpen;
							if (menuOpen) {
								initializeMenu = true;
							}
							break;

						}
						break;

					}
				}
				
				//allowed events while textbox is active
				switch (event.type) {

				case (sf::Event::MouseWheelMoved):
					FOV[0] += -(float)FOV[0] * (float)event.mouseWheel.delta / 20;
					FOV[1] = (float)FOV[0] / FOVRatio;
					break;

				case (sf::Event::KeyReleased):

					switch (event.key.code) {

					case (sf::Keyboard::Escape):
						buildWindow.close();
						break;
					}
					break;

				case (sf::Event::MouseButtonReleased):
					switch (event.mouseButton.button) {

					case (sf::Mouse::Left):
						if (textBoxes.empty()) {
							break;
						}

						bool inTextBox = false;
						for (list<shared_ptr<TextBox>>::iterator it = textBoxes.begin(); it != textBoxes.end(); it++) {
							shared_ptr<TextBox> current = *it;

							if (isInside(event.mouseButton.x, event.mouseButton.y, *current->getBox())) {
								activeBox = current;
								inTextBox = true;
							}
						}

						if (!inTextBox) {
							activeBox.reset();
						}
						break;
					}
					break;

				case (sf::Event::Closed):
					buildWindow.close();
					break;
				}
				
			}

			shared_ptr<list<shared_ptr<GameObject>>> objects = area.getObjects();
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
				float magnitude = ms / 1000 * BUILD_CAM_SPEED;
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

				cameraPos[0] += cos(angle) * magnitude;
				cameraPos[1] += sin(angle) * -magnitude;

			}

			//------------------------------------------------------
			//DRAWING
			//------------------------------------------------------
			buildWindow.clear();

			//draw background
			renderBackground(area, screenSize, cameraPos, FOV);

			buildWindow.draw(*area.getBackground());

			//draw black area outside of background border
			float backgroundScreenEdgeLeft = -cameraPos[0] / (float)FOV[0] * (float)screenSize[0] + (float)screenSize[0] / 2;
			float backgroundScreenEdgeRight = (area.getWidth() - cameraPos[0]) / (float)FOV[0] * (float)screenSize[0] + (float)screenSize[0] / 2;
			float backgroundScreenEdgeTop = -cameraPos[1] / (float)FOV[1] * (float)screenSize[1] + (float)screenSize[1] / 2;
			float backgroundScreenEdgeBottom = (area.getHeight() - cameraPos[1]) / (float)FOV[1] * (float)screenSize[1] + (float)screenSize[1] / 2;

			if (backgroundScreenEdgeLeft > 0) {
				sf::RectangleShape blackRect(sf::Vector2f(backgroundScreenEdgeLeft, screenSize[1]));
				blackRect.setPosition(0, 0);
				blackRect.setFillColor(sf::Color(0, 0, 0, 255));
				buildWindow.draw(blackRect);
			}
			if (backgroundScreenEdgeRight < screenSize[0]) {
				sf::RectangleShape blackRect(sf::Vector2f(backgroundScreenEdgeRight, screenSize[1]));
				blackRect.setPosition(backgroundScreenEdgeRight, 0);
				blackRect.setFillColor(sf::Color(0, 0, 0, 255));
				buildWindow.draw(blackRect);
			}
			if (backgroundScreenEdgeTop > 0) {
				sf::RectangleShape blackRect(sf::Vector2f(screenSize[0], backgroundScreenEdgeTop));
				blackRect.setPosition(0, 0);
				blackRect.setFillColor(sf::Color(0, 0, 0, 255));
				buildWindow.draw(blackRect);
			}
			if (backgroundScreenEdgeBottom < screenSize[1]) {
				sf::RectangleShape blackRect(sf::Vector2f(screenSize[0], backgroundScreenEdgeBottom));
				blackRect.setPosition(0, backgroundScreenEdgeBottom);
				blackRect.setFillColor(sf::Color(0, 0, 0, 255));
				buildWindow.draw(blackRect);
			}

			//draw objects and player
			for (std::list<shared_ptr<GameObject>>::iterator it = objects->begin(); it != objects->end(); it++) {
				shared_ptr<GameObject> currentObj = *it;
				renderObject(currentObj, screenSize, cameraPos, FOV);

				buildWindow.draw(*currentObj->getSprite());

			}

			//draw coords
			sf::Font courier;
			if (!courier.loadFromFile("resources/fonts/CourierPrime-Regular.ttf")) {
				return -1;
			}
			sf::Text cameraCoords;
			cameraCoords.setFont(courier);
			cameraCoords.setString("Camera X: " + std::to_string(cameraPos[0]) + " Y : " + std::to_string(cameraPos[1]));

			cameraCoords.setPosition(0, 0);

			buildWindow.draw(cameraCoords);

			

			//draw menu
			if (menuOpen) {
				sf::FloatRect menuRect(screenSize[0] * 3 / 4, 0, screenSize[0] / 4, screenSize[1]);

				if (initializeMenu) {
					textBoxes.clear();
					shared_ptr<sf::FloatRect> textBoxRect = make_shared<sf::FloatRect>(sf::FloatRect(menuRect.left + 30, menuRect.top + 80, 100, 50));
					textBoxes.push_back(make_shared<TextBox>(TextBox("", textBoxRect)));
				}

				
				sf::RectangleShape menuBack(sf::Vector2f(menuRect.width, menuRect.height));
				menuBack.setPosition(menuRect.left, menuRect.top);
				menuBack.setFillColor(sf::Color(174, 174, 174));
				buildWindow.draw(menuBack);

				float currentY = menuRect.top + 30;
				float currentX = menuRect.left + 30;

				sf::Text newText;
				newText.setFont(courier);
				newText.setString("Camera speed:");
				newText.setPosition(currentX, currentY);
				newText.setFillColor(sf::Color(0, 0, 0));
				buildWindow.draw(newText);

				currentY += 120;
				
				newText.setString("Objects:");
				newText.setPosition(currentX, currentY);
				buildWindow.draw(newText);

				currentY += 50;
				
				//draw item menu
				sf::FloatRect itemBackRect(currentX, currentY, menuRect.width - (currentX - menuRect.left) * 2, menuRect.height / 2);
				sf::RectangleShape itemBack(sf::Vector2f(itemBackRect.width, itemBackRect.height));
				itemBack.setPosition(itemBackRect.left, itemBackRect.top);
				itemBack.setFillColor(sf::Color(0, 0, 0, 0));
				itemBack.setOutlineColor(sf::Color(100, 100, 100));
				itemBack.setOutlineThickness(3);
				buildWindow.draw(itemBack);

				float positionIterator[2];
				positionIterator[0] = itemBackRect.left + itemBackRect.width * 8 / 30;
				positionIterator[1] = itemBackRect.top + itemBackRect.height * 7 / 50;

				for (vector<shared_ptr<MenuObject>>::iterator it = menuObjects.begin(); it != menuObjects.end(); it++) {
					float size[2];

					bool exists = false;
					if ((*it)->type.compare("Lamp") == 0) {
						shared_ptr<Lamp> obj = static_pointer_cast<Lamp>(*it);
						size[0] = obj->texture->getSize().x;
						size[1] = obj->texture->getSize().y;
						exists = true;
					}
					else if ((*it)->type.compare("Wall") == 0) {
						shared_ptr<Wall> obj = static_pointer_cast<Wall>(*it);
						size[0] = obj->texture->getSize().x;
						size[1] = obj->texture->getSize().y;
						exists = true;
					}

					if (exists) {
						shared_ptr<sf::Sprite> sprite = (*it)->sprite;
						sprite->setOrigin(size[0] / 2, size[1] / 2);
						sprite->setScale(itemBackRect.height / 5 / size[1], itemBackRect.height / 5 / size[1]);
						sprite->setPosition(positionIterator[0], positionIterator[1]);
						buildWindow.draw(*sprite);
					}
					positionIterator[0] += itemBackRect.width * 14 / 30;
					if (positionIterator[0] > itemBackRect.left + itemBackRect.width) {
						positionIterator[0] = itemBackRect.left + itemBackRect.width * 8 / 30;
						positionIterator[1] += itemBackRect.height * 12 / 50;
					}
				}

				for (list<shared_ptr<TextBox>>::iterator it = textBoxes.begin(); it != textBoxes.end(); it++) {
					shared_ptr<TextBox> current = *it;

					sf::FloatRect textBoxRect = *current->getBox();
					sf::RectangleShape textBox(sf::Vector2f(textBoxRect.width, textBoxRect.height));
					textBox.setPosition(textBoxRect.left, textBoxRect.top);
					textBox.setOutlineThickness(2);
					textBox.setOutlineColor(sf::Color(100, 100, 100));
					textBox.setFillColor(sf::Color(220, 220, 220));

					buildWindow.draw(textBox);

					sf::Text boxText;
					boxText.setString(current->getText());
					boxText.setFont(courier);
					boxText.setPosition(current->getBox()->left, current->getBox()->top);
					boxText.setFillColor(sf::Color(0, 0, 0));

					buildWindow.draw(boxText);
				}

				
				if (activeBox != nullptr) {
					shared_ptr<sf::FloatRect> activeRect = activeBox->getBox();
					sf::RectangleShape textBox(sf::Vector2f(activeRect->width, activeRect->height));
					textBox.setPosition(activeRect->left, activeRect->top);
					textBox.setOutlineColor(sf::Color(0, 0, 0));
					textBox.setOutlineThickness(2);
					if (blink) {
						textBox.setFillColor(sf::Color(220, 220, 220));
					}
					else {
						textBox.setFillColor(sf::Color(245, 245, 245));
					}

					buildWindow.draw(textBox);

					sf::Text enteredText;
					enteredText.setString(activeBox->getText());
					enteredText.setFont(courier);
					enteredText.setPosition(activeBox->getBox()->left, activeBox->getBox()->top);
					enteredText.setFillColor(sf::Color(0, 0, 0));

					buildWindow.draw(enteredText);
				}

				if (initializeMenu) {
					initializeMenu = false;
				}

				if ((clock() - msTimer) / (CLOCKS_PER_SEC / 1000) >= 300) {
					blink = !blink;
					msTimer = clock();
				}
			}






			//display
			buildWindow.display();

			//------------------------------------------------------
			//END OF DRAWING
			//------------------------------------------------------

		}

		return 0;
	}
	









































	//[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
	//[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
	// 
	//                                                                                    Actuall Game Play
	// 
	//[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
	//[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]

	sf::RenderWindow mainWindow(sf::VideoMode(screenSize[0], screenSize[1]), "Game", sf::Style::Fullscreen);

	int baseLightLevel = 10;
	float spawnPoint[2] = { 200, 200 };

	Player player(playerTexture, 70, 70, 40, 100);
	player.setPosition(1000, 1000);
	player.setBoundBoxOffset(0, player.getTextureHeight() / 2 - player.getBoundBoxHeight() / 2);
	player.setLightLevel(600); //level that light reaches
	

	//temporary stuff-----------------------------------------------------------
	shared_ptr<Orb> orb = make_shared<Orb>(Orb(orbTexture, 100, 100, 100));
	orb->setPosition(0, 0);

	shared_ptr<GameObject> rock1 = make_shared<GameObject> (GameObject(rockTexture, 100, 100, 60));
	rock1->setPosition(250, 250);

	shared_ptr<GameObject> rock2 = make_shared<GameObject> (GameObject(rockTexture, 300, 300, 200));
	rock2->setPosition(1000, 400);

	shared_ptr<GameObject> myWall = make_shared<GameObject> (GameObject(wallTexture, 100, 100, 100));
	myWall->setPosition(700, 900);
	myWall->setBoundBoxOffsetToBottom();

	shared_ptr<LightSource> myLamp = make_shared<LightSource> (LightSource(lampTexture, 50, 50, 30, 500));
	myLamp->setPosition(900, 800);

	Area area(backgroundTexture, 4000, 4000);
	area.addObject(rock1);
	area.addObject(rock2);
	area.addObject(myLamp);
	area.addObject(myWall);
	area.addEntity(orb);


	//for (int i = 0; i < 12; i++) {
	//	GameObject* wall2 = new GameObject();
	//	wall2->setTexture(wallTexture);
	//	wall2->setPosition(500 + i*94, 800);
	//	wall2->setBoundBox(100, 100);
	//	wall2->setTextureSize(100);
	//	wall2->setBoundBoxOffset(0, wall2->getTextureHeight() / 2 - wall2->getBoundBoxHeight() / 2);
	//	area.addObject(wall2);
	//}

	//for (int i = 0; i < 20; i++) {
	//	GameObject* wall2 = new GameObject();
	//	wall2->setTexture(wallTexture);
	//	wall2->setPosition(500, 800 + i*80);
	//	wall2->setBoundBox(100, 80);
	//	wall2->setTextureSize(100);
	//	wall2->setBoundBoxOffset(0, wall2->getTextureHeight() / 2 - wall2->getBoundBoxHeight() / 2);
	//	area.addObject(wall2);
	//}

	//end of temporary stuff--------------------------------------------

	int lightMap[lightMapWidth + 1][lightMapHeight + 1]; //extra width because of edge points

	sf::VertexArray* lightPoints = new sf::VertexArray(sf::Quads, lightMapWidth* lightMapHeight * 4);

	float squareWidth = (float)screenSize[0] / (float)lightMapWidth;
	float squareHeight = (float)screenSize[1] / (float)lightMapHeight;

	//fill vertex array with points
	for (int x = 0; x < lightMapWidth; x++) {
		for (int y = 0; y < lightMapHeight; y++) {

			(*lightPoints)[y * lightMapWidth * 4 + x * 4].position = sf::Vector2f((float)x * squareWidth, (float)y * squareHeight);
			(*lightPoints)[y * lightMapWidth * 4 + x * 4 + 1].position = sf::Vector2f((float)(x + 1) * squareWidth, (float)y * squareHeight);
			(*lightPoints)[y * lightMapWidth * 4 + x * 4 + 2].position = sf::Vector2f((float)(x + 1) * squareWidth, (float)(y + 1) * squareHeight);
			(*lightPoints)[y * lightMapWidth * 4 + x * 4 + 3].position = sf::Vector2f((float)x * squareWidth, (float)(y + 1) * squareHeight);
		}
	}

	float cameraSpeed = 0;
	float cameraPos[2] = { player.getX(), player.getY() };
	float prevHealth = player.getHealth();
	
	CollisionChecker collisionChecker;

	clock_t t = clock();
	clock_t tSecond = t;
	int frames = 0;
	int fps = 0;

	//main loop
	while (mainWindow.isOpen()) {
		float ms = (clock() - t)*1000/CLOCKS_PER_SEC;
		t = clock();
		sf::Event event;
		
		bool secondPassed = (clock() - tSecond) / CLOCKS_PER_SEC >= 1;
		frames++;
		if (secondPassed) {//1 second has passed
			tSecond = clock();
			fps = frames;
			frames = 0;
		}

		while (mainWindow.pollEvent(event)) {

			if (event.type == sf::Event::MouseWheelMoved) {
				FOV[0] += -(float)FOV[0]*(float)event.mouseWheel.delta/20;
				FOV[1] = (float)FOV[0] / FOVRatio;
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

		shared_ptr<list<shared_ptr<GameObject>>> objects = area.getObjects();
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

			if (cos(angle) * magnitude < 0) {
				player.setFaceLeft(true);
			}
			else {
				player.setFaceLeft(false);
			}
			player.movePosition(cos(angle) * magnitude, sin(angle) * -magnitude);

		}

		//collissions

		//object collisions

		sf::FloatRect playerBox;
		playerBox.left = player.getX() - player.getBoundBoxWidth() / 2 + player.getBoundBoxOffsetX();
		playerBox.top = player.getY() - player.getBoundBoxHeight() / 2 + player.getBoundBoxOffsetY();
		playerBox.width = player.getBoundBoxWidth();
		playerBox.height = player.getBoundBoxHeight();

		for (std::list<shared_ptr<GameObject>>::iterator it = objects->begin(); it != objects->end(); it++) {
			shared_ptr<GameObject> currentObj = *it;
			
			shared_ptr<Entity> entity = dynamic_pointer_cast<Entity>(currentObj);
			if (entity != nullptr) {
				if (!entity->doesPushPlayer()) {
					continue;
				}
			}

			sf::FloatRect objectBox;
			objectBox.left = currentObj->getX() - currentObj->getBoundBoxWidth() / 2 + currentObj->getBoundBoxOffsetX();
			objectBox.top = currentObj->getY() - currentObj->getBoundBoxHeight() / 2 + currentObj->getBoundBoxOffsetY();
			objectBox.width = currentObj->getBoundBoxWidth();
			objectBox.height = currentObj->getBoundBoxHeight();

			float* displacement = collisionChecker.check(playerBox, objectBox);
			float xDisplacement = *displacement;
			float yDisplacement = *(displacement + 1);

			if (xDisplacement != 0 and yDisplacement != 0) {
				if (abs(xDisplacement) < abs(yDisplacement)) {
					player.movePosition(xDisplacement, 0.0);
				}
				else {
					player.movePosition(0.0, yDisplacement);
				}
			}

			std::free(displacement);
		}

		//border collisions
		if (player.getX() - player.getBoundBoxWidth() / 2 < 0) {
			player.movePosition(0 - (player.getX() - player.getBoundBoxWidth() / 2), 0.0);
		}
		if (player.getX() + player.getBoundBoxWidth() / 2 > area.getWidth()) {
			player.movePosition(area.getWidth() - (player.getX() + player.getBoundBoxWidth() / 2), 0.0);
		}
		if (player.getY() - player.getBoundBoxHeight() / 2 < 0) {
			player.movePosition(0.0, 0 - (player.getY() - player.getBoundBoxHeight() / 2));
		}
		if (player.getY() + player.getBoundBoxHeight() / 2 > area.getHeight()) {
			player.movePosition(0.0, area.getHeight() - (player.getY() + player.getBoundBoxHeight() / 2));
		}


		//move camera
		float playerPos[2] = { player.getX(), player.getY() };
		float* cameraGapTemp = relativePosition(cameraPos[0], cameraPos[1], playerPos[0], playerPos[1]);
		float cameraGap[2] = { *cameraGapTemp, *(cameraGapTemp + 1) };
		std::free(cameraGapTemp);

		if (abs(cameraGap[0]) < 0.5) {
			cameraPos[0] = playerPos[0];
		}
		else {
			cameraPos[0] += cameraGap[0] - cameraGap[0] * pow(3, -ms/100);
		}
		if (abs(cameraGap[1]) < 0.5) {
			cameraPos[1] = playerPos[1];
		}
		else {
			cameraPos[1] += cameraGap[1] - cameraGap[1] * pow(3, -ms/100);
		}


		//update entities
		std::shared_ptr<std::vector<std::shared_ptr<Entity>>> entities = area.getEntities();
		for (int i = 0; i < entities->size(); i++) {

			entities->at(i)->update(area, player, ms);
			if (player.getHealth() < prevHealth) {
				player.getSprite()->setColor(sf::Color(255, 0, 0, 255));
			}
			else {
				player.getSprite()->setColor(sf::Color(255, 255, 255, 255));
			}
			prevHealth = player.getHealth();
			if (player.getHealth() <= 0) {
				player.setPosition(spawnPoint[0], spawnPoint[1]);
				player.setHealth(100);
			}

			/*if (secondPassed) {
				Path path;
				path.find(area, *entities->at(i), player);
			}*/
		}
		

		//------------------------------------------------------
		//DRAWING
		//------------------------------------------------------
		mainWindow.clear();

		//draw background
		renderBackground(area, screenSize, cameraPos, FOV);

		mainWindow.draw(*area.getBackground());

		//draw black area outside of background border
		float backgroundScreenEdgeLeft = -cameraPos[0] / (float)FOV[0] * (float)screenSize[0] + (float)screenSize[0] / 2;
		float backgroundScreenEdgeRight = (area.getWidth() - cameraPos[0]) / (float)FOV[0] * (float)screenSize[0] + (float)screenSize[0] / 2;
		float backgroundScreenEdgeTop = -cameraPos[1] / (float)FOV[1] * (float)screenSize[1] + (float)screenSize[1] / 2;
		float backgroundScreenEdgeBottom = (area.getHeight() - cameraPos[1]) / (float)FOV[1] * (float)screenSize[1] + (float)screenSize[1] / 2;

		if (backgroundScreenEdgeLeft > 0) {
			sf::RectangleShape blackRect(sf::Vector2f(backgroundScreenEdgeLeft, screenSize[1]));
			blackRect.setPosition(0, 0);
			blackRect.setFillColor(sf::Color(0, 0, 0, 255));
			mainWindow.draw(blackRect);
		}
		if (backgroundScreenEdgeRight < screenSize[0]) {
			sf::RectangleShape blackRect(sf::Vector2f(backgroundScreenEdgeRight, screenSize[1]));
			blackRect.setPosition(backgroundScreenEdgeRight, 0);
			blackRect.setFillColor(sf::Color(0, 0, 0, 255));
			mainWindow.draw(blackRect);
		}
		if (backgroundScreenEdgeTop > 0) {
			sf::RectangleShape blackRect(sf::Vector2f(screenSize[0], backgroundScreenEdgeTop));
			blackRect.setPosition(0, 0);
			blackRect.setFillColor(sf::Color(0, 0, 0, 255));
			mainWindow.draw(blackRect);
		}
		if (backgroundScreenEdgeBottom < screenSize[1]) {
			sf::RectangleShape blackRect(sf::Vector2f(screenSize[0], backgroundScreenEdgeBottom));
			blackRect.setPosition(0, backgroundScreenEdgeBottom);
			blackRect.setFillColor(sf::Color(0, 0, 0, 255));
			mainWindow.draw(blackRect);
		}

		//Create (empty) list of light sources
		std::vector<std::pair<sf::Vector2f, float>> sourcePositions;

		//set player screen position
		float* newCameraGapTemp = relativePosition(cameraPos[0], cameraPos[1], playerPos[0], playerPos[1]);
		float newCameraGap[2] = { *newCameraGapTemp, *(newCameraGapTemp + 1) };
		std::free(newCameraGapTemp);
		setPlayerScreenPos(player, newCameraGap, screenSize, FOV);

		//draw objects and player
		bool hasDrawnPlayer = false;
		for (std::list<shared_ptr<GameObject>>::iterator it = objects->begin(); it != objects->end(); it++) {
			shared_ptr<GameObject> currentObj = *it;
			renderObject(currentObj, screenSize, cameraPos, FOV);

			if (hasDrawnPlayer == false and
				currentObj->getSprite()->getPosition().y + currentObj->getRenderHeight() / 2 > player.getSprite()->getPosition().y + player.getRenderHeight() / 2) { //draw player if current object is in front of player
				mainWindow.draw(*player.getSprite());
				hasDrawnPlayer = true;
			}

			mainWindow.draw(*currentObj->getSprite());
			
			if (currentObj->getType().compare("lightSource") == 0) {
				float objectLightDist = currentObj->getLightLevel() / (float)FOV[0] * (float)screenSize[0];
				sourcePositions.push_back(std::pair<sf::Vector2f, float>(currentObj->getSprite()->getPosition(), objectLightDist));
			}
		}
		if (!hasDrawnPlayer) {
			mainWindow.draw(*player.getSprite());
		}

		//Add player to light source list
		float playerLightDist = player.getLightLevel() / (float)FOV[0] * (float)screenSize[0];
		sf::Vector2f playerScreenPos((float)screenSize[0] / 2 + newCameraGap[0] / (float)FOV[0] * (float)screenSize[0],
									(float)screenSize[1] / 2 + newCameraGap[1] / (float)FOV[1] * (float)screenSize[1]);
		std::pair<sf::Vector2f, float> lSource(playerScreenPos, playerLightDist);
		sourcePositions.push_back(lSource);

		//create lightMap
		float xRatio = (float)screenSize[0] / (float)lightMapWidth;
		float yRatio = (float)screenSize[1] / (float)lightMapHeight;
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
				if (alpha > 255) {
					alpha = 255;
				}
				alpha = 255 - alpha;
				lightMap[x][y] = alpha;
			}
		}

		//draw lightMap
		setPoints(lightPoints, lightMap, baseLightLevel);
		mainWindow.draw(*lightPoints);

		//draw menu
		
		//draw health bar

		float thickness = 5 * menuSize;
		float healthBarSize[2] = {screenSize[0] / 3 * menuSize, 50 * menuSize};
		float healthBarPos[2] = {screenSize[0] / 2 - healthBarSize[0] / 2, screenSize[1] - healthBarSize[1] * 2};
		sf::RectangleShape healthBar(sf::Vector2f(healthBarSize[0], healthBarSize[1]));
		healthBar.setFillColor(sf::Color(0, 0, 0, 0));
		healthBar.setOutlineColor(sf::Color(100, 100, 100, 255));
		healthBar.setOutlineThickness(-thickness);
		healthBar.setPosition(healthBarPos[0], healthBarPos[1]);
		sf::RectangleShape health(sf::Vector2f((healthBarSize[0] - thickness * 2) * (player.getHealth() / 100), healthBarSize[1] - thickness * 2));
		health.setFillColor(sf::Color(255, 59, 59, 255));
		health.setPosition(healthBarPos[0] + thickness, healthBarPos[1] + thickness);
		mainWindow.draw(healthBar);
		mainWindow.draw(health);

		//draw coords
		sf::Font courier;
		if (!courier.loadFromFile("resources/fonts/CourierPrime-Regular.ttf")) {
			return -1;
		}
		sf::Text playerCoords;
		sf::Text cameraCoords;
		playerCoords.setFont(courier);
		cameraCoords.setFont(courier);
		playerCoords.setString("Player X: " + std::to_string(playerPos[0]) + " Y: " + std::to_string(playerPos[1]));
		cameraCoords.setString("Camera X: " + std::to_string(cameraPos[0]) + " Y : " + std::to_string(cameraPos[1]));
		
		playerCoords.setPosition(0, 0);
		cameraCoords.setPosition(0, 40);

		mainWindow.draw(playerCoords);
		mainWindow.draw(cameraCoords);

		//draw FPS
		sf::Text framesText;
		framesText.setFont(courier);
		framesText.setPosition(0, 80);

		framesText.setString("FPS: " + std::to_string(fps));
		mainWindow.draw(framesText);

		mainWindow.display();

		//------------------------------------------------------
		//END OF DRAWING
		//------------------------------------------------------
	}

	return 0;
}

float* relativePosition(float primaryX, float primaryY, float secondaryX, float secondaryY) {
	float* relPos = new float[2];
	relPos[0] = secondaryX - primaryX;
	relPos[1] = secondaryY - primaryY;
	return relPos;
}

//old method
void setSquare(sf::VertexArray* lightPoints, int x, int y, int alpha, int lightMapWidth, int lightMapHeight) { //x and y < lightMapWidth and LightMapHeight
	(*lightPoints)[y * lightMapWidth * 4 + x * 4].color = sf::Color(0, 0, 0, alpha);
	(*lightPoints)[y * lightMapWidth * 4 + x * 4 + 1].color = sf::Color(0, 0, 0, alpha);
	(*lightPoints)[y * lightMapWidth * 4 + x * 4 + 2].color = sf::Color(0, 0, 0, alpha);
	(*lightPoints)[y * lightMapWidth * 4 + x * 4 + 3].color = sf::Color(0, 0, 0, alpha);
}


//This method is WAYYY better (like 100 times more efficient for same result)
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

void renderBackground(Area &area, int screenSize[2], float cameraPos[2], int FOV[2]) {
	shared_ptr<sf::Sprite> background = area.getBackground();
	sf::Vector2u backgroundSize = background->getTexture()->getSize();

	float errorX = (cameraPos[0] - (float)FOV[0] / 2) / area.getWidth() * (float)backgroundSize.x - 1 - 
					(int)((cameraPos[0] - (float)FOV[0] / 2) / area.getWidth() * (float)backgroundSize.x - 1);
	float errorY = (cameraPos[1] - (float)FOV[1] / 2) / area.getHeight() * (float)backgroundSize.y - 1 - 
					(int)((cameraPos[1] - (float)FOV[1] / 2) / area.getHeight() * (float)backgroundSize.y - 1);
	sf::IntRect backgroundRect((cameraPos[0] - (float)FOV[0] / 2) / area.getWidth() * (float)backgroundSize.x - 1, //left
		(cameraPos[1] - (float)FOV[1] / 2) / area.getHeight() * (float)backgroundSize.y - 1, //top
		(float)FOV[0] / area.getWidth() * (float)backgroundSize.x + 2, //width
		(float)FOV[1] / area.getHeight() * (float)backgroundSize.y + 2); //height
	background->setTextureRect(backgroundRect);
	background->setScale((float)screenSize[0] / ((float)background->getTextureRect().width - 2),
		(float)screenSize[1] / ((float)background->getTextureRect().height - 2));

	float backgroundX = (-errorX - 1) * ((float)screenSize[0] / (float)background->getTextureRect().width);
	float backgroundY = (-errorY - 1) * ((float)screenSize[1] / (float)background->getTextureRect().height);
	background->setPosition(backgroundX, backgroundY);
}

void setPlayerScreenPos(Player &player, float cameraGap[2], int screenSize[2], int FOV[2]) {
	player.getSprite()->setPosition((float)screenSize[0] / 2 + (cameraGap[0] - player.getTextureWidth() / 2) / (float)FOV[0] * (float)screenSize[0], //x position
							(float)screenSize[1] / 2 + (cameraGap[1] - player.getTextureHeight() / 2) / (float)FOV[1] * (float)screenSize[1]); //y position
	player.getSprite()->setScale(player.getTextureWidth() * (float)screenSize[0] / (float)FOV[0] / player.getSprite()->getTexture()->getSize().x, //x scale
								player.getTextureHeight() * (float)screenSize[1] / (float)FOV[1] / player.getSprite()->getTexture()->getSize().y); //y scale

	if (player.isFacingLeft()) {
		player.getSprite()->scale(-1.f, 1.f); //flip horizontally
		player.moveScreenPosition(player.getTextureWidth() * (float)screenSize[0] / (float)FOV[0], 0.f);
	}
}

void renderObject(shared_ptr<GameObject> obj, int screenSize[2], float cameraPos[2], int FOV[2]) {
	float objectPos[2] = { *obj->getPosition(), *(obj->getPosition() + 1) };
	float* temp = relativePosition(cameraPos[0], cameraPos[1], objectPos[0], objectPos[1]);
	float relativePosition[2] = { *temp, *(temp + 1) };
	std::free(temp);
	obj->getSprite()->setPosition((relativePosition[0] - obj->getTextureWidth() / 2) / (float)FOV[0] * (float)screenSize[0] + (float)screenSize[0] / 2,
		(relativePosition[1] - obj->getTextureHeight() / 2) / (float)FOV[1] * (float)screenSize[1] + (float)screenSize[1] / 2);
	shared_ptr<sf::Sprite> currentSprite = obj->getSprite();
	currentSprite->setScale(obj->getTextureWidth() / (float)FOV[0] * (float)screenSize[0] / currentSprite->getTexture()->getSize().x,
		obj->getTextureHeight() / (float)FOV[1] * (float)screenSize[1] / currentSprite->getTexture()->getSize().y);
}

bool isInside(float x, float y, sf::FloatRect &bounds) {
	if (x >= bounds.left and
		x <= bounds.left + bounds.width and
		y >= bounds.top and
		y <= bounds.top + bounds.height) {
		return true;
	}
	return false;
}