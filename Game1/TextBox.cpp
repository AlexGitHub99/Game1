#include "TextBox.h"

string TextBox::getText()
{
    return text;
}

shared_ptr<sf::FloatRect> TextBox::getBox()
{
    return box;
}

void TextBox::setText(string newText)
{
    text = newText;
}

void TextBox::addText(string newText)
{
    text += newText;
}

