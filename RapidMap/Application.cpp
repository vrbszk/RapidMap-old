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

	initAppParams();
	initUser();
	initWindow();

	isRunning = true;
	
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
	static sf::Vector2i prevMousePos = sf::Mouse::getPosition(*window);

	while (window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			exit();
			break;
		case sf::Event::Resized:
		{
			sf::View view = window->getView();
			view.setSize(event.size.width * zoomLevel, event.size.height * zoomLevel);
			window->setSize(sf::Vector2u(event.size.width, event.size.height));
			window->setView(view);
			break;
		}
		case sf::Event::MouseMoved:
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				sf::View view = window->getView();
				view.move((prevMousePos.x - event.mouseMove.x) * zoomLevel, (prevMousePos.y - event.mouseMove.y) * zoomLevel);
				window->setView(view);
			}
			prevMousePos.x = event.mouseMove.x;
			prevMousePos.y = event.mouseMove.y;
			break;
		}
		case sf::Event::MouseWheelScrolled:
		{
			if (event.mouseWheelScroll.delta < 0)
				zoomLevel *= 2;
			else if (event.mouseWheelScroll.delta > 0)
				zoomLevel /= 2;

			sf::View view;
			view.setCenter(window->getView().getCenter());
			view.setSize(window->getSize().x * zoomLevel, window->getSize().y * zoomLevel);
			window->setView(view);
			break;
		}
		case sf::Event::KeyPressed:
		{
			switch (event.key.code)
			{
			case sf::Keyboard::E:
				skeletonEnabled = !skeletonEnabled;
				break;
			case sf::Keyboard::L:
			{
				zoomLevel = 1;
				sf::View view;
				view.setSize(window->getSize().x, window->getSize().y);
				view.setCenter(0, 0);
				window->setView(view);
				break;
			}
			case sf::Keyboard::N:
				if (event.key.control) createProject();
				else nodeSkeletonEnabled = !nodeSkeletonEnabled;
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
	if (currProject)
	{
		for (auto it : currProject->infr.stopNodes)
		{
			window->draw(it.second);
		}
		if (skeletonEnabled)
		{
			if(nodeSkeletonEnabled)
			{
				for (auto it : currProject->infr.wayNodes)
				{
					window->draw(it.second);
				}
			}
			for (auto it : currProject->infr.streetWays)
			{
				window->draw(it.second);
			}
			for (auto it : currProject->infr.railWays)
			{
				window->draw(it.second);
			}
		}
	}

	window->display();
}





void Application::initWindow()
{
	Log::makeLog("app.initWindow started...");

	window = new sf::RenderWindow(sf::VideoMode(700, 500), "RapidMap", sf::Style::Default);

	zoomLevel = 1;

	sf::View view;
	view.setSize(window->getSize().x * zoomLevel, window->getSize().y * zoomLevel);
	view.setCenter(0, 0);
	window->setView(view);

	StatePtr menuState(new MainMenuState());
	stateMachine.AddState(std::move(menuState));

	Log::makeLog("app.initWindow finished");
}



void Application::initUser()
{
	Log::makeLog("app.initUser started...");

	username = "app";

	Log::makeLog("app.initUser finished");
}



void Application::initAppParams()
{
	Log::makeLog("app.initAppParams started...");

	version = "v0.1";

	skeletonEnabled = true;
	nodeSkeletonEnabled = true;

	Log::makeLog("app.initAppParams finished");
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

	std::string path = getOpenName();
	
	if (path != "")
	{
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
		std::string path = getSaveName();
		if(path != "")
			currProject->saveAs(path);
	}
}

void Application::saveProjectAs()
{
	std::string path = getSaveName();
	if (path != "")
		currProject->saveAs(path);
}

void Application::saveProjectCopy()
{
	std::string path = getSaveName();
	if (path != "")
		currProject->saveCopy(path);
}

void Application::attachOSMData()
{
	if (currProject)
	{
		try
		{
			currProject->attachData(load_map_data());
		}
		catch (...)
		{

		}
	}
	else MessageBox(NULL, "Create a project at first", "No project opened", MB_OK);
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

