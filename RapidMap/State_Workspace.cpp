#include "State_Workspace.hpp"

State_Workspace::State_Workspace()
{

}

void State_Workspace::init()
{

}

void State_Workspace::processEvents(sf::Event e)
{
	sf::View tempView = window->getView();
	window->setView(view);

	window->setView(workspaceView);

	switch (e.type)
	{
	case sf::Event::KeyPressed:
	{
		switch (e.key.code)
		{
		case sf::Keyboard::E:
			skeletonEnabled = !skeletonEnabled;
			break;
		case sf::Keyboard::L:
			zoomLevel = 1;
			viewCenter = sf::Vector2f(0, 0);
			break;
		case sf::Keyboard::N:
			if (e.key.control) projectManager->createProject();
			else nodeSkeletonEnabled = !nodeSkeletonEnabled;
			break;
		case sf::Keyboard::O:
			if (e.key.control) projectManager->openProject();
			break;
		case sf::Keyboard::S:
			if (e.key.control && !e.key.alt) projectManager->saveProject();
			else if (e.key.control && e.key.alt) projectManager->saveProjectAs();
			else if (!e.key.control && e.key.alt) projectManager->saveProjectCopy();
			break;
		case sf::Keyboard::T:
			if (e.key.control) projectManager->attachOSMData();
			break;
		}
		break;
	}
	case sf::Event::MouseMoved:
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			viewCenter.x = viewCenter.x + (prevMousePos.x - e.mouseMove.x) * zoomLevel;
			viewCenter.y = viewCenter.y + (prevMousePos.y - e.mouseMove.y) * zoomLevel;
		}
		prevMousePos.x = e.mouseMove.x;
		prevMousePos.y = e.mouseMove.y;
		break;
	}
	case sf::Event::MouseButtonPressed:
	{
		if (projectManager->currProject)
			if (projectManager->tool == ProjectManager::ToolList::AddStation)
			{
				sf::Vector2f viewMousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
				for (auto it = projectManager->currProject->infr.stopNodes.begin();
					it != projectManager->currProject->infr.stopNodes.end(); it++)
				{
					if (it->second.getCollideBox().contains(viewMousePos))
					{
						projectManager->currProject->line.stationids.push_back(it->first);
						projectManager->currProject->line.refresh(&projectManager->currProject->infr);
					}
				}
			}
		break;
	}
	case sf::Event::MouseWheelScrolled:
	{
		if (e.mouseWheelScroll.delta < 0)
		{
			zoomLevel *= 2;
		}
		else if (e.mouseWheelScroll.delta > 0)
		{
			zoomLevel /= 2;
		}
		break;
	}
	}

	window->setView(tempView);
}

void State_Workspace::update()
{
	workspaceView = view;
	workspaceView.setSize(workspaceView.getSize().x * zoomLevel, workspaceView.getSize().y * zoomLevel);
	workspaceView.setCenter(viewCenter);

	window->setHintText("");

	sf::View tempView = window->getView();

	window->setView(view);

	window->setView(workspaceView);

	if (projectManager->currProject)
	{
		if (projectManager->tool == ProjectManager::ToolList::SelectTool)
		{
			sf::Vector2f viewMousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

			for (auto it = projectManager->currProject->infr.stopNodes.begin();
				it != projectManager->currProject->infr.stopNodes.end(); it++)
			{
				if (it->second.getCollideBox().contains(viewMousePos))
				{
					it->second.setFillColor(sf::Color::Red);
					window->setHintText("id: " + sf::String(it->first) + "\nname:" + sf::String::fromUtf8(it->second.name.begin(), it->second.name.end()));
				}
				else
					it->second.setFillColor(sf::Color::Green);
			}
		}
		else if (projectManager->tool == ProjectManager::ToolList::AddStation)
		{
			sf::Vector2f viewMousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

			for (auto it = projectManager->currProject->infr.stopNodes.begin();
				it != projectManager->currProject->infr.stopNodes.end(); it++)
			{
				if (it->second.getCollideBox().contains(viewMousePos))
				{
					it->second.setFillColor(sf::Color::Magenta);
					//window->setHintText("id: " + it->first);
				}
				else
					it->second.setFillColor(sf::Color::Green);
			}
		}
	}

	window->setView(tempView);
}

void State_Workspace::render()
{
	if (!window)
		return;

	sf::View tempView = window->getView();
	window->setView(view);



	window->setView(workspaceView);


	if (projectManager->currProject)
	{
		for (auto it : projectManager->currProject->infr.stopNodes)
		{
			window->draw(it.second);
		}

		window->draw(projectManager->currProject->line);

		if (skeletonEnabled)
		{
			if (nodeSkeletonEnabled)
			{
				for (auto it : projectManager->currProject->infr.wayNodes)
				{
					window->draw(it.second);
				}
			}
			for (auto it : projectManager->currProject->infr.streetWays)
			{
				window->draw(it.second);
			}
			for (auto it : projectManager->currProject->infr.railWays)
			{
				window->draw(it.second);
			}
		}
	}

	window->setView(tempView);
}