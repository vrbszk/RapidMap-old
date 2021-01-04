#include "Workspace.hpp"

Workspace::Workspace()
{

}

void Workspace::processEvents(sf::Event e)
{
	sf::View tempView = window->getView();
	window->setView(view);

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

void Workspace::update()
{
	workspaceView = view;
	workspaceView.setSize(workspaceView.getSize().x * zoomLevel, workspaceView.getSize().y * zoomLevel);
	workspaceView.setCenter(viewCenter);
	//workspaceView.setViewport(sf::FloatRect(space.left / window->getSize().x, space.top / window->getSize().y,
		//space.width / window->getSize().x, space.height / window->getSize().y)); // space в даному випадку має остаточні розміри, а значить не потрібно прив'язуватися до викривленого view

	window->setHintText("");

	sf::View tempView = window->getView();
	window->setView(workspaceView);
	//window->setView(view);

	if (projectManager->currProject)
	{
		if (projectManager->tool == ProjectManager::ToolList::SelectTool)
		{
			sf::Vector2f viewMousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

			for (auto it = projectManager->currProject->infr.stopNodes.begin();
				it != projectManager->currProject->infr.stopNodes.end(); it++)
			{
				//std::cout << it->second.getCollideBox().left << " " << it->second.getCollideBox().top << std::endl;
				if (it->second.getCollideBox().contains(viewMousePos))
				{
					it->second.setFillColor(sf::Color::Red);
					//std::cout << it->second.name << std::endl;
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

void Workspace::render()
{
	if (!window)
		return;

	sf::View tempView = window->getView();
	window->setView(view);

	/*sf::RectangleShape bound(sf::Vector2f(view.getSize().x - 10 * zoomLevel, view.getSize().y - 10 * zoomLevel));
	bound.setOutlineThickness(5 * zoomLevel);
	bound.setOutlineColor(sf::Color::Yellow);
	bound.setFillColor(sf::Color::Transparent);
	bound.setOrigin(bound.getSize().x / 2, bound.getSize().y / 2);
	bound.setPosition(viewCenter);
	window->draw(bound);*/

	sf::RectangleShape bound(sf::Vector2f(view.getSize().x - 10, view.getSize().y - 10));
	bound.setOutlineThickness(5);
	bound.setOutlineColor(sf::Color::Yellow);
	bound.setFillColor(sf::Color::Transparent);
	bound.setPosition(5, 5);
	window->draw(bound);

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