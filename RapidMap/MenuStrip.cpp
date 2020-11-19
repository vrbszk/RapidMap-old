#include "MenuStrip.hpp"

MenuStrip::MenuStrip() :
	button(sf::Vector2f(40, 14))
{

}

void MenuStrip::init()
{
	button.setPosition(3, 3);
	button.text.setString("File");
	button.text.setFillColor(sf::Color::White);
	button.text.setCharacterSize(14);
	button.text.setFont(resources->GetFont("main"));
	button.text.setOrigin(button.text.getGlobalBounds().width / 2, button.text.getCharacterSize() / 2);
	button.text.setPosition(43 / 2, 17 / 2);
	button.setPassiveColor(sf::Color(192, 72, 72));
	button.setActiveColor(sf::Color(96, 24, 72));

	bg.setFillColor(sf::Color(192, 72, 72));
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

	button.update(sf::Vector2f(sf::Mouse::getPosition(*window)));

	bg.setSize(view.getSize());
	
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

	window->draw(bg);

	window->draw(button);

	window->setView(tempView);
}