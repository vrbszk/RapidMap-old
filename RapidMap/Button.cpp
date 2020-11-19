#include "Button.hpp"

Button::Button() 
	: shape()
{

}

Button::Button(sf::Vector2f size)
	: shape(size)
{
	shape.setFillColor(sf::Color::Magenta);
}

Button::~Button()
{

}

void Button::update(sf::Vector2f mousePos)
{
	if (shape.getGlobalBounds().contains(mousePos))
		shape.setFillColor(activeColor);
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