#pragma once
#include <string>
#include "OSM.hpp"
#include <SFML/Graphics.hpp>


class StreetNode : public sf::Drawable
{
public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::CircleShape shape2;

	std::string osmID;
	sf::Vector2f pos;
};

class Street : public sf::Drawable
{
public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::VertexArray path;

	std::string osmID;
	std::vector<std::string> nodeids;
};

StreetNode node_to_street(OSM_Element el, OSM_Bounds bounds);

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

	std::map<std::string, StreetNode> streetNodes;

	std::map<std::string, Street> streets;
};