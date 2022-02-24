#pragma once
#include <stdio.h>
#include <string>
#include <SFML/Graphics/Rect.hpp>
#include <memory>

using namespace std;

class TextBox
{
public:
	TextBox(string text, shared_ptr<sf::FloatRect> box) : text(text), box(box) {}
	string getText();
	shared_ptr<sf::FloatRect> getBox();
	void setText(string newText);
	void addText(string newText);

private:
	string text;
	shared_ptr<sf::FloatRect> box;
};

