#include "TextBox.h"

TextBox::TextBox(string text, shared_ptr<sf::FloatRect> box, string inputType, string id) : text(text), box(box), inputType(inputType), id(id) {};

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

string TextBox::getInputType()
{
    return inputType;
}

string TextBox::getId()
{
    return id;
}

