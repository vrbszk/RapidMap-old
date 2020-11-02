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



std::string getCurrTimestamp()
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

void mapLog(std::string text)
{
	std::ofstream logFile("log.ini", std::ios::app);
	logFile << text << "\n";
	logFile << getCurrTimestamp() << " : " << text << "\n";
	logFile.close();
}

void clearMapLog()
{
	std::ofstream logFile("log.ini");
	logFile << "Starting app..." << "\n";
	logFile << getCurrTimestamp() << "Starting app..." << "\n";
	logFile.close();
}

void launchTiming(std::string sourceName)
{
	std::ofstream timeCheck("time.ini", std::ios::app);
	timeCheck << std::endl << getCurrTimestamp() << " " << std::setw(25) << sourceName;
	timeCheck.close();
}

void timeLog()
{
	static time_t last = time(0);
	time_t now = time(0);
	int dt = now - last;
	//char buf[30];
	//struct tm tstruct;
	//localtime_s(&tstruct, &now);
	//strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	std::string timestr;
	int s = dt % 60;
	dt = dt / 60;
	int m = dt % 60;
	dt = dt / 60;
	int h = dt % 60;
	if (s >= 10)
		timestr = std::to_string(s);
	else
		timestr = "0" + std::to_string(s);

	if (m >= 10)
		timestr = std::to_string(m) + ":" + timestr;
	else
		timestr = "0" + std::to_string(m) + ":" + timestr;

	if (h >= 10)
		timestr = std::to_string(h) + ":" + timestr;
	else
		timestr = "0" + std::to_string(h) + ":" + timestr;

	std::ofstream timeCheck("time.ini", std::ios::app);
	timeCheck << " " << std::setw(8) << timestr;
	timeCheck.close();
	last = now;
}

