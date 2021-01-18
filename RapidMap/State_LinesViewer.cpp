#include "State_LinesViewer.hpp"

#include "State_LineManager.hpp"



void State_LinesViewer::init()
{

}



void State_LinesViewer::processEvents(sf::Event event)
{
	sf::View tempView = window->getView();
	window->setView(view);

	sf::Vector2f viewMousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	if (event.type == sf::Event::MouseButtonPressed)
	{
		for (int i = 0; i < textPanels.size(); i++)
		{
			if (textPanels[i].getGlobalBounds().contains(viewMousePos))
			{
				std::unique_ptr<State_LineManager> state = std::make_unique<State_LineManager>();
				state->setAssetManager(assetManager);
				state->setProjectManager(projectManager);

				if (projectManager->currProject)
				{
					state->setLine(&projectManager->currProject->schemedata.lines[i]);
				}

				state->init();

				this->holder->setState(std::move(state));
				return;
			}
		}
	}

	window->setView(tempView);
}



void State_LinesViewer::update()
{
	sf::View tempView = window->getView();
	window->setView(view);

	if (projectManager->currProject)
	{
		if (projectManager->currProject->schemedata.lines.size() != textPanels.size())
			refreshPanels();
	}

	sf::Vector2f viewMousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	for (int i = 0; i < textPanels.size(); i++)
	{
		textPanels[i].setFillColor(sf::Color::Black);
		if (textPanels[i].getGlobalBounds().contains(viewMousePos))
			textPanels[i].setFillColor(sf::Color::Red);
	}

	window->setView(tempView);
}



void State_LinesViewer::render()
{

	sf::View tempView = window->getView();
	window->setView(view);

	for (auto it : textPanels)
		window->draw(it);

	window->setView(tempView);
}



void State_LinesViewer::refreshPanels()
{
	textPanels.clear();
	if (!projectManager->currProject)
		return;
	if (projectManager->currProject->schemedata.lines.size() == 0)
		return;
	for (auto it : projectManager->currProject->schemedata.lines)
	{
		sf::Text text(it.name, assetManager->GetFont("main"), 10);
		text.setFillColor(sf::Color::Black);
		text.setPosition(10, 15 * textPanels.size() + 10);
		textPanels.push_back(text);
	}
}