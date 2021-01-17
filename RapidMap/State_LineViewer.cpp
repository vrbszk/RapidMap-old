#include "State_LineViewer.hpp"

void State_LineViewer::init()
{

}



void State_LineViewer::processEvents(sf::Event event)
{

}



void State_LineViewer::update()
{

}



void State_LineViewer::render()
{
	if (!projectManager->currProject)
		return;


	sf::String str;
	if (projectManager->currProject->schemedata.lines.size() == 0)
		str = "--- no lines ---";
	else for (auto it : projectManager->currProject->schemedata.lines)
	{
		str += it.name + "\n";
	}

	sf::Text text(str, assetManager->GetFont("main"), 10);
	text.setFillColor(sf::Color::Black);
	text.setPosition(10, 10);

	sf::View tempView = window->getView();
	window->setView(view);

	window->draw(text);

	window->setView(tempView);
}