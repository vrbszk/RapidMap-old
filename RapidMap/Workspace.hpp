#pragma once
#include "Interface.hpp"
#include "Project.hpp"

class Workspace : public Interface
{
public:
	Project* project;

	Workspace();

	void updateEvents(sf::Event e);
	void updateInterface();
	void render();

	bool skeletonEnabled;
	bool nodeSkeletonEnabled;

	float zoomLevel;
	sf::Vector2f viewCenter;
	sf::Vector2i prevMousePos;
private:

};