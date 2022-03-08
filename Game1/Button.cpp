#include "Button.h"


Button::Button(shared_ptr<sf::FloatRect>& rect, string text) : rect(rect), text(text) {
    pressed = false;
}

shared_ptr<sf::FloatRect> Button::getRect()
{
    return rect;
}

string Button::getText()
{
    return text;
}

void Button::setPressed(bool newPressed)
{
    pressed = newPressed;
}

bool Button::isPressed()
{
    return pressed;
}
