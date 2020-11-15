#pragma once
#include <SFML\Graphics.hpp>
#include <memory>

class Interface
{
public:
	sf::View view;

	void setWindow(sf::RenderWindow* win);
	sf::RenderWindow* getWindow();

	virtual void updateEvents(sf::Event e) = 0;
	virtual void updateInterface() = 0;
	virtual void render() = 0;

protected:
	sf::RenderWindow* window;
};

typedef std::unique_ptr<Interface> InterfacePtr;