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
			viewCenter.x = viewCenter.x + (prevMousePos.x - e.mouseMove.x) * zoomLevel;
			viewCenter.y = viewCenter.y + (prevMousePos.y - e.mouseMove.y) * zoomLevel;
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
		}
		else if (e.mouseWheelScroll.delta > 0)
		{
			zoomLevel /= 2;
		}
		break;
	}
	}
}

void Workspace::updateInterface(sf::FloatRect space)
{
	//view.setSize(window->getSize().x * zoomLevel, window->getSize().y * zoomLevel);
	view.setSize(space.width * zoomLevel, space.height * zoomLevel);
	view.setCenter(viewCenter);
	view.setViewport(sf::FloatRect(space.left / window->getSize().x, space.top / window->getSize().y,
		space.width / window->getSize().x, space.height / window->getSize().y)); // space в даному випадку має остаточні розміри, а значить не потрібно прив'язуватися до викривленого view
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