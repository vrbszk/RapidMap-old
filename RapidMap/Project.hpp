#pragma once
#include <string>
#include "OSM.hpp"

class Project
{
public:
	Project();
	~Project();

	void create(const std::string& username, const std::string& v); //init new project
	void open(const std::string& filepath);
	void save();
	void saveAs(const std::string& filepath);
	void saveCopy(const std::string& filepath);
	void attachData(OSM_Data data);

	std::string getName();
	std::string getFilePath();

private:
	void saveProject(const std::string& filepath);

private:
	std::string path;

	bool saved;

	std::string fileName;

	std::string name;
	std::string creator;
	std::string version;
	std::string time_created;
	std::string time_edited;
};