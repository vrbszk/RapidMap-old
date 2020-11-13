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

void OSM_Data::parse(std::string path) // parsing from osm XML file to elements
{
	std::ifstream osm_file;
	osm_file.open(path.c_str());

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

			this->encoding = line.substr(third, fourth - third);

			continue;
		}
		else if (keyword == "osm")
		{
			int first = line.find('"') + 1;
			int second = line.find('"', first);

			this->osm_version = line.substr(first, second - first);

			continue;
		}
		else if (keyword == "meta")
		{
			int first = line.find('"') + 1;
			int second = line.find('"', first);

			this->meta_stamp = line.substr(first, second - first);

			continue;
		}
		else if (keyword == "bounds")
		{
			int first = line.find('"') + 1;
			int second = line.find('"', first);

			this->bounds.minlat = line.substr(first, second - first);
			line = line.substr(second + 1, line.size());


			first = line.find('"') + 1;
			second = line.find('"', first);

			this->bounds.minlon = line.substr(first, second - first);
			line = line.substr(second + 1, line.size());

			first = line.find('"') + 1;
			second = line.find('"', first);

			this->bounds.maxlat = line.substr(first, second - first);
			line = line.substr(second + 1, line.size());

			first = line.find('"') + 1;
			second = line.find('"', first);

			this->bounds.maxlon = line.substr(first, second - first);
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

			this->elements.push_back(node);

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

			this->elements[this->elements.size() - 1].params.push_back(param);

			continue;
		}
		else if (keyword == "way")
		{
			OSM_Element way;

			way.type = "way";

			int first = line.find('"') + 1;
			int second = line.find('"', first);

			way.id = line.substr(first, second - first);

			this->elements.push_back(way);

			continue;
		}
		else if (keyword == "nd")
		{
			OSM_Member nd;
			int first = line.find('"') + 1;
			int second = line.find('"', first);

			nd.id = line.substr(first, second - first);
			nd.type = "node";

			this->elements[this->elements.size() - 1].members.push_back(nd);

			continue;
		}
		else if (keyword == "relation")
		{
			OSM_Element rel;

			rel.type = "relation";

			int first = line.find('"') + 1;
			int second = line.find('"', first);

			rel.id = line.substr(first, second - first);

			this->elements.push_back(rel);

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

			this->elements[this->elements.size() - 1].members.push_back(member);

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
}