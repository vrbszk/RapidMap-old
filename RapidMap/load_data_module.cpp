#include "load_data_module.hpp"

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
	//ofn.nMaxFileTitle = sizeof(szTitle);
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	return ofn;
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

	ofn.lpstrFilter = "All\0*.*\0OSM files(*.osm, *.xml)\0*.osm;*.xml\0CyberCity files(*.mrf)\0*.mrf\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	//ofn.nMaxFileTitle = sizeof(szTitle);
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn))
	{
		MessageBox(NULL, "Process terminated manually", "EOT", MB_OK);
		throw 3;
	}




	std::string path = ofn.lpstrFile;

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
		osm_data = parse(path);
		write_to_mrf(dir + "mp.mrf", osm_data);
	}
	else if (extension == "mrf")
	{
		osm_data = read_from_mrf(path);
	}
	else
	{
		MessageBox(NULL, "Extension is not supported by application", "EOT", MB_OK);
		throw 3;
	}

	return osm_data;
}