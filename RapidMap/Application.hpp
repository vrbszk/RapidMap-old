#pragma once


class Application
{
public:

	Application();
	~Application();

	//Functioning core
	void run();

private:
	//Subcore modules
	void updateEvents(); // processing events module
	void updateEnvironment(); //updating application module
	void update(); //updating base
	void render(); //rendering module

private:
	bool isRunning;
};