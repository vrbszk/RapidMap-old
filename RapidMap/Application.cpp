#include "Application.hpp"
#include "Log.hpp"

Application::Application()
{
	Log::clearLog();

	Log::makeLog("App created");
}

Application::~Application()
{
	delete window;

	Log::makeLog("App destroyed");
}

//CORE
void Application::run()
{
	Log::makeLog("App started");

	isRunning = true;

	initWindow();

	Log::makeLog("Init finished");

	while (isRunning)
	{
		update();
		render();
	}
}

void Application::update()
{
	updateEvents();

	updateEnvironment();
}

void Application::updateEvents()
{
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window->close();
	}
}

void Application::updateEnvironment()
{
	isRunning = window->isOpen();
}

void Application::render()
{

}

void Application::initWindow()
{
	Log::makeLog("app.initWindow started...");

	window = new sf::RenderWindow(sf::VideoMode(700, 500), "RapidMap", sf::Style::Default);

	Log::makeLog("app.initWindow finished");
}