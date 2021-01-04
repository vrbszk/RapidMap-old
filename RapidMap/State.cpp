#include "State.hpp"

void State::setWindow(Window * win)
{
	window = win;
}


void State::setView(sf::View v)
{
	view = v;
}

void State::setHolder(Interface* iptr)
{
	holder = iptr;
}

void State::setAssetManager(std::shared_ptr<AssetManager>& ptr)
{
	assetManager = ptr;
}

void State::setProjectManager(std::shared_ptr<ProjectManager>& ptr)
{
	projectManager = ptr;
}