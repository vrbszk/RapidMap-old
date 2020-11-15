#include "Workspace.hpp"

Workspace::Workspace()
{

}

void Workspace::updateEvents(sf::Event e)
{
	switch (e.type)
	{
	case sf::Event::KeyPressed:
	{
		switch (e.key.code)
		{
		case sf::Keyboard::E:
			skeletonEnabled = !skeletonEnabled;
			break;
		case sf::Keyboard::N:
			nodeSkeletonEnabled = !nodeSkeletonEnabled;
			break;
		}
		break;
	}
	case sf::Event::MouseMoved:
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			//sf::View view = window->getView();
			viewCenter.x = viewCenter.x + (prevMousePos.x - e.mouseMove.x) * zoomLevel;
			viewCenter.y = viewCenter.y + (prevMousePos.y - e.mouseMove.y) * zoomLevel;
			//view.move((prevMousePos.x - e.mouseMove.x) * zoomLevel, (prevMousePos.y - e.mouseMove.y) * zoomLevel);
			//window->setView(view);
		}
		prevMousePos.x = e.mouseMove.x;
		prevMousePos.y = e.mouseMove.y;
		break;
	}
	case sf::Event::MouseWheelScrolled:
	{
		if (e.mouseWheelScroll.delta < 0)
		{
			zoomLevel *= 2;
			//view.zoom(2);
		}
		else if (e.mouseWheelScroll.delta > 0)
		{
			zoomLevel /= 2;
			//view.zoom(0.5);
		}

		//sf::View view;
		//view.setCenter(window->getView().getCenter());
		view.setSize(window->getSize().x * zoomLevel, window->getSize().y * zoomLevel);
		//window->setView(view);
		break;
	}
	}
}

void Workspace::updateInterface()
{
	view.setSize(window->getSize().x * zoomLevel, window->getSize().y * zoomLevel);
	view.setCenter(viewCenter);
}

void Workspace::render()
{
	if (!window)
		return;

	sf::View tempView = window->getView();
	window->setView(view);

	if (project)
	{
		for (auto it : project->infr.stopNodes)
		{
			window->draw(it.second);
		}
		if (skeletonEnabled)
		{
			if (nodeSkeletonEnabled)
			{
				for (auto it : project->infr.wayNodes)
				{
					window->draw(it.second);
				}
			}
			for (auto it : project->infr.streetWays)
			{
				window->draw(it.second);
			}
			for (auto it : project->infr.railWays)
			{
				window->draw(it.second);
			}
		}
	}

	window->setView(tempView);
}