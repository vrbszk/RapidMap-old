#pragma once
#include <SFML\Graphics.hpp>
#include <memory>

class StateBox; //UI module for holding and displaying states

#include "Interface.hpp"
#include "Window.hpp"

class StateBox : public Interface
{
public:
	StateBox() { }

	//void setWindow(Window* win);
	//Window* getWindow();

	void processEvents(sf::Event event);
	void update();


	void updateEvents(sf::Event e){}
	void updateInterface(sf::FloatRect space) { setView(space); }
	void render();


	//void draw(sf::RenderTarget& target, sf::RenderStates states);


	void setView(sf::FloatRect space);
	sf::View getView();

private:
	//Window* window;

	sf::View boxView;
};