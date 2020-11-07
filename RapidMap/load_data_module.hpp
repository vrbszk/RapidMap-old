#pragma once

#include "OSM.hpp"
#include <windows.h>

OPENFILENAME getRapidMapOFN();
std::string getSaveName();
std::string getOpenName();
OSM_Data load_map_data();