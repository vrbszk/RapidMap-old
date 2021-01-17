#pragma once

class State_LineViewer;

#include "State.hpp"

class State_LineViewer : public State
{
public:
	void init();
	void processEvents(sf::Event event);
	void update();
	void render();
};