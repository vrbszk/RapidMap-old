#include <fstream>

#include "Project.hpp"
#include "Log.hpp"


Project::Project()
{
	saved = false;
}

Project::~Project()
{

}

void Project::create(const std::string& username, const std::string& v)
{
	Log::makeLog("Creating project " + v + "...");

	path = "";

	fileName = "unknown.rmp";

	name = "Unknown";
	creator = username;
	version = v;
	time_created = Log::getCurrTimestamp();
	time_edited = Log::getCurrTimestamp();
	saved = false;

	Log::makeLog("Project created");
}

void Project::open(const std::string& filepath)
{
	Log::makeLog("Opening an existing project " + filepath);

	

	path = filepath;
	std::ifstream file;
	file.open(path);

	file >> name;
	file >> creator;
	file >> version;
	file >> time_created;
	file >> time_edited;

	file.close();

	saved = true;

	Log::makeLog("Project opened");
}

void Project::save()
{
	saveProject(path);
}

void Project::saveAs(const std::string& filepath)
{
	saveProject(filepath);
	path = filepath;
}

void Project::saveCopy(const std::string& filepath)
{
	saveProject(filepath);
}

void Project::saveProject(const std::string& filepath)
{
	Log::makeLog("Saving a project to " + filepath);

	std::ofstream file;
	file.open(filepath);
	
	file << name << std::endl;
	file << creator << std::endl;
	file << version << std::endl;
	file << time_created << std::endl;
	file << time_edited << std::endl;

	file.close();

	saved = true;

	Log::makeLog("Project saved");
}

void Project::attachData(OSM_Data data)
{

}

std::string Project::getName()
{
	return name + " - " + fileName + ((saved) ? "*" : "");
}

std::string Project::getFilePath()
{
	return path;
}

