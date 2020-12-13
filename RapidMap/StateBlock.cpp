#include "StateBlock.hpp"

void StateBlock::updateEvents(sf::Event e)
{
	//if (!stateList.IsEmpty())
	//	stateList.GetActiveState()->updateEvents(e);
}

void StateBlock::updateInterface(sf::FloatRect space)
{
	stateList.ProcessStateChanges();

	//if (!stateList.IsEmpty())
	//	stateList.GetActiveState()->updateState(window);

	view.setSize(space.width, space.height);
	view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
	view.setViewport(sf::FloatRect(space.left / window->getSize().x, space.top / window->getSize().y,
		space.width / window->getSize().x, space.height / window->getSize().y));
}

void StateBlock::render()
{
	sf::View tempView = window->getView();
	window->setView(view);

	sf::RectangleShape bound(sf::Vector2f(view.getSize().x - 10, view.getSize().y - 10));
	bound.setOutlineThickness(5);
	bound.setOutlineColor(sf::Color::Green);
	bound.setFillColor(sf::Color::Transparent);
	bound.setPosition(5, 5);
	window->draw(bound);

	//if(!stateList.IsEmpty())
	//	stateList.GetActiveState()->render(window);

	window->setView(tempView);
}