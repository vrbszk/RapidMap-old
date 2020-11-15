#pragma once
#include <SFML/Graphics.hpp>

#include "Project.hpp"
#include "StateMachine.hpp"
#include "AssetManager.hpp"
#include "Interface.hpp"
#include "Workspace.hpp"
#include "MenuStrip.hpp"

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

	//Subcore modules
	void updateEvents(); // processing events module
	void updateEnvironment(); //updating application module
	void update(); //updating base
	void render(); //rendering module
	void exit();


	void createProject();
	void openProject();
	void saveProject();
	void saveProjectAs();
	void saveProjectCopy();
	void attachOSMData();


private:
	bool isRunning;


	sf::RenderWindow* window;
	sf::Event event;
	sf::View mainView;

	std::string username;
	std::string version;

	StateMachine stateMachine;
	AssetManager assetManager;
	std::vector<InterfacePtr> interfaces;
	MenuStrip menuStrip;
	Workspace workSpace;


	Project* currProject;
};