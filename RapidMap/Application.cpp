#include "Application.hpp"
#include "Log.hpp"
#include "MainMenuState.hpp"

Application::Application()
{
	Log::clearLog();

	Log::makeLog("App created");
}

Application::~Application()
{
	delete window;
	delete currProject;

	Log::makeLog("App destroyed");
}

//CORE
void Application::run()
{
	Log::makeLog("App started");

	isRunning = true;

	initAppParams();
	initUser();
	initWindow();
	
	Log::makeLog("Init finished");

	while (isRunning)
	{
		stateMachine.ProcessStateChanges();
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
		stateMachine.GetActiveState()->updateEvents(event);
	}
}

void Application::updateEnvironment()
{
	isRunning = window->isOpen();

	stateMachine.GetActiveState()->updateState(window);
}

void Application::render()
{
	window->clear(sf::Color::White);

	stateMachine.GetActiveState()->render(window);

	window->display();
}

void Application::initWindow()
{
	Log::makeLog("app.initWindow started...");

	window = new sf::RenderWindow(sf::VideoMode(700, 500), "RapidMap", sf::Style::Default);

	StatePtr menuState(new MainMenuState());
	stateMachine.AddState(std::move(menuState));

	Log::makeLog("app.initWindow finished");
}

void Application::initUser()
{
	username = "app";
}

void Application::initAppParams()
{
	version = "v0.1";
}