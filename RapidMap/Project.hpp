#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <memory>

class StreetNode;
class Stop;
class Street;
class Railway;
class CityInfrastructure;

class Project;
class ProjectManager;

#include "OSM.hpp"
#include "Application.hpp"

StreetNode node_to_street(OSM_Element el, sf::Vector2f midPos);
Stop node_to_stop(OSM_Element el, sf::Vector2f midPos);
sf::Vector2f getMidPos(OSM_Bounds bounds);

class StreetNode : public sf::Drawable, public sf::Transformable
{
public:
	StreetNode();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::string osmID;

private:
	sf::CircleShape shape;
};



class Stop : public sf::Drawable, public sf::Transformable
{
public:
	Stop();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::string osmID;

private:
	sf::CircleShape shape;
};



class Street : public sf::Drawable
{
public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::VertexArray path;

	std::string osmID;
	std::vector<std::string> nodeids;
};



class Railway : public sf::Drawable
{
public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::VertexArray path;

	std::string osmID;
	std::string railType;
	std::vector<std::string> nodeids;
};



class CityInfrastructure
{
public:
	std::map<std::string, Stop> stopNodes;
	std::map<std::string, StreetNode> wayNodes;
	std::map<std::string, Street> streetWays;
	std::map<std::string, Railway> railWays;
};



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
public:
	CityInfrastructure infr;
};



class ProjectManager
{
public:
	Application* core;
	std::unique_ptr<Project> currProject;

	void createProject();
	void openProject();
	void saveProject();
	void saveProjectAs();
	void saveProjectCopy();
	void attachOSMData();
};

