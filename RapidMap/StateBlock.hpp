#pragma once

#include "Interface.hpp"
#include "StateMachine.hpp"

class StateBlock : public Interface
{
public:
	void updateEvents(sf::Event e);
	void updateInterface(sf::FloatRect space);
	void render();

public:
	StateMachine stateList;

};