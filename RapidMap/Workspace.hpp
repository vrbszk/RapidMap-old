#pragma once

class Workspace;

#include "Interface.hpp"
#include "Project.hpp"

class Workspace : public Interface
{
public:
	Workspace();

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