#pragma once
#include <queue>
#include <string>
#include <thread>

class Console
{
public:
	Console();
	~Console();

	void open();
	void close();

	bool isOpened();

	bool pollCommand(std::string& command);
private:
	std::queue<std::string> commandlist;
	std::thread t;

	void cinThread();

	void extractCommand(std::string& line);

	bool isWorking = true;
	bool receivingOpened = true;
};