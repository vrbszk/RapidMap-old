#pragma once

#include <string>

class Log
{
	static std::string path;
public:
	static std::string getCurrTimestamp();
	static void makeLog(std::string text);
	static void clearLog();
};