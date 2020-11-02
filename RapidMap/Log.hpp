#pragma once

#include <string>

class Log
{
public:
	static std::string getCurrTimestamp();
	static void makeLog(std::string text);
	static void clearLog();
};