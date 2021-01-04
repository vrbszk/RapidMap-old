#include "ToolStrip.hpp"

ToolStrip::ToolStrip() : selectButton(sf::Vector2f(50, 50)), addStationButton(sf::Vector2f(50, 50))
{

}

void ToolStrip::init()
{
	selectButton.setPosition(0, 0);
	addStationButton.setPosition(50, 0);
	//selectButton.setPassiveColor(sf::Color(192, 72, 72));
	//selectButton.setActiveColor(sf::Color(96, 24, 72));
}

void ToolStrip::processEvents(sf::Event e)
{
	sf::View tempView = window->getView();
	window->setView(view);

	if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2f viewMousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
		if (selectButton.getCollideBox().contains(viewMousePos))
		{
			selectButton.pressed = true;//!selectButton.pressed;
			addStationButton.pressed = false;
			projectManager->tool = ProjectManager::ToolList::SelectTool;
		}
		if (addStationButton.getCollideBox().contains(viewMousePos))
		{
			selectButton.pressed = false;
			addStationButton.pressed = true;
			projectManager->tool = ProjectManager::ToolList::AddStation;
		}
	}

	if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
	{
		selectButton.pressed = false;
		addStationButton.pressed = false;
		projectManager->tool = ProjectManager::None;
	}

	window->setView(tempView);
}

void ToolStrip::update()
{
	sf::View tempView = window->getView();
	window->setView(view);
	sf::Vector2f viewMousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	window->setView(tempView);

	selectButton.update(viewMousePos);//sf::Vector2f(sf::Mouse::getPosition(*window)));
	addStationButton.update(viewMousePos);
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
	window->draw(addStationButton);

	window->setView(tempView);
}