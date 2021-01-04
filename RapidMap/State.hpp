#pragma once
#include <vector>
#include <memory>

class State;
typedef std::unique_ptr<State> StatePtr;

#include "Entity.hpp"
#include "Window.hpp"

class State
{
public:
	virtual void init() = 0;
	virtual void processEvents(sf::Event event) = 0;
	virtual void updateState() = 0;
	virtual void render() = 0;

	void setWindow(Window* win);

protected:
	std::vector<EntityPtr> entityHolder;
	Window* window;
};