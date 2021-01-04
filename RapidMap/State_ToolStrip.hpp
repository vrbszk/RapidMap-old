#pragma once

class State_ToolStrip;

#include "State.hpp"
#include "Button.hpp"

class State_ToolStrip : public State
{
public:
	State_ToolStrip();

	void init();

	void processEvents(sf::Event event);
	void update();
	void render();

public:
	Button selectButton;
	Button addStationButton;
};