#include "Window.hpp"

Window::Window() : sf::RenderWindow()
{
	standardView = this->getDefaultView();
}

Window::Window(sf::VideoMode mode, const sf::String& title, sf::Uint32 style, const sf::ContextSettings& settings) : 
	sf::RenderWindow(mode, title, style, settings)
{
	standardView = this->getDefaultView();
}

Window::Window(sf::WindowHandle handle, const sf::ContextSettings& settings) :
	sf::RenderWindow(handle, settings)
{
	standardView = this->getDefaultView();
}

Window::~Window()
{

}

void Window::addInterface(InterfacePtr ptr)
{
	ptr->setWindow(this);
	interfaces.push_back(std::move(ptr));
}

void Window::updateEvents()
{
	sf::Event event;
	while (this->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			core->closeWindow(this);
			break;
		case sf::Event::Resized:
		{
			this->standardView = sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height));
			this->setSize(sf::Vector2u(event.size.width, event.size.height));
			break;
		}

		
		}

		for (auto it = interfaces.begin(); it != interfaces.end(); it++)
		{
			(*it)->updateEvents(event);
		}
	}
}

void Window::updateWindow()
{
	sf::FloatRect spaceLeft(sf::Vector2f(0, 0), standardView.getSize());

	sf::FloatRect stripSpace(sf::Vector2f(spaceLeft.left, spaceLeft.top), sf::Vector2f(spaceLeft.width, 20));

	//menuStrip.updateInterface(stripSpace);
	interfaces[1]->updateInterface(stripSpace);

	spaceLeft.top = interfaces[1]->view.getSize().y;
	spaceLeft.height = spaceLeft.height - interfaces[1]->view.getSize().y;

	sf::FloatRect stateSpace(sf::Vector2f(spaceLeft.left + spaceLeft.width - 100, spaceLeft.top), sf::Vector2f(100, spaceLeft.height));

	//stateBlock.updateInterface(stateSpace);
	interfaces[2]->updateInterface(stateSpace);

	spaceLeft.width = spaceLeft.width - interfaces[2]->view.getSize().x;

	interfaces[0]->updateInterface(spaceLeft);
}

void Window::render()
{
	this->clear(sf::Color::White);

	this->setView(standardView);

	for (auto it = interfaces.begin(); it != interfaces.end(); it++)
	{
		(*it)->render();
	}

	this->display();
}