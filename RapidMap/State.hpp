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
	virtual void update() = 0;
	virtual void render() = 0;

	void setWindow(Window* win);
	void setView(sf::View v);

	void setHolder(Interface* iptr);

	void setAssetManager(std::shared_ptr<AssetManager>& ptr);
	void setProjectManager(std::shared_ptr<ProjectManager>& ptr);

protected:
	std::shared_ptr<AssetManager> assetManager;
	std::shared_ptr<ProjectManager> projectManager;

	std::vector<EntityPtr> entityHolder;

	Window* window;
	sf::View view;
	Interface* holder;
};