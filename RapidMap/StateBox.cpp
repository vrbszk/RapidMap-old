#include "StateBox.hpp"

void StateBox::processEvents(sf::Event event)
{

}

void StateBox::update()
{

}

//void StateBox::draw(sf::RenderTarget& target, sf::RenderStates states)
//{
//	sf::View tempView = window->getView();
//	window->setView(view);
//
//	sf::RectangleShape bound(sf::Vector2f(boxView.getSize().x - 10, boxView.getSize().y - 10));
//	bound.setOutlineThickness(5);
//	bound.setOutlineColor(sf::Color::Magenta);
//	bound.setFillColor(sf::Color::Transparent);
//	bound.setPosition(5, 5);
//
//	window->setView(tempView);
//}

void StateBox::render()
{
	sf::View tempView = window->getView();
	window->setView(view);

	sf::RectangleShape bound(sf::Vector2f(boxView.getSize().x - 10, boxView.getSize().y - 10));
	bound.setOutlineThickness(5);
	bound.setOutlineColor(sf::Color::Magenta);
	bound.setFillColor(sf::Color::Transparent);
	bound.setPosition(5, 5);

	window->draw(bound);

	window->setView(tempView);
}

//
//void StateBox::setWindow(Window* win)
//{
//	window = win;
//}
//
//
//Window* StateBox::getWindow()
//{
//	return window;
//}


void StateBox::setView(sf::FloatRect space)
{
	boxView.setSize(space.width, space.height);
	boxView.setCenter(boxView.getSize().x / 2, boxView.getSize().y / 2);
	boxView.setViewport(sf::FloatRect(space.left / window->getSize().x, space.top / window->getSize().y,
		space.width / window->getSize().x, space.height / window->getSize().y));
}

sf::View StateBox::getView()
{
	return boxView;
}