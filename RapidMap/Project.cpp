#include <fstream>

#include "Project.hpp"
#include "Log.hpp"



StreetNode node_to_street(OSM_Element el)
{
	StreetNode node;
	node.osmID = el.id;
	for (auto it : el.params)
	{
		if (it.key == "lon")
			node.x = it.value;
		if (it.key == "lat")
			node.y = it.value;
	}
	return node;
}

void StreetNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::CircleShape shape;
	shape.setFillColor(sf::Color::Black);
	shape.setRadius(2);
	shape.setPosition((atof(x.c_str()) - 29) * 1000, (atof(y.c_str()) - 49) * 300);
	target.draw(shape, states);
}


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
	std::vector<OSM_Element> roads;

	for (auto it : data.elements)
	{
		if (it.type != "way")
			continue;

		for (auto p : it.params)
		{
			if (p.key == "highway")
			{
				if (p.value == "motorway" || p.value == "trunk" || p.value == "primary" || p.value == "secondary" || p.value == "tertiary" || p.value == "unclassified" || p.value == "residential" || p.value == "motorway_link" || p.value == "trunk_link" || p.value == "primary_link" || p.value == "secondary_link" || p.value == "tertiary_link")
					roads.push_back(it);
				break;
			}
		}
	}

	std::vector<std::string> nodeids;
	for (auto it : roads)
	{
		for (auto r : it.members)
		{
			if (r.type == "node")
			{
				nodeids.push_back(r.id);
				//std::cout << r.id << std::endl;
			}
		}
	}

	for (auto it : nodeids)
	{
		if (streetNodes.find(it) != streetNodes.end())
			continue;

		for (auto n : data.elements)
			if (n.type == "node" && n.id == it)
				streetNodes[it] = node_to_street(n);
	}
	for (auto it : streetNodes)
	{
		std::cout << it.first << " " << it.second.x << " " << it.second.y << std::endl;
	}
}

std::string Project::getName()
{
	return name + " - " + fileName + ((saved) ? "*" : "");
}

std::string Project::getFilePath()
{
	return path;
}

