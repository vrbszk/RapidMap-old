#pragma once

class State_LineManager;

#include "State.hpp"

class State_LineManager : public State
{
public:
	void init();
	void processEvents(sf::Event event);
	void update();
	void render();

	void setLine(Line* l);
	void refreshData();

private:
	Line* line;
};