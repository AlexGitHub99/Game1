#pragma once

#include <SFML/Graphics.hpp>

using namespace std;

class Button
{
public:
	Button(shared_ptr<sf::FloatRect>& rect, string text);
	shared_ptr<sf::FloatRect> getRect();
	string getText();
	void setPressed(bool newPressed);
	bool isPressed();

private:
	bool pressed;
	shared_ptr<sf::FloatRect> rect;
	string text;
};

