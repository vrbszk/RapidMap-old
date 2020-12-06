#include "Application.hpp"
#include "Log.hpp"
#include "MainMenuState.hpp"
#include "Window.hpp"
#include <Windows.h>
#include "load_data_module.hpp"



#include <thread>

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
	//delete currProject;

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
		//stateMachine.ProcessStateChanges();
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
	window->updateEvents();
	//while (window->pollEvent(event))
	//{
	//	
	//	}
	//	//stateMachine.GetActiveState()->updateEvents(event);
	//	
	//}
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

	//stateMachine.GetActiveState()->updateState(window);

	window->updateWindow();
}



void Application::render()
{
	window->render();

	//window->clear(sf::Color::White);

	//window->setView(mainView);

	//workSpace.render();

	//menuStrip.render();

	//stateBlock.render();

	//window->display();
}



void Application::initWindow()
{
	Log::makeLog("app.initWindow started...");

	//window = new sf::RenderWindow(sf::VideoMode(700, 500), "RapidMap", sf::Style::Default);
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

	//workSpace.skeletonEnabled = true;
	//workSpace.nodeSkeletonEnabled = true;
	//workSpace.setWindow(window);
	//workSpace.zoomLevel = 1;
	//workSpace.prevMousePos = sf::Mouse::getPosition(*window);

	std::unique_ptr<Workspace> work(new Workspace());
	work->skeletonEnabled = true;
	work->nodeSkeletonEnabled = true;
	work->zoomLevel = 1;
	work->prevMousePos = sf::Mouse::getPosition(*window);
	work->projectManager = projectManager;
	//work->core = this;

	window->addInterface(std::move(work));

	//menuStrip.setWindow(window);
	//menuStrip.resources = &assetManager;
	//menuStrip.init();
	
	std::unique_ptr<MenuStrip> strip(new MenuStrip());
	strip->resources = &assetManager;
	strip->init();

	window->addInterface(std::move(strip));

	//stateBlock.setWindow(window);

	std::unique_ptr<StateBlock> stateblock(new StateBlock());

	window->addInterface(std::move(stateblock));

	Log::makeLog("app.initInterfaces finished");
}



void Application::initResources()
{
	Log::makeLog("app.initResources started...");

	assetManager.LoadFont("main", "Fonts/geomid.ttf");

	Log::makeLog("app.initResources finished");
}


//
//void Application::createProject()
//{
//	if (projectManager->currProject)
//	{
//		int res = MessageBox(NULL, "Do you want to save current project?", "Create new project", MB_YESNOCANCEL);
//		switch (res)
//		{
//		case IDYES:
//			saveProject();
//			break;
//		case IDNO:
//			break;
//		case IDCANCEL:
//			return;
//		default:
//			return;
//		}
//	}
//
//	//delete currProject;
//	//currProject = new Project();
//	projectManager->currProject = std::make_unique<Project>();
//	projectManager->currProject->create(username, version);
//	//workSpace.project = currProject;
//}
//
//void Application::openProject()
//{
//	if (projectManager->currProject)
//	{
//		int res = MessageBox(NULL, "Do you want to save current project?", "Open project", MB_YESNOCANCEL);
//		switch (res)
//		{
//		case IDYES:
//			saveProject();
//			break;
//		case IDNO:
//			break;
//		case IDCANCEL:
//			return;
//		default:
//			return;
//		}
//	}
//
//	std::string path = getOpenName();
//	
//	if (path != "")
//	{
//		/*delete currProject;
//		currProject = new Project();
//		currProject->open(path);
//		workSpace.project = currProject;*/
//		projectManager->currProject = std::make_unique<Project>();
//		projectManager->currProject->open(path);
//	}
//}
//
//void Application::saveProject()
//{
//	if (projectManager->currProject->getFilePath() != "")
//		projectManager->currProject->save();
//	else
//	{
//		std::string path = getSaveName();
//		if(path != "")
//			projectManager->currProject->saveAs(path);
//	}
//}
//
//void Application::saveProjectAs()
//{
//	std::string path = getSaveName();
//	if (path != "")
//		projectManager->currProject->saveAs(path);
//}
//
//void Application::saveProjectCopy()
//{
//	std::string path = getSaveName();
//	if (path != "")
//		projectManager->currProject->saveCopy(path);
//}
//
//void Application::attachOSMData()
//{
//	if (projectManager->currProject)
//	{
//		try
//		{
//			projectManager->currProject->attachData(load_map_data());
//		}
//		catch (...)
//		{
//
//		}
//	}
//	else MessageBox(NULL, "Create a project at first", "No project opened", MB_OK);
//}
//
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

