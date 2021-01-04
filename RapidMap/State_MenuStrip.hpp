#pragma once

class State_MenuStrip;

#include "State.hpp"
#include "StripButton.hpp"
#include "AssetManager.hpp"

class State_MenuStrip : public State
{
public:
	State_MenuStrip();

	void init();

	void processEvents(sf::Event e);
	void update();
	void render();

private:
	StripButton button;
	sf::RectangleShape bg;
};