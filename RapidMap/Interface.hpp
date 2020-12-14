#pragma once
#include <SFML\Graphics.hpp>
#include <memory>

class Interface;

typedef std::unique_ptr<Interface> InterfacePtr;

#include "Project.hpp"

class Interface
{
public:
	sf::View view;
	std::shared_ptr<ProjectManager> projectManager;

	void setWindow(Window* win);
	Window* getWindow();

	virtual void updateEvents(sf::Event e) = 0;
	virtual void updateInterface(sf::FloatRect space) = 0;
	virtual void render() = 0;

protected:
	Window* window;
};