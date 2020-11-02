#pragma once
#include <SFML/Graphics.hpp>

class Application
{
public:

	Application();
	~Application();

	//Functioning core
	void run();

private:
	//Init modules
	void initWindow();

	//Subcore modules
	void updateEvents(); // processing events module
	void updateEnvironment(); //updating application module
	void update(); //updating base
	void render(); //rendering module

private:
	bool isRunning;

	sf::RenderWindow* window;
	sf::Event event;
};