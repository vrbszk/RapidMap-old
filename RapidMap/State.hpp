#pragma once

#include <vector>

#include "Entity.hpp"

class State
{
public:
	virtual void Init() = 0;
	virtual void updateEvents() = 0;
	virtual void updateState() = 0;
	virtual void render() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

protected:
	std::vector<Entity> entityHolder;
};