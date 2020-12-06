#include "Console.hpp"

#include <iostream>
#include <chrono>

Console::Console() : t(&Console::cinThread, this)
{
	this->open();
}

Console::~Console()
{
	this->isWorking = false;
	//try { t.~thread(); }
	//catch (...) { }
	t.join();
}

void Console::open()
{
	receivingOpened = true;
	std::cout << "RapidMap > ";
}

void Console::close()
{
	receivingOpened = false;
}

bool Console::isOpened()
{
	return receivingOpened;
}

bool Console::pollCommand(std::string& command)
{
	if(commandlist.size() == 0)
		return false;

	command = commandlist.front();
	commandlist.pop();
	return true;
}

void Console::cinThread()
{
	std::string line;
	//std::cout << "Thread launched" << std::endl;
	this->isWorking = true;
	while (this->isWorking)
	{
		if(receivingOpened)
			extractCommand(line);

		if (line != "")
		{
			commandlist.push(line);
			this->close();
		}
		line = "";

		std::cin.clear();
	}

	std::cout << "Thread terminated" << std::endl;
}

void Console::extractCommand(std::string& line)
{
	if (std::cin.rdbuf()->in_avail())
	{
		std::getline(std::cin, line);
	}
	else std::this_thread::sleep_for(std::chrono::milliseconds(100));
}