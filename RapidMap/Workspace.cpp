#include "Workspace.hpp"

Workspace::Workspace()
{

}

void Workspace::updateEvents(sf::Event event)
{
	sf::View tempView = window->getView();
	window->setView(view);

	switch (event.type)
	{
	case sf::Event::KeyPressed:
	{
		switch (event.key.code)
		{
		case sf::Keyboard::E:
			skeletonEnabled = !skeletonEnabled;
			break;
		case sf::Keyboard::L:
			zoomLevel = 1;
			viewCenter = sf::Vector2f(0, 0);
			break;
		case sf::Keyboard::N:
			if (event.key.control) projectManager->createProject();
			else nodeSkeletonEnabled = !nodeSkeletonEnabled;
			break;
		case sf::Keyboard::O:
			if (event.key.control) projectManager->openProject();
			break;
		case sf::Keyboard::S:
			if (event.key.control && !event.key.alt) projectManager->saveProject();
			else if (event.key.control && event.key.alt) projectManager->saveProjectAs();
			else if (!event.key.control && event.key.alt) projectManager->saveProjectCopy();
			break;
		case sf::Keyboard::T:
			if (event.key.control) projectManager->attachOSMData();
			break;
		}
		break;
	}
	case sf::Event::MouseMoved:
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			viewCenter.x = viewCenter.x + (prevMousePos.x - event.mouseMove.x) * zoomLevel;
			viewCenter.y = viewCenter.y + (prevMousePos.y - event.mouseMove.y) * zoomLevel;
		}
		prevMousePos.x = event.mouseMove.x;
		prevMousePos.y = event.mouseMove.y;
		break;
	}
	case sf::Event::MouseButtonPressed:
	{
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
		if (event.mouseWheelScroll.delta < 0)
		{
			zoomLevel *= 2;
		}
		else if (event.mouseWheelScroll.delta > 0)
		{
			zoomLevel /= 2;
		}
		break;
	}
	}

	window->setView(tempView);
}

void Workspace::updateInterface(sf::FloatRect space)
{
	view.setSize(space.width * zoomLevel, space.height * zoomLevel);
	view.setCenter(viewCenter);
	view.setViewport(sf::FloatRect(space.left / window->getSize().x, space.top / window->getSize().y,
		space.width / window->getSize().x, space.height / window->getSize().y)); // space в даному випадку має остаточні розміри, а значить не потрібно прив'язуватися до викривленого view

	window->setHintText("");

	sf::View tempView = window->getView();
	window->setView(view);

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
					window->setHintText("id: " + it->first);
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
					window->setHintText("id: " + it->first);
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

	sf::RectangleShape bound(sf::Vector2f(view.getSize().x - 10 * zoomLevel, view.getSize().y - 10 * zoomLevel));
	bound.setOutlineThickness(5 * zoomLevel);
	bound.setOutlineColor(sf::Color::Yellow);
	bound.setFillColor(sf::Color::Transparent);
	bound.setOrigin(bound.getSize().x / 2, bound.getSize().y / 2);
	bound.setPosition(viewCenter);
	window->draw(bound);


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