#pragma once

class Application;

#include <SFML/Graphics.hpp>

#include "Project.hpp"
#include "StateMachine.hpp"
#include "AssetManager.hpp"
#include "Interface.hpp"
#include "Workspace.hpp"
#include "MenuStrip.hpp"
#include "StateBlock.hpp"
#include "Console.hpp"
#include "Window.hpp"

class Application
{
public:

	Application();
	~Application();

	//Functioning core
	void run();

private:
	//Init modules
	void initWindow();
	void initUser();
	void initAppParams();
	void initInterfaces();
	void initResources();

	//Subcore modules
	void updateEvents(); // processing events module
	void updateCommands();
	void updateEnvironment(); //updating application module
	void update(); //updating base
	void render(); //rendering module
	bool exit();

public:
	//void createProject();
	//void openProject();
	//void saveProject();
	//void saveProjectAs();
	//void saveProjectCopy();
	//void attachOSMData();

	void closeWindow(Window* win);

private:
	bool isRunning;

	Console console;

	//sf::RenderWindow* window;
	Window* window;
	//sf::Event event;
	//sf::View mainView;

public:
	std::string username;
	std::string version;

private:
	//StateMachine stateMachine;
	AssetManager assetManager;
	std::vector<InterfacePtr> interfaces;
	//MenuStrip menuStrip;
	//Workspace workSpace;
	//StateBlock stateBlock;
	


	//Project* currProject;
	std::shared_ptr<ProjectManager> projectManager;
};