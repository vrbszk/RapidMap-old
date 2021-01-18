#pragma once

class State_LinesViewer;

#include "State.hpp"

class State_LinesViewer : public State
{
public:
	void init();
	void processEvents(sf::Event event);
	void update();
	void render();

	void refreshPanels();

	std::vector<sf::Text> textPanels;
};