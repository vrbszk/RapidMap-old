#pragma once

class MainMenuState;

#include "State.hpp"

class MainMenuState : public State
{
	void Init();
	void updateEvents(sf::Event event);
	void updateState(sf::RenderWindow* win);
	void render(sf::RenderWindow* win);

	void Pause();
	void Resume();
};