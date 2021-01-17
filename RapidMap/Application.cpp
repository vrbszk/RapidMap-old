#include "Application.hpp"

#include <Windows.h>
#include <thread>
#include <typeinfo>

#include "Log.hpp"
#include "MainMenuState.hpp"
#include "Window.hpp"
#include "load_data_module.hpp"

#include "State_Workspace.hpp"
#include "State_MenuStrip.hpp"
#include "State_ToolStrip.hpp"
#include "State_LineViewer.hpp"

Application::Application() 
{
	assetManager = std::make_shared<AssetManager>();

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
	window->setHintFont(assetManager->GetFont("main"));

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

	version = RAPIDMAP_FILE_VERSION;

	Log::makeLog("app.initAppParams finished");
}



void Application::initInterfaces()
{
	Log::makeLog("app.initInterfaces started...");

	std::unique_ptr<Interface> interfacer;// = std::make_unique<Interface>();
	/*std::unique_ptr<MainMenuState> mms = std::make_unique<MainMenuState>();
	mms->init();
	interfacer->setState(std::move(mms));
	window->addInterface(std::move(interfacer));*/

	std::unique_ptr<State_Workspace> work = std::make_unique<State_Workspace>();
	work->setAssetManager(assetManager);
	work->setProjectManager(projectManager);
	work->skeletonEnabled = true;
	work->nodeSkeletonEnabled = true;
	work->zoomLevel = 1;
	work->prevMousePos = sf::Mouse::getPosition(*window);

	work->init();

	interfacer = std::make_unique<Interface>();
	interfacer->setState(std::move(work));

	window->addInterface(std::move(interfacer));


	
	std::unique_ptr<State_MenuStrip> strip = std::make_unique<State_MenuStrip>();
	strip->setAssetManager(assetManager);
	strip->setProjectManager(projectManager);

	strip->init();

	interfacer = std::make_unique<Interface>();
	interfacer->setState(std::move(strip));

	window->addInterface(std::move(interfacer));



	/*std::unique_ptr<MainMenuState> mms = std::make_unique<MainMenuState>();
	mms->setAssetManager(assetManager);
	mms->setProjectManager(projectManager);

	mms->init();

	interfacer = std::make_unique<Interface>();
	interfacer->setState(std::move(mms));

	window->addInterface(std::move(interfacer));*/

	std::unique_ptr<State_LineViewer> viewer = std::make_unique<State_LineViewer>();
	viewer->setAssetManager(assetManager);
	viewer->setProjectManager(projectManager);

	viewer->init();

	interfacer = std::make_unique<Interface>();
	interfacer->setState(std::move(viewer));

	window->addInterface(std::move(interfacer));



	std::unique_ptr<State_ToolStrip> toolstrip = std::make_unique<State_ToolStrip>();
	toolstrip->setAssetManager(assetManager);
	toolstrip->setProjectManager(projectManager);

	toolstrip->init();

	interfacer = std::make_unique<Interface>();
	interfacer->setState(std::move(toolstrip));

	window->addInterface(std::move(interfacer));

	Log::makeLog("app.initInterfaces finished");
}



void Application::initResources()
{
	Log::makeLog("app.initResources started...");

	assetManager->LoadFont("main", "Fonts/geomid.ttf");

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