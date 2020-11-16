#include "MenuStrip.hpp"

MenuStrip::MenuStrip()
{
	minSize = sf::Vector2f(50, 20);
}

void MenuStrip::updateEvents(sf::Event e)
{

}

void MenuStrip::updateInterface(sf::FloatRect space)
{
	view.setSize(space.width, space.height);
	view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
	view.setViewport(sf::FloatRect(space.left / window->getSize().x, space.top / window->getSize().y,
		space.width / window->getSize().x, space.height / window->getSize().y));
}

void MenuStrip::render()
{
	if (!window)
		return;

	sf::View tempView = window->getView();
	window->setView(view);

	sf::RectangleShape bound(sf::Vector2f(view.getSize().x - 10, view.getSize().y - 10));
	bound.setOutlineThickness(5);
	bound.setOutlineColor(sf::Color::Magenta);
	bound.setFillColor(sf::Color::Transparent);
	bound.setPosition(5,5);
	window->draw(bound);

	window->setView(tempView);
}