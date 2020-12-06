#include "Application.hpp"
#include "Log.hpp"
#include "MainMenuState.hpp"
#include <Windows.h>
#include "load_data_module.hpp"

#include <thread>

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

void x()
{
	
}

//CORE
void Application::run()
{
	Log::makeLog("App started");

	initAppParams();
	initResources();
	initUser();
	initWindow();
	initInterfaces();

	isRunning = true;
	
	Log::makeLog("Init finished");

	//std::thread t(x);

	while (isRunning)
	{
		stateMachine.ProcessStateChanges();
		update();
		render();
	}

	

	//t.join();
}



void Application::update()
{
	updateEvents();
	updateCommands();
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
		case sf::Event::Resized:
		{
			mainView = sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height));
			window->setSize(sf::Vector2u(event.size.width, event.size.height));
			workSpace.prevMousePos = sf::Mouse::getPosition(*window);
			break;
		}
		case sf::Event::KeyPressed:
		{
			switch (event.key.code)
			{
			case sf::Keyboard::L:
			{
				workSpace.zoomLevel = 1;
				break;
			}
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
		workSpace.updateEvents(event);
		menuStrip.updateEvents(event);
		stateBlock.updateEvents(event);
		
	}
}


void Application::updateCommands()
{
	std::string command;
	while (console.pollCommand(command))
	{
		std::cout << "New command extracted: " << command << std::endl;
	}
	console.open();
}


void Application::updateEnvironment()
{
	isRunning = window->isOpen();

	if (currProject)
		window->setTitle(currProject->getName() + " / RapidMap " + version + ", user: " + username);
	else
		window->setTitle(std::string("No project opened") + " / RapidMap " + version + ", user: " + username);

	stateMachine.GetActiveState()->updateState(window);

	sf::FloatRect spaceLeft(sf::Vector2f(0, 0), mainView.getSize());

	sf::FloatRect stripSpace(sf::Vector2f(spaceLeft.left, spaceLeft.top), sf::Vector2f(spaceLeft.width, 20));

	menuStrip.updateInterface(stripSpace);

	spaceLeft.top = menuStrip.view.getSize().y;
	spaceLeft.height = spaceLeft.height - menuStrip.view.getSize().y;

	sf::FloatRect stateSpace(sf::Vector2f(spaceLeft.left + spaceLeft.width - 100, spaceLeft.top), sf::Vector2f(100, spaceLeft.height));

	stateBlock.updateInterface(stateSpace);

	spaceLeft.width = spaceLeft.width - stateBlock.view.getSize().x;

	workSpace.updateInterface(spaceLeft);
}



void Application::render()
{
	window->clear(sf::Color::White);

	window->setView(mainView);

	workSpace.render();

	menuStrip.render();

	stateBlock.render();

	window->display();
}



void Application::initWindow()
{
	Log::makeLog("app.initWindow started...");

	window = new sf::RenderWindow(sf::VideoMode(700, 500), "RapidMap", sf::Style::Default);

	mainView = window->getDefaultView();

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

	Log::makeLog("app.initAppParams finished");
}



void Application::initInterfaces()
{
	Log::makeLog("app.initInterfaces started...");

	workSpace.skeletonEnabled = true;
	workSpace.nodeSkeletonEnabled = true;
	workSpace.setWindow(window);
	workSpace.zoomLevel = 1;
	workSpace.prevMousePos = sf::Mouse::getPosition(*window);

	menuStrip.setWindow(window);
	menuStrip.resources = &assetManager;
	menuStrip.init();
	
	stateBlock.setWindow(window);

	Log::makeLog("app.initInterfaces finished");
}



void Application::initResources()
{
	Log::makeLog("app.initResources started...");

	assetManager.LoadFont("main", "Fonts/geomid.ttf");

	Log::makeLog("app.initResources finished");
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
	workSpace.project = currProject;
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
		workSpace.project = currProject;
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

