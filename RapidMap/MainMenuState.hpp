#pragma once

class MainMenuState;

#include "State.hpp"

class MainMenuState : public State
{
	void init();
	void processEvents(sf::Event event);
	void updateState();
	void render();

	void Pause();
	void Resume();
};