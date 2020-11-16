#pragma once
#include "Interface.hpp"

//class InterfaceHolder
//{
//public:
//	virtual void updateEvents(sf::Event e) = 0;
//	virtual void updateInterfaces(sf::FloatRect space) = 0;
//	virtual void render() = 0;
//
//	virtual void setWindow(sf::RenderWindow* win) = 0;
//	sf::RenderWindow* getWindow();
//
//protected:
//	sf::RenderWindow* window;
//};

class InterfaceHolderNode : public Interface//Holder
{
public:
	InterfaceHolderNode();
	~InterfaceHolderNode();

	void updateEvents(sf::Event e);
	void updateInterface(sf::FloatRect space);
	void render();

	void setWindow(sf::RenderWindow* win);

	sf::Vector2f getMinSize();

public:
	bool borderVertcal;
	unsigned int borderSize;//dist to left or top side

	char stableNode; // -1 (left), 0 (none), 1 (right)

	Interface* inth1;
	Interface* inth2;
};

//class InterfaceHolderLeaf : public Interface//Holder
//{
//public:
//	void updateEvents(sf::Event e);
//	void updateInterfaces(sf::FloatRect space);
//	void render();
//
//	void setWindow(sf::RenderWindow* win);
//
//public:
//	Interface* inter;
//};