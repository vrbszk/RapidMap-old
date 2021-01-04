#pragma once

class StateBlock;

#include "Interface.hpp"
#include "StateMachine.hpp"

class StateBlock : public Interface
{
public:
	void processEvents(sf::Event e);
	void update();
	void render();

public:
	StateMachine stateList;
};