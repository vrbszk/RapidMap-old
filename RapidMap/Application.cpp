#include "Application.hpp"

Application::Application()
{

}

Application::~Application()
{

}

//CORE
void Application::run()
{
	isRunning = true;

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
	
}

void Application::updateEnvironment()
{

}

void Application::render()
{

}

