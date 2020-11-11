#pragma once
#include <SFML/Graphics.hpp>

#include "Project.hpp"
#include "StateMachine.hpp"
#include "AssetManager.hpp"

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
	bool skeletonEnabled;

	sf::RenderWindow* window;
	sf::Event event;

	float zoomLevel;

	std::string username;
	std::string version;

	StateMachine stateMachine;
	AssetManager assetManager;

	Project* currProject;
};