#include "AssetManager.hpp"
#include "Log.hpp"

AssetManager::AssetManager()
{

}

AssetManager::~AssetManager()
{

}

void AssetManager::LoadTexture(std::string name, std::string fileName)
{
	if(textures.find(name) != textures.end())
		Log::makeLog("Warning:: Reloading texture " + name + " " + fileName);

	sf::Texture tex;

	if (tex.loadFromFile(fileName))
	{
		textures[name] = tex;
	}
	else
		Log::makeLog("Error:: Failed to load texture " + name + " " + fileName);
}

sf::Texture& AssetManager::GetTexture(std::string name)
{
	if (textures.find(name) == textures.end())
		Log::makeLog("Error:: Failed to get texture " + name);

	return textures.at(name);
}

void AssetManager::LoadFont(std::string name, std::string fileName)
{
	if (fonts.find(name) != fonts.end())
		Log::makeLog("Warning:: Reloading font " + name + " " + fileName);

	sf::Font font;

	if (font.loadFromFile(fileName))
	{
		fonts[name] = font;
	}
	else
		Log::makeLog("Error:: Failed to load font " + name + " " + fileName);
}

sf::Font& AssetManager::GetFont(std::string name)
{
	if (fonts.find(name) == fonts.end())
		Log::makeLog("Error:: Failed to get font " + name);

	return fonts.at(name);
}