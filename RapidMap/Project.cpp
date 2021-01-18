#include "Project.hpp"

#include <fstream>
#include <cmath>
#include <iomanip>
#include <Windows.h>

#include "Log.hpp"
#include "load_data_module.hpp"

#define PI 3.14159
#define EARTH_RADIUS (6378137 / 5)

#define DEG2RAD(a) ((a) / (180 / PI))
#define RAD2DEG(a) ((a) * (180 / PI))


double y2lat_m(double y)
{
	return RAD2DEG(2 * std::atan(std::exp(y / EARTH_RADIUS)) - PI / 2);
}

double x2lon_m(double x)
{
	return RAD2DEG(x / EARTH_RADIUS);
}

double lat2y_m(double lat)
{
	return std::log(std::tan(DEG2RAD(lat) / 2 + PI / 4)) * EARTH_RADIUS;
}

double lon2x_m(double lon)
{
	return DEG2RAD(lon) * EARTH_RADIUS;
}

sf::Vector2f getMidPos(OSM_Bounds bounds)
{
	float toplon = stof(bounds.maxlon);
	float toplat = stof(bounds.maxlat);
	float lowlon = stof(bounds.minlon);
	float lowlat = stof(bounds.minlat);

	float dlon = toplon - lowlon;
	float dlat = toplat - lowlat;

	float midlat = toplat - dlat / 2;
	float midlon = toplon - dlon / 2;

	sf::Vector2f midPos;

	midPos.x = lon2x_m(midlon);
	midPos.y = lat2y_m(midlat);

	return midPos;
}

StreetNode node_to_street(OSM_Element el, sf::Vector2f midPos)
{
	StreetNode node;
	node.osmID = el.id;
	float nlat, nlon;
	for (auto it : el.params)
	{
		if (it.key == "lon")
			nlon = stof(it.value);
		if (it.key == "lat")
			nlat = stof(it.value);
	}

	sf::Vector2f globalPos;

	globalPos.x = lon2x_m(nlon);
	globalPos.y = lat2y_m(nlat);

	sf::Vector2f pos;

	pos.x = globalPos.x - midPos.x;
	pos.y = midPos.y - globalPos.y;

	node.setPosition(pos);

	return node;
}

Stop node_to_stop(OSM_Element el, sf::Vector2f midPos)
{
	Stop node;
	node.osmID = el.id;
	float nlat, nlon;
	for (auto it : el.params)
	{
		if (it.key == "lon")
			nlon = stof(it.value);
		if (it.key == "lat")
			nlat = stof(it.value);
	}

	sf::Vector2f globalPos;

	globalPos.x = lon2x_m(nlon);
	globalPos.y = lat2y_m(nlat);

	sf::Vector2f pos;

	pos.x = globalPos.x - midPos.x;
	pos.y = midPos.y - globalPos.y;

	node.setPosition(pos);

	return node;
}

Stop::Stop()
{
	shape.setFillColor(sf::Color::Green);
	shape.setRadius(5);
	shape.setOrigin(5, 5);
}

StreetNode::StreetNode()
{
	shape.setFillColor(sf::Color::Black);
	shape.setRadius(2);
	shape.setOrigin(2, 2);
}

void Stop::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();
	target.draw(shape, states);
}

sf::FloatRect Stop::getCollideBox()
{
	sf::FloatRect res = shape.getGlobalBounds();
	res.left += this->getPosition().x;
	res.top += this->getPosition().y;
	return res;
}

void Stop::setFillColor(sf::Color c)
{
	shape.setFillColor(c);
}

void StreetNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();
	target.draw(shape, states);
}

void Street::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(path, states);
}

void Railway::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(path, states);
}

void Line::refresh(CityInfrastructure* cif)
{
	path = sf::VertexArray(sf::LineStrip, stationids.size());
	int i = 0;
	for (auto it : stationids)
	{
		path[i].position = cif->stopNodes[it].getPosition();
		path[i].color = sf::Color::Blue;
		i++;
	}
}

