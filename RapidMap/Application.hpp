#pragma once
#include <SFML/Graphics.hpp>

class Application;

#include "Project.hpp"
#include "AssetManager.hpp"
#include "Console.hpp"
#include "Window.hpp"

class Application
{
public:

	Application();
	~Application();

	//Functioning core
	void run();

	void closeWindow(Window* win);

private:
	//Init modules
	void initWindow();
	void initUser();
	void initAppParams();
	void initInterfaces();
	void initResources();

	//Subcore modules
	void updateEvents(); // processing events module
	void updateCommands(); // processing commands
	void updateEnvironment(); //updating application module
	void update(); //updating base
	void render(); //rendering module
	bool exit();

public:
	std::string username;
	std::string version;

private:
	bool isRunning;

	Console console;

	Window* window;

	//StateMachine stateMachine;
	AssetManager assetManager;

	std::shared_ptr<ProjectManager> projectManager;
};