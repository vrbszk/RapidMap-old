#pragma once

class ToolStrip;

#include "Interface.hpp"
#include "Button.hpp"

class ToolStrip : public Interface
{
public:
	ToolStrip();

	void processEvents(sf::Event e);
	void update();
	void render();

	void init();

public:
	Button selectButton;
	Button addStationButton;
};