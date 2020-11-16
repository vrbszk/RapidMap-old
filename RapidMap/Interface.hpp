#pragma once
#include <SFML\Graphics.hpp>
#include <memory>

#include "Project.hpp"

class Interface
{
public:
	sf::View view;
	sf::Vector2f minSize;

	void setWindow(sf::RenderWindow* win);
	sf::RenderWindow* getWindow();

	virtual void updateEvents(sf::Event e) = 0;
	virtual void updateInterface(sf::FloatRect space) = 0;
	virtual void render() = 0;

	sf::Vector2f getMinSize();

protected:
	sf::RenderWindow* window;
	
};

typedef std::unique_ptr<Interface> InterfacePtr;