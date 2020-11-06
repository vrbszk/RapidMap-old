#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#include <sstream>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <stdio.h>


#include <Windows.h>


#include "Log.hpp"

std::string Log::path = "";

std::string Log::getCurrTimestamp()
{
	auto times = std::chrono::system_clock::now();
	std::time_t stamp = std::chrono::system_clock::to_time_t(times);
	char buf[30];
	time_t now = time(0);
	struct tm tstruct;
	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	return buf;
}



void Log::makeLog(std::string text)
{
	std::ofstream logFile(path, std::ios::app);
	logFile << getCurrTimestamp() << " : " << text << "\n";
	logFile.close();
}



void Log::clearLog()
{
	char szFileName[250];
	GetModuleFileName(NULL, szFileName, 250);

	path = szFileName;
	auto it = path.find_last_of('\\');
	path = path.substr(0, it + 1);
	path += "log.ini";

	std::ofstream logFile(path);
	logFile << getCurrTimestamp() << " : " << "Starting app..." << "\n";
	logFile.close();
}
