#include "TextBox.h"

string TextBox::getText()
{
    return text;
}

shared_ptr<sf::FloatRect> TextBox::getBox()
{
    return box;
}
