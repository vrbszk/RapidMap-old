#pragma once
#include <SFML\Graphics.hpp>
#include <memory>

class Interface;

typedef std::unique_ptr<Interface> InterfacePtr;

#include "Project.hpp"

class Interface
{
public:
	//sf::View view;
	std::shared_ptr<ProjectManager> projectManager;

	void setWindow(Window* win);
	Window* getWindow();

	void setView(sf::View v);
	void createView(sf::FloatRect space);
	sf::View getView();

	virtual void processEvents(sf::Event e);
	virtual void update();
	virtual void render();

	bool f_drawBorder = true;

protected:
	Window* window;
	sf::View view;
};