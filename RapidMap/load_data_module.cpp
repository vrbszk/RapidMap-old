#include "load_data_module.hpp"

#include "Log.hpp"

OPENFILENAME getRapidMapOFN()
{
	OPENFILENAME ofn;

	char szFile[100];
	char szTitle[100];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);

	ofn.lpstrFilter = "RapidMap Project(*.rmp)\0*.rmp\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST;

	return ofn;
}

std::string getSaveName()
{
	OPENFILENAME ofn;

	char szFile[100];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);

	ofn.lpstrFilter = "RapidMap Project(*.rmp)\0*.rmp\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST;

	if (GetOpenFileName(&ofn))
	{
		std::string path = ofn.lpstrFile;
		return path;
	}

	return "";
}

std::string getOpenName()
{
	OPENFILENAME ofn;

	char szFile[100];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);

	ofn.lpstrFilter = "RapidMap Project(*.rmp)\0*.rmp\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST;

	if (GetSaveFileName(&ofn))
	{
		std::string path = ofn.lpstrFile;
		return path;
	}

	return "";
}

OSM_Data load_map_data()
{
	OPENFILENAME ofn;

	char szFile[100];
	char szTitle[100];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);

	ofn.lpstrFilter = "All\0*.*\0OSM files(*.osm, *.xml)\0*.osm;*.xml\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn))
	{
		MessageBox(NULL, "Process terminated manually", "EOT", MB_OK);
		throw 3;
	}

	


	std::string path = ofn.lpstrFile;

	Log::makeLog("Reading OSM data from " + path + "...");

	std::string dir = path.substr(0, ofn.nFileOffset);
	std::string file = path.substr(ofn.nFileOffset, path.size());
	std::string extension = "";
	if (ofn.nFileExtension > 0)
		extension = path.substr(ofn.nFileExtension, path.size());
	std::string filename;
	if (ofn.nFileExtension > 0)
		filename = path.substr(ofn.nFileOffset, ofn.nFileExtension - ofn.nFileOffset);
	else
		filename = path.substr(ofn.nFileOffset, path.size());

	OSM_Data osm_data;

	if (extension == "xml" || extension == "osm" || extension == "")
	{
		osm_data.parse(path);
	}
	else
	{
		MessageBox(NULL, "Extension is not supported by application", "EOT", MB_OK);
		throw 3;
	}

	Log::makeLog("Data reading finished");

	return osm_data;
}