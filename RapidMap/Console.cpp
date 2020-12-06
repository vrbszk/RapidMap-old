#include "Console.hpp"

#include <iostream>

Console::Console() : t(&Console::cinThread, this)
{
	
}

Console::~Console()
{
	this->isWorking = false;
	t.join();
}

void Console::open()
{
	receivingOpened = true;
}

void Console::close()
{
	receivingOpened = false;
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
	std::cout << "RapidMap > ";
	std::getline(std::cin, line);
}