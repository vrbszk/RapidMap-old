#pragma once

class StateBlock;

#include "State.hpp"
#include "StateMachine.hpp"

class StateBlock : public State
{
public:
	void init();
	void processEvents(sf::Event event);
	void update();
	void render();

public:
	StateMachine stateList;

	std::shared_ptr<ProjectManager> projectManager;
};