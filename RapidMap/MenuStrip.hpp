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

	void processEvents(sf::Event e);
	void update();
	void render();

private:
	StripButton button;
	sf::RectangleShape bg;
	
public:
	AssetManager* resources;
};