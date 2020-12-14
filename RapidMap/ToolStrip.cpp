#include "ToolStrip.hpp"

ToolStrip::ToolStrip() : selectButton(sf::Vector2f(50, 50))
{

}

void ToolStrip::init()
{
	//selectButton.setPosition(3, 3);
	//selectButton.setPassiveColor(sf::Color(192, 72, 72));
	//selectButton.setActiveColor(sf::Color(96, 24, 72));
}

void ToolStrip::updateEvents(sf::Event e)
{

}

void ToolStrip::updateInterface(sf::FloatRect space)
{
	view.setSize(space.width, space.height);
	view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
	view.setViewport(sf::FloatRect(space.left / window->getSize().x, space.top / window->getSize().y,
		space.width / window->getSize().x, space.height / window->getSize().y));

	sf::View tempView = window->getView();
	window->setView(view);
	sf::Vector2f viewMousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	window->setView(tempView);

	selectButton.update(viewMousePos);//sf::Vector2f(sf::Mouse::getPosition(*window)));
}

void ToolStrip::render()
{
	if (!window)
		return;

	sf::View tempView = window->getView();
	window->setView(view);

	sf::RectangleShape bound(sf::Vector2f(view.getSize().x - 10, view.getSize().y - 10));
	bound.setOutlineThickness(5);
	bound.setOutlineColor(sf::Color::Red);
	bound.setFillColor(sf::Color::Transparent);
	bound.setPosition(5, 5);
	window->draw(bound);

	window->draw(selectButton);

	window->setView(tempView);
}