#pragma once

class State_LinesViewer;
class TextPanel;

#include "State.hpp"

class TextPanel : public Entity
{
	/*void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(sf::Vector2f mousePos);*/
};

class State_LinesViewer : public State
{
public:
	void init();
	void processEvents(sf::Event event);
	void update();
	void render();

	void refreshPanels();

	std::vector<sf::Text> textPanels;
};