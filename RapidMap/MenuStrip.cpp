#include "MenuStrip.hpp"

void MenuStrip::updateEvents(sf::Event e)
{

}

void MenuStrip::updateInterface(sf::FloatRect space)
{
	//view.setSize(window->getSize().x, 20);
	view.setSize(space.width, 20);
	view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
	//sf::Vector2f portPos(space.left / window->getSize().x, space.top / window->getSize().y);
	view.setViewport(sf::FloatRect(space.left / window->getSize().x, space.top / window->getSize().y,
		view.getSize().x / window->getSize().x, view.getSize().y / window->getSize().y));
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
	//bound.setOrigin(bound.getSize().x / 2, bound.getSize().y / 2);
	bound.setPosition(5,5);
	window->draw(bound);

	window->setView(tempView);
}