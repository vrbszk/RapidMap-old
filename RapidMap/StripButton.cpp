#include "StripButton.hpp"

StripButton::StripButton(sf::Vector2f size):
	Button(size)
{

}

void StripButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Button::draw(target, states);

	states.transform *= this->getTransform();

	target.draw(text);
}