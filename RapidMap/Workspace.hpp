#pragma once
#include "Interface.hpp"
#include "Project.hpp"

//class ProjectManager;

class Workspace : public Interface
{
public:
	Workspace();

	void updateEvents(sf::Event e);
	void updateInterface(sf::FloatRect space);
	void render();

public:
	std::shared_ptr<ProjectManager> projectManager;

	bool skeletonEnabled;
	bool nodeSkeletonEnabled;

	float zoomLevel;
	sf::Vector2f viewCenter;
	sf::Vector2i prevMousePos;
private:
	
};