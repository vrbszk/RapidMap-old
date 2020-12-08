#include "MainMenuState.hpp"

#include "Button.hpp"




#include <iostream>

void MainMenuState::Init()
{
	std::unique_ptr<Button> button(new Button(sf::Vector2f(300, 100)));
	button->setActiveColor(sf::Color::Yellow);
	button->setPassiveColor(sf::Color::Magenta);
	entityHolder.push_back(std::move(button));
	std::cout << "MainMenuState inited" << std::endl;
}

void MainMenuState::updateEvents(sf::Event event)
{
	
}

void MainMenuState::updateState(sf::RenderWindow* win)
{
	for (auto it = entityHolder.begin(); it != entityHolder.end(); it++)
	{
		it->get()->update(sf::Vector2f(sf::Mouse::getPosition(*win)));
	}
}

void MainMenuState::render(sf::RenderWindow* win)
{
	for (auto it = entityHolder.begin(); it != entityHolder.end(); it++)
	{
		win->draw(*it->get());
	}
}

void MainMenuState::Pause()
{

}
void MainMenuState::Resume()
{

}