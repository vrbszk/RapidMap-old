#pragma once
#include <string>

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
private:
	void saveProject(const std::string& filepath);

private:
	std::string path;

	bool saved;

	std::string name;
	std::string creator;
	std::string version;
	std::string time_created;
	std::string time_edited;
};