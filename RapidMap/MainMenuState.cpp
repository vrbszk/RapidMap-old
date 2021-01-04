#include "MainMenuState.hpp"

#include "Button.hpp"





#include <iostream>

void MainMenuState::init()
{
	std::unique_ptr<Button> button(new Button(sf::Vector2f(300, 100)));
	button->setActiveColor(sf::Color::Yellow);
	button->setPassiveColor(sf::Color::Magenta);
	entityHolder.push_back(std::move(button));
}

void MainMenuState::processEvents(sf::Event event)
{
	
}

void MainMenuState::updateState()
{
	sf::Vector2f viewMousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	for (auto it = entityHolder.begin(); it != entityHolder.end(); it++)
	{
		it->get()->update(viewMousePos);
	}
}

void MainMenuState::render()
{
	for (auto it = entityHolder.begin(); it != entityHolder.end(); it++)
	{
		window->draw(*it->get());
	}
}

void MainMenuState::Pause()
{

}
void MainMenuState::Resume()
{

}