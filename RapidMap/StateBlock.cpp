#include "StateBlock.hpp"

void StateBlock::processEvents(sf::Event e)
{
	//if (!stateList.IsEmpty())
	//	stateList.GetActiveState()->updateEvents(e);
}

void StateBlock::update()
{
	stateList.ProcessStateChanges();

	//if (!stateList.IsEmpty())
	//	stateList.GetActiveState()->updateState(window);
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