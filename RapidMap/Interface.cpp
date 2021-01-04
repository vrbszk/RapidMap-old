#include "Interface.hpp"

void Interface::setWindow(Window* win)
{
	window = win;

	if (state)
		state->setWindow(window);
}

Window* Interface::getWindow()
{
	return window;
}

void Interface::setView(sf::View v)
{
	view = v;
}

void Interface::createView(sf::FloatRect space)
{
	view.setSize(space.width, space.height);
	view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
	if (window)
		view.setViewport(sf::FloatRect(space.left / window->getSize().x, space.top / window->getSize().y,
			space.width / window->getSize().x, space.height / window->getSize().y));
}

sf::View Interface::getView()
{
	return view;
}

void Interface::setState(StatePtr s)
{
	state = std::move(s);
	
	if (state)
		state->setWindow(window);
}

StatePtr& Interface::getState()
{
	return state;
}

void Interface::processEvents(sf::Event e)
{
	sf::View tempView = window->getView();
	window->setView(view);

	if (state)
		state->processEvents(e);

	window->setView(tempView);
}

void Interface::update()
{
	sf::View tempView = window->getView();
	window->setView(view);

	if (state)
		state->updateState();

	window->setView(tempView);
}

void Interface::render()
{
	if (!window)
		return; 

	sf::View tempView = window->getView();
	window->setView(view);

	if (f_drawBorder)
	{
		sf::View tempView = window->getView();
		window->setView(view);

		sf::RectangleShape bound(sf::Vector2f(view.getSize().x - 10, view.getSize().y - 10));
		bound.setOutlineThickness(5);
		bound.setOutlineColor(sf::Color::Magenta);
		bound.setFillColor(sf::Color::Transparent);
		bound.setPosition(5, 5);
		window->draw(bound);
	}

	if (state)
		state->render();

	window->setView(tempView);
}