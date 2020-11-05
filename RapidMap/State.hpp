#pragma once

#include <vector>

#include "Entity.hpp"

class State
{
public:
	virtual void Init() = 0;
	virtual void updateEvents(sf::Event event) = 0;
	virtual void updateState(sf::RenderWindow* win) = 0;
	virtual void render(sf::RenderWindow* win) = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	//void setRenderWindow(sf::RenderWindow* win);
protected:
	std::vector<EntityPtr> entityHolder;
	//sf::RenderWindow* window;
};