#include "Application.hpp"

#include <Windows.h>
#include <thread>

#include "Log.hpp"
#include "MainMenuState.hpp"
#include "Window.hpp"
#include "load_data_module.hpp"

#include "Workspace.hpp"
#include "MenuStrip.hpp"
#include "StateBlock.hpp"

Application::Application() 
{
	projectManager = std::make_shared<ProjectManager>();
	projectManager->core = this;

	Log::clearLog();

	Log::makeLog("App created");
}



Application::~Application()
{
	delete window;

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

	while (isRunning)
	{
		//stateMachine.ProcessStateChanges();
		update();
		render();
	}
}



void Application::update()
{
	updateEvents();
	updateCommands();
	updateEnvironment();
}



void Application::updateEvents()
{
	window->updateEvents();
}


void Application::updateCommands()
{
	std::string command;
	while (console.pollCommand(command))
	{
		std::cout << "New command extracted: " << command << std::endl;
	}
	if(!console.isOpened())
		console.open();
}


void Application::updateEnvironment()
{
	isRunning = window->isOpen();

	if (projectManager->currProject)
		window->setTitle(projectManager->currProject->getName() + " / RapidMap " + version + ", user: " + username);
	else
		window->setTitle(std::string("No project opened") + " / RapidMap " + version + ", user: " + username);

	window->updateWindow();
}



void Application::render()
{
	window->render();
}



void Application::initWindow()
{
	Log::makeLog("app.initWindow started...");

	window = new Window(sf::VideoMode(700, 500), "RapidMap", sf::Style::Default);
	window->core = this;

	//StatePtr menuState(new MainMenuState());
	//stateMachine.AddState(std::move(menuState));

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

	std::unique_ptr<Workspace> work(new Workspace());
	work->skeletonEnabled = true;
	work->nodeSkeletonEnabled = true;
	work->zoomLevel = 1;
	work->prevMousePos = sf::Mouse::getPosition(*window);
	work->projectManager = projectManager;

	window->addInterface(std::move(work));
	
	std::unique_ptr<MenuStrip> strip(new MenuStrip());
	strip->resources = &assetManager;
	strip->init();

	window->addInterface(std::move(strip));

	std::unique_ptr<StateBlock> stateblock = std::make_unique<StateBlock>();//(new StateBlock());

	StatePtr menuState = std::make_unique<MainMenuState>();
	
	stateblock->stateList.AddState(std::move(menuState));

	window->addInterface(std::move(stateblock));

	Log::makeLog("app.initInterfaces finished");
}



void Application::initResources()
{
	Log::makeLog("app.initResources started...");

	assetManager.LoadFont("main", "Fonts/geomid.ttf");

	Log::makeLog("app.initResources finished");
}



void Application::closeWindow(Window* win)
{
	if (win == window)
	{
		if (this->exit())
			win->close();
		else return;
	}
}



bool Application::exit()
{
	if (projectManager->currProject)
	{
		int res = MessageBox(NULL, "Do you want to save current project?", "Exit", MB_YESNOCANCEL);
		switch (res)
		{
		case IDYES:
			projectManager->saveProject();
			break;
		case IDNO:
			break;
		case IDCANCEL:
			return false;
		default:
			break;
		}
	}

	return true;
}