void Line::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(path, states);
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
	fileName = path.substr(path.find_last_of('\\') + 1, path.size());
	std::ifstream file;
	file.open(path);

	file >> name;
	file >> creator;
	file >> version;
	file >> time_created;
	file >> time_edited;

	int nodes;
	file >> nodes;
	for (int i = 0; i < nodes; i++)
	{
		StreetNode node;
		sf::Vector2f p;
		file >> node.osmID >> p.x >> p.y;
		node.setPosition(p);
		infr.wayNodes[node.osmID] = node;
	}

	int streetscount;
	file >> streetscount;
	for (int i = 0; i < streetscount; i++)
	{
		Street street;
		int nodecount;
		file >> street.osmID >> nodecount;
		for (int j = 0; j < nodecount; j++)
		{
			std::string nodeid;
			file >> nodeid;
			street.nodeids.push_back(nodeid);
		}
		infr.streetWays[street.osmID] = street;
	}

	int railcount;
	file >> railcount;
	for (int i = 0; i < railcount; i++)
	{
		Railway rail;
		int nodecount;
		file >> rail.osmID >> rail.railType >> nodecount;
		for (int j = 0; j < nodecount; j++)
		{
			std::string nodeid;
			file >> nodeid;
			rail.nodeids.push_back(nodeid);
		}
		infr.railWays[rail.osmID] = rail;
	}

	int stopscount;
	file >> stopscount;
	for (int i = 0; i < stopscount; i++)
	{
		Stop stop;
		sf::Vector2f p;
		file >> stop.osmID >> p.x >> p.y;
		if (version == "v0.2")
		{
			std::string name;
			std::getline(file, name);
			int t = name.find_first_not_of(' ');
			name = name.substr(t);
			if (name == "none") name = "";
			stop.name = name;
		}
		stop.setPosition(p);
		infr.stopNodes[stop.osmID] = stop;
	}

	if (file.eof())
		goto FINISH;

	int linecount;
	file >> linecount;
	//std::string l;
	//std::getline(file, l);
	for (int i = 0; i < linecount; i++)
	{
		Line line;
		std::string name;
		file >> line.index_reserved;
		std::getline(file, name);
		name = name.substr(1);
		if (name == "none") name = "";
		line.name = name;

		int stopscount;
		file >> stopscount;
		for (int j = 0; j < stopscount; j++)
		{
			std::string stopid;
			file >> stopid;
			line.stationids.push_back(stopid);
		}
		schemedata.lines.push_back(line);
	}

	FINISH:

	file.close();


	for (auto it = infr.streetWays.begin(); it != infr.streetWays.end(); it++)
	{
		it->second.path = sf::VertexArray(sf::LineStrip, it->second.nodeids.size());
		for (int i = 0; i < it->second.nodeids.size(); i++)
		{
			it->second.path[i].position = infr.wayNodes[it->second.nodeids[i]].getPosition();
			it->second.path[i].color = sf::Color::Blue;
		}
	}

	for (auto it = infr.railWays.begin(); it != infr.railWays.end(); it++)
	{
		it->second.path = sf::VertexArray(sf::LineStrip, it->second.nodeids.size());
		for (int i = 0; i < it->second.nodeids.size(); i++)
		{
			it->second.path[i].position = infr.wayNodes[it->second.nodeids[i]].getPosition();
			it->second.path[i].color = sf::Color::Red;
		}
	}


	saved = true;

	Log::makeLog("Project opened");
}

void Project::save(bool updateVersion)
{
	saveProject(path, updateVersion);
}

void Project::saveAs(const std::string& filepath, bool updateVersion)
{
	saveProject(filepath, updateVersion);
	path = filepath;
	fileName = path.substr(path.find_last_of('\\') + 1, path.size());
}

void Project::saveCopy(const std::string& filepath, bool updateVersion)
{
	saveProject(filepath, updateVersion);
}

