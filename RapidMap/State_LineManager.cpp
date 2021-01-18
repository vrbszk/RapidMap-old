#include "State_LineManager.hpp"

void State_LineManager::init()
{

}



void State_LineManager::processEvents(sf::Event event)
{
	sf::View tempView = window->getView();
	window->setView(view);



	window->setView(tempView);
}



void State_LineManager::update()
{
	sf::View tempView = window->getView();
	window->setView(view);



	window->setView(tempView);
}



void State_LineManager::render()
{
	sf::View tempView = window->getView();
	window->setView(view);

	if (line && projectManager->currProject)
	{
		sf::Text header(line->name, assetManager->GetFont("main"), 20);
		header.setFillColor(sf::Color::Black);
		header.setPosition(10, 20);

		window->draw(header);

		sf::String str;
		for (auto it : line->stationids)
		{
			std::string name = projectManager->currProject->infr.stopNodes[it].name;
			
			str += it + ": " + sf::String::fromUtf8(name.begin(), name.end()) + "\n";
		}
		sf::Text temp(str, assetManager->GetFont("main"), 10);
		temp.setFillColor(sf::Color::Black);
		temp.setPosition(10, 50);

		window->draw(temp);
	}



	window->setView(tempView);
}



void State_LineManager::setLine(Line* l)
{
	line = l;
}



void State_LineManager::refreshData()
{

}