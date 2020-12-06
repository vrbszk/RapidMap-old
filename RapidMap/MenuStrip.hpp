#pragma once

class MenuStrip;

#include "Interface.hpp"
#include "StripButton.hpp"
#include "AssetManager.hpp"

class MenuStrip : public Interface
{
public:
	MenuStrip();

	void init();

	void updateEvents(sf::Event e);
	void updateInterface(sf::FloatRect space);
	void render();

private:
	StripButton button;
	sf::RectangleShape bg;
	
public:
	AssetManager* resources;
};