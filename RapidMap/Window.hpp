#pragma once
#include <SFML/Graphics.hpp>

class Window;

#include "Interface.hpp"
#include "Application.hpp"

class Window : public sf::RenderWindow
{
public:
	Window();
	Window(sf::VideoMode mode, const sf::String& title, sf::Uint32 style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());
	Window(sf::WindowHandle handle, const sf::ContextSettings& settings = sf::ContextSettings());

	~Window();

	void render();

	void addInterface(InterfacePtr ptr);

	void updateEvents();
	void updateWindow();

	void setHintText(std::string str);
	void setHintFont(sf::Font f);

	Application* core;
private:
	std::vector<InterfacePtr> interfaces;
	sf::View standardView;

	std::string hintString;
	sf::Font hintFont;
};