void Project::saveProject(const std::string& filepath, bool updateVersion)
{
	if (line.stationids.size() > 0)
	{
		line.name = Log::getCurrTimestamp();
		schemedata.lines.push_back(line);
	}








	Log::makeLog("Saving a project to " + filepath);

	//if (updateVersion)
		version = RAPIDMAP_FILE_VERSION;

	time_edited = Log::getCurrTimestamp();

	std::ofstream file;
	file.open(filepath);
	
	file << name << std::endl;
	file << creator << std::endl;
	file << version << std::endl;
	file << time_created << std::endl;
	file << time_edited << std::endl;

	file << infr.wayNodes.size() << std::endl;
	for (auto it : infr.wayNodes)
	{
		file << it.second.osmID << " " << it.second.getPosition().x << " " << it.second.getPosition().y << std::endl;
	}

	file << infr.streetWays.size() << std::endl;
	for (auto it : infr.streetWays)
	{
		file << it.second.osmID << " " << it.second.nodeids.size() << std::endl;
		for (auto n : it.second.nodeids)
		{
			file << n << " ";
		}
		file << std::endl;
	}

	file << infr.railWays.size() << std::endl;
	for (auto it : infr.railWays)
	{
		file << it.second.osmID << " " << it.second.railType << " " << it.second.nodeids.size() << std::endl;
		for (auto n : it.second.nodeids)
		{
			file << n << " ";
		}
		file << std::endl;
	}

	file << infr.stopNodes.size() << std::endl;
	for (auto it : infr.stopNodes)
	{
		file << it.second.osmID << " " << it.second.getPosition().x << " " << it.second.getPosition().y << " ";
//		if(version == "v0.2")
		if (it.second.name == "")
			file << "none" << std::endl;
		else
			file << it.second.name << std::endl;
	}

	file << schemedata.lines.size() << std::endl;
	for (auto it : schemedata.lines)
	{
		file << it.index_reserved << " ";
		if (it.name == "")
			file << "none" << std::endl;
		else
			file << it.name << std::endl;
		file << it.stationids.size() << std::endl;
		for (auto st : it.stationids)
		{
			file << st << " ";
		}
		file << std::endl;
	}

	file.close();

	saved = true;

	Log::makeLog("Project saved");
}

void Project::attachData(OSM_Data data)
{
	Log::makeLog("Attaching osm data to project...");
	std::vector<OSM_Element> stops;
	std::vector<OSM_Element> roads;
	std::vector<std::pair<OSM_Element, std::string>> rails;
	std::map<std::string, OSM_Element> nodeBuf;
	std::map<std::string, OSM_Element> wayBuf;
	std::map<std::string, OSM_Element> relBuf;

	sf::Vector2f midPos = getMidPos(data.bounds);

	for (auto it : data.elements)
	{

		if (it.type == "node")
		{
			nodeBuf[it.id] = it;
			for (auto p : it.params)
			{
				if (p.key == "highway" && p.value == "bus_stop")
					stops.push_back(it);

				else if (p.key == "public_transport")
				{
					if (p.value == "platform" || p.value == "stop_position")
						stops.push_back(it);
				}
			}
		}

		if (it.type == "way")
		{
			wayBuf[it.id] = it;
			for (auto p : it.params)
			{
				if (p.key == "highway")
				{
					if (p.value == "motorway" || p.value == "trunk" || p.value == "primary" || p.value == "secondary" || p.value == "tertiary" || p.value == "unclassified" || p.value == "residential" || p.value == "motorway_link" || p.value == "trunk_link" || p.value == "primary_link" || p.value == "secondary_link" || p.value == "tertiary_link")
					{
						roads.push_back(it);
						break;
					}

				}

				if (p.key == "railway")
				{
					if (p.value == "funicular" || p.value == "light_rail" || p.value == "monorail" || p.value == "narrow_gauge" || p.value == "rail" || p.value == "subway" || p.value == "tram")
					{
						rails.push_back(std::make_pair(it, p.value));
						break;
					}


				}
			}
		}

		if (it.type == "relation")
		{
			relBuf[it.id] = it;
		}
	}

	Log::makeLog("Element processing finished");

	std::vector<std::string> nodeids;
	for (auto it : roads)
	{
		Street street;

		street.osmID = it.id;

		for (auto r : it.members)
		{
			if (r.type == "node")
			{
				nodeids.push_back(r.id);
				street.nodeids.push_back(r.id);
			}
		}
		infr.streetWays[street.osmID] = street;
	}

	Log::makeLog("roads processed");

	for (auto it : rails)
	{
		Railway rail;
		rail.osmID = it.first.id;
		rail.railType = it.second;
		for (auto r : it.first.members)
		{
			if (r.type == "node")
			{
				nodeids.push_back(r.id);
				rail.nodeids.push_back(r.id);
			}
		}
		infr.railWays[rail.osmID] = rail;
	}

	Log::makeLog("rails processed");

	for (auto it : nodeids)
	{
		/*if (infr.wayNodes.find(it) != infr.wayNodes.end())
			continue;

		for (auto n : data.elements)
			if (n.type == "node" && n.id == it)
			{
				infr.wayNodes[it] = node_to_street(n, data.bounds);
				break;
			}*/
		infr.wayNodes[it] = node_to_street(nodeBuf[it], midPos);
	}

	Log::makeLog("nodes processed");

	for (auto it : stops)
	{
		std::string name = "";
		for (auto p : it.params)
		{
			if (p.key == "name")
			{
				name = p.value;
				break;
			}
		}
		infr.stopNodes[it.id] = node_to_stop(it, midPos);
		infr.stopNodes[it.id].name = name;
	}

	Log::makeLog("stops processed");

	for (auto it = infr.streetWays.begin(); it != infr.streetWays.end(); it++)
	{
		it->second.path = sf::VertexArray(sf::LineStrip, it->second.nodeids.size());
		for (int i = 0; i < it->second.nodeids.size(); i++)
		{
			it->second.path[i].position = infr.wayNodes[it->second.nodeids[i]].getPosition();
			it->second.path[i].color = sf::Color::Blue;
		}
	}

	for (auto it = infr.railWays.begin(); it != infr.railWays.end(); it++)
	{
		it->second.path = sf::VertexArray(sf::LineStrip, it->second.nodeids.size());
		for (int i = 0; i < it->second.nodeids.size(); i++)
		{
			it->second.path[i].position = infr.wayNodes[it->second.nodeids[i]].getPosition();
			it->second.path[i].color = sf::Color::Red;
		}
	}

	Log::makeLog("pathes created");

	Log::makeLog("Data attached");
}

