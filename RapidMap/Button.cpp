#include "Button.hpp"

Button::Button() 
	: shape()
{
	init();
}

Button::Button(sf::Vector2f size)
	: shape(size)
{
	init();
}

void Button::init()
{
	activeColor = sf::Color(100, 100, 100);
	passiveColor = sf::Color(200, 200, 200);
	pressedColor = sf::Color(150, 150, 150);

	pressed = false;
}

Button::~Button()
{

}

void Button::update(sf::Vector2f mousePos)
{
	if (this->getCollideBox().contains(mousePos))
		shape.setFillColor(activeColor);
	else if (pressed)
		shape.setFillColor(pressedColor);
	else
		shape.setFillColor(passiveColor);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();

	target.draw(shape, states);
}

void Button::setActiveColor(sf::Color c)
{
	activeColor = c;
}

void Button::setPassiveColor(sf::Color c)
{
	passiveColor = c;
}

void Button::setPressedColor(sf::Color c)
{
	pressedColor = c;
}

sf::FloatRect Button::getCollideBox()
{
	sf::FloatRect rect = shape.getGlobalBounds();
	rect.left += this->getPosition().x;
	rect.top += this->getPosition().y;
	return rect;
}