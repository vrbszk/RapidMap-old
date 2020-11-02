#include "OSM.hpp"

#include <iomanip>

std::ostream& operator<<(std::ostream& c, OSM_Element el)
{
	c << el.type << " " << el.id << std::endl;
	for (auto it : el.params)
		c << "     " << std::setw(20) << it.key << " = " << it.value << std::endl;
	for (auto it : el.members)
		c << "     " << std::setw(10) << it.type << " " << std::setw(10) << it.id << " " << it.role << std::endl;

	return c;
}

std::ostream& operator<<(std::ostream& c, OSM_Data data)
{
	c << "Encoding: " << data.encoding << std::endl;
	c << "OSM version: " << data.osm_version << std::endl;
	c << "Timestamp: " << data.meta_stamp << std::endl;
	c << std::endl;
	c << "Bounds " << std::endl;
	c << "     " << data.bounds.minlat << std::endl;
	c << "     " << data.bounds.minlon << std::endl;
	c << "     " << data.bounds.maxlat << std::endl;
	c << "     " << data.bounds.maxlon << std::endl;
	c << std::endl;
	for (auto it : data.elements)
		c << it << std::endl;


	c << "---End of OSM data---" << std::endl;

	return c;
}

OSM_Data parse(std::string path) // parsing from osm XML file to elements
{
	std::ifstream osm_file;
	osm_file.open(path.c_str());

	OSM_Data osm_data;

	while (!osm_file.eof())
	{
		std::string line;
		std::getline(osm_file, line);

		int it = line.find('<');
		if (it >= line.size())
			continue;
		line = line.substr(it + 1, line.size());

		it = line.find_last_of('>');
		line = line.substr(0, it);


		std::string keyword;
		it = line.find(' ');
		if (it >= line.size())
		{
			keyword = line;
			line = "";
		}
		else
		{
			keyword = line.substr(0, it);
			line = line.substr(it + 1, line.size());
		}


		if (keyword == "?xml")
		{
			int first = line.find('"') + 1;
			int second = line.find('"', first) + 1;
			int third = line.find('"', second) + 1;
			int fourth = line.find('"', third);

			osm_data.encoding = line.substr(third, fourth - third);

			continue;
		}
		else if (keyword == "osm")
		{
			int first = line.find('"') + 1;
			int second = line.find('"', first);

			osm_data.osm_version = line.substr(first, second - first);

			continue;
		}
		else if (keyword == "meta")
		{
			int first = line.find('"') + 1;
			int second = line.find('"', first);

			osm_data.meta_stamp = line.substr(first, second - first);

			continue;
		}
		else if (keyword == "bounds")
		{
			int first = line.find('"') + 1;
			int second = line.find('"', first);

			osm_data.bounds.minlat = line.substr(first, second - first);
			line = line.substr(second + 1, line.size());


			first = line.find('"') + 1;
			second = line.find('"', first);

			osm_data.bounds.minlon = line.substr(first, second - first);
			line = line.substr(second + 1, line.size());

			first = line.find('"') + 1;
			second = line.find('"', first);

			osm_data.bounds.maxlat = line.substr(first, second - first);
			line = line.substr(second + 1, line.size());

			first = line.find('"') + 1;
			second = line.find('"', first);

			osm_data.bounds.maxlon = line.substr(first, second - first);
		}
		else if (keyword == "node")
		{
			OSM_Element node;

			node.type = "node";

			int first = line.find('"') + 1;
			int second = line.find('"', first);

			node.id = line.substr(first, second - first);
			line = line.substr(second + 1, line.size());

			first = line.find('"') + 1;
			second = line.find('"', first);

			OSM_Param lat;
			lat.key = "lat";
			lat.value = line.substr(first, second - first);
			line = line.substr(second + 1, line.size());
			node.params.push_back(lat);

			OSM_Param lon;
			lon.key = "lon";
			lon.value = line.substr(first, second - first);
			node.params.push_back(lon);

			osm_data.elements.push_back(node);

			continue;
		}
		else if (keyword == "tag")
		{
			OSM_Param param;
			int first = line.find('"') + 1;
			int second = line.find('"', first);

			param.key = line.substr(first, second - first);
			line = line.substr(second + 1, line.size());

			first = line.find('"') + 1;
			second = line.find('"', first);

			param.value = line.substr(first, second - first);

			osm_data.elements[osm_data.elements.size() - 1].params.push_back(param);

			continue;
		}
		else if (keyword == "way")
		{
			OSM_Element way;

			way.type = "way";

			int first = line.find('"') + 1;
			int second = line.find('"', first);

			way.id = line.substr(first, second - first);

			osm_data.elements.push_back(way);

			continue;
		}
		else if (keyword == "nd")
		{
			OSM_Member nd;
			int first = line.find('"') + 1;
			int second = line.find('"', first);

			nd.id = line.substr(first, second - first);
			nd.type = "node";

			osm_data.elements[osm_data.elements.size() - 1].members.push_back(nd);

			continue;
		}
		else if (keyword == "relation")
		{
			OSM_Element rel;

			rel.type = "relation";

			int first = line.find('"') + 1;
			int second = line.find('"', first);

			rel.id = line.substr(first, second - first);

			osm_data.elements.push_back(rel);

			continue;
		}
		else if (keyword == "member")
		{
			OSM_Member member;
			int first = line.find('"') + 1;
			int second = line.find('"', first);

			member.type = line.substr(first, second - first);
			line = line.substr(second + 1, line.size());

			first = line.find('"') + 1;
			second = line.find('"', first);

			member.id = line.substr(first, second - first);
			line = line.substr(second + 1, line.size());

			first = line.find('"') + 1;
			second = line.find('"', first);

			member.role = line.substr(first, second - first);

			osm_data.elements[osm_data.elements.size() - 1].members.push_back(member);

			continue;
		}
		else if (keyword == "/node" || keyword == "/way" || keyword == "/relation" || keyword == "/osm")
		{
			continue;
		}
		else
			std::cout << keyword << line << std::endl;
	}

	osm_file.close();

	//std::cout << std::endl << std::endl << osm_data;

	std::cout << "---End of processing---" << std::endl;

	return osm_data;
}

