#include "Application.hpp"
#include "Log.hpp"
#include "MainMenuState.hpp"
#include <Windows.h>
#include "load_data_module.hpp"

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
		switch (event.type)
		{
		case sf::Event::Closed:
			exit();
			break;
		case sf::Event::KeyPressed:
		{
			switch (event.key.code)
			{
			case sf::Keyboard::N:
				if (event.key.control) createProject();
				break;
			case sf::Keyboard::O:
				if (event.key.control) openProject();
				break;
			case sf::Keyboard::S:
				if (event.key.control && !event.key.alt) saveProject();
				else if (event.key.control && event.key.alt) saveProjectAs();
				else if (!event.key.control && event.key.alt) saveProjectCopy();
				break;
			case sf::Keyboard::T:
				if (event.key.control) attachOSMData();
				break;
			}
			break;
		}
		}
		stateMachine.GetActiveState()->updateEvents(event);
	}
}



void Application::updateEnvironment()
{
	isRunning = window->isOpen();

	if (currProject)
		window->setTitle(currProject->getName() + " / RapidMap " + version + ", user: " + username);
	else
		window->setTitle(std::string("No project opened") + " / RapidMap " + version + ", user: " + username);

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





void Application::createProject()
{
	if (currProject)
	{
		int res = MessageBox(NULL, "Do you want to save current project?", "Create new project", MB_YESNOCANCEL);
		switch (res)
		{
		case IDYES:
			saveProject();
			break;
		case IDNO:
			break;
		case IDCANCEL:
			return;
		default:
			return;
		}
	}

	delete currProject;
	currProject = new Project();
	currProject->create(username, version);
}

void Application::openProject()
{
	if (currProject)
	{
		int res = MessageBox(NULL, "Do you want to save current project?", "Open project", MB_YESNOCANCEL);
		switch (res)
		{
		case IDYES:
			saveProject();
			break;
		case IDNO:
			break;
		case IDCANCEL:
			return;
		default:
			return;
		}
	}

	OPENFILENAME ofn = getRapidMapOFN();

	if (GetOpenFileName(&ofn))
	{
		std::string path = ofn.lpstrFile;
		delete currProject;
		currProject = new Project();
		currProject->open(path);
	}
}

void Application::saveProject()
{
	if (currProject->getFilePath() != "")
		currProject->save();
	else
	{
		OPENFILENAME ofn = getRapidMapOFN();

		if (GetOpenFileName(&ofn))
		{
			std::string path = ofn.lpstrFile;
			currProject->saveAs(path);
		}
	}
}

void Application::saveProjectAs()
{
	OPENFILENAME ofn = getRapidMapOFN();

	if (GetSaveFileName(&ofn))
	{
		std::string path = ofn.lpstrFile;
		currProject->saveAs(path);
	}
}

void Application::saveProjectCopy()
{
	OPENFILENAME ofn = getRapidMapOFN();

	if (GetOpenFileName(&ofn))
	{
		std::string path = ofn.lpstrFile;
		currProject->saveCopy(path);
	}
}

void Application::exit()
{
	if (currProject)
	{
		int res = MessageBox(NULL, "Do you want to save current project?", "Exit", MB_YESNOCANCEL);
		switch (res)
		{
		case IDYES:
			saveProject();
			break;
		case IDNO:
			break;
		case IDCANCEL:
			return;
		default:
			break;
		}
	}

	window->close();
}

void Application::attachOSMData()
{
	currProject->attachData(load_map_data());
}