std::string Project::getName()
{
	return name + " - " + fileName + ((saved) ? "" : "*");
}

std::string Project::getFilePath()
{
	return path;
}



void ProjectManager::createProject()
{
	if (currProject)
	{
		int res = MessageBox(NULL, "Do you want to save current project?", "Create new project", MB_YESNOCANCEL);
		switch (res)
		{
		case IDYES:
			saveProject();
			break;
		case IDNO:
			break;
		case IDCANCEL:
			return;
		default:
			return;
		}
	}

	currProject = std::make_unique<Project>();
	currProject->create(core->username, core->version);
}

void ProjectManager::openProject()
{
	if (currProject)
	{
		int res = MessageBox(NULL, "Do you want to save current project?", "Open project", MB_YESNOCANCEL);
		switch (res)
		{
		case IDYES:
			saveProject();
			break;
		case IDNO:
			break;
		case IDCANCEL:
			return;
		default:
			return;
		}
	}

	std::string path = getOpenName();

	if (path != "")
	{
		currProject = std::make_unique<Project>();
		currProject->open(path);
	}
}

void ProjectManager::saveProject()
{
	if (!currProject) return;

	if (currProject->getFilePath() != "")
		currProject->save();
	else
	{
		std::string path = getSaveName();
		if (path != "")
			currProject->saveAs(path);
	}
}

void ProjectManager::saveProjectAs()
{
	if (!currProject) return;

	std::string path = getSaveName();
	if (path != "")
		currProject->saveAs(path);
}

void ProjectManager::saveProjectCopy()
{
	if (!currProject) return;

	std::string path = getSaveName();
	if (path != "")
		currProject->saveCopy(path);
}

void ProjectManager::attachOSMData()
{
	if (currProject)
	{
		try
		{
			currProject->attachData(load_map_data());
		}
		catch (...)
		{

		}
	}
	else MessageBox(NULL, "Create a project at first", "No project opened", MB_OK);
}

