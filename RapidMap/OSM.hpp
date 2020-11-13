#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

struct OSM_Param // osm params k = v
{
public:
	std::string key;
	std::string value;
};

struct OSM_Member // osm members related to other structures (node to way or all to relations)
{
public:
	std::string id;
	std::string type;
	std::string role;
};

struct OSM_Element // oversimplified osm element. Useful for nodes, ways and relations (lat and lon are params)
{
public:
	std::string type;
	std::string id;
	std::vector<OSM_Param> params; // vector for element's params like type or lat
	std::vector<OSM_Member> members; // vector for members (ndid, wayid, relid). Empty for nodes. way's ndid is a member's type
};

struct OSM_Bounds // bounds of osm map
{
	std::string minlat;
	std::string minlon;
	std::string maxlat;
	std::string maxlon;
};

struct OSM_Data // all of osm data written in file
{
public:
	std::string encoding;
	std::string osm_version;
	std::string meta_stamp;

	OSM_Bounds bounds;

	std::vector<OSM_Element> elements;

	void parse(std::string path);
};

std::ostream& operator<<(std::ostream& c, OSM_Element el);
std::ostream& operator<<(std::ostream& c, OSM_Data data);

OSM_Data parse(std::string path); // parsing from osm XML file to elements