OSM_Data read_from_mrf(std::string path)
{
	std::ifstream file;
	file.open(path);

	OSM_Data osm_data;

	while (!file.eof())
	{
		std::string keyword;

		file >> keyword;

		if (keyword == "META")
		{
			file >> osm_data.meta_stamp;

			continue;
		}
		else if (keyword == "BOUNDS")
		{
			file >> osm_data.bounds.minlat >> osm_data.bounds.minlon >> osm_data.bounds.maxlat >> osm_data.bounds.maxlon;

			continue;
		}
		else if (keyword == "NODE")
		{
			OSM_Element node;

			node.type = "node";

			file >> node.id;

			OSM_Param lat;
			lat.key = "lat";
			file >> lat.value;
			node.params.push_back(lat);

			OSM_Param lon;
			lon.key = "lon";
			file >> lon.value;
			node.params.push_back(lon);

			osm_data.elements.push_back(node);

			continue;
		}
		else if (keyword == "PARAM")
		{
			OSM_Param param;
			file >> param.key;
			std::getline(file, param.value);

			osm_data.elements[osm_data.elements.size() - 1].params.push_back(param);

			continue;
		}
		else if (keyword == "WAY")
		{
			OSM_Element way;

			way.type = "way";

			file >> way.id;

			osm_data.elements.push_back(way);

			continue;
		}
		else if (keyword == "REL")
		{
			OSM_Element relation;

			relation.type = "relation";

			file >> relation.id;

			osm_data.elements.push_back(relation);

			continue;
		}
		else if (keyword == "NDID")
		{
			OSM_Member nd;

			nd.type = "node";

			file >> nd.id;
			std::getline(file, nd.role);
			if (nd.role != "")
				nd.role = nd.role.substr(1, nd.role.size());
			if (nd.role == "NONE")
				nd.role = "";

			osm_data.elements[osm_data.elements.size() - 1].members.push_back(nd);

			continue;
		}
		else if (keyword == "WAYID")
		{
			OSM_Member w;

			w.type = "way";

			file >> w.id;
			std::getline(file, w.role);
			if (w.role != "")
				w.role = w.role.substr(1, w.role.size());
			if (w.role == "NONE")
				w.role = "";

			osm_data.elements[osm_data.elements.size() - 1].members.push_back(w);

			continue;
		}
		else if (keyword == "RELID")
		{
			OSM_Member r;

			r.type = "relation";

			file >> r.id;
			std::getline(file, r.role);
			if (r.role != "")
				r.role = r.role.substr(1, r.role.size());
			if (r.role == "NONE")
				r.role = "";

			osm_data.elements[osm_data.elements.size() - 1].members.push_back(r);

			continue;
		}
		else
		{
			std::string line;
			std::getline(file, line);
			std::cout << keyword << " --- " << line << std::endl;
		}
	}

	file.close();
	
	//std::cout << std::endl << std::endl << osm_data;

	std::cout << "---End of processing---" << std::endl;

	return osm_data;
}

void write_to_mrf(std::string path, const OSM_Data& osm_data)
{
	std::ofstream file;
	file.open(path);

	file << "META " << osm_data.meta_stamp << std::endl;
	file << "BOUNDS " << osm_data.bounds.minlat << " "
		<< osm_data.bounds.minlon << " "
		<< osm_data.bounds.maxlat << " "
		<< osm_data.bounds.maxlon << std::endl;

	for (auto el : osm_data.elements)
	{
		if (el.type == "node") file << "NODE " << el.id;
		else if (el.type == "way") file << "WAY " << el.id << std::endl;
		else if (el.type == "relation") file << "REL " << el.id << std::endl;

		for (auto m : el.members)
		{
			if (m.type == "node") file << "NDID ";
			else if (m.type == "way") file << "WAYID ";
			else if (m.type == "relation") file << "RELID ";

			file << m.id << " ";

			if (m.role == "" || m.role == " ")
				file << "NONE" << std::endl;
			else file << m.role << std::endl;
		}

		for (auto p : el.params)
		{
			if (p.key == "lat") file << p.value << " ";
			else if (p.key == "lon") file << p.value << std::endl;
			else file << "PARAM " << p.key << " " << p.value << std::endl;
		}
	}
}

void replace_elements_to_list(OSM_Data* osm_data)
{
	for (auto it = osm_data->elements.begin(); it != osm_data->elements.end(); it++)
	{
		if (it->type == "node")
		{
			OSM_Node node;
			node.id = it->id;
			node.lat = it->params[0].value;
			node.lon = it->params[1].value;

			for (auto p : it->params)
				node.params.push_back(p);

			osm_data->nodelist[node.id] = node;

			//osm_data.elements.erase(it);
		}
	}
}