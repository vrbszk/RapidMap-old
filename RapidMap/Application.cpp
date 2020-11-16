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
	delete interfaceRoot;

	Log::makeLog("App destroyed");
}



//CORE
void Application::run()
{
	Log::makeLog("App started");

	initAppParams();
	initUser();
	initWindow();
	initInterfaces();

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
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			exit();
			break;
		case sf::Event::Resized:
		{
			sf::Vector2f newSize(event.size.width, event.size.height);
			if (interfaceRoot)
			{
				sf::Vector2f minSize = interfaceRoot->getMinSize();
				if (newSize.x < minSize.x)
					newSize.x = minSize.x;
				if (newSize.y < minSize.y)
					newSize.y = minSize.y;
			}
			mainView = sf::View(sf::FloatRect(sf::Vector2f(0, 0), newSize));
			window->setSize(sf::Vector2u(newSize));
			workSpace->prevMousePos = sf::Mouse::getPosition(*window);
			break;
		}
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
		if (interfaceRoot)
			interfaceRoot->updateEvents(event);
		//workSpace.updateEvents(event);
		//menuStrip.updateEvents(event);
		//stateBlock.updateEvents(event);
		
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

	if (interfaceRoot)
		interfaceRoot->updateInterface(sf::FloatRect(sf::Vector2f(0, 0), mainView.getSize()));

	/*sf::FloatRect spaceLeft(sf::Vector2f(0, 0), mainView.getSize());

	sf::FloatRect stripSpace(sf::Vector2f(spaceLeft.left, spaceLeft.top), sf::Vector2f(spaceLeft.width, 20));

	menuStrip.updateInterface(stripSpace);

	spaceLeft.top = menuStrip.view.getSize().y;
	spaceLeft.height = spaceLeft.height - menuStrip.view.getSize().y;

	sf::FloatRect stateSpace(sf::Vector2f(spaceLeft.left + spaceLeft.width - 100, spaceLeft.top), sf::Vector2f(100, spaceLeft.height));

	stateBlock.updateInterface(stateSpace);

	spaceLeft.width = spaceLeft.width - stateBlock.view.getSize().x;

	workSpace.updateInterface(spaceLeft);*/
}



void Application::render()
{
	window->clear(sf::Color::White);

	window->setView(mainView);

	if (interfaceRoot)
		interfaceRoot->render();

	//workSpace.render();

	//menuStrip.render();

	//stateBlock.render();

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
	interfaceRoot = new InterfaceHolderNode();

	InterfaceHolderNode* tempRoot;

	workSpace = new Workspace();

	workSpace->skeletonEnabled = true;
	workSpace->nodeSkeletonEnabled = true;
	workSpace->setWindow(window);
	workSpace->zoomLevel = 1;
	workSpace->prevMousePos = sf::Mouse::getPosition(*window);

	menuStrip = new MenuStrip();

	menuStrip->setWindow(window);
	
	StateBlock* block = new StateBlock();

	block->setWindow(window);

	interfaceRoot->inth1 = workSpace;
	interfaceRoot->inth2 = block;

	interfaceRoot->stableNode = 1;
	interfaceRoot->borderSize = 500;
	interfaceRoot->borderVertcal = true;

	tempRoot = interfaceRoot;

	interfaceRoot = new InterfaceHolderNode();
	interfaceRoot->inth1 = menuStrip;
	interfaceRoot->inth2 = tempRoot;

	interfaceRoot->stableNode = -1;
	interfaceRoot->borderSize = 20;
	interfaceRoot->borderVertcal = false;

	interfaceRoot->getMinSize();
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
	workSpace->project = currProject;
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
		workSpace->project = currProject;
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

