#include "MainMenuState.hpp"

#include "Button.hpp"

void MainMenuState::Init()
{
	EntityPtr button(new Button(sf::Vector2f(300, 100)));
	entityHolder.push_back(std::move(button));
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