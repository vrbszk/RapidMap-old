#pragma once

class State_Workspace;

#include "Interface.hpp"
#include "Project.hpp"

class State_Workspace : public State
{
public:
	State_Workspace();

	void init();
	void processEvents(sf::Event e);
	void update();
	void render();

public:
	

	bool skeletonEnabled;
	bool nodeSkeletonEnabled;

	float zoomLevel;
	sf::Vector2f viewCenter;
	sf::Vector2i prevMousePos;
private:
	sf::View workspaceView;
};