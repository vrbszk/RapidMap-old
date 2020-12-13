#pragma once

class ToolStrip;

#include "Interface.hpp"
#include "Button.hpp"

class ToolStrip : public Interface
{
public:
	ToolStrip();

	void updateEvents(sf::Event e);
	void updateInterface(sf::FloatRect space);
	void render();

	void init();

public:
	Button selectButton;
};