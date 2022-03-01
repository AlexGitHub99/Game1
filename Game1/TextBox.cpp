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

bool TextBox::backspace()
{
    if (text.size() == 0) return false;
    text.pop_back();
    return true;
}

