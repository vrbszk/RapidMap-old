#include "Interface.hpp"

void Interface::setWindow(sf::RenderWindow* win)
{
	window = win;
}

sf::RenderWindow* Interface::getWindow()
{
	return window;
}

sf::Vector2f Interface::getMinSize()
{
	return minSize;
}